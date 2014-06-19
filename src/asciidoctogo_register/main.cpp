/*****************************************************************************
* main.cpp
*------------------------------------------------------------------------*//*!
* \brief  Application entry point for asciidoctogo_register
* \author Carsten Breuer
*
* This program inserts a registry based entry in the windows explorer
* context menu for txt file. This is placed in a seperate application,
* because i have to be done with administration privileges/rights.
*
* Copyright (c) 2014 Carsten Breuer
* LGPL (refer to License.html)
*****************************************************************************/

/************************************
* Includes
************************************/
#include <QtCore>
#ifdef Q_OS_WIN
#   include "windows.h"
#endif
#include <iostream>

/************************************
* This program only make sense on
* windows.
************************************/
#ifdef Q_OS_WIN

/*****************************************************************************
* GetRegistryTxtHandler
*------------------------------------------------------------------------*//*!
* \brief  Get windows registry shell handler for txt files.
* \return Handlername of txt files.
*****************************************************************************/
QString GetRegistryTxtHandler()
{
    QSettings settings("HKEY_CLASSES_ROOT\\.txt",
                        QSettings::NativeFormat);
    QString txtfile = settings.value("Default", "").toString();
    return txtfile;
}

/*****************************************************************************
* RegisterAsciidocHandler
*------------------------------------------------------------------------*//*!
* \brief  Register a entry in txt shell context menu.
* \param  context_menu_text The text in teh context menu.
* \param  cmd               The cmd to be executed.
* \return True on success, else false.
*****************************************************************************/
bool RegisterAsciidocHandler(const QString& context_menu_text, const QString& cmd)
{
    QString txtfile = GetRegistryTxtHandler();
    if (txtfile.isEmpty())
    {
        printf("Error: Cant found class handler for txt files in registry!");
        return false;
    }

    SECURITY_ATTRIBUTES security_attr = { sizeof(security_attr), NULL, true};
    HKEY parent = 0;

    int result = RegCreateKeyEx( HKEY_LOCAL_MACHINE,
                                 L"SOFTWARE\\Classes\\txtfile\\shell\\AsciidocToGo\\command",
                                 0,
                                 NULL,
                                 REG_OPTION_NON_VOLATILE,
                                 KEY_ALL_ACCESS,
                                 &security_attr,
                                 &parent,
                                 NULL);
    // Refer to error codes: http://msdn.microsoft.com/en-us/library/windows/desktop/ms681381%28v=vs.85%29.aspx
    if (result != ERROR_SUCCESS)
    {
        printf("Error: Cant create registry entry for Asciidoc in txt handler!\nError code %d", result);
        return false;
    }


    QString cmd_copy = cmd;
    cmd_copy.replace("\\", "\\\\");

    result = RegSetValue( parent,
                          L"",
                          REG_SZ,
                          cmd_copy.toStdWString().c_str(),
                          cmd_copy.length());
    RegCloseKey(parent);
    if (result != ERROR_SUCCESS)
    {
        printf("Error: Cant write registry settings for AsciidocToGo handler. Error Code %d\n", result);
        return false;
    }
    printf("AsciidocToGo has successfully registered the conext menu handler for txt files.");
    return true;
}
#endif


/*****************************************************************************
* main
*------------------------------------------------------------------------*//*!
* \brief  Application entry point
* \param  argc Number of arguments
* \param  argv Pointerarray of arguments strings
*****************************************************************************/
#ifdef Q_OS_WIN
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString app_path = QDir::toNativeSeparators(a.applicationDirPath());
    QString app_cmd  = app_path + "\\asciidoctogo.exe" + " %1";
    //app_cmd.replace("\\", "\\\\");
    qDebug() << app_cmd;

    if (! RegisterAsciidocHandler("Convert with AsciidocToGo", app_cmd))
    {
        return -3;
    }
    // User have to do this in a cmd.exe
    //printf ("Press any key to continue...\n");
    //std::cin.get();
    return 0;
}
#else
int main(int argc, char *argv[])
{
    printf("This program registers a shell extension in windows.\n Function is nor supported under this OS\n")
    return -1;
}
#endif

