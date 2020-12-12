QT += widgets sql printsupport

SOURCES += \
    src/AAA.cpp \
    src/Invoice/AddInvoice.cpp \
    src/Invoice/InvoiceWindow.cpp \
    src/MediaBillDetail.cpp \
    src/MediaPaymentDetail.cpp \
    src/PaymentReceiptDetail.cpp \
    src/PopplerInterface.cpp \
    src/pdftroninterface.cpp \
    src/ReleaseOrder/AddReleaseOrder.cpp \
    src/UserUI/NewUser.cpp \
    src/UserUI/UserWindow.cpp \
    src/IOHandler/DataEngine.cpp \
    src/IOHandler/IOHandler.cpp \
    src/IOHandler/SQLiteHandler.cpp \
    src/JobType.cpp \
    src/app.cpp

HEADERS += \
    include/AAA.h \
    include/AddInvoice.h \
    include/CustomItemDelegate.h \
    include/InvoiceWindow.h \
    include/MediaBillDetail.h \
    include/MediaPaymentDetail.h \
    include/PaymentReceiptDetail.h \
    include/PopplerInterface.h \
    include/PrintInterface.h \
    include/IOHandler/SQLiteHandler.h \
    include/ReleaseOrder/AddReleaseOrder.h \
    include/UserUI/NewUser.h \
    include/UserUI/UserWindow.h \
    include/AbstractUser.h \
    include/IOHandler/DataEngine.h \
    include/IOHandler/IOHandler.h \
    include/JobType.h \
    include/pdftroninterface.h

INCLUDEPATH += include \
               $$PWD/PDFTron/Headers \
                /usr/include/poppler/

DESTDIR = $$PWD/build/ \

DISTFILES += .gitignore \
            $$PWD/Data/DBMigratingTool/run.py

LIBS += -L$$PWD/PDFTron/Lib  -lPDFNetC -lstdc++ -lpthread -lm -lc -lPDFNetC -lstdc++ -lpthread -lm -lc -lpoppler-qt5


RESOURCES += \
    Resource.qrc

