#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <atomic>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void init();

private slots:
    void onNewConnection();
    void closed();
    void socketDisconnected();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);

private:
    Ui::MainWindow* ui;

    std::atomic<bool> close;
    QWebSocketServer* webserver = nullptr;
    QWebSocket* socket = nullptr;
};

#endif // MAINWINDOW_H
