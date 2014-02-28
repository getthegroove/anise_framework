QT       += core

QT       -= gui

TARGET = anids-framework
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

#FORMS     += forms/mainwindow.ui
#RESOURCES += resources/images.qrc

CONFIG(debug,debug|release) {
  # Debug...
  DESTDIR = bin/debug
  OBJECTS_DIR = build/debug
  UI_DIR = build/debug/forms
  UI_HEADERS_DIR = src/forms
  UI_SOURCES_DIR = src/forms
  MOC_DIR = build/debug/moc
  RCC_DIR = build/debug/rcc
} else {
  # Release...
  DESTDIR = bin/release
  OBJECTS_DIR = build/release
  UI_DIR = build/release/forms
  UI_HEADERS_DIR = src/forms
  UI_SOURCES_DIR = src/forms
  MOC_DIR = build/release/moc
  RCC_DIR = build/release/rcc
  DEFINES += QT_NO_DEBUG_OUTPUT
  #QMAKE_CXXFLAGS_RELEASE += -O2
}


SOURCES += src/main.cpp \
    src/framework.cpp \
    src/node.cpp \
    src/message.cpp \
    src/nodeconfig.cpp \
    src/gatebox.cpp \
    src/gate.cpp \
    src/nodes/filenode.cpp \
    src/nodes/splitnode.cpp

HEADERS += \
    src/framework.h \
    src/node.h \
    src/message.h \
    src/nodeconfig.h \
    src/gatebox.h \
    src/gate.h \
    src/nodes/filenode.h \
    src/nodeinfo.h \
    src/nodes/splitnode.h
