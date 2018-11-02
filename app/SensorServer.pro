QT += \
    core \
    gui \
    multimedia \
    network \
    positioning \
    qml \
    quick \
    quickwidgets \
    sensors \
    widgets

TARGET = SensorServer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += \
    c++14 \
    mobility

CONFIG += common/pch.h
PRECOMPILED_HEADER = common/pch.h
win32-msvc* {
    PRECOMPILED_SOURCE = common/pch.cpp
}

SOURCES += \
    common/pch.cpp \
    common/settings.cpp \
    main.cpp \
    model/model.cpp \
    model/receiver.cpp \
    model/videofilter.cpp \
    presenter/presenter.cpp \
    view/view.cpp \
    model/sensordata.cpp \
    model/transmitter.cpp

HEADERS += \
    common/pch.h \
    common/settings.h \
    model/model.h \
    model/receiver.h \
    model/videofilter.h \
    presenter/presenter.h \
    view/view.h \
    model/sensordata.h \
    model/transmitter.h

FORMS += \
    view/view.ui
	
RESOURCES += \
    resource/qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
