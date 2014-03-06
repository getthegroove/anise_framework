#ifndef GATEBOX_H
#define GATEBOX_H

#include "gate.h"
#include <QObject>
#include <QSharedPointer>

class CGateBox : public QObject
{
  Q_OBJECT

  private:
    bool m_sync;
    QList<QSharedPointer<CGate>> m_gates;

  public:
    explicit CGateBox(bool sync = true, QObject *parent = 0);
    void addGate(QSharedPointer<CGate> gate);

  signals:

  public slots:

};

#endif // GATEBOX_H
