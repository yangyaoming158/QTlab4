#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include "loginview.h"
#include "welcomeview.h"
#include "doctorview.h"
#include "departmentview.h"
#include "patienteditview.h"
#include "patientview.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class MasterView : public QWidget
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

public slots:
    void goLoginView();
    void goWelcomView();
    void goDoctorView();
    void goDepartmentView();
    void goPatientEditView(int patientId);
    void goPatientView();
    void goPreviousView();

private slots:
    void on_btn_return_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btn_Logout_clicked();

private:
    Ui::MasterView *ui;

    void pushWidgetToStackView(QWidget *widget);

    WelcomeView *welcomeView;
    LoginView *loginView;
    DoctorView *doctorView;
    DepartmentView *departmentView;
    PatientEditView *patientEditView;
    PatientView *patientView;
};
#endif // MASTERVIEW_H
