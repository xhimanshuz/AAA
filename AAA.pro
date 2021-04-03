QT += widgets sql


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
    svginterface.cpp

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
    svginterface.h
message($$QMAKE_HOST.arch == x86_64)
win32 {
    CONFIG += static
    message(Building for Windows)
}
unix {
    message(Building for Unix)
}

INCLUDEPATH += include

DESTDIR = $$PWD/build/ \


DISTFILES += .gitignore \
            $$PWD/Data/DBMigratingTool/run.py \
            $$PWD/build/

RESOURCES += \
    Resource.qrc
