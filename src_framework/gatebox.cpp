#include "gatebox.h"


//------------------------------------------------------------------------------
// Constructor and Destructor

CGateBox::CGateBox(bool sync/* = true*/, QObject *parent)
  : QObject(parent)
  , m_sync(sync)
  , m_gates()
{

}


//------------------------------------------------------------------------------
// Public Functions

void CGateBox::addGate(QSharedPointer<CGate> gate)
{
    m_gates.append(gate);
}


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots


