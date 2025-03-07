#include <QRegularExpression>

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_chart = new QChart;
    m_series = new QLineSeries;

    m_chart->addSeries(m_series);

    m_axisX = new QValueAxis;
    m_axisX->setLabelFormat("%g");
    m_axisX->setTitleText("Samples");

    m_axisY = new QValueAxis;
    m_axisY->setTitleText("Level");

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);
    m_chart->setTitle("Data");

    ui->widget->layout()->addWidget(new QChartView(m_chart));

    connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
        appendItem(ui->lineEdit->text());
    });

    appendItems(QStringList()
                << "stp 1 Vin 14,02 VF 5,52 VBat 0,00 Dif 668 Cor 1 PWR 0 IN 1"
                << "stp 2 Vin 14,02 VF 5,79 VBat 0,00 Dif 638 Cor 1 PWR 0 IN 1"
                << "stp 3 Vin 14,02 VF 6,01 VBat 0,00 Dif 629 Cor 1 PWR 0 IN 1"
                << "stp 4 Vin 14,02 VF 6,22 VBat 0,00 Dif 627 Cor 1 PWR 0 IN 1"
                << "stp 5 Vin 14,02 VF 6,41 VBat 0,00 Dif 629 Cor 1 PWR 0 IN 1"
                << "stp 6 Vin 14,02 VF 6,58 VBat 0,00 Dif 633 Cor 1 PWR 0 IN 1"
                << "stp 7 Vin 14,02 VF 6,72 VBat 0,00 Dif 638 Cor 1 PWR 0 IN 1"
                << "stp 8 Vin 14,02 VF 6,85 VBat 0,00 Dif 643 Cor 1 PWR 0 IN 1"
                << "stp 9 Vin 14,02 VF 6,97 VBat 0,00 Dif 649 Cor 1 PWR 0 IN 1"
                << "stp 10 Vin 14,02 VF 7,09 VBat 0,00 Dif 641 Cor 1 PWR 0 IN 1"
                << "stp 11 Vin 14,02 VF 7,19 VBat 0,00 Dif 658 Cor 1 PWR 0 IN 1"
                );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendItem(const QString &s)
{
    if (s.isEmpty())
        return;

    const ItemData itemData = stringToDataItem(s);
    if (!itemData.isValid)
        return;

    ui->listWidget->addItem(s);
    ui->listWidget->scrollToBottom();

    const QPointF point(currentX(), itemData.vf);

    m_pointsX.append(point);
    updateAxisRange(false);
    updateChart();
}

void MainWindow::appendItems(const QStringList &l)
{
    foreach (const auto &s, l)
        appendItem(s);
}

MainWindow::ItemData MainWindow::stringToDataItem(const QString &s) const
{
    ItemData itemData;
    static QRegularExpression re("^stp (\\d+) Vin ([+-]?[0-9]*[.,]?[0-9]+) VF ([+-]?[0-9]*[.,]?[0-9]+) VBat ([+-]?[0-9]*[.,]?[0-9]+) Dif (\\d+) Cor (\\d+) PWR (\\d+) IN (\\d+)$");
    QRegularExpressionMatch match = re.match(s);
    if (match.hasMatch()) {
        itemData.isValid = true;
        itemData.stp = match.captured(1).toInt();
        itemData.vin = match.captured(2).replace(",", ".").toDouble();
        itemData.vf = match.captured(3).replace(",", ".").toDouble();
        itemData.vbat = match.captured(4).replace(",", ".").toDouble();
        itemData.dif = match.captured(5).toInt();
        itemData.cor = match.captured(6).toInt();
        itemData.pwr = match.captured(7).toInt();
        itemData.in = match.captured(8).toInt();
    }
    return itemData;
}

void MainWindow::updateAxisRange(bool verifyAllPoints)
{
    for (int i = verifyAllPoints ? 0 : currentX() - 1; i < currentX(); ++i) {
        const QPointF point = m_pointsX.at(i);
        m_maxY = MAX(m_maxY, point.y());
        m_minY = MIN(m_minY, point.y());
    }
}

void MainWindow::updateChart()
{
    m_axisX->setRange(0, currentX() - 1);
    m_axisY->setRange(m_minY, m_maxY);
    m_series->replace(m_pointsX);
}

int MainWindow::currentX() const
{
    return m_pointsX.length();
}
