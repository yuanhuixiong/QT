#include "patienteditview.h"
#include "ui_patienteditview.h"
#include <QDataWidgetMapper>
#include "idatabase.h"
PatientEditView::PatientEditView(QWidget *parent,int row)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = IDatabase::getInstance().patientTabModel;
    dataMapper->setModel(tabModel);

    // 改为 ManualSubmit 策略
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIdcard, tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbHeight, tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbWeight, tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbMobile, tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbDate, tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbSex, tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbCreateTime, tabModel->fieldIndex("CREATEDTIMESTAMP"));

    dataMapper->setCurrentIndex(row);
    ui->dbEditID->setEnabled(false);

    // 确保ID字段有值
    QString currentId = ui->dbEditID->text();
    if (currentId.isEmpty()) {
        // 从模型中获取ID
        QModelIndex idIndex = tabModel->index(row, tabModel->fieldIndex("ID"));
        currentId = tabModel->data(idIndex).toString();

        // 如果模型中的ID也为空，生成新的
        if (currentId.isEmpty()) {
            currentId = QUuid::createUuid().toString(QUuid::WithoutBraces);
            tabModel->setData(idIndex, currentId);
        }

        // 更新界面显示
        ui->dbEditID->setText(currentId);
    }
}
PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_pushButton_clicked()
{
    dataMapper->submit();

    // 然后提交整个模型到数据库
    if (IDatabase::getInstance().submitPatientEdit()) {
        emit goPreviousView();
    } else {
        qDebug() << "数据库提交失败";
        // 可以显示错误消息
    }
}

void PatientEditView::on_pushButton_2_clicked()
{
    IDatabase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

