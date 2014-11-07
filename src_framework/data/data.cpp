#include "data.h"

//------------------------------------------------------------------------------
// Constructor and Destructor

CData::CData()
{

}

CData::~CData()
{

}


//------------------------------------------------------------------------------
// Public Functions


//------------------------------------------------------------------------------
// Public Slots

QString CData::getType() const
{
    return m_type_name;
}


//------------------------------------------------------------------------------
// Private Functions

void CData::setTypeName(QString type_name)
{
    m_type_name = type_name;
}
