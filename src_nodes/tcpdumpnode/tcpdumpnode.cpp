#include "tcpdumpnode.h"
#include "data/datafactory.h"
#include <QDebug>
#include <QFile>

//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpDumpNode::CTcpDumpNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
    , m_tcpdump(nullptr)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CTcpDumpNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "TCP Dump file", "TCP dump file to be read from disk.");

    // Add inputs and outputs
    config.addInput("in", "misc");
    config.addOutput("out", "tcpdump");
}

void CTcpDumpNode::data(QSharedPointer<CData> data)
{
    // Empty because data is not received by this node.
    Q_UNUSED(data);
}


//------------------------------------------------------------------------------
// Protected Functions

void CTcpDumpNode::init(const CDataFactory &data_factory)
{
    qDebug() << "CTcpDumpNode.init() Info: Init Called.";

    // Create the TCP Dump data structure.
    CTcpDumpData *tcpdump = static_cast<CTcpDumpData *>(data_factory.createData("tcpdump"));
    m_tcpdump = QSharedPointer<CTcpDumpData>(tcpdump);
}

void CTcpDumpNode::start()
{
    if(inputLinkCount("in") != 0) {
        // If someone is connected do not perform anything at the start.
        return;
    }

    // TODO: perform error checking.
    QVariant filename = getConfig().getParameter("file")->value;
    qDebug() << filename;
    QFile file(filename.toString());
    if(!file.open(QIODevice::ReadOnly)) {
        // Error opening the file.
        commitError("out", "File not found.");
        return;
    }
    QByteArray blob = file.readAll();
    m_tcpdump->parse(blob);

    // Show how many packets were parsed.
    qDebug() << "CTcpDumpNode.start() Info: Packets parsed: "
             << m_tcpdump->availablePackets();

    // Commit the file to the "out" gate.
    commit("out", m_tcpdump);
}
