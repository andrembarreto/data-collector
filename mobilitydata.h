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
    void registerGeolocation(const QGeoLocation &geolocation);
    bool sendRegisteredData();
    void discardRegisteredData();

    QVariantMap getAccelerationValues();

signals:
    void accelerationValuesChanged(QVariantMap newValues);

private:
    QVariantMap _mobilityData;
    QVariantMap _accelerationValues;
    QAccelerometer _accelerometer;
};

#endif // MOBILITYDATA_H
