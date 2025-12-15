#ifndef PATIENTEDITVIEW_H
#define PATIENTEDITVIEW_H

#include <QWidget>

namespace Ui {
class PatientEditView;
}

class PatientEditView : public QWidget
{
    Q_OBJECT

public:
    // 增加 patientId 参数，默认值为 0 (代表新增)
    explicit PatientEditView(QWidget *parent = nullptr, int patientId = 0);
    ~PatientEditView();

private slots:
    void on_btn_Save_clicked();   // 保存按钮
    void on_btn_Cancel_clicked(); // 取消按钮

signals:
    void goPreviousView(); // 返回上一页的信号

private:
    Ui::PatientEditView *ui;

    // 用于存储当前编辑的患者ID
    int m_patientId;

    // 加载数据函数（修改模式下使用）
    void loadDataToUI();
};

#endif // PATIENTEDITVIEW_H
