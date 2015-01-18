#ifndef TCPSTREAMSDATA_H
#define TCPSTREAMSDATA_H

#include "tcpstream.h"
#include "data/data.h"
#include "tcpdumpdata/tcpdumppacket.h"
#include <QMap>


class CTcpKey
{
  public:
    quint32 source_addr;
    quint32 destination_addr;
    quint16 source_port;
    quint16 destination_port;

    CTcpKey(): source_addr(0), destination_addr(0),
               source_port(0), destination_port(0) {}

    CTcpKey(const QSharedPointer<const CTcpDumpPacket> &packet):
        source_addr(packet->src()),
        destination_addr(packet->dest()),
        source_port(packet->src_port()),
        destination_port(packet->dest_port()) {}

    bool operator <(const CTcpKey& tcp_key) const
    {
        if(source_addr != tcp_key.source_addr) {
          return source_addr < tcp_key.source_addr;
        }
        else if(destination_addr != tcp_key.destination_addr) {
          return destination_addr < tcp_key.destination_addr;
        }
        else if(source_port != tcp_key.source_port) {
          return source_port < tcp_key.source_port;
        }

        return destination_port < tcp_key.destination_port;
    }
};


class CTcpStreamsData: public CData
{
  private:
    QMap<CTcpKey, CTcpStream> m_tcp_open_streams;
    QMap<CTcpKey, CTcpStream> m_tcp_closed_streams;
    quint32 m_max_payload_size;

  public:
    explicit CTcpStreamsData();
    virtual CDataPointer clone() const;
    void setMaxPayloadSize(quint32 size) { m_max_payload_size = size; }

    // Add a TCP packet to a new or existing TCPStream.
    void addTcpPacket(const QSharedPointer<const CTcpDumpPacket> &tcp_packet);
};

#endif // TCPSTREAMSDATA_H

