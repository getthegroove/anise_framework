#ifndef ERRORDATA_H
#define ERRORDATA_H

#include "data.h"
#include <QList>
#include <QString>

class CErrorData: public CData
{
private:
    QString m_message;

public:
    explicit CErrorData();
    // Create an instance of this class.
    static CData *maker();

    // Set the error message.
    void setMessage(QString message);
    QString getMessage() const;
};

#endif // ERRORDATA_H
