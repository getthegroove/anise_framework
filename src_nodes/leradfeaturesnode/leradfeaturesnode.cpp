#include "leradfeaturesnode.h"
#include "data/datafactory.h"
#include "tcpdumpdata/tcpdumpdata.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CLeradFeaturesNode::CLeradFeaturesNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CLeradFeaturesNode::configure(CNodeConfig &config)
{
    // Add parameters
    //config.addFilename("file", "Input File", "File to be read from disk.");

    // Add the gates.
    config.addInput("inTcpDump", "tcpdump");
    config.addOutput("outFeatures", "leradfeatures");
}

void CLeradFeaturesNode::data(QSharedPointer<CData> data)
{
    qDebug() << "CLeradFeaturesNode.data():: Info:"  << getConfig().getName()
             << ": Data received.";

    // Receive TCP Dump data and extract the LERAD features from it.
    QSharedPointer<CTcpDumpData> tcpdumpdata = data.staticCast<CTcpDumpData>();
}


//------------------------------------------------------------------------------
// Protected Functions

void CLeradFeaturesNode::init(const CDataFactory &data_factory)
{
    qDebug() << "CLeradFeaturesNode.init():: Info:" << getConfig().getName()
             << ": Init called.";

    // Create the LERAD Features data structure.
    CLeradFeaturesData *leradfe = static_cast<CLeradFeaturesData *>(data_factory.createData("leradfeatures"));
    m_leradfeatures = QSharedPointer<CLeradFeaturesData>(leradfe);
}

void CLeradFeaturesNode::start()
{
    qDebug() << "CLeradFeaturesNode.start():: Info:" << getConfig().getName()
             << ": Start called.";

    // if(inputLinkCount("in") != 0) {
    //     // If someone is connected do not perform anything at the start.
    //     return;
    // }

    // Commit data to a gate.
    //commit("out", m_table);
}
