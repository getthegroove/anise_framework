#ifndef GATE_H
#define GATE_H

#include "data/data.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QQueue>
#include <QMutex>

class CNode;

class CGate: public QObject
{
  Q_OBJECT

  private:
    // String identifier assigned to this gate.
    QString m_name;
    // The type of message this gate is expected to receive.
    QString m_msg_type;
    // Node that might be linked to this gate.
    CNode *m_linked_node;
    // Gates that we might link against.
    QList<QSharedPointer<CGate>> m_linked_gates;
    // Number of inputs going into this gate.
    int m_input_count;

  public:
    explicit CGate(QString name, QString msg_type, QObject *parent = 0);
    inline bool operator==(const CGate &gate) const;
    inline bool operator==(const QString gate_name) const;
    inline QString name() const;
    inline QString type() const;
    inline int inputLinks() const;
    // Connect this gate with an internal Node function.
    void link(CNode *node);
    // Connect two gates together.
    bool link(QSharedPointer<CGate> &gate);

  public slots:
    // Push or put a data structure into this gate so that the processing unit
    // ... of the node can process the data in a thread.
    void inputData(CConstDataPointer &data);
};


// Inline functions.
bool CGate::operator==(const CGate &gate) const
{
    return this->m_name == gate.m_name;
}

bool CGate::operator==(const QString gate_name) const
{
    return this->m_name == gate_name;
}

QString CGate::name() const
{
    return m_name;
}

QString CGate::type() const
{
    return m_msg_type;
}

int CGate::inputLinks() const
{
    return m_input_count;
}

#endif // GATE_H
