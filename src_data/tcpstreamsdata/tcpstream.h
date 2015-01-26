#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include "tcpdumpdata/tcpdumppacket.h"
#include <algorithm>
#include <QtGlobal>
#include <QVector>
#include <QSharedPointer>


class CTcpStream
{
  public:
    quint32 source_addr;
    quint32 destination_addr;
    quint16 source_port;
    quint16 destination_port;
    QVector<quint8> data;
    quint32 data_size;
    quint32 seq; // The initial TCP sequence number.
    double start_time, finish_time; // Time of first and last packet
    quint8 flags_first, flags_before_last, flags_last;

  public:
    CTcpStream(): source_addr(0), destination_addr(0),
                  source_port(0), destination_port(0),
                  data_size(0),
                  seq(0), start_time(0), finish_time(0), flags_first(0),
                  flags_before_last(0), flags_last(0) {}

    bool isNull() const
    {
        return start_time == 0 ? true : false;
    }

    void init(const QSharedPointer<const CTcpDumpPacket> &tcp_packet)
    {
        source_addr = tcp_packet->src();
        destination_addr = tcp_packet->dest();
        source_port = tcp_packet->src_port();
        destination_port = tcp_packet->dest_port();

        start_time = tcp_packet->time;
        flags_first = tcp_packet->tcpflags();
        seq = tcp_packet->tcpseq();
    }

    void update(const QSharedPointer<const CTcpDumpPacket> &tcp_packet)
    {
        finish_time = tcp_packet->time;
        // Update the TCP flags.
        flags_before_last = flags_last;
        flags_last = tcp_packet->tcpflags();
    }

    void copy(const QSharedPointer<const CTcpDumpPacket> &tcp_packet,
              qint32 offset, qint32 length)
    {
        // Increse the memory allocation of the vector.
        if(data.size() < offset + length) {
            data.resize(offset + length);
        }

        // Copy the contents.
        auto begin = tcp_packet->data.begin() + tcp_packet->appl;
        auto end = tcp_packet->data.begin() + tcp_packet->end;
        auto result = data.begin() + offset;
        qCopy(begin, end, result);
    }
};

#endif // TCPSTREAM_H
