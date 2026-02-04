#ifndef TRIP_H
#define TRIP_H

#include <QString>
#include <QDate>
#include <QJsonObject>

struct Trip {
    QString from;
    QString to;
    QDate date;
    QString method;
    int travelers;
    double cost;
    QString note;

    QJsonObject toJson() const;
    static Trip fromJson(const QJsonObject &obj);
};

#endif
