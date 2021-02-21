QT += widgets sql

CONFIG += static
RC_ICONS = icon.ico

SOURCES += \
    ConfigUi.cpp \
    src/AAA.cpp \
    src/Invoice/AddInvoice.cpp \
    src/Invoice/InvoiceWindow.cpp \
    src/MediaBillDetail.cpp \
    src/MediaPaymentDetail.cpp \
    src/PaymentReceiptDetail.cpp \
    src/ReleaseOrder/AddReleaseOrder.cpp \
    src/UserUI/NewUser.cpp \
    src/UserUI/UserWindow.cpp \
    src/IOHandler/DataEngine.cpp \
    src/IOHandler/IOHandler.cpp \
    src/IOHandler/SQLiteHandler.cpp \
    src/JobType.cpp \
    src/app.cpp \
    src/pdftroninterface.cpp

HEADERS += \
    ConfigUi.h \
    Configure.h \
    HeaderUi.h \
    Log.h \
    ROSqlTableModel.h \
    include/AAA.h \
    include/AddInvoice.h \
    include/CustomItemDelegate.h \
    include/InvoiceWindow.h \
    include/MediaBillDetail.h \
    include/MediaPaymentDetail.h \
    include/PaymentReceiptDetail.h \
    include/IOHandler/SQLiteHandler.h \
    include/ReleaseOrder/AddReleaseOrder.h \
    include/UserUI/NewUser.h \
    include/UserUI/UserWindow.h \
    include/AbstractUser.h \
    include/IOHandler/DataEngine.h \
    include/IOHandler/IOHandler.h \
    include/JobType.h \
    include/pdftroninterface.h
message($$QMAKE_HOST.arch == x86_64)
win32 {
    message(Building for Windows)
    PDFTRON_DIR = PDFTron/Win32
}
unix {
    message(Building for Unix)
    PDFTRON_DIR = PDFTron
}
message(PdfTron Dir = $$PWD/$$PDFTRON_DIR )

INCLUDEPATH += include \
               $$PWD/$$PDFTRON_DIR/Headers \
               /home/gnu/Documents/Codes/git/spdlog/include

DESTDIR = $$PWD/build/ \


DISTFILES += .gitignore \
            $$PWD/Data/DBMigratingTool/run.py \
            $$PWD/build/

LIBS += -L$$PWD/$$PDFTRON_DIR/Lib  -lPDFNetC

RESOURCES += \
    Resource.qrc

