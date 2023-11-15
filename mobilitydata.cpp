#include "mobilitydata.h"

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

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

}

bool MobilityData::sendRegisteredData() {
    return false;
}

void MobilityData::discardRegisteredData() {

}

QVariantMap MobilityData::getAccelerationValues() {
    return _accelerationValues;
}
