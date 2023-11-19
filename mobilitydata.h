#ifndef MOBILITYDATA_H
#define MOBILITYDATA_H

#include <QObject>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QtPositioning>
#include <QMap>
#include <QJsonObject>

class MobilityData : public QObject
{
    Q_OBJECT
public:
    explicit MobilityData(QObject *parent = nullptr);
    ~MobilityData();

public slots:
    void registerAccelerometerReading(const QAccelerometerReading &reading);
    void registerGeolocation(const QGeoPositionInfo &geolocation);
    void handleGeolocationError(const QGeoPositionInfoSource::Error error);
    bool sendRegisteredData();
    void discardRegisteredData();

    void startCollecting();
    void stopCollecting();

    QVariantMap getAccelerationValues();

signals:
    void accelerationValuesChanged(QVariantMap newValues);
    void noAccessToGeolocation();

private:
    QVector<QJsonObject> *_mobilityData;
    QVariantMap _accelerationValues;
    QAccelerometer _accelerometer;
    QGeoPositionInfoSource *_source;

    bool _accessToPosition;
};

#endif // MOBILITYDATA_H
