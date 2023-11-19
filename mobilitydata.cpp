#include "mobilitydata.h"

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

    _mobilityData = new QVector<QJsonObject>;

    _accelerometer.setAccelerationMode(QAccelerometer::User);

    connect(&_accelerometer, &QAccelerometer::readingChanged, this, [this](){
        registerAccelerometerReading(_accelerometer.reading());
    });

    _source = QGeoPositionInfoSource::createDefaultSource(this);

    if(_source) {
        connect(_source, &QGeoPositionInfoSource::positionUpdated, this, &MobilityData::registerGeolocation);
        connect(_source, &QGeoPositionInfoSource::errorOccurred, this, &MobilityData::handleGeolocationError);
        _source->setUpdateInterval(1000);
    }
    else {
        emit noAccessToGeolocation();
    }
}

void MobilityData::registerAccelerometerReading(const QAccelerometerReading &reading) {
    _accelerationValues.insert("x", reading.x());
    _accelerationValues.insert("y", reading.y());
    _accelerationValues.insert("z", reading.z());
    emit accelerationValuesChanged(_accelerationValues);
}

void MobilityData::registerGeolocation(const QGeoPositionInfo &geolocation) {
    QJsonObject data {
        {"timestamp", geolocation.timestamp().toString()},
        {"latitude", geolocation.coordinate().latitude()},
        {"longitude", geolocation.coordinate().longitude()},
        {"acceleration_x", _accelerationValues.value("x").toJsonValue()},
        {"acceleration_y", _accelerationValues.value("y").toJsonValue()},
        {"acceleration_z", _accelerationValues.value("z").toJsonValue()}
    };

    _mobilityData->append(data);
}

void MobilityData::handleGeolocationError(const QGeoPositionInfoSource::Error error) {

}

void MobilityData::startCollecting() {
    _accelerometer.start();
    _source->startUpdates();
}

void MobilityData::stopCollecting() {
    _accelerometer.stop();
    _source->stopUpdates();
}

bool MobilityData::sendRegisteredData() {
    return false;
}

void MobilityData::discardRegisteredData() {
    _mobilityData->clear();
}

QVariantMap MobilityData::getAccelerationValues() {
    return _accelerationValues;
}

MobilityData::~MobilityData() {
    delete _mobilityData;
}
