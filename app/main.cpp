#include "common/pch.h"

#include "view/mainwindow.h"

class VideoFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame
    run(QVideoFrame*, const QVideoSurfaceFormat&, RunFlags) override;
};

class VideoFilter : public QAbstractVideoFilter {
public:
    QVideoFilterRunnable* createFilterRunnable();
signals:
    void finished(QObject*);
};

QVideoFrame
VideoFilterRunnable::run(QVideoFrame* frame, const QVideoSurfaceFormat&,
                         QVideoFilterRunnable::RunFlags) {
    static std::size_t count = 0;
    qDebug()
        << "Received frame:" << ++count
        << "pixelformat=" << frame->pixelFormat() << "size=" << frame->size();
    return *frame;
}

QVideoFilterRunnable* VideoFilter::createFilterRunnable() {
    return new VideoFilterRunnable();
}

void run(int argc, char** argv) {
    qmlRegisterType<VideoFilter>("SensorServer", 1, 0, "VideoFilter");

    auto app = std::make_unique<QApplication>(argc, argv);
    auto window = std::make_unique<MainWindow>();
    window->show();
    app->exec();
}

int main(int argc, char** argv) {
    try {
        run(argc, argv);
    } catch (std::exception& e) {
        std::cout << std::endl << "caught exception" << std::endl;
        std::cout << "type:" << typeid(e).name() << std::endl;
        std::cout << "what:" << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cout << std::endl << "caught unknown exception" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
