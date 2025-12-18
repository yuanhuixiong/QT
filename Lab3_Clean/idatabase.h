#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>


class IDatabase : public QObject
{
    Q_OBJECT
public:

    static IDatabase &getInstance()
    {
        static IDatabase    instance;
        return instance;
    }

    IDatabase(IDatabase const&)               = delete;
    void operator=(IDatabase const&)  = delete;
    QString userLogin(QString userName,QString password);
    bool initPatientModle();
    QSqlTableModel *patientTabModel;//数据模型
    QItemSelectionModel *thepatientSelection;//选择模型

    int addNewpatient();
    bool searchPatient(QString filter);
    void deleteCurrentPatient();
    bool submitPatientEdit();
    void revertPatientEdit();

private:

    explicit IDatabase(QObject *parent = nullptr);
    QSqlDatabase database;
    void ininDatabase();


signals:
};

#endif // IDATABASE_H
