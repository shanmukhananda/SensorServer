#include "app/common/pch.h"

#include "app/model/model.h"
#include "app/presenter/presenter.h"
#include "app/view/view.h"

Presenter::Presenter(std::unique_ptr<Model> model_,
                     std::unique_ptr<View> view_) {
    LOG_SCOPE;
    _model = std::move(model_);
    _view = std::move(view_);
    init();
}

Presenter::~Presenter() {
    LOG_SCOPE;
}

void Presenter::init() {
    LOG_SCOPE;
    auto connected = false;

    // View to Model
    connected = connect(_view.get(), SIGNAL(view_initialized(QCamera*)),
                        _model.get(), SLOT(view_initialized(QCamera*)));
    Q_ASSERT(connected);
    connected =
        connect(_view.get(), SIGNAL(start_sensor_server(QString, QString)),
                _model.get(), SLOT(started_sensor_server(QString, QString)));
    Q_ASSERT(connected);

    connected = connect(_view.get(), SIGNAL(stop_sensor_server()), _model.get(),
                        SLOT(stopped_sensor_server()));
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
    connected = connect(_model.get(), SIGNAL(model_initialized(Settings*)),
                        _view.get(), SLOT(model_initialized(Settings*)));
    Q_ASSERT(connected);

    // Model to Presenter
    connected = connect(_model.get(), SIGNAL(status(QString)), this,
                        SLOT(status(QString)));
    Q_ASSERT(connected);

    // Presenter to View
    connected = connect(this, SIGNAL(update_status(QString)), _view.get(),
                        SLOT(update_status(QString)));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

void Presenter::run() {
    LOG_SCOPE;
    _model->run();
}

void Presenter::status(const QString& status_) {
    LOG_SCOPE;
    auto status = status_.toUpper();
    emit update_status(status);
}
