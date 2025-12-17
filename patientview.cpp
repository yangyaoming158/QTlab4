#include "patientview.h"
#include "ui_patientview.h"
#include "idatabase.h"
#include <QMessageBox>
#include <QSqlRecord>
#include <QDebug>
#include <QStyledItemDelegate>

// 定义一个自定义代理类
class SexDelegate : public QStyledItemDelegate
{
public:
    SexDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    // 重写显示文本的函数
    QString displayText(const QVariant &value, const QLocale &locale) const override
    {
        // 这里假设数据库存的是：1代表男，2代表女（根据你之前的截图）
        // 请根据你实际情况修改判断逻辑
        int sex = value.toInt();
        if (sex == 1) {
            return "男";
        } else if (sex == 2) {
            return "女";
        } else {
            return "未知"; // 或者返回 value.toString()
        }
    }
};

PatientView::PatientView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PatientView)
{
    ui->setupUi(this);

    // 初始化表格模型
    initModel();
}

PatientView::~PatientView()
{
    delete ui;
    // model 的 parent 是 this，会自动释放，不需要手动 delete
}

void PatientView::initModel()
{
    // 1. 创建模型，关联名为 "Patient" 的表
    model = new QSqlTableModel(this);
    model->setTable("Patient");

    // 2. 设置表头别名（显示中文，而不是数据库字段名）
    // 对应ER 图字段
    model->setHeaderData(model->fieldIndex("ID"), Qt::Horizontal, "ID");
    model->setHeaderData(model->fieldIndex("NAME"), Qt::Horizontal, "姓名");
    model->setHeaderData(model->fieldIndex("ID_CARD"), Qt::Horizontal, "身份证");
    model->setHeaderData(model->fieldIndex("SEX"), Qt::Horizontal, "性别");
    model->setHeaderData(model->fieldIndex("DOB"), Qt::Horizontal, "出生日期");
    model->setHeaderData(model->fieldIndex("MOBILEPHONE"), Qt::Horizontal, "手机号");
    model->setHeaderData(model->fieldIndex("HEIGHT"), Qt::Horizontal, "身高");
    model->setHeaderData(model->fieldIndex("WEIGHT"), Qt::Horizontal, "体重");
    // 给时间列设置一个中文表头
    model->setHeaderData(model->fieldIndex("CREATEDTIMESTAMP"), Qt::Horizontal, "创建时间");

    // 3. 设置数据编辑策略：界面上修改后，需要手动提交才能保存到数据库
    // 因为有专门的 EditView，所以这里其实主要用于展示
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 4. 将模型绑定到 UI 上的 tableView
    ui->tableView->setModel(model);

    // 【新增】给性别列安装代理
    // 1. 获取性别列的索引
    int sexColumnIndex = model->fieldIndex("SEX");

    // 2. 创建并设置代理
    // 注意：SexDelegate 会随着 PatientView 的销毁而自动销毁，因为传了 this
    ui->tableView->setItemDelegateForColumn(sexColumnIndex, new SexDelegate(this));
    // 6. UI 交互优化
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中整行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止在列表中直接双击编辑
    ui->tableView->setAlternatingRowColors(true); // 隔行变色

    // 7. 初次加载数据
    model->select();
}

// 提供给外部调用的刷新函数
void PatientView::refreshTable()
{
    if(model) {
        model->select(); // 重新从数据库读取数据
    }
}

// ---------------- 按钮逻辑 ----------------

// 1. 添加按钮
void PatientView::on_btn_Add_clicked()
{
    // 发送信号，ID为0代表新增
    emit ansPatientEditView(0);
}

// 2. 查找按钮
void PatientView::on_btn_Search_clicked()
{
    QString filterText = ui->lineEdit_Search->text();

    if (filterText.isEmpty()) {
        // 如果搜索框为空，清除过滤器，显示所有
        model->setFilter("");
    } else {
        // 根据姓名进行模糊查询 ( WHERE NAME LIKE '%张%' )
        QString filterStr = QString("NAME LIKE '%%1%'").arg(filterText);
        model->setFilter(filterStr);
    }

    // 执行查询
    model->select();
}

// 3. 删除按钮
void PatientView::on_btn_Delete_clicked()
{
    // 获取当前选中的行
    int curRow = ui->tableView->currentIndex().row();
    if (curRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要删除的行");
        return;
    }

    // 弹出确认框
    int ret = QMessageBox::question(this, "确认删除",
                                    "确定要删除该患者信息吗？\n此操作不可恢复。",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // 执行删除
        model->removeRow(curRow); // 此时只是在模型中标记删除

        // 提交到数据库
        if (model->submitAll()) {
            QMessageBox::information(this, "成功", "删除成功");
        } else {
            // 如果失败（比如数据库锁死），回滚模型状态
            model->revertAll();
            QMessageBox::warning(this, "失败", "删除失败: " + model->lastError().text());
        }
    }
}

// 4. 修改按钮
void PatientView::on_btn_Edit_clicked()
{
    // 1. 获取当前行
    int curRow = ui->tableView->currentIndex().row();

    // 【调试信息】看看点没点击，选没选中
    qDebug() << "点击修改按钮，当前选中行号：" << curRow;

    if (curRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要修改的患者");
        return;
    }

    // 2. 获取 ID
    // 注意：这里的 "ID" 必须和在 initModel() 里 setHeaderData 之前的数据库字段名大小写一致
    // 数据库里是大写的 "ID"
    QSqlRecord record = model->record(curRow);
    int patientId = record.value("ID").toInt();

    // 【调试信息】看看获取到的 ID 是多少
    qDebug() << "获取到的 ID:" << patientId;

    // 3. 发送信号
    emit ansPatientEditView(patientId);
}
