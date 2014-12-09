#include "framework.h"
#include "data/data.h"
#include "messagehandler.h"
#include <QCoreApplication>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    // Our own custom message writer.
    // QT4:
    // qInstallMsgHandler(customMessageWriterQt4);
    // QT5:
    qInstallMessageHandler(customMessageWriterQt5);

    // Register special types.
    // qRegisterMetaType<CDataSharedPointer>();
    qRegisterMetaType<QSharedPointer<CData>>();

    // Create the application.
    QCoreApplication app(argc, argv);

    CFramework framework(&app);
    QMetaObject::invokeMethod(&framework, "main", Qt::QueuedConnection);

    return app.exec();
}
