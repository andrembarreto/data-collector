#ifndef MOBILITYDATA_H
#define MOBILITYDATA_H

#include <QObject>
#include <QAccelerometer>
#include <QAccelerometerReading>
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
    Q_PROPERTY(QVariantMap currentCoordinates READ getCurrentCoordinates NOTIFY currentCoordinatesChanged)
    Q_PROPERTY(bool accessToPosition MEMBER m_accessToPosition NOTIFY accessToPositionChanged)
    Q_PROPERTY(bool currentlyCollecting MEMBER m_currentlyCollecting NOTIFY collectionStatusChanged)
public:
    explicit MobilityData(QObject *parent = nullptr);
    ~MobilityData();

public slots:
    void registerAccelerometerReading();
    void registerGeolocation(const QGeoPositionInfo &geolocation);
    void handleGeolocationError(const QGeoPositionInfoSource::Error error);
    bool sendRegisteredData();
    void discardRegisteredData();

    void startCollecting();
    void stopCollecting();

    QVariantMap getAccelerationValues();
    QVariantMap getCurrentCoordinates();

signals:
    void accelerationValuesChanged(QVariantMap newValues);
    void currentCoordinatesChanged(QVariantMap newValues);
    void accessToPositionChanged(bool accessStatus);
    void collectionStatusChanged(bool collectionStatus);

private:
    QVector<QJsonObject> *_mobilityData;
    QVariantMap m_accelerationValues;
    QVariantMap m_currentCoordinates;
    QAccelerometer *_accelerometer;
    QGeoPositionInfoSource *_source;
    QNetworkAccessManager *_networkManager;

    bool m_accessToPosition;
    bool m_currentlyCollecting;

    void initializeAccelerationValues();
    void initializeCoordinateValues();
};

#endif // MOBILITYDATA_H
