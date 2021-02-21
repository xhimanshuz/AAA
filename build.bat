@echo off
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
set PATH=%PATH%;D:\Qt\qt5\qtbase\bin\
qmake AAA.pro
nmake release
copy PDFTron\Win32\Lib\PDFNetC.dll build\
cd build
mkdir Data
cd Data
mkdir invoice
mkdir receipt
mkdir ro
mkdir SamplePdf
cd SamplePdf
copy ..\..\..\Data\PDFSample\*.pdf .
cd ..\..\
copy ..\Data\DBMigratingTool\location.db .
cd ..
@call "make_archive.bat"
pause