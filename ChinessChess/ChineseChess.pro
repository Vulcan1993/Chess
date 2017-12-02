HEADERS += \
    board.h \
    mainwindow.h \
    dockctrlpanel.h \
    controlpanel.h \
    chessman.h \
    aimode.h \
    netclient.h \
    selectdialog.h \
    stacked.h \
    stackedpage.h \
    rule.h \
    compconfig.h

SOURCES += \
    board.cpp \
    main.cpp \
    mainwindow.cpp \
    dockctrlpanel.cpp \
    controlpanel.cpp \
    aimode.cpp \
    netclient.cpp \
    selectdialog.cpp \
    stacked.cpp \
    stackedpage.cpp \
    rule.cpp

QT += widgets gui network

RESOURCES += \
    chessman1.qrc \
    chessman.qrc \
    chessman2.qrc \
    board.qrc \
    icon.qrc

FORMS += \
    rule.ui
