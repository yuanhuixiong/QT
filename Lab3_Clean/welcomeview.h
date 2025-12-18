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

signals:
    void goDepartmentView();
    void goDoctorView();
    void goPatientView();

private slots:

    void on_btDoctor_clicked();

    void on_btPatient_clicked();

    void on_btDepartment_clicked();

private:
    Ui::WelcomeView *ui;
};

#endif // WELCOMEVIEW_H
