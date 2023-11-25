#include "mobilitydata.h"

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

    _mobilityData = new QVector<QJsonObject>;
    initializeAccelerationValues();
    initializeCoordinateValues();
    m_currentlyCollecting = false;

    _accelerometer = new QAccelerometer(this);
    connect(_accelerometer, SIGNAL(readingChanged()), this, SLOT(registerAccelerometerReading()));

    _rotationSensor = new QRotationSensor(this);
    connect(_rotationSensor, SIGNAL(readingChanged()), this, SLOT(registerRotationReading()));

    _orientationSensor = new QOrientationSensor(this);
    connect(_orientationSensor, SIGNAL(readingChanged()), this, SLOT(registerDeviceOrientation()));
    m_currentOrientation = QOrientationReading::Undefined;

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

void MobilityData::registerRotationReading() {
    QRotationReading *reading = _rotationSensor->reading();
    m_rotationValues.insert("x", reading->x());
    m_rotationValues.insert("y", reading->y());
    m_rotationValues.insert("z", reading->z());
    emit rotationValuesChanged(m_rotationValues);
}

void MobilityData::registerDeviceOrientation() {
    QOrientationReading *reading = _orientationSensor->reading();
    m_currentOrientation = reading->orientation();
    emit currentOrientationChanged(deviceOrientationToString(m_currentOrientation));
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

    addMobilityDataEntry(m_accelerationValues, m_rotationValues, m_currentOrientation, geoPositionInfo);
}

void MobilityData::addMobilityDataEntry(QVariantMap accelerationValues, QVariantMap rotationValues,
                                        QOrientationReading::Orientation deviceOrientation, QGeoPositionInfo geoPositionInfo) {
    QJsonObject data {
        {"timestamp", geoPositionInfo.timestamp().toString()},
        {"latitude", geoPositionInfo.coordinate().latitude()},
        {"longitude", geoPositionInfo.coordinate().longitude()},
        {"acceleration_x", accelerationValues.value("x").toJsonValue()},
        {"acceleration_y", accelerationValues.value("y").toJsonValue()},
        {"acceleration_z", accelerationValues.value("z").toJsonValue()},
        {"rotation_x", rotationValues.value("x").toJsonValue()},
        {"rotation_y", rotationValues.value("y").toJsonValue()},
        {"rotation_z", rotationValues.value("z").toJsonValue()},
        {"device_orientation", deviceOrientation}
    };

    _mobilityData->append(data);
}

void MobilityData::handleGeolocationError(const QGeoPositionInfoSource::Error error) {

}

void MobilityData::startCollecting() {
    _accelerometer->start();
    _rotationSensor->start();
    _orientationSensor->start();
    _source->startUpdates();

    m_currentlyCollecting = true;
    emit collectionStatusChanged(m_currentlyCollecting);
}

void MobilityData::stopCollecting() {
    _accelerometer->stop();
    _rotationSensor->stop();
    _orientationSensor->stop();
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

QVariantMap MobilityData::getRotationValues() {
    return m_rotationValues;
}

QString MobilityData::getCurrentOrientation() {
    return deviceOrientationToString(m_currentOrientation);
}

QVariantMap MobilityData::getCurrentCoordinates() {
    return m_currentCoordinates;
}

void MobilityData::initializeAccelerationValues() {
    m_accelerationValues = {{"x", "0.0"}, {"y", "0.0"}, {"z", "0.0"}};
    emit accelerationValuesChanged(m_accelerationValues);
}

void MobilityData::initializeRotationValues() {
    m_rotationValues = {{"x", "0.0"}, {"y", "0.0"}, {"z", "0.0"}};
    emit rotationValuesChanged(m_rotationValues);
}

void MobilityData::initializeCoordinateValues() {
    m_currentCoordinates = {{"latitude", ""}, {"longitude", ""}};
    emit currentCoordinatesChanged(m_currentCoordinates);
}

QString MobilityData::deviceOrientationToString(const QOrientationReading::Orientation &orientation) {
    switch(orientation) {
    case QOrientationReading::TopUp:
        return "Top Up";
    case QOrientationReading::TopDown:
        return "Top Down";
    case QOrientationReading::LeftUp:
        return "Left Up";
    case QOrientationReading::RightUp:
        return "Right Up";
    case QOrientationReading::FaceUp:
        return "Face Up";
    case QOrientationReading::FaceDown:
        return "Face Down";
    default:
        return "Undefined";
    }
}

MobilityData::~MobilityData() {
    delete _mobilityData;
    delete _accelerometer;
    delete _rotationSensor;
    delete _orientationSensor;
    delete _networkManager;
}
