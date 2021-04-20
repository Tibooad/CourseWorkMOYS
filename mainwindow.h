#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

private slots:
    void on_StartButton_clicked();

    void on_QQLineEdit_textChanged(const QString &arg1);

    void on_Q1minLineEdit_textChanged(const QString &arg1);

    void on_Q2minLineEdit_textChanged(const QString &arg1);

    void on_Q1maxLineEdit_textChanged(const QString &arg1);

    void on_Q2maxLineEdit_textChanged(const QString &arg1);

    void on_Step1LineEdit_textChanged(const QString &arg1);

    void on_Step2LineEdit_textChanged(const QString &arg1);

    void on_R1LineEdit_textChanged(const QString &arg1);

    void on_R2LineEdit_textChanged(const QString &arg1);

    void on_HdtLineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QLineSeries *Step1LineSeries;
    QChart *Step1Chart;
    QLineSeries *Step2LineSeries;
    QChart *Step2Chart;
    QValueAxis *Step1AxisX;
    QValueAxis *Step2AxisX;
    QValueAxis *Step1AxisY;
    QValueAxis *Step2AxisY;

    qreal Q=3600;
    qreal Q1min=1100, Q1max=2300;
    qreal Q2min=1300, Q2max=2500;
    qreal Hdt=60;
    qreal Step1=100, Step2=10;
    qreal R1=0.000008, R2=0.000007;

    bool okQ;
    bool okQ1min, okQ1max;
    bool okQ2min, okQ2max;
    bool okHdt;
    bool okStep1, okStep2;
    bool okR1, okR2;

    std::vector<qreal> Q1Step1vals;
    std::vector<qreal> Q2Step1vals;
    std::vector<qreal> H1Step1vals;
    std::vector<qreal> H2Step1vals;
    std::vector<qreal> NStep1vals;

    std::vector<qreal> Q1Step2vals;
    std::vector<qreal> Q2Step2vals;
    std::vector<qreal> H1Step2vals;
    std::vector<qreal> H2Step2vals;
    std::vector<qreal> NStep2vals;

    void calculate();
    void showData();
    bool checkInputData();
};
#endif // MAINWINDOW_H
