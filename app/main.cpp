#include "app/common/pch.h"

#include "app/model/model.h"
#include "app/model/sensordata.h"
#include "app/model/videofilter.h"
#include "app/presenter/presenter.h"
#include "app/view/view.h"

void keep_screen_on() {
    LOG_SCOPE;
#ifdef Q_OS_ANDROID
    auto on = true;
    QtAndroid::runOnAndroidThread([on] {
        auto activity = QtAndroid::androidActivity();
        if (!activity.isValid())
            return;

        auto window = activity.callObjectMethod("getWindow",
                                                "()Landroid/"
                                                "view/"
                                                "Window;");

        if (!window.isValid())
            return;

        const auto FLAG_KEEP_SCREEN_ON = 128;
        if (on)
            window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        else
            window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);

        QAndroidJniEnvironment env;
        if (env->ExceptionCheck())
            env->ExceptionClear();
    });
#endif
}

void run(int argc, char** argv) {
    LOG_SCOPE;
    qSetMessagePattern("[%{threadid}][%{function}][%{line}]: %{message}");
    qmlRegisterType<VideoFilter>("SensorServer", 1, 0, "VideoFilter");
    qRegisterMetaType<std::shared_ptr<SensorData>>(
        "std::shared_ptr<SensorData>");

    keep_screen_on();

    auto app = std::make_unique<QApplication>(argc, argv);

    auto model = std::make_unique<Model>();
    auto view = std::make_unique<View>();
    auto presenter =
        std::make_unique<Presenter>(std::move(model), std::move(view));
    presenter->run();

    app->exec();
}

int main(int argc, char** argv) {
    LOG_SCOPE;
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
