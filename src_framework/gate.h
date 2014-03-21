#ifndef GATE_H
#define GATE_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QQueue>

class CGate : public QObject
{
  Q_OBJECT

  public:
    explicit CGate(QString name_id, QString msg_type, QObject *parent = 0);
    inline bool operator==(const CGate &gate) const;
    inline bool operator==(const QString gate_name) const;
    inline QString name() const;
    inline QString type() const;
    // Set the slot of a receiver as the callback function that will
    // ... be sent data objects sent by this gate.
    bool link(QObject *receiver, const char* slot);
    // Connect two gates together.
    bool link(QSharedPointer<CGate> gate);

  private:
    // String identifier preassigned to this gate.
    QString m_name_id;
    // The type of message this gate is expected to receive.
    QString m_msg_type;
    // Queue of messages waiting to be processed.

  public slots:
    // Receive data structures here. If the data received is what we are
    // ... expecting, forward it to the node for processing.
    void inputData();

  signals:
    // Signal emitted whenever a data object is to be forwarded to the
    // ... object that owns this gate.
    void processData();
};

// Inline functions.
bool CGate::operator==(const CGate &gate) const
{
    return this->m_name_id == gate.m_name_id;
}

bool CGate::operator==(const QString gate_name) const
{
    return this->m_name_id == gate_name;
}

QString CGate::name() const
{
    return m_name_id;
}

QString CGate::type() const
{
    return m_msg_type;
}

#endif // GATE_H
