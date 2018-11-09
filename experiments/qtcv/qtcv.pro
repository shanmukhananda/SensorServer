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

TARGET = qtcv
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY = 


android {
    message("configuring android")
    QT += androidextras
    ANDROID_OPENCV=C:/Shanmukha/Shared/Android/OpenCV-android-sdk
    INCLUDEPATH += $$ANDROID_OPENCV/sdk/native/jni/include
    CV_THIRD_PARTY_LIBS_DIR = $$ANDROID_OPENCV/sdk/native/3rdparty/libs/armeabi-v7a
    CV_LIBS_DIR = $$ANDROID_OPENCV/sdk/native/staticlibs/armeabi-v7a
    CV_DYN_LIBS_DIR = $$ANDROID_OPENCV/sdk/native/libs/armeabi-v7a
    LIBS += \
        $$CV_LIBS_DIR/libopencv_calib3d.a \
        $$CV_LIBS_DIR/libopencv_core.a \
        $$CV_LIBS_DIR/libopencv_dnn.a \
        $$CV_LIBS_DIR/libopencv_features2d.a \
        $$CV_LIBS_DIR/libopencv_flann.a \
        $$CV_LIBS_DIR/libopencv_highgui.a \
        $$CV_LIBS_DIR/libopencv_imgcodecs.a \
        $$CV_LIBS_DIR/libopencv_imgproc.a \
        $$CV_LIBS_DIR/libopencv_ml.a \
        $$CV_LIBS_DIR/libopencv_objdetect.a \
        $$CV_LIBS_DIR/libopencv_photo.a \
        $$CV_LIBS_DIR/libopencv_shape.a \
        $$CV_LIBS_DIR/libopencv_stitching.a \
        $$CV_LIBS_DIR/libopencv_superres.a \
        $$CV_LIBS_DIR/libopencv_video.a \
        $$CV_LIBS_DIR/libopencv_videoio.a \
        $$CV_LIBS_DIR/libopencv_videostab.a
    LIBS += \
        $$CV_THIRD_PARTY_LIBS_DIR/libIlmImf.a \
        $$CV_THIRD_PARTY_LIBS_DIR/libcpufeatures.a \
        $$CV_THIRD_PARTY_LIBS_DIR/liblibjasper.a \
        $$CV_THIRD_PARTY_LIBS_DIR/liblibjpeg-turbo.a \
        $$CV_THIRD_PARTY_LIBS_DIR/liblibpng.a \
        $$CV_THIRD_PARTY_LIBS_DIR/liblibprotobuf.a \
        $$CV_THIRD_PARTY_LIBS_DIR/liblibtiff.a \
        $$CV_THIRD_PARTY_LIBS_DIR/liblibwebp.a \
        $$CV_THIRD_PARTY_LIBS_DIR/libtbb.a \
        $$CV_THIRD_PARTY_LIBS_DIR/libtegra_hal.a
    LIBS += \
        $$CV_DYN_LIBS_DIR/libopencv_java3.so

    target.path = /opt/$${TARGET}/bin
    INSTALLS += target
    ANDROID_EXTRA_LIBS += $$CV_DYN_LIBS_DIR/libopencv_java3.so
}

win32 {
    message("configuring win32")
    msvc* {
        message("Configuring win32:msvc*")
        debug {
            message("configuring win32:msvc*:debug")
        }
        release {
            message("configuring win32:msvc*:release")
        }
    }
    g++ {
        message("win32:g++ not configured")
    }
}

for(var, $$list($$enumerate_vars())) {
    message($$var = $$eval($$var))
}
