/*****************************************************************************
* cmanindialog.h
*------------------------------------------------------------------------*//*!
* \brief  The main dialog of AsciidocToGo
* \author Carsten Breuer
*
* This is the main dialog of asciidoctogo
* Copyright (c) 2014 Carsten Breuer
* LGPL (refer to License.html)
*****************************************************************************/
#ifndef CMAINDIALOG_H
#define CMAINDIALOG_H

/************************************
* Includes
************************************/
#include <QDialog>
#include <QProcessEnvironment>
#include <QMap>
#include <QDir>
#include <QUrl>

/************************************
* Types and Enums
************************************/
namespace Ui {
    class CMainDialog;
}

enum eVIEWER_TYPE
{
    vtNONE,
    vtBROWSER,
    vtPDF
};

/*****************************************************************************
* CMainDialog
*------------------------------------------------------------------------*//*!
* \brief  For more information refer to cpp file
*****************************************************************************/
class CMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMainDialog(QStringList& args, QWidget *parent = 0);
    ~CMainDialog();
public Q_SLOTS:
    void anchorClicked(const QUrl & link);
protected:
    bool SetupPathList();
    bool SetupEnvironment();
    QString AdtgPathList2SystemPathList();
    void AddAdtgSearchPath(const QString name, const QString rel_path);
    void UpdateCmdText(void);
    void UpdateA2xCmdText(void);
    void SetOrRemoveInCmd(const QString param, QString& cmd, bool active);
    void ReloadLastSettings(void);
    void SaveLastSettings(void);
    void UpdateButtonStates(bool is_running);
    void UpdateProcessLog();
    void UpdateSrcFileName(const QString& src);
    void ShowDocument(eVIEWER_TYPE type);
    QString GetRegistryTxtHandler();
    void timerEvent (QTimerEvent *p_event);
    void setLogProcessCtrlValue(int value);
protected: // Data
    QDir        m_base_path;
    QMap<QString,QString> m_path_list;
    QProcessEnvironment m_env;
    QProcess     m_process;
    uint         m_timerId;
    QStringList  m_cmd_line_args;
    eVIEWER_TYPE m_viewer_type;
private slots:
    void on_pb_cancel_clicked();
    void on_pb_run_clicked();
    void on_pb_select_source_clicked();
    void on_cb_verbose_clicked();
    void on_cb_data_uri_clicked();
    void on_cb_use_icons_clicked();
    void on_cb_add_toc_clicked();
    void on_pb_close_clicked();
    void on_pb_abort_clicked();
    void on_pb_select_source_a2x_clicked();
    void on_cb_verbose_a2x_clicked();
    void on_pb_run_a2x_clicked();
    void on_pb_create_registry_cm_entry_clicked();
    void on_pb_cancel_a2x_clicked();


private:
    Ui::CMainDialog *ui;
};

#endif // CMAINDIALOG_H
