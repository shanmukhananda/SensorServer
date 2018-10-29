#pragma once

#include "common/pch.h"

#include "model/videofilter.h"

class Model : public QObject {
    Q_OBJECT
public:
    Model();
    ~Model() override;
    void start();

private slots:
    void view_initialized(QCamera*);

private:
};
