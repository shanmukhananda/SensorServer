#pragma once

#include "common/pch.h"

class View;
class Model;

class Presenter : public QObject {
    Q_OBJECT
public:
    Presenter(std::unique_ptr<Model>&, std::unique_ptr<View>&);
    ~Presenter() override;
    void start();

private slots:
    void view_initialized(QCamera*);

private:
    std::unique_ptr<Model>& _model;
    std::unique_ptr<View>& _view;
};
