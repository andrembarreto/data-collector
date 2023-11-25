#ifndef MOBILITYDATA_H
#define MOBILITYDATA_H

#include <QObject>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QRotationSensor>
#include <QRotationReading>
#include <QOrientationSensor>
#include <QOrientationReading>
#include <QtPositioning>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class MobilityData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap accelerationValues READ getAccelerationValues NOTIFY accelerationValuesChanged)
    Q_PROPERTY(QVariantMap rotationValues READ getRotationValues NOTIFY rotationValuesChanged)
    Q_PROPERTY(QVariantMap currentCoordinates READ getCurrentCoordinates NOTIFY currentCoordinatesChanged)
    Q_PROPERTY(QString currentOrientation READ getCurrentOrientation NOTIFY currentOrientationChanged)
    Q_PROPERTY(bool accessToPosition MEMBER m_accessToPosition NOTIFY accessToPositionChanged)
    Q_PROPERTY(bool currentlyCollecting MEMBER m_currentlyCollecting NOTIFY collectionStatusChanged)
public:
    explicit MobilityData(QObject *parent = nullptr);
    ~MobilityData();

    QByteArray mobilityDataToJson();

public slots:
    void registerAccelerometerReading();
    void registerRotationReading();
    void registerDeviceOrientation();
    void registerGeolocation(const QGeoPositionInfo &geolocation);
    void handleGeolocationError(const QGeoPositionInfoSource::Error error);
    bool sendRegisteredData();
    void discardRegisteredData();

    void startCollecting();
    void stopCollecting();

    QVariantMap getAccelerationValues();
    QVariantMap getRotationValues();
    QVariantMap getCurrentCoordinates();
    QString getCurrentOrientation();

signals:
    void accelerationValuesChanged(QVariantMap newValues);
    void rotationValuesChanged(QVariantMap newValues);
    void currentCoordinatesChanged(QVariantMap newValues);
    void currentOrientationChanged(QString newOrientation);
    void accessToPositionChanged(bool accessStatus);
    void collectionStatusChanged(bool collectionStatus);

private:
    QVariantMap m_accelerationValues;
    QVariantMap m_rotationValues;
    QVariantMap m_currentCoordinates;
    QOrientationReading::Orientation m_currentOrientation;
    bool m_accessToPosition;
    bool m_currentlyCollecting;

    QVector<QJsonObject> *_mobilityData;
    QAccelerometer *_accelerometer;
    QRotationSensor *_rotationSensor;
    QOrientationSensor *_orientationSensor;
    QGeoPositionInfoSource *_source;
    QNetworkAccessManager *_networkManager;

    void initializeCollectionValues();
    void initializeAccelerationValues();
    void initializeRotationValues();
    void initializeCoordinateValues();
    QString deviceOrientationToString(QOrientationReading::Orientation orientation);
};

#endif // MOBILITYDATA_H
