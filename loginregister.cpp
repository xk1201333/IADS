#include "loginregister.h"
#include "ui_loginregister.h"


loginregister::loginregister(QWidget *parent) :QWidget(parent),ui(new Ui::loginregister){
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    //设置gif动画
    QMovie *movie=new QMovie(":/2.gif");
    ui->label->setMovie(movie);
    movie->start();
    ui->label->setScaledContents(true);

    QMovie *movie2=new QMovie(":/4.gif");
    ui->label2->setMovie(movie2);
    movie2->start();
    ui->label2->setScaledContents(true);

    //设置主
    ui->label_zhu->setText("还没有账号？");
    ui->pushButton_zhu->setText("去注册");
}

loginregister::~loginregister(){
    delete ui;
}

//加载qss
void loginregister::loadStyleSheet(const QString &styleSheetFile){  //styleSheetFile是qss文件的Path
    QFile file(styleSheetFile);
    if(file.open(QFile::ReadOnly)){
        //如果在MainWindow使用了setStyleSheet,对qss文件的内容进行追加
        //QString styleSheet = this->styleSheet();
        //styleSheet += QLatin1String(file.readAll());//读取样式表文件
        QString styleSheet = QLatin1String(file.readAll());  //QByteArray可直接转换为QString
        this->setStyleSheet(styleSheet);
        file.close();
    }else{
        QMessageBox::information(this,"tip","Failed to open file");
    }
}

void loginregister::on_pushButton_zhu_clicked(){
    if(ui->pushButton_zhu->text()=="去注册"){
        ui->stackedWidget->setCurrentIndex(1);
        ui->label_zhu->setText("已有账号？");
        ui->pushButton_zhu->setText("去登录");
    }else{
        ui->stackedWidget->setCurrentIndex(0);
        ui->label_zhu->setText("还没有账号？");
        ui->pushButton_zhu->setText("去注册");
    }
}

//登录
void loginregister::on_pushButton_clicked(){
    //接收用户输入
    QString name = ui->lineEdit->text();      //账号
    QString password = ui->lineEdit2->text();    //密码

    //查询数据：
    QSqlQuery query;    //操作数据库
    QString qs = QString("select * from xk_tuxiang where name = '%1' and password='%2'").arg(name).arg(password);

    if(!query.exec(qs)){    //如果没有查到记录
       QMessageBox::information(this,"登录","账号或密码错误！");
       return;
    }
    if(query.next()){
       denlu=true;
       QMessageBox::StandardButton result=QMessageBox::information (this,"登录","登录成功");
       if(result==QMessageBox::Ok){
          this->close();
       }
    }
}


//注册
void loginregister::on_pushButton_2_clicked(){
    //接收用户输入
    QString name = ui->lineEdit_2->text();      //账号
    QString password = ui->lineEdit_3->text();    //密码

    //查询数据：
    QSqlQuery query;    //操作数据库

    QString qs = QString("insert into xk_tuxiang(name,password) values('%1','%2')").arg(name).arg(password);
    if(query.exec(qs)){     //如果插入数据成功
        QMessageBox::information(this,"注册","注册成功，去登录吧！");
    }else{
        QMessageBox::information(this,"注册","注册失败");
        qDebug() <<query.lastError().text(); //输出错误信息
        return;
    }

}
