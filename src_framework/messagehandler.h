#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QtGlobal>

// QT4 message handler
void humanMessageWriterQt4(QtMsgType type, const char *msg);

// QT5 message handler
void humanMessageWriterQt5(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void machineMessageWriterQt5(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // MESSAGEHANDLER_H
