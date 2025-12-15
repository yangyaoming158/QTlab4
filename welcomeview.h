#ifndef WELCOMEVIEW_H
#define WELCOMEVIEW_H

#include <QWidget>


namespace Ui {
class WelcomeView;
}

class WelcomeView : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeView(QWidget *parent = nullptr);
    ~WelcomeView();

private slots:
    void on_btn_Department_clicked();

    void on_btn_Doctor_clicked();

    void on_btn_patient_clicked();

private:
    Ui::WelcomeView *ui;

signals:
    void ansDepartmentView();
    void ansDoctorView();
    void ansPatientView();
};

#endif // WELCOMEVIEW_H
