#ifndef MYSERVER_H
#define MYSERVER_H

#include <QDebug>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <atomic>

class MyServer : public QObject {
    Q_OBJECT
public:
    explicit MyServer(QObject* parent = nullptr);

signals:

public slots:
    void newConnection();
    void ClientDisconnected();

private:
    QTcpServer* server;
    std::atomic<bool> close{false};
};

#endif // MYSERVER_H
