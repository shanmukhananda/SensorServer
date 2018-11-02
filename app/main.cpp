#include "common/pch.h"

#include "model/model.h"
#include "model/videofilter.h"
#include "presenter/presenter.h"
#include "view/view.h"

void run(int argc, char** argv) {
    qmlRegisterType<VideoFilter>("SensorServer", 1, 0, "VideoFilter");

    auto app = std::make_unique<QApplication>(argc, argv);

    auto model = std::make_unique<Model>();
    auto view = std::make_unique<View>();
    auto presenter =
        std::make_unique<Presenter>(std::move(model), std::move(view));
    presenter->run();

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
