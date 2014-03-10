#ifndef CFRAMEWORK_H
#define CFRAMEWORK_H

#include <QObject>

class CFramework : public QObject
{
  Q_OBJECT

  public:
    explicit CFramework(QObject *parent = 0);

  public slots:
    // The framework's main funcion.
    void main();
};

#endif // CFRAMEWORK_H
