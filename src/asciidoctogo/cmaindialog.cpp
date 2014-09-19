/*****************************************************************************
* cmanindialog.cpp
*------------------------------------------------------------------------*//*!
* \brief  The main dialog of AsciidocToGo
* \author Carsten Breuer
*
* This is the main dialog of asciidoctogo
* Copyright (c) 2014 Carsten Breuer
* LGPL (refer to License.html)
*****************************************************************************/

/************************************
* Includes
************************************/
#include <windows.h>
#include <QtGui>
#include "cmaindialog.h"
#include "ui_cmaindialog.h"

/************************************
* Constants
************************************/
static const  quint8 PROGR_START =  5;
static const  quint8 PROGR_END   = 95;

/************************************
* Variables
************************************/
static quint8 progr_idx = PROGR_START;
static bool   progr_up  = true;

/*****************************************************************************
* CMainDialog::CMainDialog
*****************************************************************************/
CMainDialog::CMainDialog(QStringList& args, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMainDialog)
{
    m_timerId     = 0;
    m_viewer_type = vtNONE;

    //QMessageBox::critical(this, "Info", args.join("|"));
    m_cmd_line_args = args;

    ui->setupUi(this);
    ui->pages->setCurrentIndex(0);
    ui->tabpages->setCurrentIndex(0);
    SetupPathList();
    ui->cmd_edit->setPlainText(QString("%1\\python.exe %2\\asciidoc.py")
                               .arg(m_path_list["python"])
                               .arg(m_path_list["asciidoc"]));
    ui->cmd_edit_a2x->setPlainText(QString("%1\\python.exe %2\\a2x.py")
                               .arg(m_path_list["python"])
                               .arg(m_path_list["asciidoc"]));
    ReloadLastSettings();
    UpdateCmdText();
    UpdateA2xCmdText();
    if (args.count() > 1)
    {
        UpdateSrcFileName(args[1]);
    }
    UpdateButtonStates(false);
    ui->output_theme_panel->setVisible(false);          // Not yet implemented
    ui->adtg_options_group_box->setVisible(false);      // Not yet implemented
    ui->adtg_options_group_box_a2x->setVisible(false);  // Not yet implemented
    connect(ui->license_view,
            SIGNAL(anchorClicked(const QUrl&)),
            SLOT(anchorClicked(const QUrl&)));
}

/*****************************************************************************
* CMainDialog::~CMainDialog
*****************************************************************************/
CMainDialog::~CMainDialog()
{
    delete ui;
}

/*****************************************************************************
* CMainDialog::AddAdtgSearchPath
*------------------------------------------------------------------------*//*!
* \brief  Adds the relative path "rel_apth" to the ADTG-Pathlist.
* \param  name     Alias for the path
* \param  rel_path The path relative to ADTG root directory
*****************************************************************************/
void CMainDialog::AddAdtgSearchPath(const QString name, const QString rel_path)
{
    QString path = QDir::toNativeSeparators(m_base_path.absoluteFilePath(rel_path));
    m_path_list[name] = path;
}

/*****************************************************************************
* CMainDialog::SetupPathList
*------------------------------------------------------------------------*//*!
* \brief  Setups the path of all asciidoc tools (python, asciidoc, ...)
*****************************************************************************/
bool CMainDialog::SetupPathList()
{
    m_base_path.setCurrent(QApplication::applicationDirPath());
    m_path_list.clear();
    AddAdtgSearchPath("python",         "sw\\python\\2.7.6");
    AddAdtgSearchPath("python_scripts", "sw\\python\\2.7.6\\Scripts");
    AddAdtgSearchPath("asciidoc",       "sw\\asciidoc\\8.6.9");
    AddAdtgSearchPath("miktex_base",    "sw\\miktex\\2.9.5105");
    AddAdtgSearchPath("miktex_bin",     "sw\\miktex\\2.9.5105\\miktex\\bin");
    AddAdtgSearchPath("docbook",        "sw\\docbook\\1.78.1\\doc");
    AddAdtgSearchPath("graphviz",       "sw\\graphviz\\2.3.8\\bin");
    AddAdtgSearchPath("highlight",      "sw\\highlight\\3.1.8");
    AddAdtgSearchPath("mathml",         "sw\\LaTeXMathML");
    AddAdtgSearchPath("xslt",           "sw\\libxslt\\1.1.26\\bin");
    AddAdtgSearchPath("lynx",           "sw\\lynx\\2.8.8");
    AddAdtgSearchPath("dblatex",        "sw\\dblatex");
    AddAdtgSearchPath("inkscape",       "sw\\inkscape\\0.48.4-1");

    QStringList missing;
    QStringList keys = m_path_list.keys();
    uint cnt = keys.count();
    for (uint idx = 0; idx < cnt; idx++)
    {
        QString path = m_path_list[keys[idx]];
        QDir dir(path);
        if (! dir.exists())
        {
            missing.append(path);
        }
    }
    if (missing.count() != 0)
    {
        QMessageBox::critical(this,
                              "Missing components",
                              QString(
                                  "The following path is/are invalid:\n\n"
                                  "%1"
                                  "\n\n"
                                  "Make sure that the \"AsciidocToGo Third Party Softwarepackage\" is installed "
                                  "or download it from the project website. Refer to README.html for "
                                  "more information").arg(missing.join("\n")));
        return false;
    }
    return true;
}

/*****************************************************************************
* CMainDialog::SetupPathList
*------------------------------------------------------------------------*//*!
* \brief  Create a string with the too path in DOS %PATH% format.
* \return String with the concatenated string (semikolon separated).
*****************************************************************************/
QString CMainDialog::AdtgPathList2SystemPathList()
{
    QStringList list;
    list.append(m_path_list["python"]);
    list.append(m_path_list["python_scripts"]);
    list.append(m_path_list["asciidoc"]);
    list.append(m_path_list["miktex_bin"]);
    list.append(m_path_list["graphviz"]);
    list.append(m_path_list["highlight"]);
    list.append(m_path_list["xslt"]);
    list.append(m_path_list["lynx"]);
    list.append(m_path_list["docbook"]);
    list.append(m_path_list["dblatex"]);
    list.append(m_path_list["inkscape"]);
    return list.join(";");
}

/*****************************************************************************
* CMainDialog::SetupEnvironment
*------------------------------------------------------------------------*//*!
* \brief  Creates a ADTG environment for QProcess
* \return True if setup is ok, else false.
*****************************************************************************/
bool CMainDialog::SetupEnvironment()
{
    if (! SetupPathList())
    {
        return false;
    }
    m_env = QProcessEnvironment::systemEnvironment();
    // TODO: Sort path
    m_env.insert("PATH", AdtgPathList2SystemPathList() + ";" + m_env.value("PATH"));
    m_env.insert("TEMPDIR", m_env.value("TEMP"));
    m_env.insert("TMP", m_env.value("TEMP"));
    m_env.insert("TMPDIR", m_env.value("TEMP"));
    m_env.insert("GSC", "mgs.exe");
    m_env.insert("HOME", m_env.value("HOMEPATH"));
    // Environment variables for Miktex (copied from miktex command console)
    m_env.insert("MIKTEX_BINDIR", m_path_list["miktex_bin"]);
    QDir miktex(m_path_list["miktex_base"]);
    m_env.insert("MIKTEX_COMMONSTARTUPFILE",
                 QDir::toNativeSeparators(miktex.absoluteFilePath("miktex\\config\\miktexstartup.ini")));
    m_env.insert("MIKTEX_GS_LIB",
                 QDir::toNativeSeparators(miktex.absoluteFilePath("ghostscript\\base") + ";" +
                                          miktex.absoluteFilePath("fonts")));
    m_env.insert("MIKTEX_USERSTARTUPFILE",
                 QDir::toNativeSeparators(miktex.absoluteFilePath("miktex\\config\\miktexstartup.ini")));
    m_env.insert("ADTG_PYTHON_PATH", m_path_list["python"]);
    QStringList elist = m_env.toStringList();
    QFile ofile("C:\\temp\\env.txt");
    ofile.open(QFile::WriteOnly);
    ofile.write(elist.join("\n").toLocal8Bit());
    ofile.close();

    return true;
}

/*****************************************************************************
* CMainDialog::ReloadLastSettings
*------------------------------------------------------------------------*//*!
* \brief  Reloads the last used settings
*****************************************************************************/
void CMainDialog::ReloadLastSettings(void)
{
    QSettings settings;
    ui->cb_verbose->setChecked(settings.value("gui/verbose", QVariant(true)).toBool());
    ui->cb_use_icons->setChecked(settings.value("gui/use_icons", QVariant(true)).toBool());
    ui->cb_data_uri->setChecked(settings.value("gui/data_uri", QVariant(true)).toBool());
    ui->cb_add_toc->setChecked(settings.value("gui/add_toc", QVariant(true)).toBool());
    ui->cb_conv_utf8->setChecked(settings.value("gui/conv_utf8", QVariant(true)).toBool());
    ui->cb_execute_browser->setChecked(settings.value("gui/exec_browser", QVariant(false)).toBool());
    ui->browserpath->setText(settings.value("gui/browser_path", QVariant("")).toString());
    ui->cb_execute_pdf_viewer->setChecked(settings.value("gui/exec_pdf", QVariant(false)).toBool());
    ui->pdfviewerpath->setText(settings.value("gui/pdf_path", QVariant("")).toString());
    ui->outformat->setEditText(settings.value("gui/genformat", QVariant("Html4")).toString());
    ui->theme->setEditText(settings.value("gui/theme", QVariant("Default")).toString());
}

/*****************************************************************************
* CMainDialog::SaveLastSettings
*------------------------------------------------------------------------*//*!
* \brief  Save the last used settings
*****************************************************************************/
void CMainDialog::SaveLastSettings(void)
{
    QSettings settings;
    settings.setValue("gui/verbose",      QVariant(ui->cb_verbose->isChecked()));
    settings.setValue("gui/use_icons",    QVariant(ui->cb_use_icons->isChecked()));
    settings.setValue("gui/data_uri",     QVariant(ui->cb_data_uri->isChecked()));
    settings.setValue("gui/add_toc",      QVariant(ui->cb_add_toc->isChecked()));
    settings.setValue("gui/conv_utf8",    QVariant(ui->cb_conv_utf8->isChecked()));
    settings.setValue("gui/exec_browser", QVariant(ui->cb_execute_browser->isChecked()));
    settings.setValue("gui/exec_pdf",     QVariant(ui->cb_execute_pdf_viewer->isChecked()));
    settings.setValue("gui/browser_path", QVariant(ui->browserpath->text()));
    settings.setValue("gui/pdf_path",     QVariant(ui->pdfviewerpath->text()));
    settings.setValue("gui/genformat",    QVariant(ui->outformat->currentText()));
    settings.setValue("gui/theme",        QVariant(ui->theme->currentText()));
}

/*****************************************************************************
* CMainDialog::UpdateButtonStates
*------------------------------------------------------------------------*//*!
* \brief  Switches the visible buttons if the log is shown
* \param  is_running True if asciidoc conversion is active
*****************************************************************************/
void CMainDialog::UpdateButtonStates(bool is_running)
{
    ui->pb_abort->setVisible(is_running);
    ui->pb_close->setVisible(!is_running);
    if (! is_running)
    {
        progr_idx = PROGR_START;
    }
}

/*****************************************************************************
* CMainDialog::SetOrRemoveInCmd
*------------------------------------------------------------------------*//*!
* \brief  Switches the visible buttons if the log is shown
* \param  param  The parameter, that have to be added or removed from the cmd
*                string (e. g -a data_uri).
* \param  cmd    Reference to the cmd string.
* \param  active True if the parameter should be inserted or false if it
                 should be removed.
*****************************************************************************/
void CMainDialog::SetOrRemoveInCmd(const QString param, QString& cmd, bool active)
{
    if (!active)
    {
        cmd.remove(QString(" %1").arg(param));
    }
    else if (!cmd.contains(param))
    {
        if (! cmd.endsWith(' '))
        {
            cmd.append(QString(" %1").arg(param));
        }
        else
        {
            cmd.append(param);
        }
    }
}

/*****************************************************************************
* CMainDialog::UpdateCmdText
*------------------------------------------------------------------------*//*!
* \brief  Updates the asciidoc cmd string in dependency of the
          selected GUI elements.
*****************************************************************************/
void CMainDialog::UpdateCmdText(void)
{
    QString cmd = ui->cmd_edit->toPlainText().simplified();
    SetOrRemoveInCmd("-a trace --verbose", cmd, ui->cb_verbose->isChecked());
    SetOrRemoveInCmd("-a data_uri", cmd, ui->cb_data_uri->isChecked());
    SetOrRemoveInCmd("-a icons", cmd, ui->cb_use_icons->isChecked());
    SetOrRemoveInCmd("-a toc", cmd, ui->cb_add_toc->isChecked());
    ui->cmd_edit->setPlainText(cmd);
}

/*****************************************************************************
* CMainDialog::UpdateA2xCmdText
*------------------------------------------------------------------------*//*!
* \brief  Updates the a2x cmd string in dependency of the selected GUI elements.
*****************************************************************************/
void CMainDialog::UpdateA2xCmdText(void)
{
    QString cmd = ui->cmd_edit_a2x->toPlainText().simplified();
    SetOrRemoveInCmd("-a trace --verbose", cmd, ui->cb_verbose_a2x->isChecked());
    ui->cmd_edit_a2x->setPlainText(cmd);
}

/*****************************************************************************
* CMainDialog::UpdateProcessLog
*------------------------------------------------------------------------*//*!
* \brief  Read the stdout/stderr from the proces and wrights it in the log
*****************************************************************************/
void CMainDialog::UpdateProcessLog()
{
    QTextStream in(m_process.readAllStandardOutput());
    QString result = in.readAll();
    if ((! result.isEmpty()) &&
        (  result != "\n"))
    {
        ui->log_ctrl->appendPlainText(result);
    }
}

/*****************************************************************************
* CMainDialog::ShowDocument
*------------------------------------------------------------------------*//*!
* \brief  Opens the webbrowser or pdf viewer if enabled.
* \param  type Defines the Application that should be opened.
*****************************************************************************/
void CMainDialog::ShowDocument(eVIEWER_TYPE type)
{
    QString exe_path;
    QString outname;
    QString viewer_cmd;
    switch (type)
    {
        case vtBROWSER:
        {
            if (ui->cb_execute_browser->isChecked())
            {
                exe_path   = ui->browserpath->text().remove("\"");
                outname    = ui->dest_file_name->text().simplified();
                viewer_cmd = QString("\"%1\" file:///%2").arg(exe_path).arg(outname);
                ui->log_ctrl->appendPlainText("Starting browser...\n");
            }
            break;
        }
        case vtPDF:
        {
            if (ui->cb_execute_pdf_viewer->isChecked())
            {
                exe_path   = ui->pdfviewerpath->text().remove("\"");
                outname    = ui->dest_file_name_a2x->text().simplified();
                viewer_cmd = QString("\"%1\" %2").arg(exe_path).arg(outname);
                ui->log_ctrl->appendPlainText("Starting PDF viewer...\n");
            }
            break;
        }
        default:
        {
            return;
        }
    }

    if ((! exe_path.isEmpty()) &&
        (! outname.isEmpty() ))
    {
        if (! QProcess::startDetached(viewer_cmd))
        {
            ui->log_ctrl->appendPlainText(tr("Can't execute \"%1\"\n").arg(viewer_cmd));
        }
    }
}

/*****************************************************************************
* CMainDialog::setLogProcessCtrlValue
*------------------------------------------------------------------------*//*!
* \brief  Sets the progressbar of the log window
* \param  value -1 = red background, 0 - 100 = progressbar value
*****************************************************************************/
void CMainDialog::setLogProcessCtrlValue(int value)
{
    if (value < 0)
    {
        ui->log_progress->setValue(0);
        ui->log_progress->setStyleSheet("border-color: rgb(255, 0, 0);\nbackground-color: rgb(255, 0, 0);");
    }
    else
    {
        ui->log_progress->setValue(qMin(100, value));
        ui->log_progress->setStyleSheet("");
    }
}

/*****************************************************************************
* CMainDialog::timerEvent
*------------------------------------------------------------------------*//*!
* \brief  reads the process oztput durch conversion
*****************************************************************************/
void CMainDialog::timerEvent (QTimerEvent *)
{
    killTimer(m_timerId);

    if (m_process.error() == QProcess::FailedToStart)
    {
        UpdateProcessLog();
        ui->log_ctrl->appendPlainText(tr("ERROR: Can't execute python"));
        setLogProcessCtrlValue(-1);
        UpdateButtonStates(false);
        return;
    }
    if (m_process.error() == QProcess::Crashed)
    {
        UpdateProcessLog();
        ui->log_ctrl->appendPlainText(tr("ERROR: Python (asciidoc/a2x) crashed"));
        setLogProcessCtrlValue(-1);
        UpdateButtonStates(false);
        return;
    }
    if (m_process.state() == QProcess::NotRunning)
    {
        if (m_process.exitStatus() == QProcess::NormalExit)
        {
            int err_code = m_process.exitCode();
            if (err_code == 0)
            {   // No error
                ui->log_ctrl->appendPlainText(("Document successfully created!\n"));
                setLogProcessCtrlValue(100);
                ShowDocument(m_viewer_type);
            }
            else
            {
                UpdateProcessLog();
                ui->log_ctrl->appendPlainText(QString("Error: Asciidoc/a2x returns with error code %1\n").arg(err_code));
                setLogProcessCtrlValue(-1);
            }
        }
        else
        {
            switch (m_process.error())
            {
                case QProcess::FailedToStart:
                {
                    UpdateProcessLog();
                    ui->log_ctrl->appendPlainText("Process cannot be started!");
                    setLogProcessCtrlValue(-1);
                    UpdateButtonStates(false);
                    return;
                }
                case QProcess::Crashed:
                {
                    UpdateProcessLog();
                    ui->log_ctrl->appendPlainText("Asciidoc or pathon crashed");
                    setLogProcessCtrlValue(-1);
                    UpdateButtonStates(false);
                    return;
                }
                case QProcess::Timedout:
                {
                    UpdateProcessLog();
                    ui->log_ctrl->appendPlainText("Operation timeout");
                    setLogProcessCtrlValue(-1);
                    UpdateButtonStates(false);
                    return;
                }
                case QProcess::ReadError:
                case QProcess::WriteError:
                case QProcess::UnknownError:
                default:
                {
                    UpdateProcessLog();
                    ui->log_ctrl->appendPlainText("Process error");
                    setLogProcessCtrlValue(-1);
                    UpdateButtonStates(false);
                    return;
                }
            }
        }
        UpdateButtonStates(false);
    }
    else
    {
        if (progr_up)
        {
            if (progr_idx > PROGR_END)
            {
                progr_up = false;
            }
            else
            {
                progr_idx++;
            }
        }
        else if (progr_idx < PROGR_START)
        {
            progr_up = true;
        }
        else
        {
            progr_idx--;
        }
        ui->log_progress->setValue(progr_idx);
        UpdateProcessLog();
        UpdateButtonStates(true);
        m_timerId = startTimer(100);
    }
}

/*****************************************************************************
* CMainDialog::on_pb_cancel_clicked
*------------------------------------------------------------------------*//*!
* \brief  Push-Button "cancel" clicked on the asciidoc tab.
*****************************************************************************/
void CMainDialog::on_pb_cancel_clicked()
{
    close();
}

/*****************************************************************************
* CMainDialog::on_pb_run_clicked
*------------------------------------------------------------------------*//*!
* \brief  Push-Button "run" clicked on the asciidoc tab.
*
* This executes the asciidoc toolchain with the parameters from the
* asciidoc tab.
*****************************************************************************/
void CMainDialog::on_pb_run_clicked()
{
    QString infile = ui->src_file_name->text();
    QFileInfo info(infile);
    if (!info.exists())
    {
        QMessageBox::critical(this, tr("Error"), tr("Source file doesn't exist"));
        return;
    }
    bool exec_browser = ui->cb_execute_browser->isChecked();
    QString browser   = ui->browserpath->text().remove("\"");
    if (exec_browser)
    {
        QFileInfo info(browser);
        if (! info.exists())
        {
            QMessageBox::critical(this, tr("Misconfigured browser"), "The brwoser path is invalid.\n\nPlease specify a valid path and file name or uncheck the execute browser checkbox.\n\nCommand aborted.");
            return;
        }
    }
    SaveLastSettings();

    if (! SetupEnvironment())
    {
        return;
    }
    m_process.setProcessChannelMode(QProcess::MergedChannels);
    m_process.setProcessEnvironment(m_env);
    QStringList args;
    args.append("asciidoc.py");
    args.append("infile");
    //asciidoc.start("python.exe", args);
    QString cmd_base = ui->cmd_edit->toPlainText();
    QString out_name = ui->dest_file_name->text();
    QString cmd = QString("%1 -o %2 %3")
                        .arg(cmd_base)
                        .arg(out_name)
                        .arg(infile);

    ui->log_ctrl->setPlainText(tr("Starting document generation\n\n%1\n").arg(cmd));
    ui->pages->setCurrentIndex(1); // Switch to log window

    m_viewer_type = vtBROWSER;
    m_process.start(cmd);
    m_timerId = startTimer(300); // Starte Timer mit 300ms.
}

/*****************************************************************************
* CMainDialog::on_pb_run_a2x_clicked
*------------------------------------------------------------------------*//*!
* \brief  Push-Button "run" clicked on the a2x tab.
*
* This executes the a2x/dblatex toolchain with the parameters from the
* a2x tab.
*****************************************************************************/
void CMainDialog::on_pb_run_a2x_clicked()
{
    QString infile = ui->src_file_name_a2x->text();
    QFileInfo info(infile);
    if (!info.exists())
    {
        QMessageBox::critical(this, tr("Error"), tr("Source file doesn't exist"));
        return;
    }
    bool exec_viewer = ui->cb_execute_pdf_viewer->isChecked();
    QString viewer   = ui->pdfviewerpath->text().remove("\"");
    if (exec_viewer)
    {
        QFileInfo info(viewer);
        if (! info.exists())
        {
            QMessageBox::critical(this, tr("Misconfigured PDF-Viewer"), "The pdf viewer path is invalid.\n\nPlease specify a valid path and file name or uncheck the checkbox.\n\nCommand aborted.");
            return;
        }
    }
    SaveLastSettings();

    if (! SetupEnvironment())
    {
        return;
    }
    m_process.setProcessChannelMode(QProcess::MergedChannels);
    m_process.setProcessEnvironment(m_env);
    QStringList args;
    args.append("a2x.py");
    args.append("infile");
    //asciidoc.start("python.exe", args);
    QString cmd_base = ui->cmd_edit_a2x->toPlainText();
    QString out_name = ui->dest_file_name_a2x->text();
    QString cmd = QString("%1 %2")
                        .arg(cmd_base)
                        .arg(infile);

    ui->log_ctrl->setPlainText(tr("Starting document generation\n\n%1\n").arg(cmd));
    ui->pages->setCurrentIndex(1); // Switch to log window

    m_viewer_type = vtPDF;
    m_process.start(cmd);
    m_timerId = startTimer(300); // Starte Timer mit 300ms.

}

/*****************************************************************************
* CMainDialog::GetRegistryTxtHandler
*------------------------------------------------------------------------*//*!
* \brief  Get windows registry shell handler for txt files.
* \return Handlername of txt files.
*****************************************************************************/
/* QString CMainDialog::GetRegistryTxtHandler()
{
    QSettings settings("HKEY_CLASSES_ROOT\\.txt",
                        QSettings::NativeFormat);
    QString txtfile = settings.value("Default", "").toString();
    return txtfile;
} */

/*****************************************************************************
* CMainDialog::on_pb_create_registry_cm_entry_clicked
*------------------------------------------------------------------------*//*!
* \brief  Create a AsciidocToGo entry in windows explorer context menu
*         for txt files.
*
* This function calls asciidoctogo_register.exe to register a shell
* extension context menu item in windows explorer. This can only be done
* with administration rights.
*****************************************************************************/
void CMainDialog::on_pb_create_registry_cm_entry_clicked()
{
#ifdef Q_OS_WIN
    int ret = QProcess::execute("./asciidoctogo_register.exe");
    switch (ret)
    {
        case -2:
        {
            int ret =
            QMessageBox::critical(this,
                                  "Error",
                                  "Can't start process asciidoctogo_register.exe\n"
                                  "One reason for this is that you don't have administration\n"
                                  "rights on this computer or the rights could not been evelated.\n\n"
                                  "Open Explorer and try to press SHIFT and right click on\n"
                                  "asciidoctogo_register.exe and select \"Run as Administrator\" or\n"
                                  "\"Run as Different User\"\n\n"
                                  "Should i open the explorer in the right path for you?",
                                  QMessageBox::Yes, QMessageBox::No);
            if (ret == QMessageBox::Yes)
            {
                QProcess process;
                QString  work_dir = QDir::toNativeSeparators(QApplication::applicationDirPath());
                process.setWorkingDirectory(work_dir);
                QString  param = QString("/select,%1").arg(work_dir);
                param += "\\asciidoctogo_register.exe";
                QMessageBox::critical(this, "Fehler", param);
                process.execute("explorer.exe", QStringList(param));
            }
            break;
        }
        case 0:
        {
            QMessageBox::information(this, "Success", "AsciidocToGo successfully registered for *.txt files");
            break;
        }
        default:
        {
            QMessageBox::critical(this,
                                  "Error",
                                  "Can't register AsciidocToGo shell extension!\n"
                                  "Try asciidoctogo_register.exe manually");
            break;
        }
    }
#endif
}


/*****************************************************************************
* CMainDialog::UpdateSrcFileName
*------------------------------------------------------------------------*//*!
* \brief  Updates the src and dest filenames in asciidoc and a2x tabs.
* \param  infile The Name of the input file.
*****************************************************************************/
void CMainDialog::UpdateSrcFileName(const QString& infile)
{
    QFileInfo info(infile);
    if (! info.exists())
    {
        return;
    }
    QString bname = info.completeBaseName();
    QDir path(info.absolutePath());
    ui->src_file_name->setText(infile);
    ui->dest_file_name->setText(QDir::toNativeSeparators(path.absoluteFilePath(bname+".html")));
    ui->src_file_name_a2x->setText(infile);
    ui->dest_file_name_a2x->setText(QDir::toNativeSeparators(path.absoluteFilePath(bname+".pdf")));
}

/*****************************************************************************
* CMainDialog::on_pb_select_source_clicked
*------------------------------------------------------------------------*//*!
* \brief  Select asciidoc source txt file
*****************************************************************************/
void CMainDialog::on_pb_select_source_clicked()
{
    const QString infile = QFileDialog::getOpenFileName(
                              this,
                              tr("Open Asciidoc Textfile"),
                              "",
                              tr("Asciidoc Text Files (*.txt)"));
    UpdateSrcFileName(infile);
}

/*****************************************************************************
* CMainDialog::on_cb_verbose_clicked
*------------------------------------------------------------------------*//*!
* \brief  Verbose Checkbox on asciidoc tab clicked.
*****************************************************************************/
void CMainDialog::on_cb_verbose_clicked() //!<
{
    UpdateCmdText();
}

/*****************************************************************************
* CMainDialog::on_cb_data_uri_clicked
*------------------------------------------------------------------------*//*!
* \brief  Data-URI checkbox on asciidoc tab clicked.
*****************************************************************************/
void CMainDialog::on_cb_data_uri_clicked()
{
    UpdateCmdText();
}

/*****************************************************************************
* CMainDialog::on_cb_use_icons_clicked
*------------------------------------------------------------------------*//*!
* \brief  Use icons checkbox on asciidoc tab clicked.
*****************************************************************************/
void CMainDialog::on_cb_use_icons_clicked()
{
    UpdateCmdText();
}

/*****************************************************************************
* CMainDialog::on_cb_add_toc_clicked
*------------------------------------------------------------------------*//*!
* \brief  Add table of content checkbox on asciidoc tab clicked.
*****************************************************************************/
void CMainDialog::on_cb_add_toc_clicked()
{
    UpdateCmdText();
}

/*****************************************************************************
* CMainDialog::on_pb_close_clicked
*------------------------------------------------------------------------*//*!
* \brief  Close pushbutton on asciidoc tab clicked.
*****************************************************************************/
void CMainDialog::on_pb_close_clicked()
{
    setLogProcessCtrlValue(PROGR_START);
    ui->pages->setCurrentIndex(0);
}

/*****************************************************************************
* CMainDialog::on_pb_abort_clicked
*------------------------------------------------------------------------*//*!
* \brief  Abot pushbutton on asciidoc tab clicked.
*****************************************************************************/
void CMainDialog::on_pb_abort_clicked()
{
    if (m_process.state() == QProcess::Running)
    {
        m_process.terminate();
    }
}

/*****************************************************************************
* CMainDialog::on_pb_select_source_clicked
*------------------------------------------------------------------------*//*!
* \brief  Select a2x source txt file
*****************************************************************************/
void CMainDialog::on_pb_select_source_a2x_clicked()
{
    on_pb_select_source_clicked();
}

/*****************************************************************************
* CMainDialog::on_cb_verbose_a2x_clicked
*------------------------------------------------------------------------*//*!
* \brief  Verbose Checkbox on a2x tab clicked.
*****************************************************************************/
void CMainDialog::on_cb_verbose_a2x_clicked()
{
    UpdateA2xCmdText();
}

/*****************************************************************************
* CMainDialog::on_pb_cancel_a2x_clicked
*------------------------------------------------------------------------*//*!
* \brief  Close/Cancel pushbutton on a2x tab clicked.
*****************************************************************************/
void CMainDialog::on_pb_cancel_a2x_clicked()
{
    close();
}

/*****************************************************************************
* CMainDialog::anchorClicked
*------------------------------------------------------------------------*//*!
* \brief  Slot for the click on the license link in options->license
*****************************************************************************/
void CMainDialog::anchorClicked(const QUrl & link)
{
    QString base_path = qApp->applicationDirPath();
    QDir dir(base_path);
    //QMessageBox::critical(this, "", dir.absoluteFilePath(link.toString()));
    QString path = dir.absoluteFilePath(link.toString());
    QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(path), QUrl::TolerantMode));
    //QProcess::execute();
}
