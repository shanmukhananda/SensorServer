#ifndef COMMON_PCH_H
#define COMMON_PCH_H

#include <QAbstractVideoBuffer>
#include <QAccelerometer>
#include <QApplication>
#include <QByteArray>
#include <QCamera>
#include <QCameraImageCapture>
#include <QGeoPositionInfoSource>
#include <QGuiApplication>
#include <QGyroscope>
#include <QImageEncoderSettings>
#include <QMainWindow>
#include <QMessageLogger>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickView>
#include <QQuickWidget>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QUdpSocket>
#include <QVideoFilterRunnable>
#include <QVideoProbe>
#include <QtDebug>
#include <QtQml>
#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#endif

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>

#include "app/common/log.h"

#endif
