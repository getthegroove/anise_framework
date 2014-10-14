#ifndef NODESTARTTASK_H
#define NODESTARTTASK_H

#include "node.h"
#include <QRunnable>
#include <QObject>

#include <QObject>

class CNodeStartTask : public QObject, public QRunnable
{
    Q_OBJECT

    private:
      CNode &m_node;

    public:
      explicit CNodeStartTask(CNode &node, QObject *parent = 0);
      // Call the start() function of m_node.
      virtual void run();

    signals:
      void taskFinished(bool success);
};

#endif // NODESTARTTASK_H
