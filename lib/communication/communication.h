/// Serial communication handler.

/*
 * Protocol documentation
 *
 * Character set: ASCII, case insensitive, whitespace insensitive.
 *
 * Examples:
 *
 *      Get a property:
 *          >>> test_property
 *          <<< 100
 *
 *      Set a property:
 *          >>> test_property=200
 *          <<< OK
 *
 *      Error setting a property:
 *          >>> test_property=invalid
 *          <<< ERROR
 *
 *      Unknown property:
 *          >>> unknown_property
 *          <<< UNKNOWN
 *
 *      Unknown method:
 *          >>> machine_uuid=200
 *          <<< DENIED
 *
 *      Malformed request:
 *          >>> test_property=20=200
 *          <<< MALFORMED
 *
 */

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <Arduino.h>
#include <CircularBuffer.h>
#include <assert.h>

/// Maximum length of a packet in bytes.
#define PACKET_LENGTH 64

/// Character used to mark the end of a packet.
#define PACKET_DELIMITER '\n'

/// Character used to mark end of the property name and the start of the packet payload.
#define PAYLOAD_DELIMITER '='

/// Response used when setting the property was successful.
#define RESPONSE_OK "OK"

/// Response used when there was an error setting the property.
#define RESPONSE_ERROR "ERROR"

/// Response used when the property is not defined.
#define RESPONSE_UNKNOWN "UNKNOWN"

/// Response used when the property's getter/setter is not defined.
#define RESPONSE_DENIED "DENIED"

/// Response used when the query is malformed.
#define RESPONSE_MALFORMED "MALFORMED"

typedef char *(*Getter)();
typedef bool (*Setter)(char *);

struct Property
{
    const char *name;
    Getter get;
    Setter set;
};

template <size_t PROPERTY_COUNT> class CommunicationHandler
{
  public:
    /// Start the command handler using the supplied stream as a source.
    void begin(Stream *stream)
    {
        assert(stream != nullptr);
        stream_ = stream;
    };

    /// Check if any commands were received and call the respective handlers, then send the response.
    void tick()
    {
        while (fillBuffer())
        {
            Packet packet = parsePacket();
            flushBuffer();

            if (packet.property == nullptr)
            {
                stream_->print(RESPONSE_MALFORMED);
                stream_->print(PACKET_DELIMITER);
                continue;
            }

            Property *property = findProperty(packet.property);

            if (property == nullptr)
            {
                stream_->print(RESPONSE_UNKNOWN);
                stream_->print(PACKET_DELIMITER);
                continue;
            }

            if (packet.payload != nullptr)
            {
                bool ok = property->set(packet.payload);

                if (!ok)
                {
                    stream_->print(RESPONSE_ERROR);
                    stream_->print(PACKET_DELIMITER);
                }
                else
                {
                    stream_->print(RESPONSE_OK);
                    stream_->print(PACKET_DELIMITER);
                }
            }
            else
            {
                char *payload = property->get();

                if (payload == nullptr)
                {
                    stream_->print(RESPONSE_ERROR);
                    stream_->print(PACKET_DELIMITER);
                }
                else
                {
                    stream_->print(payload);
                    stream_->print(PACKET_DELIMITER);
                }
            }
        }
    }

    /// Define a property with handlers for retrieving and setting the value.
    void attachProperty(Property property)
    {
        properties_[properties_size_] = property;
    };

  private:
    struct Packet
    {
        char *property, *payload;
    };

    Stream *stream_;

    Property properties_[PROPERTY_COUNT];
    size_t properties_size_ = 0;

    char serial_buffer_[PACKET_LENGTH];
    size_t serial_buffer_size_ = 0;

    /// Store the next incoming packet into the serial buffer.
    /// Returns true if a packet is currently stored in the buffer.
    bool fillBuffer()
    {
        for (;;)
        {
            if (serial_buffer_size_ > 0 && serial_buffer_[serial_buffer_size_ - 1] == PACKET_DELIMITER)
            {
                return true;
            }

            if (!stream_->available())
            {
                return false;
            }

            char next = stream_->read();

            if (next != PACKET_DELIMITER && next != PAYLOAD_DELIMITER && isspace(next))
            {
                continue;
            }

            assert(serial_buffer_size_ <= PACKET_LENGTH);
            serial_buffer_[serial_buffer_size_] = next;
            serial_buffer_size_++;
        }
    }

    /// Reset the serial buffer item count.
    void flushBuffer()
    {
        serial_buffer_size_ = 0;
    }

    /// Parse a packet from the serial buffer.
    /// Returns nullptr on the property field if the packet is malformed.
    /// Returns nullptr on the payload field if the packet does not have a payload.
    Packet parsePacket()
    {
        char *property_start = nullptr;
        char *payload_start = nullptr;

        for (size_t i = 0; i < serial_buffer_size_; i++)
        {
            if (serial_buffer_[i] == PACKET_DELIMITER)
            {
                if (i != 0)
                {
                    property_start = serial_buffer_;
                    serial_buffer_[i] = '\0';
                }

                break;
            }

            if (serial_buffer_[i] == PAYLOAD_DELIMITER)
            {
                if (payload_start != nullptr)
                {
                    property_start = nullptr;
                    payload_start = nullptr;
                    break;
                }
                else
                {
                    payload_start = serial_buffer_ + i + 1;
                    serial_buffer_[i] = '\0';
                    continue;
                }
            }

            if (!isascii(serial_buffer_[i]))
            {
                property_start = nullptr;
                payload_start = nullptr;
                break;
            }
        }

        return Packet{property_start, payload_start};
    }

    /// Find a property with the corresponding name in the properties array, nullptr otherwise.
    Property *findProperty(char *search)
    {
        for (size_t i = 0; i < PROPERTY_COUNT; i++)
        {
            if (strcasecmp(properties_[i].name, search) == 0)
            {
                return &properties_[i];
            }
        }
        return nullptr;
    }
};

#endif
