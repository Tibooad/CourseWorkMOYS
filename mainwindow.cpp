#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts>
#include <QtMath>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent,flags)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Step1TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Step2TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->Step1ChartView->setRenderHint(QPainter::Antialiasing);
    ui->Step2ChartView->setRenderHint(QPainter::Antialiasing);

    okQ=true;
    okQ1min=true; okQ1max=true;
    okQ2min=true; okQ2max=true;
    okHdt=true;
    okStep1=true; okStep2=true;
    okR1=true; okR2=true;


    // Создаём график и добавляем в него серии
    Step1Chart = new QChart();
    Step2Chart = new QChart();

    Step1LineSeries=new QLineSeries;
    Step1LineSeries->setName("Step1");
    Step2LineSeries=new QLineSeries;
    Step2LineSeries->setName("Step2");
    Step2LineSeries->setColor(Qt::red);
    Step1Chart->addSeries(Step1LineSeries);
    Step2Chart->addSeries(Step2LineSeries);


    // Настройка осей графика
    Step1AxisX = new QValueAxis;
    Step1AxisX->setTitleText("Q");
    Step1AxisX->setLabelFormat("%i");
    Step1AxisX->setTickCount(11);
    Step1Chart->addAxis(Step1AxisX, Qt::AlignBottom);

    Step2AxisX = new QValueAxis;
    Step2AxisX->setTitleText("Q");
    Step2AxisX->setLabelFormat("%i");
    Step2AxisX->setTickCount(11);
    Step2Chart->addAxis(Step2AxisX, Qt::AlignBottom);

    Step1AxisY = new QValueAxis;
    Step1AxisY->setTitleText("N");
    Step1AxisY->setLabelFormat("%i");
    Step1AxisY->setTickCount(11);
    Step1Chart->addAxis(Step1AxisY, Qt::AlignLeft);


    Step2AxisY = new QValueAxis;
    Step2AxisY->setTitleText("N");
    Step2AxisY->setLabelFormat("%i");
    Step2AxisY->setTickCount(11);
    Step2Chart->addAxis(Step2AxisY, Qt::AlignLeft);

    Step1LineSeries->attachAxis(Step1AxisX);
    Step2LineSeries->attachAxis(Step2AxisX);
    Step1LineSeries->attachAxis(Step1AxisY);
    Step2LineSeries->attachAxis(Step2AxisY);


    // Устанавливаем график в представление
    ui->Step1ChartView->setChart(Step1Chart);
    ui->Step2ChartView->setChart(Step2Chart);
}

//private methods
void MainWindow::calculate(){
    Q1Step1vals.clear();
    Q2Step1vals.clear();
    H1Step1vals.clear();
    H2Step1vals.clear();
    NStep1vals.clear();

    Q1Step2vals.clear();
    Q2Step2vals.clear();
    H1Step2vals.clear();
    H2Step2vals.clear();
    NStep2vals.clear();



    qreal H1, H2, N;
    qreal q1opt, q2opt, nopt;
    H1=Hdt+R1*qPow(Q1min,2);
    H2=Hdt+R2*qPow(Q2max,2);
    nopt=Q1min*H1+Q2max*H2;
    for(qreal q1=Q1min,q2=Q2max;q1<=Q1max&&q2>=Q2min;q1+=Step1,q2-=Step1){
        H1=Hdt+R1*qPow(q1,2);
        H2=Hdt+R2*qPow(q2,2);
        N=q1*H1+q2*H2;
        if(nopt>N){
            q1opt=q1;
            q2opt=q2;
            nopt=N;
        }

        Q1Step1vals.push_back(q1);
        Q2Step1vals.push_back(q2);
        H1Step1vals.push_back(H1);
        H2Step1vals.push_back(H2);
        NStep1vals.push_back(N);
    }
    for(qreal q1=q1opt-Step1,q2=q2opt+Step1;q1<=q1opt+Step1&&q2>=q2opt-Step1;q1+=Step2,q2-=Step2){
        H1=Hdt+R1*qPow(q1,2);
        H2=Hdt+R2*qPow(q2,2);
        N=q1*H1+q2*H2;

        Q1Step2vals.push_back(q1);
        Q2Step2vals.push_back(q2);
        H1Step2vals.push_back(H1);
        H2Step2vals.push_back(H2);
        NStep2vals.push_back(N);
    }


}
void MainWindow::showData(){

    Step1LineSeries->clear();
    Step2LineSeries->clear();

    for(int i=0;i<Q1Step1vals.size();i++){
        Step1LineSeries->append(QPointF(Q1Step1vals[i],NStep1vals[i]));
    }

    for(int i=0;i<Q1Step2vals.size();i++){
        Step2LineSeries->append(QPointF(Q1Step2vals[i],NStep2vals[i]));
    }
    Step1AxisX->setRange(Q1Step1vals[0],Q1Step1vals[Q1Step1vals.size()-1]);
    Step2AxisX->setRange(Q1Step2vals[0],Q1Step2vals[Q1Step2vals.size()-1]);

    Step1AxisY->setRange(*std::min_element(NStep1vals.begin(),NStep1vals.end()),*std::max_element(NStep1vals.begin(),NStep1vals.end()));
    Step2AxisY->setRange(*std::min_element(NStep2vals.begin(),NStep2vals.end()),*std::max_element(NStep2vals.begin(),NStep2vals.end()));



    ui->Step1TableWidget->setRowCount(0);
    ui->Step2TableWidget->setRowCount(0);
    QTableWidgetItem *item;
    QTableWidgetItem *scrlItem;
    int coloredElement=std::distance(NStep1vals.begin(),std::min_element(NStep1vals.begin(),NStep1vals.end()));
    for(int i=0;i<Q1Step1vals.size();i++){
        ui->Step1TableWidget->insertRow(i);

        if(i==coloredElement|| i==coloredElement-1|| i==coloredElement+1){
            item=new QTableWidgetItem(QString::number(Q1Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,165,0)));
            ui->Step1TableWidget->setItem(i,0, item);
            item=new QTableWidgetItem(QString::number(Q2Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter |Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,165,0)));
            ui->Step1TableWidget->setItem(i,1, item);

            item=new QTableWidgetItem(QString::number(H1Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,165,0)));
            ui->Step1TableWidget->setItem(i,2, item);
            item=new QTableWidgetItem(QString::number(H2Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,165,0)));
            ui->Step1TableWidget->setItem(i,3, item);

            item=new QTableWidgetItem(QString::number(NStep1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,165,0)));
            ui->Step1TableWidget->setItem(i,4, item);

            scrlItem=item;
        }else{
            item=new QTableWidgetItem(QString::number(Q1Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step1TableWidget->setItem(i,0, item);
            item=new QTableWidgetItem(QString::number(Q2Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter |Qt::AlignmentFlag::AlignBottom);
            ui->Step1TableWidget->setItem(i,1, item);

            item=new QTableWidgetItem(QString::number(H1Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step1TableWidget->setItem(i,2, item);
            item=new QTableWidgetItem(QString::number(H2Step1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step1TableWidget->setItem(i,3, item);

            item=new QTableWidgetItem(QString::number(NStep1vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step1TableWidget->setItem(i,4, item);
        }
    }
    ui->Step1TableWidget->setFocus();
    ui->Step1TableWidget->scrollToItem(scrlItem);
    ui->Step1TableWidget->clearFocus();

    coloredElement=std::distance(NStep2vals.begin(),std::min_element(NStep2vals.begin(),NStep2vals.end()));
    for(int i=0;i<Q1Step2vals.size();i++){
        ui->Step2TableWidget->insertRow(i);


        if(i==coloredElement){
            item=new QTableWidgetItem(QString::number(Q1Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,0,0)));
            ui->Step2TableWidget->setItem(i,0, item);
            item=new QTableWidgetItem(QString::number(Q2Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,0,0)));
            ui->Step2TableWidget->setItem(i,1, item);

            item=new QTableWidgetItem(QString::number(H1Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,0,0)));
            ui->Step2TableWidget->setItem(i,2, item);
            item=new QTableWidgetItem(QString::number(H2Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,0,0)));
            ui->Step2TableWidget->setItem(i,3, item);

            item=new QTableWidgetItem(QString::number(NStep2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            item->setBackground(QBrush(QColor::fromRgb(255,0,0)));
            ui->Step2TableWidget->setItem(i,4, item);

            scrlItem=item;
        }else{
            item=new QTableWidgetItem(QString::number(Q1Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step2TableWidget->setItem(i,0, item);
            item=new QTableWidgetItem(QString::number(Q2Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step2TableWidget->setItem(i,1, item);

            item=new QTableWidgetItem(QString::number(H1Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step2TableWidget->setItem(i,2, item);
            item=new QTableWidgetItem(QString::number(H2Step2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step2TableWidget->setItem(i,3, item);

            item=new QTableWidgetItem(QString::number(NStep2vals[i]));
            item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);
            ui->Step2TableWidget->setItem(i,4, item);
        }

    }
    ui->Step2TableWidget->setFocus();
    ui->Step2TableWidget->scrollToItem(scrlItem);
    ui->Step2TableWidget->clearFocus();


    //fill fields
    ui->Q1optLineEdit->setText(QString::number(Q1Step2vals[coloredElement]));
    ui->Q2optLineEdit->setText(QString::number(Q2Step2vals[coloredElement]));

    ui->H1optLineEdit->setText(QString::number(H1Step2vals[coloredElement]));
    ui->H2optLineEdit->setText(QString::number(H2Step2vals[coloredElement]));

    ui->NoptLineEdit->setText(QString::number(NStep2vals[coloredElement]));

}
bool MainWindow::checkInputData(){

    return (okQ && okQ1max && okQ2min && okQ1min && okQ2max && okHdt && okStep1 && okStep2 && okR1 && okR2);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_StartButton_clicked()
{
    if(checkInputData()){

        calculate();
        showData();
    }else{

        QMessageBox::critical(this,"Wrong input","Check your input data again");
    }
}

void MainWindow::on_QQLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    Q=ui->QQLineEdit->text().toDouble(&ok);
    if(!ok || Q <= 0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->QQLineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->QQLineEdit->mapToGlobal(QPoint(0,0)),ui->QQLineEdit->toolTip());
        //set flag
        okQ=false;
    }else{
        on_Q1minLineEdit_textChanged("");
        on_Q2minLineEdit_textChanged("");
        on_Q1maxLineEdit_textChanged("");
        on_Q2maxLineEdit_textChanged("");
        //set white color on Line edit
        QPalette whitePalette;
        whitePalette.setColor(QPalette::Base,Qt::white);
        ui->QQLineEdit->setPalette(whitePalette);
        //set flag
        okQ=true;
    }
}

void MainWindow::on_Q1minLineEdit_textChanged(const QString &arg1)
{
    bool okQ1min,okQ2max,okQ;
    Q1min=ui->Q1minLineEdit->text().toDouble(&okQ1min);

    Q2max=ui->Q2maxLineEdit->text().toDouble(&okQ2max);
    Q=ui->QQLineEdit->text().toDouble(&okQ);
    if(!okQ1min || Q1min <= 0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q1minLineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->Q1minLineEdit->mapToGlobal(QPoint(0,0)),ui->Q1minLineEdit->toolTip());

        //set flag
        this->okQ1min=false;
    }else if(!okQ2max){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q1minLineEdit->setPalette(orangePalette);

        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q2maxLineEdit->setPalette(redPalette);

        this->okQ1min=false;
        this->okQ2max=false;

    }else if(!okQ){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q1minLineEdit->setPalette(orangePalette);
        ui->Q2maxLineEdit->setPalette(orangePalette);

        this->okQ1min=true;
        this->okQ2max=true;

    }else if(okQ2max && okQ){
        if(Q1min + Q2max !=Q){
            QPalette orangePalette;
            orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
            ui->Q1minLineEdit->setPalette(orangePalette);
            ui->Q2maxLineEdit->setPalette(orangePalette);

            //set flag
            this->okQ1min=false;
            this->okQ2max=false;
        }else{
            //set white color on Line edit
            QPalette whitePalette;
            whitePalette.setColor(QPalette::Base,Qt::white);
            ui->Q1minLineEdit->setPalette(whitePalette);
            ui->Q2maxLineEdit->setPalette(whitePalette);

            //set flag
            this->okQ1min=true;
            this->okQ2max=true;
        }
    }
}

void MainWindow::on_Q2minLineEdit_textChanged(const QString &arg1)
{
    bool okQ2min,okQ1max,okQ;
    Q2min=ui->Q2minLineEdit->text().toDouble(&okQ2min);

    Q1max=ui->Q1maxLineEdit->text().toDouble(&okQ1max);
    Q=ui->QQLineEdit->text().toDouble(&okQ);
    if(!okQ2min || Q2min <= 0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q2minLineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->Q2minLineEdit->mapToGlobal(QPoint(0,0)),ui->Q2minLineEdit->toolTip());

        //set flag
        this->okQ2min=false;
    }else if(!okQ1max){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q2minLineEdit->setPalette(orangePalette);

        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q1maxLineEdit->setPalette(redPalette);


        //set flag
        this->okQ1max=false;
        this->okQ2min=false;
    }else if(!okQ){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q2minLineEdit->setPalette(orangePalette);
        ui->Q1maxLineEdit->setPalette(orangePalette);

        //set flag
        this->okQ1max=false;
        this->okQ2min=false;
    }else if(okQ1max && okQ){
        if(Q2min + Q1max !=Q){
            QPalette orangePalette;
            orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
            ui->Q2minLineEdit->setPalette(orangePalette);
            ui->Q1maxLineEdit->setPalette(orangePalette);

            //set flag
            this->okQ1max=false;
            this->okQ2min=false;
        }else{

            //set white color on Line edit
            QPalette whitePalette;
            whitePalette.setColor(QPalette::Base,Qt::white);
            ui->Q2minLineEdit->setPalette(whitePalette);
            ui->Q1maxLineEdit->setPalette(whitePalette);

            //set flag
            this->okQ2min=true;
            this->okQ1max=true;
        }
    }
}

void MainWindow::on_Q1maxLineEdit_textChanged(const QString &arg1)
{
    bool okQ2min,okQ1max,okQ;
    Q1max=ui->Q1maxLineEdit->text().toDouble(&okQ1max);

    Q2min=ui->Q2minLineEdit->text().toDouble(&okQ2min);
    Q=ui->QQLineEdit->text().toDouble(&okQ);
    if(!okQ1max || Q1max <= 0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q1maxLineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->Q1maxLineEdit->mapToGlobal(QPoint(0,0)),ui->Q1maxLineEdit->toolTip());

        //set flag
        this->okQ1max=false;
    }else if(!Q2min){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q1maxLineEdit->setPalette(orangePalette);

        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q2minLineEdit->setPalette(redPalette);

        //set flag
        this->okQ2min=false;
        this->okQ1max=false;
    }else if(!okQ){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q1maxLineEdit->setPalette(orangePalette);
        ui->Q2minLineEdit->setPalette(orangePalette);

        //set flag
        this->okQ2min=false;
        this->okQ1max=false;
    }else if(Q2min && okQ){
        if(Q2min + Q1max !=Q){
            QPalette orangePalette;
            orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
            ui->Q1maxLineEdit->setPalette(orangePalette);
            ui->Q2minLineEdit->setPalette(orangePalette);

            //set flag
            this->okQ2min=false;
            this->okQ1max=false;
        }else{
            //set white color on Line edit
            QPalette whitePalette;
            whitePalette.setColor(QPalette::Base,Qt::white);
            ui->Q1maxLineEdit->setPalette(whitePalette);
            ui->Q2minLineEdit->setPalette(whitePalette);

            //set flag
            this->okQ2min=true;
            this->okQ1max=true;
        }
    }
}

void MainWindow::on_Q2maxLineEdit_textChanged(const QString &arg1)
{
    bool okQ1min,okQ2max,okQ;
    Q2max=ui->Q2maxLineEdit->text().toDouble(&okQ2max);

    Q1min=ui->Q1minLineEdit->text().toDouble(&okQ1min);
    Q=ui->QQLineEdit->text().toDouble(&okQ);
    if(!okQ2max || Q2max <= 0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q2maxLineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->Q2maxLineEdit->mapToGlobal(QPoint(0,0)),ui->Q2maxLineEdit->toolTip());

        //set flag
        this->okQ2max=false;

    }else if(!okQ1min){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q2maxLineEdit->setPalette(orangePalette);
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Q1minLineEdit->setPalette(redPalette);

        //set flag
        this->okQ2max=false;
        this->okQ1min=false;
    }else if(!okQ){
        QPalette orangePalette;
        orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
        ui->Q2maxLineEdit->setPalette(orangePalette);
        ui->Q1minLineEdit->setPalette(orangePalette);

        //set flag
        this->okQ2max=false;
        this->okQ1min=false;
    }else if(okQ1min && okQ){
        if(Q1min + Q2max !=Q){
            QPalette orangePalette;
            orangePalette.setColor(QPalette::Base,QColor::fromRgb(255,165,0));
            ui->Q1minLineEdit->setPalette(orangePalette);
            ui->Q2maxLineEdit->setPalette(orangePalette);

            //set flag
            this->okQ2max=false;
            this->okQ1min=false;
        }else{
            //set white color on Line edit
            QPalette whitePalette;
            whitePalette.setColor(QPalette::Base,Qt::white);
            ui->Q1minLineEdit->setPalette(whitePalette);
            ui->Q2maxLineEdit->setPalette(whitePalette);

            //set flag
            this->okQ2max=true;
            this->okQ1min=true;
        }
    }
}

void MainWindow::on_HdtLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    Hdt=ui->HdtLineEdit->text().toDouble(&ok);
    if(!ok || Hdt<=0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->HdtLineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->HdtLineEdit->mapToGlobal(QPoint(0,0)),ui->HdtLineEdit->toolTip());

        //set flag
        this->okHdt=false;
    }else{
        //set white color on Line edit
        QPalette whitePalette;
        whitePalette.setColor(QPalette::Base,Qt::white);
        ui->HdtLineEdit->setPalette(whitePalette);

        //set flag
        this->okHdt=true;
    }
}

void MainWindow::on_Step1LineEdit_textChanged(const QString &arg1)
{
    bool ok;
    Step1=ui->Step1LineEdit->text().toDouble(&ok);
    if(!ok || Step1<=0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Step1LineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->Step1LineEdit->mapToGlobal(QPoint(0,0)),ui->Step1LineEdit->toolTip());

        //set flag
        this->okStep1=false;
    }else{
        //set white color on Line edit
        QPalette whitePalette;
        whitePalette.setColor(QPalette::Base,Qt::white);
        ui->Step1LineEdit->setPalette(whitePalette);

        //set flag
        this->okStep1=true;
    }
}

void MainWindow::on_Step2LineEdit_textChanged(const QString &arg1)
{
    bool ok;
    Step2=ui->Step2LineEdit->text().toDouble(&ok);
    if(!ok || Step2<=0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->Step2LineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->Step2LineEdit->mapToGlobal(QPoint(0,0)),ui->Step2LineEdit->toolTip());

        //set flag
        this->okStep2=false;
    }else{
        //set white color on Line edit
        QPalette whitePalette;
        whitePalette.setColor(QPalette::Base,Qt::white);
        ui->Step2LineEdit->setPalette(whitePalette);

        //set flag
        this->okStep1=true;
    }
}

void MainWindow::on_R1LineEdit_textChanged(const QString &arg1)
{
    bool ok;
    R1=ui->R1LineEdit->text().toDouble(&ok);
    if(!ok || R1<=0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->R1LineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->R1LineEdit->mapToGlobal(QPoint(0,0)),ui->R1LineEdit->toolTip());

        //set flag
        this->okR1=false;
    }else{
        //set white color on Line edit
        QPalette whitePalette;
        whitePalette.setColor(QPalette::Base,Qt::white);
        ui->R1LineEdit->setPalette(whitePalette);

        //set flag
        this->okR1=true;
    }
}

void MainWindow::on_R2LineEdit_textChanged(const QString &arg1)
{
    bool ok;
    ui->R2LineEdit->text().toDouble(&ok);
    if(!ok || R2<=0){
        //set red color on Line edit
        QPalette redPalette;
        redPalette.setColor(QPalette::Base,Qt::red);
        ui->R2LineEdit->setPalette(redPalette);
        //show tooltip
        QToolTip::showText(ui->R2LineEdit->mapToGlobal(QPoint(0,0)),ui->R2LineEdit->toolTip());

        //set flag
        this->okR2=false;
    }else{
        //set white color on Line edit
        QPalette whitePalette;
        whitePalette.setColor(QPalette::Base,Qt::white);
        ui->R2LineEdit->setPalette(whitePalette);

        //set flag
        this->okR2=true;
    }
}


