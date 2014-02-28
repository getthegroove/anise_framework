#include "node.h"

//------------------------------------------------------------------------------
// Constructor and Destructor

CNode::CNode(const CNodeConfig &config, QObject *parent/*= 0*/)
 : QObject(parent)
 , m_input_box()
 , m_output_box()
{
    // Create the gates and gate boxes of this node.
    setup(config);
}

CNode::~CNode()
{

}

//------------------------------------------------------------------------------
// Public Functions


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Private Functions

void CNode::setup(const CNodeConfig & config)
{

}


//------------------------------------------------------------------------------
// Private Slots



