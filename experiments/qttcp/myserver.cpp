#include "myserver.h"
#include <QThread>
#include <chrono>
#include <thread>

MyServer::MyServer(QObject* parent) : QObject(parent) {
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    QHostAddress ip_add;
    ip_add.setAddress("127.0.0.1");
    if (!server->listen(ip_add, 1234)) {
        qDebug() << QThread::currentThreadId() << ":Server could not start";
    } else {
        qDebug() << QThread::currentThreadId() << ":Server started";
    }
}

void MyServer::ClientDisconnected() {
    qDebug() << QThread::currentThreadId() << ":ClientDisconnected";
    close = true;
}

void MyServer::newConnection() {
    qDebug() << QThread::currentThreadId() << ":New Connection recieved";
    close = false;
    QTcpSocket* csocket = server->nextPendingConnection();
    connect(csocket, &QTcpSocket::disconnected, this,
            &MyServer::ClientDisconnected);

    while (!close) {
        csocket->write("HelloClient\n");
        csocket->flush();
        csocket->waitForBytesWritten();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    qDebug() << QThread::currentThreadId() << ":closing socket";
    csocket->close();
}
