#ifndef TRIPMANAGER_H
#define TRIPMANAGER_H

#include <QList>
#include <QString>
#include "trip.h"

class TripManager {
private:
    static QString m_customPath;

public:
    static void setCustomPath(const QString &path);
    static QString getFilePath();
    static void saveTrips(const QList<Trip> &trips);
    static QList<Trip> loadTrips();
};

#endif
