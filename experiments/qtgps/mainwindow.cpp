#include "mainwindow.h"
#include <QGeoPositionInfoSource>
#include <QtDebug>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();
}

void MainWindow::init() {
    QGeoPositionInfoSource* source =
        QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this,
                SLOT(positionUpdated(QGeoPositionInfo)));
        source->startUpdates();
    }
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::positionUpdated(const QGeoPositionInfo& info) {
    qDebug() << "Position updated:" << info;
    qDebug() << "VerticalAccuracy:"
             << info.hasAttribute(QGeoPositionInfo::VerticalAccuracy);
}
