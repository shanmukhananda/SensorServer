QT += \
    core \
    gui \
    multimedia \
    multimedia-private \
    network \
    positioning \
    qml \
    quick \
    quickwidgets \
    sensors \
    websockets \
    widgets

TARGET = SensorServer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += \
    c++14 \
    mobility \
    precompile_header

PRECOMPILED_HEADER = sensorserver/common/pch.h

SOURCES += \
    sensorserver/common/pch.cpp \
    sensorserver/common/settings.cpp \
    sensorserver/main.cpp \
    sensorserver/model/camerareceiver.cpp \
    sensorserver/model/compressor.cpp \
    sensorserver/model/georeceiver.cpp \
    sensorserver/model/imureceiver.cpp \
    sensorserver/model/model.cpp \
    sensorserver/model/sensordata.cpp \
    sensorserver/model/tcpsender.cpp \
    sensorserver/model/transmitter.cpp \
    sensorserver/model/udpsender.cpp \
    sensorserver/model/wssender.cpp \
    sensorserver/presenter/presenter.cpp \
    sensorserver/view/view.cpp

HEADERS += \
    sensorserver/common/log.h \
    sensorserver/common/pch.h \
    sensorserver/common/settings.h \
    sensorserver/model/camerareceiver.h \
    sensorserver/model/compressor.h \
    sensorserver/model/georeceiver.h \
    sensorserver/model/imureceiver.h \
    sensorserver/model/model.h \
    sensorserver/model/sender.h \
    sensorserver/model/sensordata.h \
    sensorserver/model/tcpsender.h \
    sensorserver/model/transmitter.h \
    sensorserver/model/udpsender.h \
    sensorserver/model/wssender.h \
    sensorserver/presenter/presenter.h \
    sensorserver/view/view.h

FORMS += \
    sensorserver/view/view.ui

RESOURCES += \
    sensorserver/resource/qml.qrc

android {
    message("configuring android")
    QT += androidextras
    debug {
        message("configuring android:debug")
        INCLUDEPATH += $$PWD/.vendor/installed/armeabi-v7a-android-debug/include
        LIBS += -L"$$PWD/.vendor/installed/armeabi-v7a-android-debug/lib/libflatbuffers.a"
    }
    release {
        message("configuring android:release")
        INCLUDEPATH += $$PWD/.vendor/installed/armeabi-v7a-android-release/include
        LIBS += -L"$$PWD/.vendor/installed/armeabi-v7a-android-release/lib/libflatbuffers.a"
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
            INCLUDEPATH += $$PWD/.vendor/installed/msvc-2017-x64-windows-debug/include
            LIBS += -L"$$PWD/.vendor/installed/msvc-2017-x64-windows-debug/lib/libflatbuffers.a"
        }
        release {
            message("configuring win32:msvc*:release")
            INCLUDEPATH += $$PWD/.vendor/installed/msvc-2017-x64-windows-release/include
            LIBS += -L"$$PWD/.vendor/installed/msvc-2017-x64-windows-release/lib/libflatbuffers.a"
        }
    }
    g++ {
        message("win32:g++ not configured")
    }
}

for(var, $$list($$enumerate_vars())) {
    message($$var = $$eval($$var))
}
