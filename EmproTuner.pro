#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T03:07:00
#
#-------------------------------------------------

QT += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EmproTuner
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    vocalObject.cpp \
    mdisubwindowcustom.cpp \
    helpers.cpp \
    dialogserialportsettings.cpp \
    serial.cpp \
    dialogchannelchoose.cpp \
    dialogdatalogsettings.cpp \
    dialogsensorsetup.cpp \
    dialogchannelsetup.cpp \
    dialogecusettings.cpp \
    dialogfueltable.cpp \
    ecudata.cpp \
    floattablemodel.cpp \
    floattableview.cpp \
    floattableitemdelegate.cpp \
    verticallabel.cpp \
    Widgets/RoundDial/round_dial.cpp \
    Widgets/Datalog/plotdatalog.cpp \
    Widgets/Datalog/pointlist.cpp \
    Widgets/Datalog/widgetdatalogviewcontrol.cpp \
    Widgets/Datalog/dialogplotchannelchoose.cpp \
    Qne/qneblock.cpp \
    Qne/qneconnection.cpp \
    Qne/qneport.cpp \
    Qne/qneqgraphicsscene.cpp \
    Qne/qneqgraphicsview.cpp \
    Qne/qnodeseditor.cpp \
    Qne/qne.cpp \
    Widgets/tabwidget.cpp

HEADERS  += mainwindow.h \
    vocalObject.h \
    mdisubwindowcustom.h \
    helpers.h \
    dialogserialportsettings.h \
    serial.h \
    dialogchannelchoose.h \
    dialogdatalogsettings.h \
    dialogsensorsetup.h \
    dialogchannelsetup.h \
    dialogecusettings.h \
    dialogfueltable.h \
    ecudata.h \
    floattablemodel.h \
    floattableview.h \
    floattableitemdelegate.h \
    verticallabel.h \
    StopAndWaitCOPS/COBS.h \
    StopAndWaitCOPS/StopAndWait.h \
    Widgets/RoundDial/round_dial.h \
    Widgets/Datalog/plotdatalog.h \
    Widgets/Datalog/pointlist.h \
    Widgets/Datalog/widgetdatalogviewcontrol.h \
    Widgets/Datalog/dialogplotchannelchoose.h \
    Qne/qneblock.h \
    Qne/qneconnection.h \
    Qne/qneport.h \
    Qne/qneqgraphicsscene.h \
    Qne/qneqgraphicsview.h \
    Qne/qnodeseditor.h \
    Qne/qne.h \
    Widgets/tabwidget.h

FORMS    += mainwindow.ui \
    dialogchannelsetup.ui \
    dialogserialportsettings.ui \
    dialogchannelchoose.ui \
    dialogdatalogsettings.ui \
    dialogsensorsetup.ui \
    dialogecusettings.ui \
    dialogfueltable.ui \
    Widgets/Datalog/dialogplotchannelchoose.ui

RESOURCES += \
    resources.qrc \
    qdarkstyle/style.qrc \
    icons.qrc

DISTFILES += \
    uncrustify.cfg \
    uncrustify.exe \
    qdarkstyle/rc/checkbox_indeterminate_disabled.png \
    README.txt
