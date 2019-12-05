#-------------------------------------------------
#
# Project created by QtCreator 2012-12-28T23:07:06
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = MazeSolver
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mazenode.cpp \
    mazeui.cpp \
    msolver.cpp

HEADERS  += mainwindow.h \
    mazenode.h \
    mazeui.h \
    msolver.h
OTHER_FILES += \
            README.md

FORMS    += mainwindow.ui

