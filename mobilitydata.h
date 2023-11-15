#ifndef MOBILITYDATA_H
#define MOBILITYDATA_H

#include <QObject>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QtPositioning>
#include <QMap>

class MobilityData : public QObject
{
    Q_OBJECT
public:
    explicit MobilityData(QObject *parent = nullptr);

public slots:
    void registerAccelerometerReading(const QAccelerometerReading &reading);
    void registerGeolocation(const QGeoPositionInfo &geolocation);
    void handleGeolocationError(const QGeoPositionInfoSource::Error error);
    bool sendRegisteredData();
    void discardRegisteredData();

    QVariantMap getAccelerationValues();

signals:
    void accelerationValuesChanged(QVariantMap newValues);
    void noAccessToGeolocation();

private:
    QVariantMap _mobilityData;
    QVariantMap _accelerationValues;
    QAccelerometer _accelerometer;
    QGeoPositionInfoSource *_source;
};

#endif // MOBILITYDATA_H
