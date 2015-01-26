#include "tcpstreamsdata.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpStreamsData::CTcpStreamsData()
    : CData()
    , m_max_payload_size(0)
{

}


//------------------------------------------------------------------------------
// Public Functions

CDataPointer CTcpStreamsData::clone() const
{
    qCritical() << "Not implemented yet.";
    return CDataPointer();
}

void CTcpStreamsData::addTcpPacket(const QSharedPointer<const CTcpDumpPacket> &tcp_packet)
{
    // Ignore non IP packets.
    if(!tcp_packet->tcp) {
        return;
    }

    // Get the length of the payload.
    qint32 payload_length = tcp_packet->end - tcp_packet->appl;

    // Ignore packages with no payload that are irrelevant to the streams' status.
    if(payload_length == 0 && !tcp_packet->syn() &&
       !tcp_packet->fin() && !tcp_packet->rst()) {
        return;
    }

    // Get the key associated with this TCP stream and get the Stream (if any).
    CTcpKey tcp_key = CTcpKey(tcp_packet);
    CTcpStream &tcp_stream = m_tcp_open_streams[tcp_key];

    // Is the stream new?
    if(tcp_stream.isNull()) {
        // It's new, so initialize it.
        tcp_stream.init(tcp_packet);
    }

    // Update the stream records.
    tcp_stream.update(tcp_packet);

    // Store the payload of the packet into the stream.
    if(payload_length > 0) {
        // Record the payload length.
        tcp_stream.data_size += payload_length;
        // Where to store the payload.
        qint32 offset = tcp_packet->tcpseq() - tcp_stream.seq;
        if(offset >= 0 && offset < static_cast<qint32>(m_max_payload_size)) {
            // Will the payload of the current packet go beyond the limit?
            if(offset + payload_length >= static_cast<qint32>(m_max_payload_size)) {
                // Chop the payload length to fit.
                payload_length = m_max_payload_size - offset;
            }
            // Copy payload_length into the stream data starting from offset.
            tcp_stream.copy(tcp_packet, offset, payload_length);
        }
    }

    // If this was a FIN or RST marked packet, close the stream.
    if(tcp_packet->fin() || tcp_packet->rst()) {
        // Move the stream to the structure of the closed ones.
        m_tcp_closed_streams.insert(tcp_key, m_tcp_open_streams.take(tcp_key));
    }
}


//------------------------------------------------------------------------------
// Private Functions

