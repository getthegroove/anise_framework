#ifndef CFRAMEWORK_H
#define CFRAMEWORK_H

#include "nodemesh.h"
#include <QObject>

class CFramework : public QObject
{
  Q_OBJECT

  public:
    CNodeMesh m_mesh;

  public:
    explicit CFramework(QObject *parent = 0);

  public slots:
    // The framework's main funcion.
    void main();
};

#endif // CFRAMEWORK_H
