#!/bin/sh

sysdocpath="/usr/share/gtk-doc/html"
docpath="/Contents/Resources/Documents"

echo "Copying files for gtk3 docset..."
mkdir -p GTK3.docset${docpath}
cp -r ${sysdocpath}/gtk3/* GTK3.docset${docpath}
sed -i 's_/usr/share/gtk-doc/html/glib_../../../../GLib.docset/Contents/Resources/Documents_g' GTK3.docset${docpath}/*.html
sed -i 's_/usr/share/gtk-doc/html/gio_../../../../GIO.docset/Contents/Resources/Documents_g' GTK3.docset${docpath}/*.html
sed -i 's_/usr/share/gtk-doc/html/gobject_../../../../GObject.docset/Contents/Resources/Documents_g' GTK3.docset${docpath}/*.html
sed -i 's_/usr/share/gtk-doc/html/atk_../../../../ATK.docset/Contents/Resources/Documents_g' GTK3.docset${docpath}/*.html
sed -i 's_/usr/share/gtk-doc/html/pango_../../../../Pango.docset/Contents/Resources/Documents_g' GTK3.docset${docpath}/*.html
sed -i 's_/usr/share/gtk-doc/html/cairo_../../../../Cairo.docset/Contents/Resources/Documents_g' GTK3.docset${docpath}/*.html

echo "Copying files for docset..."
mkdir -p GTK2.docset${docpath}
cp -r ${sysdocpath}/gtk2/* GTK2.docset${docpath}
sed -i 's_http://library.gnome.org/devel/glib/unstable_../../../../GLib.docset/Contents/Resources/Documents_g' GTK2.docset${docpath}/*.html
sed -i 's_http://library.gnome.org/devel/gio/unstable_../../../../GIO.docset/Contents/Resources/Documents_g' GTK2.docset${docpath}/*.html
sed -i 's_http://library.gnome.org/devel/gobject/unstable_../../../../GObject.docset/Contents/Resources/Documents_g' GTK2.docset${docpath}/*.html


echo "Copying files for cairo docset..."
mkdir -p Cairo.docset${docpath}
cp -r ${sysdocpath}/cairo/* Cairo.docset${docpath}

echo "Generating docset databases..."
python3 docsetgen.py
