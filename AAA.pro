QT += widgets sql printsupport

SOURCES += \
    AAA.cpp \
    AddInvoice.cpp \
    InvoiceWindow.cpp \
    MediaBillDetail.cpp \
    MediaPaymentDetail.cpp \
    PaymentReceiptDetail.cpp \
    PopplerInterface.cpp \
    pdftroninterface.cpp \
    src/ReleaseOrder/AddReleaseOrder.cpp \
    src/ReleaseOrder/roform.cpp \
    src/UserUI/NewUser.cpp \
    src/UserUI/UserWindow.cpp \
    src/IOHandler/DataEngine.cpp \
    src/IOHandler/IOHandler.cpp \
    src/IOHandler/SQLiteHandler.cpp \
    src/JobType.cpp \
    src/app.cpp

HEADERS += \
    AAA.h \
    AddInvoice.h \
    CustomItemDelegate.h \
    InvoiceWindow.h \
    MediaBillDetail.h \
    MediaPaymentDetail.h \
    PaymentReceiptDetail.h \
    PopplerInterface.h \
    PrintInterface.h \
    include/IOHandler/SQLiteHandler.h \
    include/ReleaseOrder/AddReleaseOrder.h \
    include/UserUI/NewUser.h \
    include/UserUI/UserWindow.h \
    include/AbstractUser.h \
    include/IOHandler/DataEngine.h \
    include/IOHandler/IOHandler.h \
    include/JobType.h \
    pdftroninterface.h \
    src/ReleaseOrder/roform.h

INCLUDEPATH += include \
               $$PWD/PDFTron/Headers \
                /usr/include/poppler/

DESTDIR = $$PWD/build/ \

DISTFILES += .gitignore \
            $$PWD/Data/DBMigratingTool/run.py

LIBS += -L$$PWD/PDFTron/Lib  -lPDFNetC -lstdc++ -lpthread -lm -lc -lPDFNetC -lstdc++ -lpthread -lm -lc -lpoppler-qt5


RESOURCES += \
    Resource.qrc

