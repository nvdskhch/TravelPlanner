#include <QtTest>
#include <QJsonObject>
#include <QFile>
#include "../trip.h"
#include "../tripmanager.h"
#include "../priceservice.h"

class TravelTests : public QObject
{
    Q_OBJECT

private slots:
    void testTripStructure() {
        Trip trip;
        trip.from = "Moscow";
        trip.to = "Paris";
        trip.cost = 50000;
        trip.travelers = 1;
        trip.date = QDate(2026, 1, 1);

        QCOMPARE(trip.from, QString("Moscow"));
        QCOMPARE(trip.to, QString("Paris"));
        QCOMPARE(trip.cost, 50000.0);
        QCOMPARE(trip.travelers, 1);
    }

    void testJsonConversion() {
        Trip trip;
        trip.from = "Dubai";
        trip.to = "London";
        trip.cost = 100.0;
        trip.travelers = 2;
        trip.date = QDate(2026, 5, 20);

        QJsonObject json = trip.toJson();

        QVERIFY(!json.isEmpty());
        QCOMPARE(json["from"].toString(), QString("Dubai"));
        QCOMPARE(json["to"].toString(), QString("London"));
        QCOMPARE(json["travelers"].toInt(), 2);
    }

    void testTripManagerFileOperations() {
        Trip t1;
        t1.from = "TestCity";
        t1.to = "AnotherCity";
        t1.cost = 123.0;
        t1.travelers = 1;
        t1.date = QDate::currentDate();

        QList<Trip> trips;
        trips.append(t1);

        TripManager::saveTrips(trips);

        QString path = TripManager::getFilePath();
        QVERIFY(QFile::exists(path));

        QList<Trip> loadedTrips = TripManager::loadTrips();
        QVERIFY(!loadedTrips.isEmpty());
        QCOMPARE(loadedTrips.first().from, QString("TestCity"));
    }
};

QTEST_APPLESS_MAIN(TravelTests)
#include "tst_traveltests.moc"
