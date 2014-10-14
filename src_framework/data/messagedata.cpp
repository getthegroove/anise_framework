#include "messagedata.h"


//------------------------------------------------------------------------------
// Static Functions

CData *CMessageData::maker()
{
    return new CMessageData();
}


//------------------------------------------------------------------------------
// Constructor and Destructor

CMessageData::CMessageData()
    : CData()
{

}


//------------------------------------------------------------------------------
// Public Functions

void CMessageData::setMessage(QString message)
{
    m_message = message;
}

QString CMessageData::getMessage() const
{
    return m_message;
}


//------------------------------------------------------------------------------
// Private Functions
