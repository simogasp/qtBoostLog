TEMPLATE = app
TARGET   = Logger
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

CONFIG   += c++11

QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK

SOURCES  += main.cpp \
    heater.cpp \
    logExample.cpp \
    temperaturelog.cpp \
    eventlog.cpp

HEADERS  += \
    heater.h \
    logExample.h \
    temperaturelog.h \
    eventlog.h

FORMS    += \
    logExample.ui

unix:!macx: LIBS += -lboost_log -lboost_thread -lboost_date_time -lboost_system -lboost_filesystem -lpthread
