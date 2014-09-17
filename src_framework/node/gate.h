#ifndef GATE_H
#define GATE_H

#include "data/data.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QQueue>

class CNode;

class CGate: public QObject
{
  Q_OBJECT

  private:
    // String identifier assigned to this gate.
    QString m_name;
    // The type of message this gate is expected to receive.
    QString m_msg_type;
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
    bool link(CNode *receiver, const char* slot);
    // Connect two gates together.
    bool link(QSharedPointer<CGate> gate);

  public slots:
    // Data structures are received here.
    void inputData(QSharedPointer<CData> data);

  signals:
    // Signal emitted whenever a data object is to be forwarded to the
    // ... object that owns this gate.
    void forwardData(QSharedPointer<CData> data);
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
