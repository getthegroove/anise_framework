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
    config.addInput("Start", "start_data", SLOT(data()));
    config.addOutput("File Table", "table_data");
}

void CFileNode::process()
{
    qDebug() << "Hello from the deep space of the dynamic library loading thing." << endl;

    QList<int> list;
    list << 1 << 2 << 3;
    m_table->addRow(list);
}


//------------------------------------------------------------------------------
// Public Functions

void CFileNode::data()
{
    qDebug() << "Data processing done" << endl;
}

//------------------------------------------------------------------------------
// Protected Functions

void CFileNode::init(const CDataFactory &data_factory)
{
    qDebug() << "CFileNode.init() Info: Init Called." << endl;
    m_table = static_cast<CTableData *>(data_factory.createData("table"));
}


