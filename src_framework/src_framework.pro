QT       += core
QT       -= gui

TARGET = anids-framework
CONFIG += console
CONFIG -= app_bundle
QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

LIBS += -ldl -rdynamic

#FORMS     += forms/mainwindow.ui
#RESOURCES += resources/images.qrc

CONFIG(debug,debug|release) {
  # Debug...
  DESTDIR = ../bin/debug
  OBJECTS_DIR = build/debug
  UI_DIR = build/debug/forms
  UI_HEADERS_DIR = forms
  UI_SOURCES_DIR = forms
  MOC_DIR = build/debug/moc
  RCC_DIR = build/debug/rcc
} else {
  # Release...
  DESTDIR = ../bin/release
  OBJECTS_DIR = build/release
  UI_DIR = build/release/forms
  UI_HEADERS_DIR = forms
  UI_SOURCES_DIR = forms
  MOC_DIR = build/release/moc
  RCC_DIR = build/release/rcc
  DEFINES += QT_NO_DEBUG_OUTPUT
  #QMAKE_CXXFLAGS_RELEASE += -O2
}

QMAKE_CLEAN += $$DESTDIR/*$$TARGET*

SOURCES += main.cpp \
    framework.cpp \
    node.cpp \
    nodeconfig.cpp \
    gate.cpp \
    nodefactory.cpp \
    dynamicfactory.cpp \
    datafactory.cpp \
    data.cpp

HEADERS += \
    framework.h \
    node.h \
    nodeconfig.h \
    gate.h \
    nodeinfo.h \
    nodefactory.h \
    dynamicfactory.h \
    datafactory.h \
    data.h \
    datainfo.h
