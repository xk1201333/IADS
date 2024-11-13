#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>



int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    //连接MySQL数据库：
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");      //创建Mysql数据库连接
    db.setHostName("localhost"); //主机服务器
    db.setPort(3306);   //窗口
    db.setUserName("root");
    db.setPassword("19991201333");   //密码
    db.setDatabaseName("xk-qt"); //数据库名
    if(db.open()){
        qDebug()<<"数据库连接成功";
    }else{
        qDebug()<<"数据库连接失败";
        qDebug()<<db.lastError().text();//输出错误信息
    }


    MainWindow w;
    w.loadStyleSheet(":/IADS.qss");
    w.show();
    return a.exec();
}



