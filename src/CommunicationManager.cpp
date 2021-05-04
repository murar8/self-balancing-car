#include "CommunicationManager.h"
#include <HardwareSerial.h>

void CommunicationManager::tick()
{
    while (fillBuffer())
    {
        Packet packet = parsePacket();
        handleRequest(&packet);
        flushBuffer();
    }
}

void CommunicationManager::handleRequest(Packet *packet)
{
    Handler *handler = findProperty(packet->property);

    if (packet->property == nullptr)
    {
        writePacket(RESPONSE_MALFORMED);
        return;
    }

    if (handler == nullptr)
    {
        writePacket(RESPONSE_UNKNOWN);
        return;
    }

    if (packet->payload != nullptr && handler->set != nullptr)
    {
        bool error = handler->set(packet->payload);

        if (!error)
        {
            writePacket(RESPONSE_OK);
            return;
        }
        else
        {
            writePacket(RESPONSE_ERROR);
            return;
        }
    }
    else if (packet->payload == nullptr && handler->get != nullptr)
    {
        bool error = handler->get(serial_buffer_);

        if (!error)
        {
            writePacket(serial_buffer_);
            return;
        }
        else
        {
            writePacket(RESPONSE_ERROR);
            return;
        }
    }

    writePacket(RESPONSE_DENIED);
    return;
}

bool CommunicationManager::fillBuffer()
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

        if (serial_buffer_size_ == PACKET_SIZE)
        {
            serial_buffer_[serial_buffer_size_ - 1] = next;
            continue;
        }

        serial_buffer_[serial_buffer_size_] = next;
        serial_buffer_size_++;
    }
}

void CommunicationManager::flushBuffer()
{
    serial_buffer_size_ = 0;
}

CommunicationManager::Packet CommunicationManager::parsePacket()
{
    if (serial_buffer_size_ >= PACKET_SIZE)
    {
        return Packet{nullptr, nullptr};
    }

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

Handler *CommunicationManager::findProperty(char *search)
{
    if (search == nullptr)
    {
        return nullptr;
    }

    for (size_t i = 0; i < handlers_size_; i++)
    {
        if (strcasecmp(handlers_[i].name, search) == 0)
        {
            return &handlers_[i];
        }
    }

    return nullptr;
}

void CommunicationManager::writePacket(const char *data)
{
    stream_->print(data);
    stream_->print(PACKET_DELIMITER);
}
