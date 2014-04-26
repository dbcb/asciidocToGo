asciidocToGo
============

Portable version of asciidoc including plugins (dot, latex, code highlighting) 

Asciidoc is a great tool to create good looking html documents out of plain text files.
To use it with all features, a lot of additional packages need to be included.
This project tries to make it easier for users to "just use" asciddoc without the hassle
to deal with installations and dependencies.

Packages
--------

asciidocToGo is delivered as a stand alone package for windows, linux, and macos
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
* Fop
* w3m
* Lynx (?)
* epubcheck

Usage
-----

```asciidoctogo <options> <filename>```

asciidocToGo configures at the first start the configuration of all
components and forwards the parameter to asciidoc then.

Current State
-------------

Working on the packages for asciidoc-8.6.9











