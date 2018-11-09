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
    mobility \
    precompile_header

PRECOMPILED_HEADER = app/common/pch.h

SOURCES += \
    app/common/pch.cpp \
    app/common/settings.cpp \
    app/main.cpp \
    app/model/camerareceiver.cpp \
    app/model/georeceiver.cpp \
    app/model/imureceiver.cpp \
    app/model/model.cpp \
    app/model/sensordata.cpp \
    app/model/tcpsender.cpp \
    app/model/transmitter.cpp \
    app/presenter/presenter.cpp \
    app/view/view.cpp \
    app/model/udpsender.cpp

HEADERS += \
    app/common/log.h \
    app/common/pch.h \
    app/common/settings.h \
    app/model/camerareceiver.h \
    app/model/georeceiver.h \
    app/model/imureceiver.h \
    app/model/model.h \
    app/model/sensordata.h \
    app/model/tcpsender.h \
    app/model/transmitter.h \
    app/presenter/presenter.h \
    app/view/view.h \
    app/model/udpsender.h \
    app/model/sender.h

FORMS += \
    app/view/view.ui

RESOURCES += \
    app/resource/qml.qrc

android {
    message("configuring android")
    QT += androidextras
    debug {
        message("configuring android:debug")
        INCLUDEPATH += $$PWD/vendor/installed/armeabi-v7a-android-debug/include
        LIBS += -L"$$PWD/vendor/installed/armeabi-v7a-android-debug/lib/liblatbuffers.a"
    }
    release {
        message("configuring android:release")
        INCLUDEPATH += $$PWD/vendor/installed/armeabi-v7a-android-release/include
        LIBS += -L"$$PWD/vendor/installed/armeabi-v7a-android-release/lib/libflatbuffers.a"
    }
    target.path = /opt/$${TARGET}/bin
    INSTALLS += target
}

win32 {
    message("configuring win32")
    msvc* {
        message("Configuring win32:msvc*")
        debug {
            message("configuring win32:msvc*:debug")
            INCLUDEPATH += $$PWD/vendor/installed/msvc-2017-x64-windows-debug/include
            LIBS += -L"$$PWD/vendor/installed/msvc-2017-x64-windows-debug/lib/liblatbuffers.a"
        }
        release {
            message("configuring win32:msvc*:release")
            INCLUDEPATH += $$PWD/vendor/installed/msvc-2017-x64-windows-release/include
            LIBS += -L"$$PWD/vendor/installed/msvc-2017-x64-windows-release/lib/libflatbuffers.a"
        }
    }
    g++ {
        message("win32:g++ not configured")
    }
}

for(var, $$list($$enumerate_vars())) {
    message($$var = $$eval($$var))
}
