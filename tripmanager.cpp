#include "tripmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

QString TripManager::m_customPath = "";

void TripManager::setCustomPath(const QString &path) {
    m_customPath = path;
}

QString TripManager::getFilePath() {
    if (!m_customPath.isEmpty()) {
        return m_customPath;
    }
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/trips_v4.json";
}

void TripManager::saveTrips(const QList<Trip> &trips) {
    QJsonArray arr;
    for (const auto &t : trips) {
        arr.append(t.toJson());
    }
    QJsonDocument doc(arr);

    QString path = getFilePath();

    QFileInfo fileInfo(path);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(path);
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
