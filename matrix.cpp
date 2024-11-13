#include "matrix.h"
#include "ui_matrix.h"

matrix::matrix(QWidget *parent) :QWidget(parent),ui(new Ui::matrix){
    ui->setupUi(this);
}

matrix::~matrix(){
    delete ui;
}
//加载qss
void matrix::loadStyleSheet(const QString &styleSheetFile){  //styleSheetFile是qss文件的Path
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

void matrix::on_pb00_clicked(){
    if(ui->pb00->text()=="0"){
        ui->pb00->setText("1");
    }else{
        ui->pb00->setText("0");
    }
    kernel2.ptr<int>(0)[0]=ui->pb00->text().toInt(); //字符串转数字
    //ui->label->setText(QString::number(kernel2.ptr<int>(0)[0])); //数字转字符串

}

void matrix::on_pb01_clicked()
{
    if(ui->pb01->text()=="0"){
        ui->pb01->setText("1");
    }else{
        ui->pb01->setText("0");
    }
    kernel2.ptr<int>(0)[1]=ui->pb01->text().toInt();
}


void matrix::on_pb02_clicked()
{
    if(ui->pb02->text()=="0"){
        ui->pb02->setText("1");
    }else{
        ui->pb02->setText("0");
    }
    kernel2.ptr<int>(0)[2]=ui->pb02->text().toInt();

}


void matrix::on_pb03_clicked()
{
    if(ui->pb03->text()=="0"){
        ui->pb03->setText("1");
    }else{
        ui->pb03->setText("0");
    }
    kernel2.ptr<int>(0)[3]=ui->pb03->text().toInt(); //字符串转数字
}

void matrix::on_pb04_clicked()
{
    if(ui->pb04->text()=="0"){
        ui->pb04->setText("1");
    }else{
        ui->pb04->setText("0");
    }

    kernel2.ptr<int>(0)[4]=ui->pb04->text().toInt();
}

void matrix::on_pb05_clicked()
{
    if(ui->pb05->text()=="0"){
        ui->pb05->setText("1");
    }else{
        ui->pb05->setText("0");
    }
    kernel2.ptr<int>(0)[5]=ui->pb05->text().toInt();
}

void matrix::on_pb06_clicked()
{
    if(ui->pb06->text()=="0"){
        ui->pb06->setText("1");
    }else{
        ui->pb06->setText("0");
    }
    kernel2.ptr<int>(0)[6]=ui->pb06->text().toInt();
}


void matrix::on_pb10_clicked()
{
    if(ui->pb10->text()=="0"){
        ui->pb10->setText("1");
    }else{
        ui->pb10->setText("0");
    }
    kernel2.ptr<int>(1)[0]=ui->pb10->text().toInt();
}

void matrix::on_pb11_clicked()
{
    if(ui->pb11->text()=="0"){
        ui->pb11->setText("1");
    }else{
        ui->pb11->setText("0");
    }
    kernel2.ptr<int>(1)[1]=ui->pb11->text().toInt();
}

void matrix::on_pb12_clicked()
{
    if(ui->pb12->text()=="0"){
        ui->pb12->setText("1");
    }else{
        ui->pb12->setText("0");
    }
    kernel2.ptr<int>(1)[2]=ui->pb12->text().toInt();
}

void matrix::on_pb13_clicked()
{
    if(ui->pb13->text()=="0"){
        ui->pb13->setText("1");
    }else{
        ui->pb13->setText("0");
    }
    kernel2.ptr<int>(1)[3]=ui->pb13->text().toInt();
}

void matrix::on_pb14_clicked()
{
    if(ui->pb14->text()=="0"){
        ui->pb14->setText("1");
    }else{
        ui->pb14->setText("0");
    }
    kernel2.ptr<int>(1)[4]=ui->pb14->text().toInt();
}

void matrix::on_pb15_clicked()
{
    if(ui->pb15->text()=="0"){
        ui->pb15->setText("1");
    }else{
        ui->pb15->setText("0");
    }
    kernel2.ptr<int>(1)[5]=ui->pb15->text().toInt();
}

void matrix::on_pb16_clicked()
{
    if(ui->pb16->text()=="0"){
        ui->pb16->setText("1");
    }else{
        ui->pb16->setText("0");
    }
    kernel2.ptr<int>(1)[6]=ui->pb16->text().toInt();
}



void matrix::on_pb20_clicked(){
    if(ui->pb20->text()=="0"){
        ui->pb20->setText("1");
    }else{
        ui->pb20->setText("0");
    }
    kernel2.ptr<int>(2)[0]=ui->pb20->text().toInt();

}

void matrix::on_pb21_clicked()
{
    if(ui->pb21->text()=="0"){
        ui->pb21->setText("1");
    }else{
        ui->pb21->setText("0");
    }
    kernel2.ptr<int>(2)[1]=ui->pb21->text().toInt();
}


void matrix::on_pb22_clicked()
{
    if(ui->pb22->text()=="0"){
        ui->pb22->setText("1");
    }else{
        ui->pb22->setText("0");
    }
    kernel2.ptr<int>(2)[2]=ui->pb22->text().toInt();
}


void matrix::on_pb23_clicked()
{
    if(ui->pb23->text()=="0"){
        ui->pb23->setText("1");
    }else{
        ui->pb23->setText("0");
    }
    kernel2.ptr<int>(2)[3]=ui->pb23->text().toInt();
}

void matrix::on_pb24_clicked()
{
    if(ui->pb24->text()=="0"){
        ui->pb24->setText("1");
    }else{
        ui->pb24->setText("0");
    }
    kernel2.ptr<int>(2)[4]=ui->pb24->text().toInt();
}

void matrix::on_pb25_clicked()
{
    if(ui->pb25->text()=="0"){
        ui->pb25->setText("1");
    }else{
        ui->pb25->setText("0");
    }
    kernel2.ptr<int>(2)[5]=ui->pb25->text().toInt();
}

void matrix::on_pb26_clicked()
{
    if(ui->pb26->text()=="0"){
        ui->pb26->setText("1");
    }else{
        ui->pb26->setText("0");
    }
    kernel2.ptr<int>(2)[6]=ui->pb26->text().toInt();
}




void matrix::on_pb30_clicked(){
    if(ui->pb30->text()=="0"){
        ui->pb30->setText("1");
    }else{
        ui->pb30->setText("0");
    }
    kernel2.ptr<int>(3)[0]=ui->pb30->text().toInt();

}

void matrix::on_pb31_clicked()
{
    if(ui->pb31->text()=="0"){
        ui->pb31->setText("1");
    }else{
        ui->pb31->setText("0");
    }
    kernel2.ptr<int>(3)[1]=ui->pb31->text().toInt();
}


void matrix::on_pb32_clicked()
{
    if(ui->pb32->text()=="0"){
        ui->pb32->setText("1");
    }else{
        ui->pb32->setText("0");
    }
    kernel2.ptr<int>(3)[2]=ui->pb32->text().toInt();
}


void matrix::on_pb33_clicked()
{
    if(ui->pb33->text()=="0"){
        ui->pb33->setText("1");
    }else{
        ui->pb33->setText("0");
    }
    kernel2.ptr<int>(3)[3]=ui->pb33->text().toInt();
}

void matrix::on_pb34_clicked()
{
    if(ui->pb34->text()=="0"){
        ui->pb34->setText("1");
    }else{
        ui->pb34->setText("0");
    }
    kernel2.ptr<int>(3)[4]=ui->pb34->text().toInt();
}

void matrix::on_pb35_clicked()
{
    if(ui->pb35->text()=="0"){
        ui->pb35->setText("1");
    }else{
        ui->pb35->setText("0");
    }
    kernel2.ptr<int>(3)[5]=ui->pb35->text().toInt();
}

void matrix::on_pb36_clicked()
{
    if(ui->pb36->text()=="0"){
        ui->pb36->setText("1");
    }else{
        ui->pb36->setText("0");
    }
    kernel2.ptr<int>(3)[6]=ui->pb36->text().toInt();
}


void matrix::on_pb40_clicked(){
    if(ui->pb40->text()=="0"){
        ui->pb40->setText("1");
    }else{
        ui->pb40->setText("0");
    }
    kernel2.ptr<int>(4)[0]=ui->pb40->text().toInt();

}

void matrix::on_pb41_clicked()
{
    if(ui->pb41->text()=="0"){
        ui->pb41->setText("1");
    }else{
        ui->pb41->setText("0");
    }
    kernel2.ptr<int>(4)[1]=ui->pb41->text().toInt();
}


void matrix::on_pb42_clicked()
{
    if(ui->pb42->text()=="0"){
        ui->pb42->setText("1");
    }else{
        ui->pb42->setText("0");
    }
    kernel2.ptr<int>(4)[2]=ui->pb42->text().toInt();
}


void matrix::on_pb43_clicked()
{
    if(ui->pb43->text()=="0"){
        ui->pb43->setText("1");
    }else{
        ui->pb43->setText("0");
    }
    kernel2.ptr<int>(4)[3]=ui->pb43->text().toInt();
}

void matrix::on_pb44_clicked()
{
    if(ui->pb44->text()=="0"){
        ui->pb44->setText("1");
    }else{
        ui->pb44->setText("0");
    }
    kernel2.ptr<int>(4)[4]=ui->pb44->text().toInt();
}

void matrix::on_pb45_clicked()
{
    if(ui->pb45->text()=="0"){
        ui->pb45->setText("1");
    }else{
        ui->pb45->setText("0");
    }
    kernel2.ptr<int>(4)[5]=ui->pb45->text().toInt();
}

void matrix::on_pb46_clicked()
{
    if(ui->pb46->text()=="0"){
        ui->pb46->setText("1");
    }else{
        ui->pb46->setText("0");
    }
    kernel2.ptr<int>(4)[6]=ui->pb46->text().toInt();
}

void matrix::on_pb50_clicked(){
    if(ui->pb50->text()=="0"){
        ui->pb50->setText("1");
    }else{
        ui->pb50->setText("0");
    }
    kernel2.ptr<int>(5)[0]=ui->pb50->text().toInt();

}

void matrix::on_pb51_clicked()
{
    if(ui->pb51->text()=="0"){
        ui->pb51->setText("1");
    }else{
        ui->pb51->setText("0");
    }
    kernel2.ptr<int>(5)[1]=ui->pb51->text().toInt();
}


void matrix::on_pb52_clicked()
{
    if(ui->pb52->text()=="0"){
        ui->pb52->setText("1");
    }else{
        ui->pb52->setText("0");
    }
    kernel2.ptr<int>(5)[2]=ui->pb52->text().toInt();
}


void matrix::on_pb53_clicked()
{
    if(ui->pb53->text()=="0"){
        ui->pb53->setText("1");
    }else{
        ui->pb53->setText("0");
    }
    kernel2.ptr<int>(5)[3]=ui->pb53->text().toInt();
}

void matrix::on_pb54_clicked()
{
    if(ui->pb54->text()=="0"){
        ui->pb54->setText("1");
    }else{
        ui->pb54->setText("0");
    }
    kernel2.ptr<int>(5)[4]=ui->pb54->text().toInt();
}

void matrix::on_pb55_clicked()
{
    if(ui->pb55->text()=="0"){
        ui->pb55->setText("1");
    }else{
        ui->pb55->setText("0");
    }
    kernel2.ptr<int>(5)[5]=ui->pb55->text().toInt();
}

void matrix::on_pb56_clicked()
{
    if(ui->pb56->text()=="0"){
        ui->pb56->setText("1");
    }else{
        ui->pb56->setText("0");
    }
    kernel2.ptr<int>(5)[6]=ui->pb56->text().toInt();
}



void matrix::on_pb60_clicked(){
    if(ui->pb60->text()=="0"){
        ui->pb60->setText("1");
    }else{
        ui->pb60->setText("0");
    }
    kernel2.ptr<int>(6)[0]=ui->pb60->text().toInt();

}

void matrix::on_pb61_clicked()
{
    if(ui->pb61->text()=="0"){
        ui->pb61->setText("1");
    }else{
        ui->pb61->setText("0");
    }
    kernel2.ptr<int>(6)[1]=ui->pb61->text().toInt();
}


void matrix::on_pb62_clicked()
{
    if(ui->pb62->text()=="0"){
        ui->pb62->setText("1");
    }else{
        ui->pb62->setText("0");
    }
    kernel2.ptr<int>(6)[2]=ui->pb62->text().toInt();
}


void matrix::on_pb63_clicked()
{
    if(ui->pb63->text()=="0"){
        ui->pb63->setText("1");
    }else{
        ui->pb63->setText("0");
    }
    kernel2.ptr<int>(6)[3]=ui->pb63->text().toInt();
}

void matrix::on_pb64_clicked()
{
    if(ui->pb64->text()=="0"){
        ui->pb64->setText("1");
    }else{
        ui->pb64->setText("0");
    }
    kernel2.ptr<int>(6)[4]=ui->pb64->text().toInt();
}

void matrix::on_pb65_clicked()
{
    if(ui->pb65->text()=="0"){
        ui->pb65->setText("1");
    }else{
        ui->pb65->setText("0");
    }
    kernel2.ptr<int>(6)[5]=ui->pb65->text().toInt();
}

void matrix::on_pb66_clicked()
{
    if(ui->pb66->text()=="0"){
        ui->pb66->setText("1");
    }else{
        ui->pb66->setText("0");
    }
    kernel2.ptr<int>(6)[6]=ui->pb66->text().toInt();
}

