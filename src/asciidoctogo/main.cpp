/*****************************************************************************
* main.cpp
*------------------------------------------------------------------------*//*!
* \brief  Application entry point for the AsciidocToGo GUI
* \author Carsten Breuer
*
* This is the main dialog of asciidoctogo
* Copyright (c) 2014 Carsten Breuer
* LGPL (refer to License.html)
*****************************************************************************/

/************************************
* Includes
************************************/
#include <QApplication>
#include "cmaindialog.h"

/*****************************************************************************
* main
*------------------------------------------------------------------------*//*!
* \brief  Application entry point
* \param  argc Number of arguments
* \param  argv Pointerarray of arguments strings
*****************************************************************************/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Carsten Breuer");
    a.setOrganizationDomain("dbcb.github.io");
    a.setApplicationName("AsciidocToGo");
    QStringList args = a.arguments();
    CMainDialog w(args);
    w.show();

    return a.exec();
}
