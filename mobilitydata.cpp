#include "mobilitydata.h"

#define RECEIVE_SUCCESS 0

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

    _mobilityData = new QVector<QJsonObject>;
    _busLine = "";
    initializeAccelerationValues();
    initializeRotationValues();
    initializeCoordinateValues();
    m_currentlyCollecting = false;

    _accelerometer = new QAccelerometer(this);
    connect(_accelerometer, SIGNAL(readingChanged()), this, SLOT(registerAccelerometerReading()));

    _rotationSensor = new QRotationSensor(this);
    connect(_rotationSensor, SIGNAL(readingChanged()), this, SLOT(registerRotationReading()));

    m_accessToPosition = false;
    _source = QGeoPositionInfoSource::createDefaultSource(this);

    if(_source) {
        connect(_source, &QGeoPositionInfoSource::positionUpdated, this, &MobilityData::registerGeolocation);
        connect(_source, &QGeoPositionInfoSource::errorOccurred, this, &MobilityData::handleGeolocationError);
        _source->setUpdateInterval(1000);
    }

    _networkManager = new QNetworkAccessManager(this);

}

void MobilityData::registerAccelerometerReading() {
    QAccelerometerReading *reading = _accelerometer->reading();
    m_accelerationValues.insert("x", reading->x());
    m_accelerationValues.insert("y", reading->y());
    m_accelerationValues.insert("z", reading->z());
    emit accelerationValuesChanged(m_accelerationValues);
}

void MobilityData::registerRotationReading() {
    QRotationReading *reading = _rotationSensor->reading();
    m_rotationValues.insert("x", reading->x());
    m_rotationValues.insert("y", reading->y());
    m_rotationValues.insert("z", reading->z());
    emit rotationValuesChanged(m_rotationValues);
}

void MobilityData::registerGeolocation(const QGeoPositionInfo &geoPositionInfo) {
    double latitude = geoPositionInfo.coordinate().latitude();
    double longitude = geoPositionInfo.coordinate().longitude();

    if(latitude == m_currentCoordinates.value("latitude") &&
       longitude == m_currentCoordinates.value("longitude"))
       return;

    m_currentCoordinates.insert("latitude", latitude);
    m_currentCoordinates.insert("longitude", longitude);
    emit currentCoordinatesChanged(m_currentCoordinates);

    addMobilityDataEntry(m_accelerationValues, m_rotationValues, geoPositionInfo);
}

void MobilityData::addMobilityDataEntry(QVariantMap accelerationValues, QVariantMap rotationValues, QGeoPositionInfo geoPositionInfo) {
    QJsonObject data {
        {"timestamp", geoPositionInfo.timestamp().toString()},
        {"latitude", geoPositionInfo.coordinate().latitude()},
        {"longitude", geoPositionInfo.coordinate().longitude()},
        {"acceleration_x", accelerationValues.value("x").toJsonValue()},
        {"acceleration_y", accelerationValues.value("y").toJsonValue()},
        {"acceleration_z", accelerationValues.value("z").toJsonValue()},
        {"rotation_x", rotationValues.value("x").toJsonValue()},
        {"rotation_y", rotationValues.value("y").toJsonValue()},
        {"rotation_z", rotationValues.value("z").toJsonValue()}
    };

    _mobilityData->append(data);
}

void MobilityData::handleGeolocationError(const QGeoPositionInfoSource::Error error) {

}

void MobilityData::startCollecting() {
    _accelerometer->start();
    _rotationSensor->start();
    _source->startUpdates();

    m_currentlyCollecting = true;
    emit collectionStatusChanged(m_currentlyCollecting);
}

void MobilityData::stopCollecting() {
    _accelerometer->stop();
    _rotationSensor->stop();
    _source->stopUpdates();

    m_currentlyCollecting = false;
    emit collectionStatusChanged(m_currentlyCollecting);
}

bool MobilityData::sendRegisteredData() {
    QByteArray jsonData = getFormattedData();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:5000/receive-mobility-data"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, jsonData.size());

    QNetworkReply *reply = _networkManager->post(request, jsonData);
    if(!reply->waitForReadyRead(10000))
        return false;
    else {
        if(reply->readAll().contains(RECEIVE_SUCCESS)) return true;
    }
    return false;
}

void MobilityData::discardRegisteredData() {
    _mobilityData->clear();
}

QVariantMap MobilityData::getAccelerationValues() {
    return m_accelerationValues;
}

QVariantMap MobilityData::getRotationValues() {
    return m_rotationValues;
}

QVariantMap MobilityData::getCurrentCoordinates() {
    return m_currentCoordinates;
}

void MobilityData::initializeAccelerationValues() {
    m_accelerationValues = {{"x", 0.0}, {"y", 0.0}, {"z", 0.0}};
    emit accelerationValuesChanged(m_accelerationValues);
}

void MobilityData::initializeRotationValues() {
    m_rotationValues = {{"x", 0.0}, {"y", 0.0}, {"z", 0.0}};
    emit rotationValuesChanged(m_rotationValues);
}

void MobilityData::initializeCoordinateValues() {
    m_currentCoordinates = {{"latitude", ""}, {"longitude", ""}};
    emit currentCoordinatesChanged(m_currentCoordinates);
}

void MobilityData::setBusLine(QString busLineId) {
    _busLine = busLineId;
}

QByteArray MobilityData::getFormattedData() {
    QJsonObject data;
    data.insert("bus_line", _busLine);

    QJsonArray points;
    for(const auto& point: *_mobilityData) {
        points.append(point);
    }
    data.insert("points", points);

    QJsonDocument document(data);
    QByteArray formattedData = document.toJson(QJsonDocument::Compact);

    return formattedData;
}

MobilityData::~MobilityData() {
    delete _mobilityData;
    delete _accelerometer;
    delete _rotationSensor;
    delete _networkManager;
}
