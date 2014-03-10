#ifndef GATE_H
#define GATE_H

#include "message.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QQueue>

class CGate : public QObject
{
  Q_OBJECT

  public:
    explicit CGate(QString name_id, QString msg_type, QObject *parent = 0);
    inline bool operator==(const CGate &gate);

  private:
    // String identifier preassigned to this gate.
    QString m_name_id;
    // The type of message this gate is expected to receive.
    QString m_msg_type;
    // Queue of messages waiting to be processed.
    QQueue<QSharedPointer<CMessage>> m_message_queue;

  public slots:
    // TODO: replace QString with the Message Base class.
    void processMessage(QString msg_type);
};

// Inline functions.
bool CGate::operator==(const CGate &gate)
{
    return this->m_name_id == gate.m_name_id;
}

#endif // GATE_H
