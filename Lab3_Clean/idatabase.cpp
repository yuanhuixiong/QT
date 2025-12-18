#include "idatabase.h"
#include <QModelIndex>
#include <QUuid>

void IDatabase::ininDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");//添加驱动
    QString aFile = "D:/QT/Lab3_Clean/Lab3.db";
    database.setDatabaseName(aFile);//设置数据库名称
    if(!database.open()){
        qDebug()<< "fail to open database";
    }else
        qDebug()<< "open database is ok" << database.connectionName();
}

bool IDatabase::initPatientModle()
{
    patientTabModel = new QSqlTableModel(this,database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientTabModel->setSort(patientTabModel->fieldIndex("name"),Qt::AscendingOrder);//排序
    if(!(patientTabModel->select()))//查询
        return false;
    thepatientSelection = new QItemSelectionModel(patientTabModel);
    return true;
}

int IDatabase::addNewpatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount()-1,1);
    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);
    curRec.setValue("CREATEDTIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));
    patientTabModel->setRecord(curRecNo,curRec);

    return curIndex.row();
}

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

void IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thepatientSelection->currentIndex();
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();
}


bool IDatabase::submitPatientEdit()
{
    bool success = patientTabModel->submitAll();
    if (success) {
        patientTabModel->select(); // 重新查询确保数据同步
    } else {
        qDebug() << "提交失败：" << patientTabModel->lastError().text();
    }
    return success;
}

void IDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

QString IDatabase::userLogin(QString userName, QString password)
{
    QSqlQuery query;
    query.prepare("select username,password from user where username = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("username").isValid()){
        QString passwd = query.value("password").toString();
        if(passwd == password)
        {
            return "OK";
        }
        else
        {
            qDebug() << "Password is wrong";
            return "wrongPassword";
        }
    }else{
        qDebug() << "no such user";
        return "wrongUsername";
    }
}

IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}
