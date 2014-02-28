#include "filenode.h"

//------------------------------------------------------------------------------
// Constructor and Destructor

CFileNode::CFileNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CFileNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addFilename("file", "Input File", "File to be read from disk.");

    // Add the gates.
    int inputbox = config.addInputBox();
    config.addInputGate(inputbox, "Start", "start_msg");

    int outputbox = config.addOutputBox();
    config.addOutputGate(outputbox, "File Table", "table_msg");
}


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Protected Functions

void CFileNode::process()
{

}


//------------------------------------------------------------------------------
// Private Slots
