#pragma once

#include "common/pch.h"

#include "model/videofilter.h"

class Model : public QObject {
    Q_OBJECT

public:
    Model();
    ~Model() override;
    void start();

signals:
    void model_initalized(QObject*);

private slots:
    void view_initialized(QCamera*);

private:
    std::unique_ptr<VideoFilter> _video_filter;
    QCamera* _camera;
};
