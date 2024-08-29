<h1 align="center">DocSets</h1>

A number of different ways to create docsets for Zeal and other apps.<br>
Although these will create usable docsets, these files are experimental.<br>
<br>
GTKDocset:<br>
Just run:
```console
./autogen.sh
cp -r *.docset ~/.local/share/Zeal/Zeal/docsets
```
XlibDocSet:<br>
Just run:<br>
```console
./makedocset
```
Files are automatically copied to~/.local/share/Zeal/Zeal/docsets<br>
See the README.md file.<br>
<br>
<br>
Qt5And6Docsets:
The 'official' qt5/6 docsets downloadable via zeal are out of date.
Just run:
```console
make install 2>/dev/null
```
Various options can be changed, see the Makefile.<br>
The documentation will automatically be downloaded, if you already have the docs do ( obviously change the paths to suit ) eg:<br>
```console
make install QT6DOCS=/usr/share/doc/qt6 QT5DOCS=/usr/share/doc/qt5 2>/dev/null
```
<br>
DocSetFromDevHelp:<br>
To ( for instance ) create a docset from installed cairo gtk-docs:
```console
DOCSET_NAME="cairo" SOURCEHTML=/usr/share/gtk-doc/html/cairo ./createdocset
```
To ( for instance ) create a docset from installed harfbuzz gtk-docs, show entries and not installing:
```console
DOCSET_NAME="harfbuzz" SOURCEHTML=/usr/share/gtk-doc/html/harfbuzz DOCSETOP="/tmp/docs" VERBOSE=1 ./createdocset
```