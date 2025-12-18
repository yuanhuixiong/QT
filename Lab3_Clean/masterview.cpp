#include "masterview.h"
#include "ui_masterview.h"
#include "idatabase.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    goLoginView();
    IDatabase::getInstance();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    loginView = new LoginView(this);
    pushwidgetto(loginView);

    connect(loginView,SIGNAL(loginSuccess()),this,SLOT(goWelcomeView()));
}

void MasterView::goWelcomeView()
{
    welcomeView = new WelcomeView(this);
    pushwidgetto(welcomeView);
    connect(welcomeView,SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
    connect(welcomeView,SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(goPatientView()),this,SLOT(goPatientView()));
}

void MasterView::goDoctorView()
{
    doctorView = new DoctorView(this);
    pushwidgetto(doctorView);
}

void MasterView::goDepartmentView()
{
    departmentView = new DepartmentView(this);
    pushwidgetto(departmentView);
}

void MasterView::goPatientEditView(int row)
{
    patientEditView = new PatientEditView(this,row);
    pushwidgetto(patientEditView);
    connect(patientEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::goPatientView()
{
    patientView = new PatientView(this);
    pushwidgetto(patientView);
    connect(patientView,SIGNAL(goPatientEditView(int)),this,SLOT(goPatientEditView(int)));
}

void MasterView::goPreviousView()
{
    int count = ui->stackedWidget->count();
    if(count > 1){
        QWidget *widget = ui->stackedWidget->widget(count - 1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
        int newCount = ui->stackedWidget->count();
        ui->stackedWidget->setCurrentIndex(newCount - 1);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());
    }
}

void MasterView::pushwidgetto(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count-1);
    ui->labelTitle->setText(widget->windowTitle());
}



void MasterView::on_btBack_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count = ui->stackedWidget->count();
    if(count>1)
        ui->btBack->setEnabled(true);
    else
        ui->btBack->setEnabled(false);
    QString title = ui->stackedWidget->currentWidget()->windowTitle();
    if(title == "欢迎"){
        ui->btLogout->setEnabled(true);
        ui->btBack->setEnabled(false);
    }else
        ui->btLogout->setEnabled(false);
}


void MasterView::on_btLogout_clicked()
{
    goPreviousView();
}

