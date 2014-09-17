#ifndef TCPDUMPPACKET_H
#define TCPDUMPPACKET_H

#include <QVector>

class CTcpDumpPacket
{
  public:
    // Seconds since 1970
    double time;
    // Ethernet header
    QVector<quint8> data;
    // Offsets in "data" pointing to specific parts in the packet.
    // ip - position in "data" where the IP header starts
    // tcp - position in "data" where the TCP header starts
    // appl - position where application data begins
    // end - offset at the end of the packet
    qint32 ip, tcp, udp, icmp, appl, end;

    CTcpDumpPacket(): time(0), ip(0), tcp(0), udp(0), icmp(0), appl(0), end(0) {}

    // Get numeric fields of 1, 2, or 4 bytes
    quint8 get1(qint32 offset) const {return data[offset];}
    quint16 get2(qint32 offset) const {return (data[offset]<<8)+data[offset+1];}
    quint32 get4(qint32 offset) const {return (get2(offset)<<16)+get2(offset+2);}

    // Get IP fields by name
    quint16 fragoffset() const {return ip ? get2(ip+6)&0x1fff : 0;}
    bool fragfollows() const {return ip ? (get1(ip+6)>>5)&1 : 0;}
    quint16 fragid() const {return ip ? get2(ip+4) : 0;}
    quint16 iplen() const {return ip ? get2(ip+2) : 0;}
    quint16 ipheaderlen() const {return ip ? (get1(ip)&15)*4 : 0;}
    quint16 ipdatalen() const {return iplen()-ipheaderlen();}
    quint32 src() const {return ip ? get4(ip+12) : 0;}
    quint32 dest() const {return ip ? get4(ip+16) : 0;}
    quint8 protocol() const {return ip ? get1(ip+9) : 0;}

    // TCP/UDP fields
    quint16 src_port() const {return tcp ? get2(tcp) : udp ? get2(udp) : 0;}
    quint16 dest_port() const {return tcp ? get2(tcp+2) : udp ? get2(udp+2) : 0;}
    quint8 tcpheaderlen() const {return tcp ? (get1(tcp+12)>>4)*4 : 0;}
    quint32 tcpseq() const {return tcp ? get4(tcp+4) : 0;}
    quint32 tcpack() const {return tcp ? get4(tcp+8) : 0;}
    quint8 tcpflags() const {return get1(tcp+13);}
    bool ack() const {return tcp && (get1(tcp+13)&16);}
    bool rst() const {return tcp && (get1(tcp+13)&4);}
    bool syn() const {return tcp && (get1(tcp+13)&2);}
    bool fin() const {return tcp && (get1(tcp+13)&1);}
};

#endif // TCPDUMPPACKET_H
