#ifndef PRICESERVICE_H
#define PRICESERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class PriceService : public QObject {
    Q_OBJECT
    QNetworkAccessManager *manager;

public:
    explicit PriceService(QObject *parent = nullptr);
    void calculatePrice(const QString &fromCity, const QString &toCity, const QString &method, int travelers);

signals:
    void priceReady(double price);
    void errorOccurred(QString error);

private slots:
    void onNetworkReply(QNetworkReply *reply);
};

#endif
