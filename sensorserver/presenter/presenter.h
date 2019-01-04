#pragma once

#include "sensorserver/common/pch.h"

class View;
class Model;

class Presenter : public QObject {
    Q_OBJECT

public:
    Presenter(std::unique_ptr<Model>, std::unique_ptr<View>);
    ~Presenter() override;
    void run();

signals:
    void update_status(const QString&);

private slots:
    void status(const QString&);

private:
    void init();

    std::unique_ptr<Model> _model;
    std::unique_ptr<View> _view;
};
