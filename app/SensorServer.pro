QT += \
    core \
    gui \
    network \
    widgets \
    multimedia \
    sensors \
    quick \
    quickwidgets \
    qml \
    positioning

TARGET = SensorServer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++14 \
    mobility

SOURCES += \
    main.cpp \
    view/mainwindow.cpp \
    common/pch.cpp

HEADERS += \
    view/mainwindow.h \
    common/pch.h

FORMS += \
    view/mainwindow.ui
	
RESOURCES += \
    resource/qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

