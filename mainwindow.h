#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QChart;
class QLineSeries;
class QValueAxis;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct ItemData {
        bool isValid{false};
        int stp{0};
        double vin{0};
        double vf{0};
        double vbat{0};
        int dif{0};
        int cor{0};
        int pwr{0};
        int in{0};
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void appendItem(const QString &s);
    void appendItems(const QStringList &l);
    ItemData stringToDataItem(const QString &s) const;
    void updateAxisRange(bool verifyAllPoints);
    void updateChart();
    int currentX() const;
    inline double MIN(double a, double b) { return((a) < (b) ? a : b); }
    inline double MAX(double a, double b) { return((a) > (b) ? a : b); }

private:
    Ui::MainWindow *ui;
    QChart *m_chart;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QList<QPointF> m_pointsX;
    double m_minY, m_maxY;
};
#endif // MAINWINDOW_H
