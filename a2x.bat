@echo off
:: Check if environment is allready configured
IF NOT "%ADTG_SW_PATH%"=="" GOTO exec_cmd


set ADTG_LAST_PATH=%PATH%
:: Set AscidocToGo Path
set ADTG_BASE_PATH=%~dp0
:: Attention ADTG_BASE_PATH contains allready a final backslash
set ADTG_SW_PATH=%ADTG_BASE_PATH%sw

:: Additional temp path
set TEMPDIR=%TEMP%\
set TMP=%TEMP%\
set TMPDIR=%TEMP%\

:: Home path
set GSC=mgs.exe
set HOME=%HOMEPATH%

:: Software Path
set ADTG_ASCIIDOC_PATH=%ADTG_SW_PATH%\asciidoc\8.6.9
set ADTG_DOCBOOK_PATH=%ADTG_SW_PATH%\docbook\1.78.1\doc
set ADTG_GRAPHVIZ_PATH=%ADTG_SW_PATH%\graphviz\2.3.8\bin
set ADTG_HIGHLIGHT_PATH=%ADTG_SW_PATH%\highlight\3.1.8
set ADTG_LATEXMATHML_PATH=%ADTG_SW_PATH%\LaTeXMathML
set ADTG_LIBXLST_PATH=%ADTG_SW_PATH%\libxslt\1.1.26\bin
set ADTG_LYNX_PATH=%ADTG_SW_PATH%\lynx\2.8.8
set ADTG_MIKTEX_BIN_PATH=%ADTG_SW_PATH%\miktex\2.9.5105\miktex\bin
set ADTG_MIKTEX_PATH=%ADTG_SW_PATH%\miktex\2.9.5105
set ADTG_PYTHON_PATH=%ADTG_SW_PATH%\python\2.7.6
set ATDG_PYTHON_SCRIPTS_PATH=%ADTG_PYTHON_PATH%\Scripts
set ADTG_DBLATEX_PATH=%ADTG_SW_PATH%\dblatex
set ADTG_INKSCAPE_PATH=%ADTG_SW_PATH%\Inkscape\0.48.4-1

:: Setup Miktex (copied from miktex command console)
set MIKTEX_BINDIR=%ADTG_MIKTEX_BIN_PATH%
set MIKTEX_COMMONSTARTUPFILE=%ADTG_MIKTEX_PATH%\miktex\config\miktexstartup.ini
set MIKTEX_GS_LIB=%ADTG_MIKTEX_PATH%\ghostscript\base;%MIKTEX_PATH%\fonts
set MIKTEX_USERSTARTUPFILE=%ADTG_MIKTEX_PATH%\miktex\config\miktexstartup.ini

:: Setup path vaiable
set PATH=%ADTG_BASE_PATH%;%ADTG_PYTHON_PATH%;%ATDG_PYTHON_SCRIPTS_PATH%;%ADTG_ASCIIDOC_PATH%;%ADTG_MIKTEX_BIN_PATH%;%ADTG_GRAPHVIZ_PATH%;%ADTG_HIGHLIGHT_PATH%;%ADTG_LIBXLST_PATH%;%ADTG_LYNX_PATH%;%ADTG_DOCBOOK_PATH%;%ADTG_DBLATEX_PATH%;%ADTG_INKSCAPE_PATH%;%PATH%

:exec_cmd

python.exe %ADTG_ASCIIDOC_PATH%\a2x.py %*

IF "%LAST_PATH%"=="" GOTO end
set PATH=%LAST_PATH%
:end
::pause
