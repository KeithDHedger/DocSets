# MakeXLibDocset
Make a docset for zeal from the xlib programming manual website here https://tronche.com/gui/x/xlib<br>
<br>
Just run ./makedocset from this folder to create a docset from the xlib programming manual, only the first run downloads the webpages, if you want you can delete the folder 'html' after use but it will be downloaded the next time you run makedocset.<br>
The doc set is by default built in ~/.local/share/Zeal/Zeal/docsets, but you can change this by using:<br>
DESTDIR=/some/other/path ./makedocset<br>
Other variables like the name can be adjusted, see the makedocset file.<br>
Including downloading the html files it takes about 2/3 mins to complete, depending on your system.
<br>
A small qt5 xmlreader will be built also on the first run, if you have qt6 installed instead just change "Qt5Core" in getresources to "Qt6Core".<br>

These scripts could also be used ( with a bit of tweaking ) to create a docset for any well structured html pages.<br>

Changelog:<br>
Added some extra tokens manually.<br>
Added some more extra tokens manually.<br>