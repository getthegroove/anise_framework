#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include "data.h"
#include <QList>
#include <QString>

class CMessageData : public CData
{
  private:
    QString m_message;

  public:
    CMessageData();
    // Create an instance of this class.
    static CData *maker();
    virtual CDataPointer clone() const { return CDataPointer(); }

    // Set the error message.
    void setMessage(QString message);
    QString getMessage() const;
};

#endif // MESSAGEDATA_H
