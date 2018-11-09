#include "myserver.h"
#include <QThread>
#include <chrono>
#include <thread>

MyServer::MyServer(QObject* parent) : QObject(parent) {
    socket = new QUdpSocket(this);
    socket->bind(1234);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    transmit();
}

void MyServer::transmit() {
    while (true) {
        QByteArray data;
        data.append("hello from udp");
        socket->writeDatagram(data, QHostAddress::Broadcast, 1234);
        qDebug() << "sending data with len:" << data.size();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void MyServer::readyRead() {
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 sender_port;
    socket->readDatagram(buffer.data(), buffer.size(), &sender, &sender_port);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << sender_port;
    qDebug() << "Message: " << buffer;
}

void MyServer::connected() {
    qDebug() << "connected";
}

void MyServer::disconnected() {
    qDebug() << "disconnected";
}
