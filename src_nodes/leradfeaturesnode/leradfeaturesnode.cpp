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


//------------------------------------------------------------------------------
// Protected Functions

void CLeradFeaturesNode::data(QString gate_name, const CConstDataPointer &data)
{
    Q_UNUSED(gate_name);
    qDebug() << getConfig().getName()
             << "Data received.";

    // Receive TCP Dump data and extract the LERAD features from it.
    QSharedPointer<const CTcpDumpData> tcpdumpdata = data.staticCast<const CTcpDumpData>();
}

//void CLeradFeaturesNode::init(const CDataFactory &data_factory)
//{
//    qDebug() << getConfig().getName()
//             << "Init called.";
//
//    // Create the LERAD Features data structure.
//    CLeradFeaturesData *leradfe = static_cast<CLeradFeaturesData *>(data_factory.createData("leradfeatures"));
//    m_leradfeatures = QSharedPointer<CLeradFeaturesData>(leradfe);
//}

bool CLeradFeaturesNode::start()
{
    qDebug() << getConfig().getName()
             << "Start called.";

    // if(inputLinkCount("in") != 0) {
    //     // If someone is connected do not perform anything at the start.
    //     return;
    // }

    // Commit data to a gate.
    //commit("out", m_table);

    return true;
}
