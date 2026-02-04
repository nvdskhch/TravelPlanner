#include "tripmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>

QString TripManager::getFilePath() {
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/trips_v4.json";
}

void TripManager::saveTrips(const QList<Trip> &trips) {
    QJsonArray arr;
    for (const auto &t : trips) {
        arr.append(t.toJson());
    }
    QJsonDocument doc(arr);
    QFile file(getFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

QList<Trip> TripManager::loadTrips() {
    QList<Trip> trips;
    QFile file(getFilePath());
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray arr = doc.array();
        for (const auto &val : arr) {
            trips.append(Trip::fromJson(val.toObject()));
        }
    }
    return trips;
}
