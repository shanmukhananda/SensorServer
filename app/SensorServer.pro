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

SOURCES += \
    common/pch.cpp \
    main.cpp \
    model/model.cpp \
    model/videofilter.cpp \
    presenter/presenter.cpp \
    view/view.cpp

HEADERS += \
    common/pch.h \
    model/model.h \
    model/videofilter.h \
    presenter/presenter.h \
    view/view.h

FORMS += \
    view/view.ui
	
RESOURCES += \
    resource/qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
