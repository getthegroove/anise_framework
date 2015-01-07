#include "messagehandler.h"
#include <iostream>
#include <QTextStream>
#include <QTime>

#define COLOR_DEBUG "\033[32;1m"
#define COLOR_WARN "\033[33;1m"
#define COLOR_CRITICAL "\033[31;1m"
#define COLOR_FATAL "\033[33;1m"
#define COLOR_RESET "\033[0m"

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"


void humanMessageWriterQt4(QtMsgType type, const char *msg)
{
    QTextStream err(stderr);

    switch (type) {
        case QtDebugMsg:
            err << QTime::currentTime().toString("hh:mm:ss.zzz")
                << BLUE   << "     Info: " << COLOR_RESET
                << msg << endl;
            break;
        case QtWarningMsg:
            err << QTime::currentTime().toString("hh:mm:ss.zzz")
                << YELLOW << "  Warning: " << COLOR_RESET
                << msg << endl;
            break;
        case QtCriticalMsg:
            err << QTime::currentTime().toString("hh:mm:ss.zzz")
                << RED    << " Critical: " << COLOR_RESET
                << msg << endl;
            break;
        case QtFatalMsg:
            err << QTime::currentTime().toString("hh:mm:ss.zzz")
                << RED    << "    FATAL: " << COLOR_RESET
                << msg << endl;
            break;
    }
}

void humanMessageWriterQt5(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QTextStream out(stdout);
    QTextStream err(stderr);

    switch (type) {
        case QtDebugMsg:
            out << QTime::currentTime().toString("hh:mm:ss.zzz")
                << BLUE   << "     Info: " << COLOR_RESET
                //<< context.function << "::" << context.line << ":: "
                << msg << endl;
            break;
        case QtWarningMsg:
            err << QTime::currentTime().toString("hh:mm:ss.zzz")
                << YELLOW << "  Warning: " << COLOR_RESET
                //<< context.function << "::" << context.line << ":: "
                << msg << endl;
            break;
        case QtCriticalMsg:
            err << QTime::currentTime().toString("hh:mm:ss.zzz")
                << RED    << " Critical: " << COLOR_RESET
                //<< context.function << "::" << context.line << ":: "
                << msg << endl;
            break;
        case QtFatalMsg:
            err << QTime::currentTime().toString("hh:mm:ss.zzz")
                << RED    << "    FATAL: " << COLOR_RESET
                //<< context.function << "::" << context.line << ":: "
                << msg << endl;
            break;
    }
}

void machineMessageWriterQt5(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QTextStream out(stdout);
    QTextStream err(stderr);

    switch (type) {
        case QtDebugMsg:
          // Do not print debug messages unless the message starts with a '@'.
          if(msg.at(0) == '@') {
              // Chop the '@' sign before printing.
              out << msg.mid(1) << endl;
          }
          break;
      default:
        // Print everything else that is not a debug message.
        err << msg << endl;
    }
}
