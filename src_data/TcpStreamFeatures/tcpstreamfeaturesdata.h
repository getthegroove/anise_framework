#ifndef TCPSTREAMFEATURESDATA_H
#define TCPSTREAMFEATURESDATA_H

#include "data/data.h"

class CTcpStreamFeaturesData: public CData
{
  public:
    explicit CTcpStreamFeaturesData();
    virtual CDataPointer clone() const;
};

#endif // TCPSTREAMFEATURESDATA_H

