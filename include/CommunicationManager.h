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

#ifndef _COMMUNICATION_MANAGER_H_
#define _COMMUNICATION_MANAGER_H_

#include <Stream.h>
#include <assert.h>

struct Handler
{
    /// Write the state value into the provided buffer.
    /// Return true if an error occured during the operation.
    typedef bool (*Getter)(char *buffer);

    /// Write the data present in the buffer into the state value.
    /// Return true if an error occured during the operation.
    typedef bool (*Setter)(char *buffer);

    const char *name;
    Getter get;
    Setter set;
};

class CommunicationManager
{
  public:
    /// Attach a Stream instance as the data source.
    template <size_t LEN> void begin(Stream *stream, Handler (&handlers)[LEN])
    {
        assert(stream != nullptr);
        assert(handlers != nullptr);

        stream_ = stream;
        handlers_ = handlers;
        handlers_size_ = LEN;
    }

    void tick();

  private:
    /// Maximum length of a packet in bytes.
    static const size_t PACKET_SIZE = 64;

    /// Character used to mark the end of a packet.
    static const char PACKET_DELIMITER = '\n';

    /// Character used to mark end of the property name and the start of the packet payload.
    static const char PAYLOAD_DELIMITER = '=';

    /// Response used when setting the property was successful.
    static constexpr const char *RESPONSE_OK = "OK";

    /// Response used when there was an error setting the property.
    static constexpr const char *RESPONSE_ERROR = "ERROR";

    /// Response used when the property is not defined.
    static constexpr const char *RESPONSE_UNKNOWN = "UNKNOWN";

    /// Response used when the property's getter/setter is not defined.
    static constexpr const char *RESPONSE_DENIED = "DENIED";

    /// Response used when the query is malformed.
    static constexpr const char *RESPONSE_MALFORMED = "MALFORMED";

    struct Packet
    {
        char *property, *payload;
    };

    Stream *stream_;

    Handler *handlers_;
    size_t handlers_size_ = 0;

    char serial_buffer_[PACKET_SIZE];
    size_t serial_buffer_size_ = 0;

    /// Store the next incoming packet into the serial buffer.
    /// Returns true if a packet is currently stored in the buffer.
    bool fillBuffer();

    /// Reset the serial buffer item count.
    void flushBuffer();

    /// Parse a packet from the serial buffer.
    /// Returns nullptr on the property field if the packet is malformed.
    /// Returns nullptr on the payload field if the packet does not have a payload.
    Packet parsePacket();

    /// Find a property with the corresponding name in the properties array, nullptr otherwise.
    Handler *findProperty(char *search);

    /// Push a packet to the output stream.
    void writePacket(const char *data);

    void handleRequest(Packet *packet);
};

#endif // _COMMUNICATION_MANAGER_H_
