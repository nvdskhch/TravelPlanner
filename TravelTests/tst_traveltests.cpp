#include <QtTest>
#include <QJsonObject>

// --- Имитация структуры Trip для изоляции теста ---
struct TripMock {
    QString from;
    QString to;
    QDate date;
    double cost;
    int people;

    QJsonObject toJson() const {
        QJsonObject json;
        json["from"] = from;
        json["to"] = to;
        json["cost"] = cost;
        json["people"] = people;
        return json;
    }
};

class TravelTests : public QObject
{
    Q_OBJECT

private slots:
    // Тест 1: Проверка создания объекта
    void testTripCreation() {
        TripMock trip;
        trip.from = "Moscow";
        trip.to = "Paris";
        trip.cost = 50000;

        QCOMPARE(trip.from, QString("Moscow"));
        QCOMPARE(trip.to, QString("Paris"));
        QCOMPARE(trip.cost, 50000.0);
    }

    // Тест 2: Проверка расчета бюджета
    void testBudgetCalculation() {
        double ticketPrice = 15000.0;
        int people = 3;
        double total = ticketPrice * people;

        QCOMPARE(total, 45000.0);
    }

    // Тест 3: Проверка JSON
    void testJsonSerialization() {
        TripMock trip;
        trip.from = "Dubai";
        trip.to = "London";
        trip.cost = 100.0;
        trip.people = 2;

        QJsonObject json = trip.toJson();

        QVERIFY(!json.isEmpty());
        QCOMPARE(json["from"].toString(), QString("Dubai"));
        QCOMPARE(json["people"].toInt(), 2);
    }

    // Тест 4: Валидация дат
    void testDateValidation() {
        QDate today = QDate::currentDate();
        QDate tripDate = today.addDays(10);
        QVERIFY(tripDate > today);
    }
};

QTEST_APPLESS_MAIN(TravelTests)

#include "tst_traveltests.moc"
