#ifndef LOGINREGISTER_H
#define LOGINREGISTER_H

#include <QWidget>
#include <QMovie>
#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>    //操作数据库
#include <QDebug>
#include <QSqlError>
#include <QSettings>    //读写配置文件
//让编译器编译生成程序的执行字符集为utf-8
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class loginregister;
}

class loginregister : public QWidget
{
    Q_OBJECT

public:
    bool denlu=false;
    explicit loginregister(QWidget *parent = nullptr);
    ~loginregister();

    void loadStyleSheet(const QString &styleSheetFile);
    bool getdenlu(){
        return denlu;
    }

private slots:
    void on_pushButton_zhu_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::loginregister *ui;
};

#endif // LOGINREGISTER_H
