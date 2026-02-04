#include "priceservice.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>
#include <QNetworkRequest>

PriceService::PriceService(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

void PriceService::calculatePrice(const QString &fromCity, const QString &toCity, const QString &method, int travelers) {
    QString apiUrl = "https://gist.githubusercontent.com/nvdskhch/7b4bd93dd3d4d33c729ae201a2dfeadf/raw/b889c988fa7d4b34b0225cafbdedd40cbec47060/data.json";

    QNetworkRequest request((QUrl(apiUrl)));
    QNetworkReply *reply = manager->get(request);

    reply->setProperty("reqFrom", fromCity);
    reply->setProperty("reqTo", toCity);
    reply->setProperty("reqMethod", method);
    reply->setProperty("reqTravelers", travelers);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onNetworkReply(reply);
    });
}

void PriceService::onNetworkReply(QNetworkReply *reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Ошибка сети: " + reply->errorString());
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);

    if (!doc.isArray()) {
        emit errorOccurred("Ошибка API: Неверный формат данных.");
        return;
    }

    QString fromCity = reply->property("reqFrom").toString();
    QString toCity = reply->property("reqTo").toString();
    QString method = reply->property("reqMethod").toString();
    int travelers = reply->property("reqTravelers").toInt();

    QJsonArray routes = doc.array();
    double foundPrice = -1;

    for (const QJsonValue &value : routes) {
        QJsonObject route = value.toObject();
        bool matchFrom = (route["from"].toString().compare(fromCity, Qt::CaseInsensitive) == 0);
        bool matchTo = (route["to"].toString().compare(toCity, Qt::CaseInsensitive) == 0);
        bool matchType = (route["type"].toString().compare(method, Qt::CaseInsensitive) == 0);

        if (matchFrom && matchTo && matchType) {
            foundPrice = route["price"].toDouble() * travelers;
            break;
        }
    }

    if (foundPrice != -1) {
        emit priceReady(foundPrice);
    } else {
        emit errorOccurred(QString("Маршрут не найден!\nИз %1 в %2 на %3 билетов нет.")
                               .arg(fromCity, toCity, method));
    }
}
