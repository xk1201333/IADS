#include "ftpwindow.h"
#include "ui_ftpwindow.h"

ftpwindow::ftpwindow(QWidget *parent) :QWidget(parent),ui(new Ui::ftpwindow){
    ui->setupUi(this);
}

ftpwindow::~ftpwindow(){
    delete ui;
}

//加载qss
void ftpwindow::loadStyleSheet(const QString &styleSheetFile){  //styleSheetFile是qss文件的Path
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




void ftpwindow::on_uploadButton_clicked(){
    file = new QFile("test.jpeg");
    file->open(QIODevice::ReadOnly);
    QByteArray byte_file = file->readAll();

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QString path = "ftp://" + serverIp + "/home/桌面/";
    //QUrl url("ftp://192.168.2.12/home/test/" + ui->lineEdit->text()+ ".wav");
    QUrl url(path + ui->lineEdit->text()+ ".jpeg");
    url.setPort(portNumebr);
    url.setUserName(userName);
    url.setPassword(passWord);

    QNetworkRequest request(url);
    reply = accessManager->put(request, byte_file);

    //ui->progressBar = new QProgressBar();
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));

}

void ftpwindow::on_downloadButton_clicked(){
    //file = new QFile(QApplication::applicationDirPath() + "C:\\Users\\cxw\\Downloads\\" + ui->lineEdit->text() + ".mp3");
    file = new QFile(QApplication::applicationDirPath() + "/" + ui->lineEdit->text() + ".mp3");
    file->open(QIODevice::WriteOnly);

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    //QString serverIp = "192.168.2.12";
    QString path = "ftp://" + serverIp + "/home/test/xxx.mp3";
    QUrl url(path);
    url.setPort(portNumebr);
    url.setUserName(userName);
    url.setPassword(passWord);

    QNetworkRequest request(url);
    reply = accessManager->get(request);

    //progressBar = new QProgressBar();
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    connect((QObject *)reply, SIGNAL(readyRead()), this, SLOT(readContent()));
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
}


//下载时向本地文件中写入数据
void ftpwindow::readContent(){
    file->write(reply->readAll());
}


//删除指针，更新和关闭文件
void ftpwindow::replyFinished(QNetworkReply*){
    if(reply->error() == QNetworkReply::NoError){
            reply->deleteLater();
            file->flush();
            file->close();
        }else{
            QMessageBox::critical(NULL, tr("Error"), "Failed!!!");
        }
}

//更新进度条
void ftpwindow::loadProgress(qint64 bytesSent, qint64 bytesTotal)    {
       qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
       ui->progressBar->setMaximum(bytesTotal); //最大值
       ui->progressBar->setValue(bytesSent);  //当前值
}

//传输中的错误输出
void ftpwindow::loadError(QNetworkReply::NetworkError)    {
     qDebug()<<"Error: "<<reply->error();
}


void ftpwindow::on_setPara_clicked(){
    //ftp parameter
    serverIp = ui->serverIP->text();
    portNumebr = ui->port->text().toInt();
    userName = ui->userName->text();
    passWord = ui->passWord->text();

    qDebug()<<"serverIp："<<serverIp<<endl;
    qDebug()<<"port："<<portNumebr<<endl;
    qDebug()<<"userName："<<userName<<endl;
    qDebug()<<"passWord："<<passWord<<endl;

    //server ip
    if(serverIp == NULL){
        qDebug()<<"input serverIp invalid"<<endl;
        return ;
    }

    if(portNumebr == NULL){
        qDebug()<<"input portNumebr invalid"<<endl;
        return ;
    }

    if(userName == NULL){
        qDebug()<<"input userName invalid"<<endl;
        return ;
    }

    if(passWord == NULL){
        qDebug()<<"input passWord invalid"<<endl;
        return ;
    }

    ui->setPara->setEnabled(false);
}
