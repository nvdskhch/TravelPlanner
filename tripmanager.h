#ifndef TRIPMANAGER_H
#define TRIPMANAGER_H

#include <QList>
#include "trip.h"

class TripManager {
public:
    static QString getFilePath();
    static void saveTrips(const QList<Trip> &trips);
    static QList<Trip> loadTrips();
};

#endif
