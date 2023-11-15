#include "mobilitydata.h"

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

    _accelerometer.setAccelerationMode(QAccelerometer::User);

    QObject::connect(&_accelerometer, &QAccelerometer::readingChanged, this, [this](){
        registerAccelerometerReading(_accelerometer.reading());
    });
}

void MobilityData::registerAccelerometerReading(const QAccelerometerReading &reading) {
    _accelerationValues.insert("x", reading.x());
    _accelerationValues.insert("y", reading.y());
    _accelerationValues.insert("z", reading.z());
    emit accelerationValuesChanged(_accelerationValues);
}

void MobilityData::registerGeolocation(const QGeoLocation &geolocation) {

}

bool MobilityData::sendRegisteredData() {
    return false;
}

void MobilityData::discardRegisteredData() {

}

QVariantMap MobilityData::getAccelerationValues() {
    return _accelerationValues;
}
