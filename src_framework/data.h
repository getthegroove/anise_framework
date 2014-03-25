#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QSharedPointer>
#include <QMetaType>

class CData
{
  private:
    QString m_name;

  public:
    CData(QString name);
};

// Register CData objects so that they can be sent through queued connections.
Q_DECLARE_METATYPE(QSharedPointer<CData>)

#endif // DATA_H
