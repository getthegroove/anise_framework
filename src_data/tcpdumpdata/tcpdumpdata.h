#ifndef TCPDUMPDATA_H
#define TCPDUMPDATA_H

#include "tcpdumppacket.h"
#include "data/data.h"
#include <QList>
#include <QByteArray>
#include <QSharedPointer>

class CTcpDumpData: public CData
{
  private:
    // Structures and variables.
    //QList<QList<int>> m_table;
    QByteArray m_magic_word;
    bool m_little_endian;
    QList<QSharedPointer<CTcpDumpPacket>> m_packets;

  public:
    explicit CTcpDumpData();
    virtual ~CTcpDumpData();
    // Parse a byte array into several packets. Extract the tcp dump magic word too.
    bool parse(QByteArray blob);
    // How many packets are available.
    quint32 availablePackets();
    // If the IP checksum of the specified packet is correct, or if the packet
    // ... is not an IP packet return true.
    bool validIp(QSharedPointer<CTcpDumpPacket> packet);
    // Return true if the IP packet is not fragmented.
    bool defrag(QSharedPointer<CTcpDumpPacket> packet);

  private:
    quint32 parseHeader(const QByteArray &blob);
    // Parse a TCP packet and return how many bytes were used in the parsing.
    quint32 parsePackets(const QByteArray &blob, quint32 offset);
    // Return 4 bytes as a single number taking into account endianess.
    quint32 get4Bytes(const QByteArray &blob, quint32 offset);
    // Parse the protocol layer.
    void parseProtocol(QSharedPointer<CTcpDumpPacket> packet);
};

#endif // TCPDUMPDATA_H
