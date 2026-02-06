#include "mainwindow.h"
#include "priceservice.h"
#include "tripmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QStackedWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    priceService = new PriceService(this);

    connect(priceService, &PriceService::priceReady, this, &MainWindow::finalizeSaveTrip);
    connect(priceService, &PriceService::errorOccurred, this, &MainWindow::handleError);

    setupUI();
    loadData();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    resize(1000, 650);
    setWindowTitle("Travel Planner");

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    setupListPage();
    setupFormPage();

    stackedWidget->addWidget(listPage);
    stackedWidget->addWidget(formPage);
}

void MainWindow::setupListPage() {
    listPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(listPage);

    QLabel *header = new QLabel("Мои поездки");
    header->setStyleSheet("font-size: 20px; font-weight: bold; margin: 10px;");

    QHBoxLayout *filterLayout = new QHBoxLayout();
    checkShowFutureOnly = new QCheckBox("Показывать только предстоящие поездки");
    connect(checkShowFutureOnly, &QCheckBox::stateChanged, this, &MainWindow::updateTable);

    filterLayout->addWidget(checkShowFutureOnly);
    filterLayout->addStretch();

    tripsTable = new QTableWidget();
    tripsTable->setColumnCount(7);
    tripsTable->setHorizontalHeaderLabels({"Откуда", "Куда", "Дата", "Транспорт", "Людей", "Стоимость", "Заметка"});
    tripsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tripsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tripsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tripsTable, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    totalCostLabel = new QLabel("Итого: 0 руб.");
    totalCostLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333; margin-top: 10px;");
    totalCostLabel->setAlignment(Qt::AlignRight);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    QPushButton *btnExport = new QPushButton("Сохранить отчет (PDF)");
    btnExport->setStyleSheet("background-color: #607D8B; color: white; padding: 10px; font-weight: bold;");
    connect(btnExport, &QPushButton::clicked, this, &MainWindow::exportToPDF);

    QPushButton *btnAdd = new QPushButton("Добавить поездку");
    btnAdd->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; font-weight: bold;");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);

    buttonsLayout->addWidget(btnExport);
    buttonsLayout->addWidget(btnAdd);

    layout->addWidget(header);
    layout->addLayout(filterLayout);
    layout->addWidget(tripsTable);
    layout->addWidget(totalCostLabel);
    layout->addLayout(buttonsLayout);
}

void MainWindow::setupFormPage() {
    formPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(formPage);
    QFormLayout *form = new QFormLayout();

    QStringList cities = {"Москва", "Санкт-Петербург", "Казань", "Сочи", "Лондон", "Париж", "Дубай", "Токио"};
    cities.sort();

    comboFrom = new QComboBox();
    comboFrom->addItems(cities);
    comboTo = new QComboBox();
    comboTo->addItems(cities);

    inputDate = new QDateEdit(QDate::currentDate());
    inputDate->setCalendarPopup(true);
    inputDate->setMinimumDate(QDate::currentDate());

    inputMethod = new QComboBox();
    inputMethod->addItems({"Самолет", "Поезд", "Автомобиль"});

    inputTravelers = new QSpinBox();
    inputTravelers->setRange(1, 10);
    inputNote = new QTextEdit();
    inputNote->setPlaceholderText("Ваши заметки...");
    inputNote->setMaximumHeight(80);

    form->addRow("Откуда:", comboFrom);
    form->addRow("Куда:", comboTo);
    form->addRow("Дата:", inputDate);
    form->addRow("Транспорт:", inputMethod);
    form->addRow("Кол-во человек:", inputTravelers);
    form->addRow("Заметка:", inputNote);

    btnSave = new QPushButton("Найти билеты и Сохранить");
    btnSave->setStyleSheet("background-color: #2196F3; color: white; padding: 10px; font-weight: bold;");
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::startSaveProcess);

    QPushButton *btnCancel = new QPushButton("Отмена");
    connect(btnCancel, &QPushButton::clicked, this, [this](){
        stackedWidget->setCurrentWidget(listPage);
    });

    QHBoxLayout *btns = new QHBoxLayout();
    btns->addWidget(btnCancel);
    btns->addWidget(btnSave);

    mainLayout->addWidget(new QLabel("Планирование новой поездки"));
    mainLayout->addLayout(form);
    mainLayout->addLayout(btns);
    mainLayout->addStretch();
}

void MainWindow::loadData() {
    trips = TripManager::loadTrips();
    updateTable();
}

void MainWindow::updateTable() {
    tripsTable->setRowCount(0);

    std::sort(trips.begin(), trips.end(), [](const Trip &a, const Trip &b){
        return a.date < b.date;
    });

    double totalSum = 0;

    for (int i = 0; i < trips.size(); ++i) {
        const auto &trip = trips[i];
        bool isPast = trip.date < QDate::currentDate();

        if (checkShowFutureOnly->isChecked() && isPast) {
            continue;
        }

        totalSum += trip.cost;

        int r = tripsTable->rowCount();
        tripsTable->insertRow(r);

        QList<QTableWidgetItem *> items;
        items << new QTableWidgetItem(trip.from);
        items << new QTableWidgetItem(trip.to);
        items << new QTableWidgetItem(trip.date.toString("dd.MM.yyyy"));
        items << new QTableWidgetItem(trip.method);
        items << new QTableWidgetItem(QString::number(trip.travelers));
        items << new QTableWidgetItem(QString::number(trip.cost, 'f', 2));
        items << new QTableWidgetItem(trip.note);

        QColor rowColor = isPast ? QColor(240, 240, 240) : Qt::white;

        for (int col = 0; col < items.size(); ++col) {
            items[col]->setBackground(rowColor);
            if (isPast) items[col]->setForeground(Qt::gray);
            items[col]->setData(Qt::UserRole, i);
            tripsTable->setItem(r, col, items[col]);
        }
    }

    totalCostLabel->setText(QString("Итоговый бюджет: %1 руб.").arg(totalSum, 0, 'f', 2));
}

void MainWindow::onAddClicked() {
    editingIndex = -1;
    inputDate->setDate(QDate::currentDate());
    inputDate->setMinimumDate(QDate::currentDate());
    inputTravelers->setValue(1);
    inputNote->clear();
    btnSave->setText("Найти билеты и Сохранить");
    stackedWidget->setCurrentWidget(formPage);
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QModelIndex index = tripsTable->indexAt(pos);
    if (!index.isValid()) return;

    int realIndex = tripsTable->item(index.row(), 0)->data(Qt::UserRole).toInt();

    QMenu contextMenu(this);
    QAction *editAction = contextMenu.addAction("Редактировать");
    QAction *deleteAction = contextMenu.addAction("Удалить");

    connect(deleteAction, &QAction::triggered, this, [this, realIndex](){
        auto reply = QMessageBox::question(this, "Удаление", "Удалить эту поездку безвозвратно?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (realIndex >= 0 && realIndex < trips.size()) {
                trips.removeAt(realIndex);
                TripManager::saveTrips(trips);
                updateTable();
            }
        }
    });

    connect(editAction, &QAction::triggered, this, [this, realIndex](){
        if (realIndex >= 0 && realIndex < trips.size()) {
            editingIndex = realIndex;
            Trip t = trips[realIndex];
            comboFrom->setCurrentText(t.from);
            comboTo->setCurrentText(t.to);
            inputDate->setMinimumDate(t.date);
            inputDate->setDate(t.date);
            inputMethod->setCurrentText(t.method);
            inputTravelers->setValue(t.travelers);
            inputNote->setText(t.note);
            btnSave->setText("Обновить поездку");
            stackedWidget->setCurrentWidget(formPage);
        }
    });

    contextMenu.exec(tripsTable->viewport()->mapToGlobal(pos));
}

void MainWindow::startSaveProcess() {
    tempTrip.from = comboFrom->currentText();
    tempTrip.to = comboTo->currentText();
    tempTrip.date = inputDate->date();
    tempTrip.method = inputMethod->currentText();
    tempTrip.travelers = inputTravelers->value();
    tempTrip.note = inputNote->toPlainText();

    if (tempTrip.from == tempTrip.to) {
        QMessageBox::warning(this, "Ошибка", "Пункты отправления и прибытия совпадают!");
        return;
    }

    loadingDialog = new QProgressDialog("Расчет стоимости...", QString(), 0, 0, this);
    loadingDialog->setWindowModality(Qt::WindowModal);
    loadingDialog->show();

    priceService->calculatePrice(tempTrip.from, tempTrip.to, tempTrip.method, tempTrip.travelers);
}

void MainWindow::finalizeSaveTrip(double price) {
    if (loadingDialog) {
        loadingDialog->close();
        delete loadingDialog;
        loadingDialog = nullptr;
    }

    tempTrip.cost = price;

    if (editingIndex == -1) {
        trips.append(tempTrip);
        QMessageBox::information(this, "Успех", QString("Поездка создана!\nЦена: %1 руб.").arg(price));
    } else {
        if (editingIndex >= 0 && editingIndex < trips.size()) {
            trips[editingIndex] = tempTrip;
            QMessageBox::information(this, "Успех", QString("Поездка обновлена!\nНовая цена: %1 руб.").arg(price));
        }
        editingIndex = -1;
    }

    TripManager::saveTrips(trips);
    updateTable();
    stackedWidget->setCurrentWidget(listPage);
}

void MainWindow::handleError(QString error) {
    if (loadingDialog) {
        loadingDialog->close();
        delete loadingDialog;
        loadingDialog = nullptr;
    }
    QMessageBox::warning(this, "Маршрут недоступен", error);
}

void MainWindow::exportToPDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить отчет", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fileName);

    QString html = "<html><head><style>"
                   "h1 { text-align: center; color: #333; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                   "th, td { border: 1px solid #aaa; padding: 8px; text-align: left; }"
                   "th { background-color: #f2f2f2; font-weight: bold; }"
                   ".total { text-align: right; margin-top: 30px; font-size: 18px; font-weight: bold; }"
                   "</style></head><body>";

    html += "<h1>Отчет о запланированных поездках</h1>";
    html += "<table>";
    html += "<tr>"
            "<th>Дата</th>"
            "<th>Откуда</th>"
            "<th>Куда</th>"
            "<th>Транспорт</th>"
            "<th>Людей</th>"
            "<th>Стоимость</th>"
            "</tr>";

    double totalSum = 0;

    QList<Trip> sortedTrips = trips;
    std::sort(sortedTrips.begin(), sortedTrips.end(), [](const Trip &a, const Trip &b){
        return a.date < b.date;
    });

    for (const auto &trip : sortedTrips) {
        totalSum += trip.cost;

        QString rowStyle = "";
        if (trip.date < QDate::currentDate()) {
            rowStyle = " style='color: #888; background-color: #f9f9f9;'";
        }

        html += QString("<tr%1>").arg(rowStyle);
        html += QString("<td>%1</td>").arg(trip.date.toString("dd.MM.yyyy"));
        html += QString("<td>%1</td>").arg(trip.from);
        html += QString("<td>%1</td>").arg(trip.to);
        html += QString("<td>%1</td>").arg(trip.method);
        html += QString("<td>%1</td>").arg(trip.travelers);
        html += QString("<td>%1 руб.</td>").arg(QString::number(trip.cost, 'f', 2));
        html += "</tr>";
    }

    html += "</table>";
    html += QString("<div class='total'>Итоговый бюджет: %1 руб.</div>").arg(QString::number(totalSum, 'f', 2));
    html += "<p><em>Сгенерировано приложением Travel Planner</em></p>";
    html += "</body></html>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Экспорт", "Отчет успешно сохранен!");
}
