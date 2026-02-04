#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDate>
#include <QJsonObject>
#include <QList>
#include <QNetworkAccessManager>

class QTableWidget;
class QLineEdit;
class QDateEdit;
class QComboBox;
class QSpinBox;
class QTextEdit;
class QStackedWidget;
class QProgressDialog;
class QPushButton;
class QCheckBox;
class QLabel;

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

class PriceService : public QObject {
    Q_OBJECT
    QNetworkAccessManager *manager;

public:
    explicit PriceService(QObject *parent = nullptr);
    void calculatePrice(const QString &fromCity, const QString &toCity, const QString &method, int travelers);

signals:
    void priceReady(double price);
    void errorOccurred(QString error);
};

class TripManager {
public:
    static QString getFilePath();
    static void saveTrips(const QList<Trip> &trips);
    static QList<Trip> loadTrips();
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    PriceService *priceService;
    QStackedWidget *stackedWidget;

    QWidget *listPage;
    QTableWidget *tripsTable;
    QCheckBox *checkShowFutureOnly;
    QLabel *totalCostLabel;

    QWidget *formPage;
    QComboBox *comboFrom;
    QComboBox *comboTo;
    QDateEdit *inputDate;
    QComboBox *inputMethod;
    QSpinBox *inputTravelers;
    QTextEdit *inputNote;
    QPushButton *btnSave;

    QProgressDialog *loadingDialog;

    Trip tempTrip;
    QList<Trip> trips;
    int editingIndex = -1;

    void setupUI();
    void setupListPage();
    void setupFormPage();
    void loadData();
    void updateTable();

private slots:
    void startSaveProcess();
    void finalizeSaveTrip(double price);
    void handleError(QString error);

    void showContextMenu(const QPoint &pos);
    void onAddClicked();

    void exportToPDF();
};

#endif // MAINWINDOW_H
