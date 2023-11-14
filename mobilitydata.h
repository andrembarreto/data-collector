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

signals:

private:
    QVariantMap _mobilityData;
};

#endif // MOBILITYDATA_H
