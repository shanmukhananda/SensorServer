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
    void onConnected();
    void disonnected();
    void onTextMessageReceived(QString message);
    void binaryMessageReceived(const QByteArray& message);

private:
    Ui::MainWindow* ui;
    QWebSocket* webSocket;
};

#endif // MAINWINDOW_H
