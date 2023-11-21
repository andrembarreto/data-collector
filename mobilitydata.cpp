#include "mobilitydata.h"

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

    _mobilityData = new QVector<QJsonObject>;
    initializeAccelerationValues();
    initializeCoordinateValues();
    m_currentlyCollecting = false;

    _accelerometer.setAccelerationMode(QAccelerometer::User);

    connect(&_accelerometer, &QAccelerometer::readingChanged, this, [this](){
        registerAccelerometerReading(_accelerometer.reading());
    });

    m_accessToPosition = false;
    _source = QGeoPositionInfoSource::createDefaultSource(this);

    if(_source) {
        m_accessToPosition = true;
        emit accessToPositionChanged(m_accessToPosition);

        connect(_source, &QGeoPositionInfoSource::positionUpdated, this, &MobilityData::registerGeolocation);
        connect(_source, &QGeoPositionInfoSource::errorOccurred, this, &MobilityData::handleGeolocationError);
        _source->setUpdateInterval(1000);
    }
}

void MobilityData::registerAccelerometerReading(const QAccelerometerReading &reading) {
    m_accelerationValues.insert("x", reading.x());
    m_accelerationValues.insert("y", reading.y());
    m_accelerationValues.insert("z", reading.z());
    emit accelerationValuesChanged(m_accelerationValues);
}

void MobilityData::registerGeolocation(const QGeoPositionInfo &geolocation) {
    m_currentCoordinates.insert("latitude", geolocation.coordinate().latitude());
    m_currentCoordinates.insert("longitude", geolocation.coordinate().longitude());
    emit currentCoordinatesChanged(m_currentCoordinates);

    QJsonObject data {
        {"timestamp", geolocation.timestamp().toString()},
        {"latitude", geolocation.coordinate().latitude()},
        {"longitude", geolocation.coordinate().longitude()},
        {"acceleration_x", m_accelerationValues.value("x").toJsonValue()},
        {"acceleration_y", m_accelerationValues.value("y").toJsonValue()},
        {"acceleration_z", m_accelerationValues.value("z").toJsonValue()}
    };

    _mobilityData->append(data);
}

void MobilityData::handleGeolocationError(const QGeoPositionInfoSource::Error error) {

}

void MobilityData::startCollecting() {
    _accelerometer.start();
    _source->startUpdates();

    m_currentlyCollecting = true;
    emit collectionStatusChanged(m_currentlyCollecting);
}

void MobilityData::stopCollecting() {
    _accelerometer.stop();
    _source->stopUpdates();

    m_currentlyCollecting = false;
    emit collectionStatusChanged(m_currentlyCollecting);
}

bool MobilityData::sendRegisteredData() {
    return false;
}

void MobilityData::discardRegisteredData() {
    _mobilityData->clear();
}

QVariantMap MobilityData::getAccelerationValues() {
    return m_accelerationValues;
}

QVariantMap MobilityData::getCurrentCoordinates() {
    return m_currentCoordinates;
}

void MobilityData::initializeAccelerationValues() {
    m_accelerationValues = {{"x", ""}, {"y", ""}, {"z", ""}};
    emit accelerationValuesChanged(m_accelerationValues);
}

void MobilityData::initializeCoordinateValues() {
    m_currentCoordinates = {{"latitude", ""}, {"longitude", ""}};
    emit currentCoordinatesChanged(m_currentCoordinates);
}

MobilityData::~MobilityData() {
    delete _mobilityData;
}
