#include "trip.h"

QJsonObject Trip::toJson() const {
    QJsonObject obj;
    obj["from"] = from;
    obj["to"] = to;
    obj["date"] = date.toString(Qt::ISODate);
    obj["method"] = method;
    obj["travelers"] = travelers;
    obj["cost"] = cost;
    obj["note"] = note;
    return obj;
}

Trip Trip::fromJson(const QJsonObject &obj) {
    Trip t;
    t.from = obj["from"].toString();
    t.to = obj["to"].toString();
    t.date = QDate::fromString(obj["date"].toString(), Qt::ISODate);
    t.method = obj["method"].toString();
    t.travelers = obj["travelers"].toInt();
    t.cost = obj["cost"].toDouble();
    t.note = obj["note"].toString();
    return t;
}
