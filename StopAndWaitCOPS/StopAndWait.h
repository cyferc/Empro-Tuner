
#pragma once

#include <QObject>
#include "COBS.h"
#include "helpers.h"

#include "QDebug"

// Packet format:

// Unreliable Data
//      [8 0] [~ DATA] [16 CRC] [8 PacketBoundaryMarker]

// Reliable Data
//      [8 H1] [8 H2] [8 H3] [8 H4] [~ DATA] [16 CRC] [8 PacketBoundaryMarker]

// Reliable ACK
//      [8 H1] [8 H2] [8 H3] [8 H4] [16 CRC] [8 PacketBoundaryMarker]

// H1 (bits):
//      [1]
//          (1) Reliable
//          (0) Unreliable
//      [1]
//          (1) Handshake: seqNum is expectedSeqNum
//          (0) Not handshake, mid stream
//      [1]
//          (1) Packet is ACK
//          (0) Packet is Data
//      [5 Seq num]

// H2 (bits):
//      [8 Seq num continue]

// H3 (bits):
//      [8 Seq num continue]

// H4 (bits):
//      [8 Seq num continue]

class StopAndWait : public QObject
{
    Q_OBJECT

public:
    StopAndWait():
        _rxBufferIndex(0)
    {
    }

    void receiveData(const uint8_t data)
    {
        // PacketBoundaryMarker
        if (data == PacketBoundaryMarker)
        {
            uint8_t _decodeBuffer[_rxBufferIndex];

            size_t numDecoded = COBS::decode(_rxBuffer, _rxBufferIndex, _decodeBuffer);

            // must have at least header and trailer
            if (numDecoded >= (1 + ChecksumSize))
            {
                // Calculate checksum
                uint16_t calculatedCRC = calculateCRC(_decodeBuffer, numDecoded - ChecksumSize);
                uint16_t receivedCRC   = U16_CONCAT_U8(_decodeBuffer[numDecoded - 2], _decodeBuffer[numDecoded - 1]); // crc is the last 2 bytes in decoded packet

                if (calculatedCRC == receivedCRC)
                {
                    //qDebug() << "Checksum IS matching";

                    // Now check header
                    /// Reliable
                    if (_decodeBuffer[0] > 0)
                    {
                        /// Handshake
                        if (BIT_CHECK(_decodeBuffer[0], HeaderBitHandshake))
                        {
                            // ACK
                            if (BIT_CHECK(_decodeBuffer[0], HeaderBitAck))
                            {
                                // Handshake complete, receiver is ready for file
                                // Send new file
                            }
                            // not ACK
                            else
                            {
                                // Sender wants to send us a new file, save command and send ACK
                                // In this case, seqNum is actually the command value.
                                incomingCommand = getSequenceNum(_decodeBuffer[0], _decodeBuffer[1], _decodeBuffer[2], _decodeBuffer[3]);

                                // New file incoming
                                expectedSeqNum = 0;

                                // Send ACK with handshake bit set
                                uint16_t frameSize;
                                uint8_t *pointer;
                                uint8_t *frame = makeFrame(pointer, 0, true, true, true, incomingCommand, &frameSize);

                                // Packet is created, now send
                                putCharArray(frame, frameSize);
                            }
                        }
                        /// Not Handshake
                        else
                        {
                            // ACK
                            if (BIT_CHECK(_decodeBuffer[0], HeaderBitAck))
                            {
                                uint32_t seqNum = getSequenceNum(_decodeBuffer[0], _decodeBuffer[1], _decodeBuffer[2], _decodeBuffer[3]);

                                if (seqNum == expectedSeqNum)
                                {

                                }

                                emit processFrame(_decodeBuffer + 4, numDecoded - ChecksumSize); // All good, give packet to upper layer
                            }
                            // DATA
                            else
                            {
                                uint32_t seqNum = getSequenceNum(_decodeBuffer[0], _decodeBuffer[1], _decodeBuffer[2], _decodeBuffer[3]);

                                if (seqNum == expectedSeqNum)
                                {

                                }

                                emit processFrame(_decodeBuffer + 4, numDecoded - ChecksumSize); // All good, give packet to upper layer
                            }
                        }
                    }
                    /// Unreliable
                    else
                    {
                        emit processFrame(_decodeBuffer + 1, numDecoded - ChecksumSize); // All good, give packet to upper layer
                    }
                }
                else
                {
                    //qDebug() << "Checksum not matching";
                }
            }
            _rxBufferIndex = 0;
        }
        else
        {
            if ((_rxBufferIndex + 1) < BufferSize)
            {
                _rxBuffer[_rxBufferIndex++] = data;
            }
            else
            {
                // Error, buffer overflow if we write.
            }
        }
    }

    // Returns pointer to created packet buffer, createdPacketSize will be changed to new packet size
    uint8_t* makeFrameUnreliable(const uint8_t* dataBuffer, uint16_t dataSize, uint16_t *createdPacketSize)
    {
        return makeFrame(dataBuffer, dataSize, false, false, false, 0, createdPacketSize);
    }

signals:
    void processFrame(const uint8_t *frameBuffer, uint16_t frameLength);
    void putCharArray(const uint8_t *frameBuffer, uint16_t frameLength);

private:
    // Returns pointer to created packet buffer, createdPacketSize will be changed to new packet size
    uint8_t* makeFrame(const uint8_t* dataBuffer, uint16_t dataSize, bool reliable, bool handshake, bool ack, uint32_t sequenceNumber, uint16_t *createdPacketSize)
    {
        uint8_t headerSize;

        if(dataBuffer == 0 || dataSize == 0)
        {
            *createdPacketSize = 0;
            return 0;
        }

        if (reliable)
            headerSize = 4;
        else
            headerSize = 1;

        // Check that we have enough room
        if (BufferSize < COBS::getEncodedBufferSize(headerSize, dataSize, ChecksumSize) + 1)
        {
            *createdPacketSize = 0;
            return 0;
        }

        uint8_t header[headerSize];
        uint8_t checksum[ChecksumSize];

        /// Make Header ////////////////////////////
        header[0] = 0;

        if (reliable)
        {
            // Set sequenceNumber first, then other bits
            header[0] = U32_HI8(sequenceNumber);
            header[1] = U32_MH8(sequenceNumber);
            header[2] = U32_ML8(sequenceNumber);
            header[3] = U32_LO8(sequenceNumber);

            BIT_SET(header[0], HeaderBitReliable);

            if (handshake)
                BIT_SET(header[0], HeaderBitHandshake);

            if (ack)
                BIT_SET(header[0], HeaderBitAck);
        }

        /// Make Trailer ////////////////////////////
        // Calculate CRC of header and original data
        uint16_t crc = CRC16_CCITT_INIT_VAL;

        // Header
        for (int i = 0; i < headerSize; i++)
            crc = _crc_ccitt_update(crc, header[i]);

        // Data
        for (int i = 0; i < dataSize; i++)
            crc = _crc_ccitt_update(crc, dataBuffer[i]);

        checksum[0] = U16_HI8(crc);
        checksum[1] = U16_LO8(crc);

        uint16_t numEncoded = COBS::encode(header, headerSize,     // header
                                           dataBuffer, dataSize,   // source
                                           checksum, ChecksumSize, // checksum
                                           _txBuffer);             // destination

        // Append boundary marker
        _txBuffer[numEncoded] = PacketBoundaryMarker;

        *createdPacketSize = numEncoded + 1;
        return _txBuffer;
    }

    static __inline__ uint32_t getSequenceNum(uint8_t headerByte1, uint8_t headerByte2, uint8_t headerByte3, uint8_t headerByte4)
    {
        uint32_t seqNum = U32_CONCAT_U8(headerByte1, headerByte2, headerByte3, headerByte4);

        // Clear 3 most significant bits
        BITMASK_CLEAR(seqNum, 0x1FFFFFFF);

        // Now we have the actual seqNum
        return seqNum;
    }

    static __inline__ uint16_t calculateCRC(const uint8_t *dataBuffer, const uint16_t dataCount)
    {
        uint16_t crc = CRC16_CCITT_INIT_VAL;

        for (int i = 0; i < dataCount; i++)
            crc = _crc_ccitt_update(crc, dataBuffer[i]);

        return crc;
    }

    static __inline__ uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
    {
        data ^= U16_LO8(crc);
        data ^= data << 4;
        return((((uint16_t)data << 8) | U16_HI8(crc)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
    }

    enum HeaderBit
    {
        HeaderBitReliable = 0,
        HeaderBitHandshake,
        HeaderBitAck
    };

    static const uint8_t  PacketBoundaryMarker = 0;
    static const uint16_t BufferSize = 10240;
    static const uint16_t CRC16_CCITT_INIT_VAL = 0xFFFF;
    static const uint8_t  ChecksumSize = 2; // uint16 CRC

    uint8_t  _txBuffer[BufferSize];
    uint8_t  _rxBuffer[BufferSize];
    uint16_t _rxBufferIndex;

    uint32_t expectedSeqNum;
    uint32_t incomingCommand;
};
