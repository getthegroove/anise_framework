#include "errordata.h"

//------------------------------------------------------------------------------
// Static Functions

CData *CErrorData::maker()
{
    return new CErrorData();
}


//------------------------------------------------------------------------------
// Constructor and Destructor

CErrorData::CErrorData()
    : CData()
{

}


//------------------------------------------------------------------------------
// Public Functions

void CErrorData::setMessage(QString message)
{
    m_message = message;
}

QString CErrorData::getMessage() const
{
    return m_message;
}


//------------------------------------------------------------------------------
// Private Functions
