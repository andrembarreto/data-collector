#ifndef MOBILITYDATA_H
#define MOBILITYDATA_H

#include <QObject>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QRotationSensor>
#include <QRotationReading>
#include <QtPositioning>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#define LOGS_DIR "journey_logs"


class MobilityData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap accelerationValues READ getAccelerationValues NOTIFY accelerationValuesChanged)
    Q_PROPERTY(QVariantMap rotationValues READ getRotationValues NOTIFY rotationValuesChanged)
    Q_PROPERTY(QVariantMap currentCoordinates READ getCurrentCoordinates NOTIFY currentCoordinatesChanged)
    Q_PROPERTY(bool currentlyCollecting MEMBER m_currentlyCollecting NOTIFY collectionStatusChanged)
    Q_PROPERTY(QStringList journeyLogs READ getJourneyLogs CONSTANT)

public:
    explicit MobilityData(QObject *parent = nullptr);
    ~MobilityData();

    QByteArray getFormattedData();

public slots:
    void registerAccelerometerReading();
    void registerRotationReading();
    void registerGeolocation(const QGeoPositionInfo &geolocation);
    void handleGeolocationError(const QGeoPositionInfoSource::Error error);
    bool sendRegisteredData();
    void discardRegisteredData();
    void setBusLine(QString busLineId);
    bool sendLoggedData(int index);

    void startCollecting();
    void stopCollecting();

    QVariantMap getAccelerationValues();
    QVariantMap getRotationValues();
    QVariantMap getCurrentCoordinates();
    QStringList getJourneyLogs();

signals:
    void accelerationValuesChanged(QVariantMap newValues);
    void rotationValuesChanged(QVariantMap newValues);
    void currentCoordinatesChanged(QVariantMap newValues);
    void accessToPositionChanged(bool accessStatus);
    void collectionStatusChanged(bool collectionStatus);

private:
    QVariantMap m_accelerationValues;
    QVariantMap m_rotationValues;
    QVariantMap m_currentCoordinates;
    bool m_currentlyCollecting;
    QStringList m_journeyLogs;

    QVector<QJsonObject> *_mobilityData;
    QAccelerometer *_accelerometer;
    QRotationSensor *_rotationSensor;
    QGeoPositionInfoSource *_source;
    QNetworkAccessManager *_networkManager;
    QString _busLine;

    void initializeAccelerationValues();
    void initializeRotationValues();
    void initializeCoordinateValues();
    void addMobilityDataEntry(QVariantMap accelerationValues, QVariantMap rotationValues, QGeoPositionInfo geoPositionInfo);
    bool _sendData(const QByteArray &data) const;
    void _logRecords();
};

#endif // MOBILITYDATA_H
