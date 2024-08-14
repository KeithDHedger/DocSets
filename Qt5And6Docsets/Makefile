
BUILDDB=builddb

DOCSDESTDIR=./Docs
DESTDIR=

QT6NAME=Qt6
QT6DOCS=$(DOCSDESTDIR)/usr/share/doc/qt6
QT6ICONPATH=./qt6.png
QT6DOWNLOADDOCS="https://europe.mirror.pkgbuild.com/extra/os/x86_64/qt6-doc-6.7.2-1-any.pkg.tar.zst"

QT5NAME=Qt5
QT5DOCS=$(DOCSDESTDIR)/usr/share/doc/qt
QT5ICONPATH=./qt5.png
QT5DOWNLOADDOCS="https://europe.mirror.pkgbuild.com/extra/os/x86_64/qt5-doc-5.15.14-1-any.pkg.tar.zst"

all:
	g++ $(BUILDDB).cpp LFSTKUtilityClass.cpp -lsqlite3 -o $(BUILDDB)

clean:
	rm -f $(BUILDDB)

distclean:
	rm -f $(BUILDDB)
	rm -rf $(DOCSDESTDIR)
	
install-qt6: all
	DOCSET_NAME="$(QT6NAME)" SOURCEHTML="$(QT6DOCS)" ICONPATH="$(QT6ICONPATH)" DOCSETOP="$(DESTDIR)" ./createdocset

install-qt5: all
	DOCSET_NAME="$(QT5NAME)" SOURCEHTML="$(QT5DOCS)" ICONPATH="$(QT5ICONPATH)" DOCSETOP="$(DESTDIR)" ./createdocset

download-qt6-docs:
	if [[ ! -d $(QT6DOCS) ]];then wget -P $(DOCSDESTDIR) -c $(QT6DOWNLOADDOCS);fi
	if [[ ! -d $(QT6DOCS) ]];then cd ./Docs;tar -xvf qt6-doc-*-any.pkg.tar.zst;fi

download-qt5-docs:
	if [[ ! -d $(QT5DOCS) ]];then wget -P $(DOCSDESTDIR) -c $(QT5DOWNLOADDOCS);fi
	if [[ ! -d $(QT5DOCS) ]];then cd ./Docs;tar -xvf qt5-doc-*-any.pkg.tar.zst;fi

download-docs: download-qt6-docs download-qt5-docs

install: download-docs all install-qt6 install-qt5
