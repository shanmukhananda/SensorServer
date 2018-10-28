#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGeoPositionInfo;
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void init();
private slots:
    void positionUpdated(const QGeoPositionInfo& info);

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
