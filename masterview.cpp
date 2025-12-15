#include "masterview.h"
#include "./ui_masterview.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->goLoginView();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    qDebug() << "goLoginView";
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);

    connect(loginView,SIGNAL(loginSuccess()),this,SLOT(goWelcomView()));
}

void MasterView::goWelcomView()
{
    qDebug() << "goWelcomView";
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    connect(welcomeView,SIGNAL(ansDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(ansPatientView()),this,SLOT(goPatientView()));
    connect(welcomeView,SIGNAL(ansDepartmentView()),this,SLOT(goDepartmentView()));
}

void MasterView::goDoctorView()
{
    qDebug() << "goDoctorView";
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);
}

void MasterView::goDepartmentView()
{
    qDebug() << "goDepartmentView";
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

// 修改 goPatientEditView 接收参数
void MasterView::goPatientEditView(int patientId)
{
    qDebug() << "goPatientEditView with ID:" << patientId;

    // 【修改这里】将 patientId 传给构造函数
    patientEditView = new PatientEditView(this, patientId);

    pushWidgetToStackView(patientEditView);

    // 连接返回信号 (假设你有一个 goPatientView 函数)
    connect(patientEditView, &PatientEditView::goPreviousView, this, &MasterView::goPatientView);
}

void MasterView::goPatientView()
{
    qDebug() << "goPatientView";
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);

    // 修改连接，注意信号带参数了
    connect(patientView, &PatientView::ansPatientEditView, this, &MasterView::goPatientEditView);
}

void MasterView::goPreviousView()
{
    int count = ui->stackedWidget->count();
    if(count>1){
        ui->stackedWidget->setCurrentIndex(count-2);
        ui->label->setText(ui->stackedWidget->currentWidget()->windowTitle());
        QWidget *widget = ui->stackedWidget->widget(count-1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count-1);
    ui->label->setText(widget->windowTitle());
}



void MasterView::on_btn_return_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{

    QWidget *currWidget = ui->stackedWidget->currentWidget();
    QString title = currWidget->windowTitle();

    // 1. 如果是【登录】界面
    if (title == "登录") {
        ui->btn_return->setEnabled(false); // 登录界面禁止返回
        ui->btn_Logout->setEnabled(false); // 登录界面禁止注销
    }
    // 2. 如果是【欢迎】界面
    else if (title == "欢迎") {
        ui->btn_return->setEnabled(false); // 欢迎界面禁止返回（必须点注销）
        ui->btn_Logout->setEnabled(true);  // 允许注销
    }
    // 3. 其他所有子页面（如医生、患者、科室管理等）
    else {
        ui->btn_return->setEnabled(true);  // 子页面允许返回
        ui->btn_Logout->setEnabled(false); // 子页面禁止直接注销（通常需先返回欢迎页）
    }
}


void MasterView::on_btn_Logout_clicked()
{
    goPreviousView();
}

