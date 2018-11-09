#ifndef MYSERVER_H
#define MYSERVER_H

#include <QDebug>
#include <QObject>
#include <QUdpSocket>
#include <atomic>

class MyServer : public QObject {
    Q_OBJECT
public:
    explicit MyServer(QObject* parent = nullptr);
    void transmit();
signals:

public slots:
    void readyRead();
    void connected();
    void disconnected();

private:
    QUdpSocket* socket{nullptr};
    std::atomic<bool> close{false};
};

#endif // MYSERVER_H
