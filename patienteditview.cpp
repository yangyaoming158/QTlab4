#include "patienteditview.h"
#include "ui_patienteditview.h"
#include "idatabase.h" // 确保数据库连接已打开
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QDateTime>

// 构造函数接收 ID 并初始化成员变量
PatientEditView::PatientEditView(QWidget *parent, int patientId)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
    , m_patientId(patientId) // 初始化 ID
{
    ui->setupUi(this);

    // 根据 ID 判断模式
    if (m_patientId == 0) {
        // --- 新增模式 ---
        // 可以在这里设置一些默认值，或者清空输入框
        ui->dateEdit_DOB->setDate(QDate::currentDate()); // 出生日期默认为今天
    } else {
        // --- 修改模式 ---
        // 从数据库查询数据并填入界面
        loadDataToUI();
    }
}

PatientEditView::~PatientEditView()
{
    delete ui;
}

// 从数据库加载数据到界面
void PatientEditView::loadDataToUI()
{

    QSqlQuery query;
    query.prepare("SELECT * FROM Patient WHERE ID = :id");
    query.bindValue(":id", m_patientId);

    if (query.exec() && query.next()) {
        // 获取数据并设置到控件上
        ui->lineEdit_Name->setText(query.value("NAME").toString());
        ui->lineEdit_IDCard->setText(query.value("ID_CARD").toString());
        ui->spinBox_Height->setValue(query.value("HEIGHT").toInt());
        ui->spinBox_Weight->setValue(query.value("WEIGHT").toInt());
        ui->lineEdit_Number->setText(query.value("MOBILEPHONE").toString());

        // 设置出生日期 (数据库存的是字符串 "yyyy-MM-dd")
        QDate dob = QDate::fromString(query.value("DOB").toString(), "yyyy-MM-dd");
        ui->dateEdit_DOB->setDate(dob);

        // 设置性别 (下拉框顺序：0-男，1-女，且数据库存的是 0 或 1)
        // 如果数据库存的是文本 "男"/"女"，则用 ui->comboBox_Sex->setCurrentText(...)
        ui->comboBox_Sex->setCurrentIndex(query.value("SEX").toInt());
    }
}

// 保存按钮逻辑
void PatientEditView::on_btn_Save_clicked()
{
    // 1. 获取界面输入
    QString name = ui->lineEdit_Name->text();
    QString idCard = ui->lineEdit_IDCard->text();
    int height = ui->spinBox_Height->value();
    int weight = ui->spinBox_Weight->value();
    QString phone = ui->lineEdit_Number->text();
    QString dob = ui->dateEdit_DOB->date().toString("yyyy-MM-dd");
    int sex = ui->comboBox_Sex->currentIndex(); // 0 or 1

    // 简单校验
    if (name.isEmpty()) {
        QMessageBox::warning(this, "提示", "姓名不能为空");
        return;
    }
    // 1. 获取电脑当前时间，并格式化为字符串 (例如 "2023-10-27 15:30:00")
    QString createdTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query;
    bool success = false;

    // 2. 根据 ID 判断是 插入 还是 更新
    if (m_patientId == 0) {
        //  SQL 语句中加入 CREATEDTIMESTAMP 字段和占位符 :time
        query.prepare("INSERT INTO Patient (ID, NAME, ID_CARD, HEIGHT, WEIGHT, MOBILEPHONE, DOB, SEX, CREATEDTIMESTAMP) "
                      "VALUES (:id, :name, :idCard, :h, :w, :phone, :dob, :sex, :time)");
        //  绑定时间
        query.bindValue(":time", createdTime);
    } else {
        // --- 修改 (UPDATE) ---
        query.prepare("UPDATE Patient SET NAME = :name, ID_CARD = :idCard, "
                      "HEIGHT = :h, WEIGHT = :w, MOBILEPHONE = :phone, DOB = :dob, SEX = :sex "
                      "WHERE ID = :id");
        query.bindValue(":id", m_patientId);
    }

    // 3. 绑定通用参数
    query.bindValue(":name", name);
    query.bindValue(":idCard", idCard);
    query.bindValue(":h", height);
    query.bindValue(":w", weight);
    query.bindValue(":phone", phone);
    query.bindValue(":dob", dob);
    query.bindValue(":sex", sex);

    // 4. 执行
    if (query.exec()) {
        QMessageBox::information(this, "成功", "保存成功！");
        emit goPreviousView(); // 保存成功后返回列表页
    } else {
        QMessageBox::critical(this, "失败", "保存失败：" + query.lastError().text());
    }
}

void PatientEditView::on_btn_Cancel_clicked()
{
    emit goPreviousView(); // 直接返回
}
