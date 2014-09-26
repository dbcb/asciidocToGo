:ascii-ids:
== General

AsciiDoc2Go is a portable version of asciidoc with all necessary software components
included in one package.

== Motivation for AsciidocToGo

The first steps with asciidoc are pretty easy. Just grab python and asciidoc and
you are done. If you would like to use the more advanced features and optional
packages like syntax highlighting for source code or the diagram tools like
dot, blockdiag, mathplotlib, etc. it becomes pretty hard to get the job done.
Also some filters needs external programms and asciidoc is not very verbose
about what dependencies have to be solved. A lot of the software delivered
with AsciidocToGo was found by source code analysis.

It will go even worse if one would like to create docbook pdf's, because a complete
LaTeX-Toolchain is needed and the dblatex installer is/was broken.

AsciidocToGo have all this in a predefined environment and adds also a
shell menu in the windows explorer, so that you can just right click a
txt file and create a document out of it.

It takes over a month to create the AsciidocToGo package as it is now.

== About AsciidocToGo

Projectwebsite:: http://dbcb.github.io/asciidocToGo
Repository:: https://github.com/dbcb/asciidocToGo
License:: link:./license.html[AsciidocToGo and third party packages Licenses]
User Manual:: link:./Manual.html[]

== Included Software packages

* link:https://www.python.org/[python 2.7.6]
* link:http://www.asciidoc.org[Asciidoc 8.6.9]
* link:http://www.miktex.org/download[Miktex 2.9.5105]
* link:http://www.research.att.com/software_tools?fbid=hCb8eyIXDJA[graphviz 2.3.8]
* link:http://www.andre-simon.de/doku/highlight/en/highlight.php[highlight 3.1.8]
* link:http://matplotlib.org/downloads.html[mathplotlib 1.3.1]
* link:http://www.numpy.org/[numpy 1.8.1 (1)]  link:http://volnitsky.com/project/mplw[numpy 1.8.1 (2)]
* mpl SHA-1:75f0d009629f93f33fab04b83faca20cc35dd358::
* link:https://www.maths.nottingham.ac.uk/personal/drw/lm.html[mathml]
* link:http://www.zlatkovic.com/libxml.en.html[xsltproc 1.1.26]
* link:http://sourceforge.net/projects/docbook/files/#files[docbook 1.78.1]
* link:http://dblatex.sourceforge.net/[dblatex 0.3.4]
* link:http://lynx.isc.org/release/,https://sites.google.com/site/fredlwm/lynx.zip[lynx 2.8.8]
* link:https://code.google.com/p/asciidoc-diag-filter/downloads/list[diag_filter]
* link:https://pypi.python.org/pypi/blockdiag[blockdiag 1.3.3]
* link:https://pypi.python.org/pypi/Pillow/2.4.0[Pillow 2.4.0]
* link:https://pypi.python.org/pypi/actdiag/[actdiag 0.5.1]
* link:https://pypi.python.org/pypi/seqdiag/[seqdiag 0.9.0]
* link:https://pypi.python.org/pypi/nwdiag/[nwdiag 1.0.0]
* link:https://pypi.python.org/simple/webcolors[Webcolors 1.4]
* funcparserlib 0.3.6
* pyparsing 2.0.2
* link:https://pypi.python.org/pypi/six[six 1.7.2]
* link:http://qt-project.org/wiki/PySide[PySide 1.2.2]
* link:http://www.dabeaz.com/ply/[ply 3.4]
* link:https://pypi.python.org/pypi/setuptools#windows-7-or-graphical-install[setuptool 4.0.1]

=== Not (yet) included Software Packages

* w3m:: http://sourceforge.net/projects/w3m/files/w3m/ +
  Lynx is used instead
* epubcheck:: https://github.com/IDPF/epubcheck +
  This programm needs java and i don't want to include another scripting language.
* dita 0.9:: http://sourceforge.net/projects/ditaa/files/ditaa/0.9/ditaa0_9.zip/download
* LatexMathML, AsciimathML::
  The option "-a asciimath" doesn't work.
* Latexmath:: Needs latex2png -> Next version

== TODO:

qrencode: https://code.google.com/p/qrencode-win32/

== MikTex

Some packages are missing in miktex and have to be manually installed.

* See: http://latex-community.org/home/news/46-news-latex-distributions/469-miktex-amsmath

Install them with mpm_mfc_admin.exe!

Miktex can't install packages:

* http://entangled.wordpress.com/2009/09/18/installation-of-miktex-2-8-failed/#comment-1748

== Example of AsciidocToGo


=== Source Code Highlighting

AsciidocToGo use highlight.exe for syntax highlighting of source code

[source,C++]
-------------------------------------------
int Test(void)
{
    // SomeCode
}
-------------------------------------------

=== Mathplotlib

AsciidcoToGo has build in support for mathplotlib

// dblatex doesn't allow rel. path here
["mpl", "example_sin.png"]
-------------------------------------------
figure(figsize=(4,2))           # PNG size, inches
X = arange(0, 6, 0.1)           # 0, 0.1, 0.2 ... 6.0
Y = sin(X)                      # vector op
plot(X,Y)
-------------------------------------------

// dblatex doesn't allow rel. path here
=== Graphviz (Dot)

Diagramms printed with dor 

["graphviz", "example_dot.png"]
-------------------------------------------
digraph test123 
{
    "Change Request" -> "Validation" -> "Implementation";
    "Validation" -> "Change Request"; 
}
-------------------------------------------

=== Blockdiag

Diagramms printed with blockdiag.
Refer to the following link for more examples: link:http://blockdiag.com/en/blockdiag/examples.html#simple-diagram[Examples]

["blockdiag", target="images/example-blockdiag.svg"]
-------------------------------------------
blockdiag
{
  // branching edges to multiple children
  A -> B, C;

  // branching edges from multiple parents
  D, E -> F;
}
-------------------------------------------

=== actdiag

Diagramms printed with actdiag.
Refer to the following link for more examples: link:http://blockdiag.com/en/actdiag[Examples]

["actdiag",  target="images/example-actdiag.svg"]
-------------------------------------------
actdiag
{
  write -> convert -> image

  lane user {
     label = "User"
     write [label = "Writing reST"];
     image [label = "Get diagram IMAGE"];
  }
  lane actdiag {
     convert [label = "Convert reST to Image"];
  }
}
-------------------------------------------

=== SeqDiag

Seqdiag Is a dRawing engine for sequence diagrams.

Refer to link:http://blockdiag.com/en/seqdiag/examples.html#simple-diagram["Seqdiag Webpage"]

["seqdiag",  target="images/example-seqdiag.svg"]
-------------------------------------------
seqdiag 
{
  browser  -> webserver [label = "GET /index.html"];
  browser <-- webserver;
  browser  -> webserver [label = "POST /blog/comment"];
              webserver  -> database [label = "INSERT comment"];
              webserver <-- database;
  browser <-- webserver;
}
-------------------------------------------

=== NwDiag

NwDiag is used to draw network related stuff like:

* Networks topology,
* IP-Ranges,
* Protocols,
* ...

Refer to link:http://blockdiag.com/en/nwdiag/nwdiag-examples.html["nwdiag website"]

["nwdiag",  target="images/example-nwdiag.svg"]
-------------------------------------------
nwdiag 
{
  inet [shape = cloud];
  inet -- router;

  network {
    router;
    web01;
    web02;
  }
}
-------------------------------------------

=== PacketDiag

PacketDiag is a diagramm tool for any kind of
digital network protocols.

Please refer to: link:http://blockdiag.com/en/nwdiag/packetdiag-examples.html[PacketDiag Website]

["packetdiag", target="images/example-packetdiag.svg"]
-------------------------------------------
packetdiag 
{
  colwidth = 32
  node_height = 72

  0-15: Source Port
  16-31: Destination Port
  32-63: Sequence Number
  64-95: Acknowledgment Number
  96-99: Data Offset
  100-105: Reserved
  106: URG [rotate = 270]
  107: ACK [rotate = 270]
  108: PSH [rotate = 270]
  109: RST [rotate = 270]
  110: SYN [rotate = 270]
  111: FIN [rotate = 270]
  112-127: Window
  128-143: Checksum
  144-159: Urgent Pointer
  160-191: (Options and Padding)
  192-223: data [colheight = 3]
}
------------------------------------------- 


