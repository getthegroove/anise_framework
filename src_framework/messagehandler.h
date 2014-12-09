#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QtGlobal>


// QT4 message handler
void customMessageWriterQt4(QtMsgType type, const char *msg);

// QT5 message handler
void customMessageWriterQt5(QtMsgType type, const QMessageLogContext &context, const QString &msg);


#endif // MESSAGEHANDLER_H
