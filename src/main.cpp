#include "framework.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CFramework framework(&app);
    QMetaObject::invokeMethod(&framework, "main", Qt::QueuedConnection);

    return app.exec();
}
