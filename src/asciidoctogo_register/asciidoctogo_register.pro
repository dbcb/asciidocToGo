#-------------------------------------------------
#
# Project created by QtCreator 2014-09-12T13:21:02
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = asciidoctogo_register
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

RC_FILE = asciidoctogo_register.rc

OTHER_FILES += \
    asciidoctogo_register.exe.manifest \
    asciidoctogo_register.rc

CONFIG(debug)
{
    BUILDDIR    = ./../../build/debug
    DESTDIR     = ./../../build/debug
    OUTDIR      = ./../../build/debug
    OBJECTS_DIR = ./../../build/debug/asciidoctogo_register_temp
    MOC_DIR     = ./../../build/debug/asciidoctogo_register_temp
    RCC_DIR     = ./../../build/debug/asciidoctogo_register_temp
    UI_DIR      = ./../../build/debug/asciidoctogo_register_temp
}

CONFIG(release)
{
    BUILDDIR    = ./../../build/release
    DESTDIR     = ./../../build/release
    OUTDIR      = ./../../build/release
    OBJECTS_DIR = ./../../build/release/asciidoctogo_register_temp
    MOC_DIR     = ./../../build/release/asciidoctogo_register_temp
    RCC_DIR     = ./../../build/release/asciidoctogo_register_temp
    UI_DIR      = ./../../build/release/asciidoctogo_register_temp
}

