#include "tcpstreamfeaturesdata.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpStreamFeaturesData::CTcpStreamFeaturesData()
    : CData()
{

}


//------------------------------------------------------------------------------
// Public Functions

CDataPointer CTcpStreamFeaturesData::clone() const
{
    qWarning() << "Cloning not implemented.";
    return CDataPointer();
}


//------------------------------------------------------------------------------
// Private Functions

