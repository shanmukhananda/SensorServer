#include "common/pch.h"

#include "model/model.h"
#include "presenter/presenter.h"
#include "view/view.h"

Presenter::Presenter(std::unique_ptr<Model> model_, std::unique_ptr<View> view_)
    : _model(std::move(model_)), _view(std::move(view_)) {
    auto connected = false;

    // View to Model
    connected = connect(_view.get(), SIGNAL(view_initialized(QCamera*)),
                        _model.get(), SLOT(view_initialized(QCamera*)));
    Q_ASSERT(connected);
    connected = connect(_view.get(), SIGNAL(started(QString, QString)),
                        _model.get(), SLOT(started(QString, QString)));
    Q_ASSERT(connected);

    connected =
        connect(_view.get(), SIGNAL(stopped()), _model.get(), SLOT(stopped()));
    Q_ASSERT(connected);
    connected = connect(_view.get(), SIGNAL(resolution_vga()), _model.get(),
                        SLOT(resolution_vga()));
    Q_ASSERT(connected);
    connected = connect(_view.get(), SIGNAL(resolution_hd()), _model.get(),
                        SLOT(resolution_hd()));
    Q_ASSERT(connected);
    connected = connect(_view.get(), SIGNAL(accelerometer_toggled(bool)),
                        _model.get(), SLOT(accelerometer_toggled(bool)));
    Q_ASSERT(connected);
    connected = connect(_view.get(), SIGNAL(gyroscope_toggled(bool)),
                        _model.get(), SLOT(gyroscope_toggled(bool)));
    Q_ASSERT(connected);
    connected = connect(_view.get(), SIGNAL(gps_toggled(bool)), _model.get(),
                        SLOT(gps_toggled(bool)));
    Q_ASSERT(connected);
    connected = connect(_view.get(), SIGNAL(camera_toggled(bool)), _model.get(),
                        SLOT(camera_toggled(bool)));
    Q_ASSERT(connected);

    // Model to View
    connected =
        connect(_model.get(), SIGNAL(model_initalized(QObject*, Settings*)),
                _view.get(), SLOT(model_initalized(QObject*, Settings*)));
    Q_ASSERT(connected);

    Q_UNUSED(connected);
}

Presenter::~Presenter() {
}

void Presenter::start() {
    _model->start();
    _view->start();
}
