#include "tcpdumpdata.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpDumpData::CTcpDumpData()
    : CData()
    , m_magic_word(4, static_cast<char>(0))
    , m_little_endian(true)
    , m_packets()
{

}

CTcpDumpData::~CTcpDumpData()
{
    // Delete packets that have been parsed.
    // packets are deleted automatically because of the shared pointer.
}


//------------------------------------------------------------------------------
// Public Functions

bool CTcpDumpData::parse(const QByteArray &blob)
{
    quint32 offset = 0;

    // Get the magic word and infer the endianess from it.
    offset = parseHeader(blob);
    if(offset > 0) {
        // Extract as many packets as we can starting from 'offset'.
        offset = parsePackets(blob, offset);
        if(offset > 0) {
            qDebug() << "Parsed" << offset << "total bytes.";
            return true;
        }
    }

    return false;
}

quint32 CTcpDumpData::availablePackets()
{
    return m_packets.size();
}

bool CTcpDumpData::validIp(QSharedPointer<CTcpDumpPacket> packet)
{
    if(packet->ip == 0) {
        return true;
    }

    quint32 t = 0;
    for(int i = packet->ipheaderlen() - 2; i >= 0; i -= 2) {
        t += packet->get2((packet->ip) + i);
    }
    t = (t >> 16) + (t & 0xffff);

    return t == 0xffff;
}

bool CTcpDumpData::defrag(QSharedPointer<CTcpDumpPacket> packet)
{
    if(packet->fragoffset() == 0 && !packet->fragfollows()) {
        return true;
    }

    return false;
}


//------------------------------------------------------------------------------
// Private Functions

quint32 CTcpDumpData::parseHeader(const QByteArray &blob)
{
    // The header of tcp dumps is 24 bytes long.
    if(blob.size() < 24) {
        qWarning() << "The supplied TCP Dump does not have a valid header length.";
        return 0;
    }

    // Extract the first four bytes.
    for(int i = 0; i < 4; ++i) {
        m_magic_word[i] = blob.at(i);
    }

    // Determine the endianess
    if(m_magic_word.startsWith("\xA1\xB2\xC3\xD4")) {
        // This is a big endian file.
        m_little_endian = false;
    }
    else if(m_magic_word.startsWith("\xD4\xC3\xB2\xA1")) {
        m_little_endian = true;
    }
    else {
        qWarning() << "Invalid TCP Dump header.";
        return 0;
    }

    // Return how many bytes we read for the header.
    return 24;
}


quint32 CTcpDumpData::parsePackets(const QByteArray &blob, quint32 offset)
{
    quint32 blob_size = blob.size();

    while(offset < blob_size) {
        QSharedPointer<CTcpDumpPacket> p(new CTcpDumpPacket());

        p->time = get4Bytes(blob, offset);
        offset += 4;
        p->time += get4Bytes(blob, offset) * 0.000001; // Microseconds
        offset += 4;
        quint32 len1 = get4Bytes(blob, offset);
        offset += 4;
        quint32 len2 = get4Bytes(blob, offset);
        offset += 4;
        if(len1 != len2) {
            qWarning() << "Missmatched length found in TCP packet.";
            return offset;
        }
        if(len1 > 65535) {
            // TODO: handle this somehow.
            qWarning() << "Packet size too large.";
            return offset;
        }

        // Read ethernet part of packet.
        p->data.resize(len1);
        for(quint32 i = 0; i < len1; ++i) {
            quint8 byte;
            if(offset < blob_size) {
                byte = blob.at(offset++);
            }
            else {
                // Error: the file is not long enough.
                qWarning() << "The TCP dump file is not long enough.";
                return offset;
            }
            p->data[i] = byte;
        }

        p->end = p->data.size();

        // Parse the EtherType.
        // Set ip to the offset 14 if this packet is a IPv4 packet.
        if (p->data.size() > 34 &&
            p->get2(12) == 0x800 &&
            (p->get1(14) & 0xf0) == 0x40) {
            p->ip=14;
        }

        // Parse the protocol layer if it's a valid IP packet and it's not
        // ... fragmented.
        if(validIp(p) && defrag(p)) {
            parseIpProtocol(p);
        }

        // Save the packet.
        m_packets.append(p);
    }

    return offset;
}

quint32 CTcpDumpData::get4Bytes(const QByteArray &blob, quint32 offset)
{
    quint32 number = 0;
    quint32 blob_size = blob.size();

    if(offset + 3 >= blob_size) {
        qWarning() << "Binary blob does not have 4 bytes for parsing a number.";
        return 0;
    }

    if(m_little_endian) {
        // Extract a little endian number.
        number = static_cast<unsigned char>(blob.at(offset++));
        number |= static_cast<unsigned char>(blob.at(offset++)) << 8;
        number |= static_cast<unsigned char>(blob.at(offset++)) << 16;
        number |= static_cast<unsigned char>(blob.at(offset)) << 24;
    }
    else {
        // Extract a big endian number.
        number = static_cast<unsigned char>(blob.at(offset++)) << 24;
        number |= static_cast<unsigned char>(blob.at(offset++)) << 16;
        number |= static_cast<unsigned char>(blob.at(offset++)) << 8;
        number |= static_cast<unsigned char>(blob.at(offset));
    }

    return number;
}

void CTcpDumpData::parseIpProtocol(QSharedPointer<CTcpDumpPacket> packet)
{
    qint8 protocol = packet->protocol();

    if(packet->ip != 0) {
        packet->end = packet->ip + packet->iplen();
    }

    // ICMP protocol
    if(protocol == 1) {
        packet->icmp = packet->ip + packet->ipheaderlen();
    }
    // TCP protocol
    else if(protocol == 6) {
        packet->tcp = packet->ip + packet->ipheaderlen();
        packet->appl = packet->tcp + packet->tcpheaderlen();
    }
    // UDP protocol
    else if(protocol == 17) {
        packet->udp = packet->ip + packet->ipheaderlen();
        packet->appl = packet->udp + 8;
    }
}
