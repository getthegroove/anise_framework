#include "tcpstreamextractornode.h"
#include "tcpdumpdata/tcpdumpdata.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpStreamExtractorNode::CTcpStreamExtractorNode(const CNodeConfig &config,
                                                 QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CTcpStreamExtractorNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addUInt("payload_size", "TCP Stream Data Size",
                   "The maximum number of bytes that will be stored for "
                   "the contents of a TCP stream.",
                   102400);

    // Add the gates.
    config.addInput("in", "tcpdump");
    config.addOutput("out", "tcpstreams");
}


//------------------------------------------------------------------------------
// Protected Functions


bool CTcpStreamExtractorNode::start()
{
    // Get some user parameters.
    QVariant payload_size = getConfig().getParameter("payload_size")->value;

    // Create a new Data Structure that will hold all data streams.
    m_tcp_streams = QSharedPointer<CTcpStreamsData>(
                static_cast<CTcpStreamsData *>(createData("tcpstreams")));
    m_tcp_streams->setMaxPayloadSize(payload_size.toUInt());

    if(!m_tcp_streams.isNull()) {
        return true;
    }
    else {
        return false;
    }
}

void CTcpStreamExtractorNode::data(QString gate_name,
                                   const CConstDataPointer &data)
{
    // No need to track gates.
    Q_UNUSED(gate_name);

    if(data->getType() == "message") {
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get the TCP packets.");
            return;
        }
    }
    else if(data->getType() == "tcpdump") {
        auto tcp_dump = data.staticCast<const CTcpDumpData>();
        qint32 packet_count = tcp_dump->availablePackets();
        for(qint32 i = 0; i < packet_count; ++i) {
            m_tcp_streams->addTcpPacket(tcp_dump->getPacket(i));
        }

        commit("out", m_tcp_streams);
        return;
    }
}
