#include "common/pch.h"

#include "model/model.h"

Model::Model() {
    _video_filter = std::make_unique<VideoFilter>();
}

Model::~Model() {
}

void Model::start() {
    emit model_initalized(_video_filter.get());
}

void Model::view_initialized(QCamera* camera_) {
    _camera = camera_;
    _camera->start();
    // TODO: Set camera defaults, NO auto focus, camera frame rate, resolution
    // etc
}
