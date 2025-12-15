#include "welcomeview.h"
#include "ui_welcomeview.h"
#include <QDebug>
WelcomeView::WelcomeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeView)
{
    qDebug()<<"create WelcomeView";
    ui->setupUi(this);
}

WelcomeView::~WelcomeView()
{
    qDebug()<<"destroy WelcomeView";
    delete ui;
}

void WelcomeView::on_btn_Department_clicked()
{
    emit ansDepartmentView();
}


void WelcomeView::on_btn_Doctor_clicked()
{
    emit ansDoctorView();
}


void WelcomeView::on_btn_patient_clicked()
{
    emit ansPatientView();
}

