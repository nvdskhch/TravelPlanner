#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "trip.h"

class QTableWidget;
class QDateEdit;
class QComboBox;
class QSpinBox;
class QTextEdit;
class QStackedWidget;
class QProgressDialog;
class QPushButton;
class QCheckBox;
class QLabel;
class PriceService;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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
    void onAddClicked();
    void startSaveProcess();
    void exportToPDF();
    void showContextMenu(const QPoint &pos);

    void finalizeSaveTrip(double price);
    void handleError(QString error);
};

#endif
