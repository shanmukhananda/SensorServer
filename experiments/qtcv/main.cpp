#include <QApplication>
#include "mainwindow.h"

#include <QCamera>
#include <QtDebug>

#include "opencv/cv.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    auto cap_default = cv::VideoCapture(CV_CAP_ANDROID);

    if (!cap_default.isOpened())
        qDebug() << "unable to open CV_CAP_ANDROID";

    auto cap_back = cv::VideoCapture(CV_CAP_ANDROID_BACK);

    if (!cap_back.isOpened())
        qDebug() << "unable to open CV_CAP_ANDROID_BACK";

    auto cap_front = cv::VideoCapture(CV_CAP_ANDROID_FRONT);

    if (!cap_front.isOpened())
        qDebug() << "unable to open CV_CAP_ANDROID_FRONT";

    return a.exec();
}
