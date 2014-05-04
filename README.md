AsciidocToGo
============

AsciidocToGo is a full featured portable version of [asciidoc](http://www.asciidoc.org asciidoc) that contains
the complete toolchain to build html or docbook/latex based PDF documentation out of plain ascii files. Just download
AsciidocToGo and start writing instead of seaching day or maybe weeks to put together all of the the required
softwareparts.

Packages
--------

AsciidocToGo is delivered as a stand alone package for windows, linux, and McOS
including the following parts:

* python 2.7.x
* asciddoc
* code highlighter (hightlight or gnu)
* dot
* mathplotlib
* mathML
* dblatex
* xsltproc
* DocBook XSL Stylesheets
* Lynx
* mpw
* QSide
* Ply

Usage
-----

AsccidocToGo currently use batch files to setup an envirnonment with
the correct pathes and environment variables. This will be changed changes later to
python based QT gui. 

The installer adds the AsciidocToGo installation path to the system path.
To use AsciidocToGo just open an shell (cmd.exe) and execute one
of the following commands:

* ```asciidoc.bat <options> <filename>```
     Executes asciidoc.py in the preconfigured environment
* ```a2x.bat  <options> <filename>```
     Executes a2x.py in the preconfigured environment
* ```AsciidocToGoShell.bat```
     Opens a command shell with the preconfigured environment
* ```test_cmd.bat <command>```
     Excutes any shell command in the preconfigured environment


Current State
-------------

Working on the pre alpha packages for asciidoc-8.6.9.
If you like to test the first pre alpha releases, drop mea note
at CarstenBreuerDevATgmx.de












