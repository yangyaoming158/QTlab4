#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>
#include <QSqlTableModel> // 用于管理数据库表数据

namespace Ui {
class PatientView;
}

class PatientView : public QWidget
{
    Q_OBJECT

public:
    explicit PatientView(QWidget *parent = nullptr);
    ~PatientView();

    // 公开一个刷新函数，方便外部（如保存后）调用刷新列表
    void refreshTable();

private slots:
    void on_btn_Add_clicked();    // 添加按钮
    void on_btn_Search_clicked(); // 查找按钮
    void on_btn_Delete_clicked(); // 删除按钮
    void on_btn_Edit_clicked();   // 修改按钮

signals:
    void ansPatientEditView(int patientId = 0); // 带参数，0表示新增，非0表示修改ID

private:
    Ui::PatientView *ui;

    // 数据模型
    QSqlTableModel *model;

    // 初始化表格样式的辅助函数
    void initModel();
};

#endif // PATIENTVIEW_H
