#include "common/pch.h"

#include "model/model.h"
#include "presenter/presenter.h"
#include "view/view.h"

Presenter::Presenter(std::unique_ptr<Model>& model_,
                     std::unique_ptr<View>& view_)
    : _model(model_), _view(view_) {
    auto connected = connect(_view.get(), SIGNAL(view_initialized(QCamera*)),
                             this, SLOT(view_initialized(QCamera*)));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

Presenter::~Presenter() {
}

void Presenter::start() {
    _view->start();
}

void Presenter::view_initialized(QCamera*) {
}
