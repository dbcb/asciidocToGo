#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T01:49:57
#
#-------------------------------------------------

QT       += core gui

TARGET = asciidoctogo
TEMPLATE = app
DESTDIR = $$PWD/../../

SOURCES += main.cpp\
        cmaindialog.cpp

HEADERS  += cmaindialog.h

FORMS    += cmaindialog.ui

RESOURCES += \
    asciidoctogo.qrc

#config:win32
#{
#    QMAKE_LFLAGS += -static-libgcc \
#                    -static-libstdc++
#}

CONFIG(debug)
{
    BUILDDIR    = ./../../build/debug
    DESTDIR     = ./../../build/debug
    OUTDIR      = ./../../build/debug
    OBJECTS_DIR = ./../../build/debug/asciidoctogo_temp
    MOC_DIR     = ./../../build/debug/asciidoctogo_temp
    RCC_DIR     = ./../../build/debug/asciidoctogo_temp
    UI_DIR      = ./../../build/debug/asciidoctogo_temp
}

CONFIG(release)
{
    BUILDDIR    = ./../../build/release
    DESTDIR     = ./../../build/release
    OUTDIR      = ./../../build/release
    OBJECTS_DIR = ./../../build/release/asciidoctogo_temp
    MOC_DIR     = ./../../build/release/asciidoctogo_temp
    RCC_DIR     = ./../../build/release/asciidoctogo_temp
    UI_DIR      = ./../../build/release/asciidoctogo_temp
}
