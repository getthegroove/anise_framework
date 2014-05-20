#include "filenode.h"
#include "datafactory.h"
#include "tabledata/tabledata.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CFileNode::CFileNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
    , m_table(nullptr)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "Input File", "File to be read from disk.");

    // Add inputs and outputs
    config.addInput("in", "misc");
    config.addOutput("out", "misc");
}

void CFileNode::data(QSharedPointer<CData> data)
{
    // Empty because data is not received by this node.
    qDebug() << "CFileNode.data() Info:"  << getConfig().getName()
             << ": Data received.";

    QSharedPointer<CTableData> table_data = data.staticCast<CTableData>();
    qDebug() << "CFileNode.data() Data:" << table_data->getRow(0);
}

//------------------------------------------------------------------------------
// Protected Functions

void CFileNode::init(const CDataFactory &data_factory)
{
    qDebug() << "CFileNode.init() Info: Init Called." << endl;
    CTableData *table = static_cast<CTableData *>(data_factory.createData("table"));
    m_table = QSharedPointer<CTableData>(table);
}

void CFileNode::start()
{
    // TODO: Read file supplied by the user.

    if(inputLinkCount("in") != 0) {
        // If someone is connected do not perform anything at the start.
        return;
    }

    QList<int> list;
    list << 1 << 2 << 3;
    m_table->addRow(list);
    qDebug() << "CFileNode.start() Info:"  << getConfig().getName()
             << "Data processing done" << endl;

    // Commit the file to the "out" gate.
    commit("out", m_table);
}

