QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ./controller/controller.cc \
    ./model/bankcalculator.cc \
    ./model/calendar.cc \
    ./model/expression.cc \
    ./model/parcer.cc \
    main.cc \
    ./3dparty/qcustomplot.cpp \
    ./view/view.cc

HEADERS += \
    ./controller/controller.h \
    ./model/bankcalculator.h \
    ./model/calendar.h \
    ./model/expression.h \
    ./model/parcer.h \
    ./3dparty/qcustomplot.h \
    ./view/view.h

FORMS += \
    ./view/view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS += ./3dparty/icon.png

RESOURCES += \
    ./view/res.qrc
