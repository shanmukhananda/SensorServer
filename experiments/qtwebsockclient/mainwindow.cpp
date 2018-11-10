#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init() {
    webSocket = new QWebSocket();
    webSocket->open(QUrl(QStringLiteral("ws://192.168.1.7:9000")));
    connect(webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(webSocket, SIGNAL(disconnected()), this, SLOT(disonnected()));
}

void MainWindow::onConnected() {
    qDebug() << "connected";
    connect(webSocket, SIGNAL(textMessageReceived(QString)), this,
            SLOT(onTextMessageReceived(QString)));
    connect(webSocket, SIGNAL(binaryMessageReceived(QByteArray)), this,
            SLOT(binaryMessageReceived(QByteArray)));
    webSocket->ping(QByteArray("ping"));
}

void MainWindow::disonnected() {
    qDebug() << "disonnected";
}

void MainWindow::onTextMessageReceived(QString message) {
    webSocket->ping(QByteArray("ping"));
    qDebug() << "onTextMessageReceived";
    qDebug() << "Message received:" << message;
}

void MainWindow::binaryMessageReceived(const QByteArray& message) {
    webSocket->ping(QByteArray("ping"));
    qDebug() << "binaryMessageReceived";
    qDebug() << "Message received len=" << message.size();
}
