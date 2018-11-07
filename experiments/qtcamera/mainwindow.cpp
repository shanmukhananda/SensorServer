#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::videoFrameProbed(const QVideoFrame& frame) {
    qDebug() << __FUNCTION__;
    qDebug() << "pixelFormat=" << frame.pixelFormat();
    qDebug() << "width=" << frame.width();
    qDebug() << "height=" << frame.height();
}

void MainWindow::init() {
    qDebug() << "initializing camera";
    camera = new QCamera(QCamera::FrontFace, this);
    camera->setCaptureMode(QCamera::CaptureVideo);

    videoProbe = new QVideoProbe(this);

    if (!videoProbe->setSource(camera)) {
        qDebug() << "Unable to proble";
        return;
    }

    auto connected = false;
    connected = connect(videoProbe, SIGNAL(videoFrameProbed(QVideoFrame)), this,
                        SLOT(videoFrameProbed(QVideoFrame)));

    Q_ASSERT(connected);
    Q_UNUSED(connected);
    qDebug() << "Starting camera";
    camera->start();
}
