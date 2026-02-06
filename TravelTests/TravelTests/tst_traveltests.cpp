#include <QtTest>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>

// Подключаем РЕАЛЬНЫЕ заголовочные файлы из основного проекта
#include "../Travel/trip.h"
#include "../Travel/tripmanager.h"

class TravelTests : public QObject
{
    Q_OBJECT

private slots:
    // Тест 1: Проверяем, что структура Trip правильно хранит данные
    void testTripStructure() {
        Trip trip;
        trip.from = "Moscow";
        trip.to = "Paris";
        trip.cost = 50000;
        trip.travelers = 2;
        trip.date = QDate(2026, 5, 20);

        QCOMPARE(trip.from, QString("Moscow"));
        QCOMPARE(trip.to, QString("Paris"));
        QCOMPARE(trip.cost, 50000.0);
        QCOMPARE(trip.travelers, 2);
        QCOMPARE(trip.date, QDate(2026, 5, 20));
    }

    // Тест 2: Проверяем конвертацию в JSON и обратно (сериализацию)
    void testJsonConversion() {
        Trip originalTrip;
        originalTrip.from = "Dubai";
        originalTrip.to = "London";
        originalTrip.method = "Plane";
        originalTrip.cost = 75000;
        originalTrip.travelers = 1;
        originalTrip.date = QDate(2026, 10, 15);
        originalTrip.note = "Business trip";

        // Превращаем в JSON
        QJsonObject json = originalTrip.toJson();

        // Проверяем, что в JSON попали верные данные
        QCOMPARE(json["from"].toString(), QString("Dubai"));
        QCOMPARE(json["cost"].toDouble(), 75000.0);

        // Превращаем обратно из JSON в структуру
        Trip restoredTrip = Trip::fromJson(json);

        // Проверяем, что данные не потерялись
        QCOMPARE(restoredTrip.from, originalTrip.from);
        QCOMPARE(restoredTrip.to, originalTrip.to);
        QCOMPARE(restoredTrip.note, originalTrip.note);
    }

    // Тест 3: Проверяем TripManager (реальное сохранение и загрузка)
    void testTripManagerFileOperations() {
        // 1. Подготовка данных
        QList<Trip> tripsToSave;
        Trip t1; t1.from = "A"; t1.to = "B"; t1.cost = 100;
        Trip t2; t2.from = "C"; t2.to = "D"; t2.cost = 200;
        tripsToSave << t1 << t2;

        // 2. Сохраняем (используется реальный путь из TripManager)
        TripManager::saveTrips(tripsToSave);

        // 3. Загружаем обратно
        QList<Trip> loadedTrips = TripManager::loadTrips();

        // 4. Проверяем
        QCOMPARE(loadedTrips.size(), 2);
        QCOMPARE(loadedTrips[0].from, QString("A"));
        QCOMPARE(loadedTrips[1].cost, 200.0);

        // Очистка: удаляем созданный файл, чтобы не мусорить
        QFile file(TripManager::getFilePath());
        if (file.exists()) {
            file.remove();
        }
    }
};

QTEST_MAIN(TravelTests)

#include "tst_traveltests.moc"
