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
    quint32 seq; // The initial TCP sequence number.
    double start_time, finish_time; // Time of first and last packet
    quint8 first_flags, second_to_last_flags, last_flags;

  public:
    CTcpStream(): source_addr(0), destination_addr(0),
                  source_port(0), destination_port(0),
                  seq(0), start_time(0), finish_time(0), first_flags(0),
                  second_to_last_flags(0), last_flags(0) {}

    bool isNull() const
    {
        return start_time == 0 ? true : false;
    }

    void init(const QSharedPointer<const CTcpDumpPacket> &tcp_packet)
    {
        start_time = tcp_packet->time;
        first_flags = tcp_packet->tcpflags();
        seq = tcp_packet->tcpseq();
    }

    void update(const QSharedPointer<const CTcpDumpPacket> &tcp_packet)
    {
        finish_time = tcp_packet->time;
        // Update the TCP flags.
        second_to_last_flags = last_flags;
        last_flags = tcp_packet->tcpflags();
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
        qCopy(begin,
              end,
              result);
    }
};

#endif // TCPSTREAM_H
