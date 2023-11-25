#include "mobilitydata.h"

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

    _mobilityData = new QVector<QJsonObject>;
    initializeAccelerationValues();
    initializeCoordinateValues();
    m_currentlyCollecting = false;

    _accelerometer = new QAccelerometer(this);

    connect(_accelerometer, SIGNAL(readingChanged()), this, SLOT(registerAccelerometerReading()));

    m_accessToPosition = false;
    _source = QGeoPositionInfoSource::createDefaultSource(this);

    if(_source) {
        m_accessToPosition = true;
        emit accessToPositionChanged(m_accessToPosition);

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
    _accelerometer->start();
    _source->startUpdates();

    m_currentlyCollecting = true;
    emit collectionStatusChanged(m_currentlyCollecting);
}

void MobilityData::stopCollecting() {
    _accelerometer->stop();
    _source->stopUpdates();

    m_currentlyCollecting = false;
    emit collectionStatusChanged(m_currentlyCollecting);
}

QByteArray MobilityData::mobilityDataToJson()
{
    QJsonArray array;
    for(const auto& obj: *_mobilityData) {
        array.append(obj);
    }

    QJsonDocument document(array);
    QByteArray jsonData = document.toJson(QJsonDocument::Compact);

    return jsonData;
}

bool MobilityData::sendRegisteredData() {
    QByteArray jsonData = mobilityDataToJson();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:5000/receive-mobility-data"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, jsonData.size());

    QNetworkReply *reply = _networkManager->post(request, jsonData);
    if(!reply->waitForReadyRead(10000)) {
        return false;
    }
    else {
        if(reply->readAll().contains("success")) return true;
    }
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
    m_accelerationValues = {{"x", "0.0"}, {"y", "0.0"}, {"z", "0.0"}};
    emit accelerationValuesChanged(m_accelerationValues);
}

void MobilityData::initializeCoordinateValues() {
    m_currentCoordinates = {{"latitude", ""}, {"longitude", ""}};
    emit currentCoordinatesChanged(m_currentCoordinates);
}

MobilityData::~MobilityData() {
    delete _mobilityData;
    delete _accelerometer;
    delete _networkManager;
}
