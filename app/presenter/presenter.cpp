#include "common/pch.h"

#include "model/model.h"
#include "presenter/presenter.h"
#include "view/view.h"

Presenter::Presenter(std::unique_ptr<Model> model_, std::unique_ptr<View> view_)
    : _model(std::move(model_)), _view(std::move(view_)) {
    auto connected = false;
    connected = connect(_view.get(), SIGNAL(view_initialized(QCamera*)),
                        _model.get(), SLOT(view_initialized(QCamera*)));
    Q_ASSERT(connected);

    connected = connect(_model.get(), SIGNAL(model_initalized(QObject*)),
                        _view.get(), SLOT(model_initalized(QObject*)));
    Q_ASSERT(connected);

    Q_UNUSED(connected);
}

Presenter::~Presenter() {
}

void Presenter::start() {
    _model->start();
    _view->start();
}
