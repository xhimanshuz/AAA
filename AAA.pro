QT += widgets sql

SOURCES += \
    AAA.cpp \
    AddGeneratebill.cpp \
    GenerateBillWindow.cpp \
    MediaBillDetail.cpp \
    PaymentDetail.cpp \
    ReceiptDetail.cpp \
    src/ReleaseOrder/AddReleaseOrder.cpp \
    src/UserUI/NewUser.cpp \
    src/UserUI/UserWindow.cpp \
    src/IOHandler/DataEngine.cpp \
    src/IOHandler/IOHandler.cpp \
    src/IOHandler/SQLiteHandler.cpp \
    src/JobType.cpp \
    src/app.cpp

HEADERS += \
    AAA.h \
    AddGeneratebill.h \
    GenerateBillWindow.h \
    MediaBillDetail.h \
    PaymentDetail.h \
    ReceiptDetail.h \
    include/IOHandler/SQLiteHandler.h \
    include/ReleaseOrder/AddReleaseOrder.h \
    include/UserUI/NewUser.h \
    include/UserUI/UserWindow.h \
    include/AbstractUser.h \
    include/IOHandler/DataEngine.h \
    include/IOHandler/IOHandler.h \
    include/JobType.h

INCLUDEPATH += include

DESTDIR = $$PWD/build/ \

DISTFILES += .gitignore \
            $$PWD/Data/DBMigratingTool/run.py

