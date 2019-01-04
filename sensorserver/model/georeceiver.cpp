#include "sensorserver/common/pch.h"

#include "sensorserver/common/settings.h"
#include "sensorserver/model/georeceiver.h"
#include "sensorserver/model/sensordata.h"

GeoReceiver::GeoReceiver() {
    LOG_SCOPE;
    init();
}

void GeoReceiver::init() {
    LOG_SCOPE;
    _geo_source = QGeoPositionInfoSource::createDefaultSource(this);

    if (_geo_source) {
        auto connected = false;
        connected =
            connect(_geo_source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                    this, SLOT(position_updated(QGeoPositionInfo)));

        Q_ASSERT(connected);
        Q_UNUSED(connected);
    }
}

GeoReceiver::~GeoReceiver() {
    LOG_SCOPE;
}

void GeoReceiver::start_reception(Settings* settings_) {
    LOG_SCOPE;
    if (_geo_source) {
        if (settings_->is_gps_enabled())
            _geo_source->startUpdates();
        else
            _geo_source->stopUpdates();
    }
}

void GeoReceiver::stop_reception() {
    LOG_SCOPE;
    if (_geo_source)
        _geo_source->stopUpdates();
}

void GeoReceiver::position_updated(const QGeoPositionInfo& position_) {
    // LOG_SCOPE;
    auto timestamp = QDateTime::currentMSecsSinceEpoch();
    Q_ASSERT(timestamp > 0);

    auto geo_data = std::make_shared<GeodeticData>();
    geo_data->latitude = position_.coordinate().latitude();
    geo_data->longitude = position_.coordinate().longitude();
    geo_data->altitude = position_.coordinate().altitude();

    if (position_.hasAttribute(QGeoPositionInfo::Direction))
        geo_data->direction = position_.attribute(QGeoPositionInfo::Direction);

    if (position_.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
        geo_data->horizontal_accuracy =
            position_.attribute(QGeoPositionInfo::HorizontalAccuracy);

    if (position_.hasAttribute(QGeoPositionInfo::GroundSpeed))
        geo_data->ground_speed =
            position_.attribute(QGeoPositionInfo::GroundSpeed);
}
