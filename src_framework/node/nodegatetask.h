#ifndef NODETASK_H
#define NODETASK_H

#include "node.h"
#include <QRunnable>
#include <QObject>

class CNodeGateTask: public QObject, public QRunnable
{
  Q_OBJECT

  private:
    CNode &m_node;
    QString m_gate_name;
    QSharedPointer<CData> m_data;

  public:
    explicit CNodeGateTask(CNode &node, QString gate_name,
        QSharedPointer<CData> &data, QObject *parent = 0);
    // Execute the data processing facility of m_node.
    virtual void run();

  signals:
    void taskFinished();
};

#endif // NODETASK_H
