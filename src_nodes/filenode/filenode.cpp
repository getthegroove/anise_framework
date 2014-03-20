#include "filenode.h"
#include "datafactory.h"
#include "tabledata/tabledata.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CFileNode::CFileNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
    , CNodeInfo<CFileNode>(config)
    , m_table(nullptr)
{

}


//------------------------------------------------------------------------------
// Public Functions

const char *CFileNode::name()
{
    return "File";
}

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "Input File", "File to be read from disk.");

    // Add inputs and outputs
    config.addInput("Start", "start_data");
    config.addOutput("File Table", "table_data");
}

void CFileNode::process()
{
    qDebug() << "Hello from the deep space of the dynamic library loading thing.";
}

//------------------------------------------------------------------------------
// Protected Functions

void CFileNode::init(const CDataFactory &data_factory)
{
    m_table = static_cast<CTableData *>(data_factory.createData("Table"));
}


