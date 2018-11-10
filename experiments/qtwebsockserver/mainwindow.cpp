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
    webserver = new QWebSocketServer(QStringLiteral("Qt WebServer"),
                                     QWebSocketServer::NonSecureMode, this);

    if (webserver->listen(QHostAddress("192.168.1.7"), 9000)) {
        connect(webserver, SIGNAL(newConnection()), this,
                SLOT(onNewConnection()));
        connect(webserver, SIGNAL(closed()), this, SLOT(closed()));
    } else {
        qDebug() << "unable to listen";
    }
}

void MainWindow::onNewConnection() {
    qDebug() << "onNewConnection";
    close = false;
    socket = webserver->nextPendingConnection();

    connect(socket, SIGNAL(textMessageReceived(QString)), this,
            SLOT(processTextMessage(QString)));
    connect(socket, SIGNAL(binaryMessageReceived(QByteArray)), this,
            SLOT(processBinaryMessage(QByteArray)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    while (!close) {
        qDebug() << "writing to socket";
        try {
            auto large_data = QByteArray(3687400, 'i');
            socket->sendBinaryMessage(QByteArray("binary message"));
            socket->sendTextMessage(QString("text message"));
            socket->sendBinaryMessage(large_data);
            socket->flush();
        } catch (std::exception& e) {
            qDebug() << "caught exception:" << e.what();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}

void MainWindow::closed() {
    qDebug() << "closed";
}

void MainWindow::socketDisconnected() {
    qDebug() << "socketDisconnected";
    close = true;
}

void MainWindow::processTextMessage(QString) {
    qDebug() << "processTextMessage";
}

void MainWindow::processBinaryMessage(QByteArray) {
    qDebug() << "processBinaryMessage";
}
