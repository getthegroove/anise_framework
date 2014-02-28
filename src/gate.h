#ifndef GATE_H
#define GATE_H

#include <QObject>
#include <QString>

class CGate : public QObject
{
  Q_OBJECT

  public:
    explicit CGate(QString name_id, QObject *parent = 0);

  private:
    // String identifier preassigned to this gate.
    QString m_name_id;

};

#endif // GATE_H
