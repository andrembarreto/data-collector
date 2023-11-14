#include "mobilitydata.h"

MobilityData::MobilityData(QObject *parent)
    : QObject{parent} {

}

void MobilityData::registerAccelerometerReading(const QAccelerometerReading &reading) {

}

void MobilityData::registerGeolocation(const QGeoLocation &geolocation) {

}

bool MobilityData::sendRegisteredData() {
    return false;
}

void MobilityData::discardRegisteredData() {

}
