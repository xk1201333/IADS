#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    if(srcImg.empty()){
        srcImg = cv::imread(defaultimgpath);
        currentImg=srcImg;
        cv::cvtColor(srcImg, grayImg, cv::COLOR_BGR2GRAY);
    }

    ui->setupUi(this);      //界面初始化

    //初始化文件树
    file_model = new MyFileSytemModel;
    file_model->setRootPath(QDir::currentPath());  //设置要显示的根目录
    ui->treeView_File->setModel(file_model);    //设置数据源
    ui->treeView_File->setRootIndex(file_model->index(QDir::currentPath()));  //设置树形视图的根节点

    //去除dockwidget顶部标题栏
    QWidget* pTitleBar = ui->dockWidget_tool->titleBarWidget();
    QWidget* pEmptyWidget = new QWidget();
    ui->dockWidget_tool->setTitleBarWidget(pEmptyWidget);
    delete pTitleBar;
    pTitleBar=NULL;

    isOpeing = false;   // 是否正在读取文件
    Rad_G::m_mainWin = this; // 添加
    connect(ui->graphicsView_XY, SIGNAL(windowChanged()), this, SLOT(on_windowChanged()));
    ui->graphicsView_XY->setHighlight(true);
    connect(ui->graphicsView_XZ, SIGNAL(windowChanged()), this, SLOT(on_windowChanged()));
    connect(ui->graphicsView_YZ, SIGNAL(windowChanged()), this, SLOT(on_windowChanged()));
    connect(ui->graphicsView_XY, SIGNAL(clicked(SeriesBase::ViewType)), this, SLOT(on_clickedView(SeriesBase::ViewType)));
    connect(ui->graphicsView_XZ, SIGNAL(clicked(SeriesBase::ViewType)), this, SLOT(on_clickedView(SeriesBase::ViewType)));
    connect(ui->graphicsView_YZ, SIGNAL(clicked(SeriesBase::ViewType)), this, SLOT(on_clickedView(SeriesBase::ViewType)));
    xyS = new XYSeries(); // 添加
    xzS = new XZSeries();
    yzS = new YZSeries();


    //设置gif动画
    ui->toolBox->setCurrentIndex(7);
    QMovie *movie=new QMovie(":/2.gif");
    ui->label_gif->setMovie(movie);
    movie->start();
    ui->label_gif->setAlignment(Qt::AlignCenter); // 居中
    //ui->label_gif->setScaledContents(true); //允许缩放
    ui->stackedWidget->setCurrentIndex(29);

    // 将初始化的标签添加到底部状态栏上
    status_label = new QLabel("",this);
    ui->statusbar->addPermanentWidget(status_label);


    //加载qss格式文件
    loginregister_window.loadStyleSheet(":/login_register.qss");
    ftp_window.loadStyleSheet(":/ftp.qss");
    personalcente_window.loadStyleSheet(":/personalcenter.qss");
    matrixwindow.loadStyleSheet(":/IADS.qss");
}


MainWindow::~MainWindow(){
    delete ui;
    delete xyS;
    delete xzS;
    delete yzS;
}


//加载qss
void MainWindow::loadStyleSheet(const QString &styleSheetFile){  //styleSheetFile是qss文件的Path
    QFile file(styleSheetFile);
    if(file.open(QFile::ReadOnly)){
        QString styleSheet = QLatin1String(file.readAll());  //QByteArray可直接转换为QString
        this->setStyleSheet(styleSheet);
        file.close();
    }else{
        QMessageBox::information(this,"tip","Failed to open qss file");
    }
}


//设置初始界面
void MainWindow::on_toolBox_currentChanged(int index){
    if(index==7){
        ui->stackedWidget->setCurrentIndex(29);
        //设置gif动画
        QMovie *movie=new QMovie(":/2.gif");
        ui->label_gif->setMovie(movie);
        movie->start();
        ui->label_gif->setAlignment(Qt::AlignCenter); // 居中
        //ui->label_gif->setScaledContents(true);     //允许缩放
    }
}


//图片切换
void MainWindow::Imgchange(cv::Mat &img){
    cv::Mat imgShow;
    cvtColor(img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);

    switch (page_idx){
    case 0:
        ui->label_tu_01->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_01->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 1:
        ui->label_tu_11->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_11->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 2:
        ui->label_tu_21->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_11->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 3:
        ui->label_tu_31->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_31->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 4:
        ui->label_tu_41->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_41->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 5:
        ui->label_tu_51->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_51->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 6:
        ui->label_tu_61->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_61->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 7:
        ui->label_tu_71->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_71->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 8:
        ui->label_tu_81->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_81->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 9:
        ui->graphicsView_9->SetImage(qImg);
        break;
    case 10:
        ui->graphicsView_10->SetImage(qImg);
        break;

    case 16:
        ui->label_tu_161->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_161->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;
    case 17:
        ui->label_tu_171->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_171->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        break;

    case 35:
        ui->graphicsView_45->SetImage(qImg);
        break;
    case 36:
        ui->graphicsView_46->SetImage(qImg);
        break;
    }
}


//文件树
void MainWindow::on_treeView_File_clicked(const QModelIndex &index){
    QString FilePath =file_model->filePath(index);

    if(!FilePath.isEmpty()){
        srcImg = cv::imread(FilePath.toStdString());

        if(srcImg.empty()){
            QMessageBox::information(this,"tip","Could not open the image");
            srcImg = cv::imread(defaultimgpath);
            return ;
        }
        currentImg=srcImg;
        cvtColor(srcImg, grayImg, cv::COLOR_BGR2GRAY); //灰度化
    }
    //图片切换
    Imgchange(srcImg);
}


//菜单栏:打开图片
void MainWindow::on_actionopenfile_triggered(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片",".",tr("Image Files (*.png *.jpeg *.jpg *.bmp)"));

    if(!FileName.isEmpty()){
        srcImg = cv::imread(FileName.toStdString());

        if(srcImg.empty()){
            QMessageBox::information(this,"tip","Could not open the image");
            srcImg = cv::imread(defaultimgpath);
            return ;
        }
        currentImg=srcImg;
        cvtColor(srcImg, grayImg, cv::COLOR_BGR2GRAY); //灰度化
    }
    //图片切换
    Imgchange(srcImg);
}



//菜单栏:保存图片
void MainWindow::on_actionsaveafile_triggered(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存图片"),"",tr("Image Files (*.jpg)"));
    if (!fileName.isNull()){
        imwrite(fileName.toStdString(), currentImg);
    }else{
        qDebug() << "取消了保存";
    }
}

//菜单栏:打开文件夹
void MainWindow::on_actionioendic_triggered(){
    //获取选择的目录路径
    QString selectedDir=QFileDialog::getExistingDirectory(this,"选择一个目录","../tuxiang/dicom_path",QFileDialog::ShowDirsOnly);
    //若目录路径不为空
    if (!selectedDir.isEmpty()){
        //将路径中的斜杠替换为反斜杠
        selectedDir = selectedDir.replace(QRegExp("\\"), "/");
        dirchoose=selectedDir;
    }else{
        QMessageBox::information(this,"tip","Failed to open dir");
    }

    //显示缩略图
    QStringList imagePathList;  //所有图片路径加名字及后缀名
    QDir dir(selectedDir);
    QStringList filters;
    filters<<"*.png"<<"*.jpg";  // 设置哪些格式图片的可以显示
    dir.setNameFilters(filters);
    // 设置显示的item
    for(uint idx=0;idx<dir.count();idx++){
        QString imagePath = QString("%1\\%2").arg(selectedDir).arg(dir[idx]);
        imagePathList.push_back(imagePath);
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setIcon(QIcon(imagePathList[idx]));
        ui->ImglistWidget->addItem(pItem);
      }
}


void MainWindow::on_ImglistWidget_clicked(const QModelIndex &index){
    QDir dir(dirchoose);
    QStringList filters;
    filters<<"*.png"<<"*.jpg";  // 设置哪些格式图片的可以显示
    dir.setNameFilters(filters);
    QString imagePath = QString("%1\\%2").arg(dirchoose).arg(dir[index.row()]);


    if(!imagePath.isEmpty()){
        srcImg = cv::imread(imagePath.toStdString());

        if(srcImg.empty()){
            QMessageBox::information(this,"tip","Could not open the image");
            srcImg = cv::imread(defaultimgpath);
            return ;
        }
        currentImg=srcImg;
        cvtColor(srcImg, grayImg, cv::COLOR_BGR2GRAY); //灰度化
    }
    //图片切换
    Imgchange(srcImg);
}


//菜单栏:上传下载
void MainWindow::on_actionftp_triggered(){
    ftp_window.show();
}


//个人中心
void MainWindow::on_pushButton_centre_clicked(){
    if(loginregister_window.getdenlu()){
        personalcente_window.show();
    }else{
        loginregister_window.show();
    }

}

//*************************************************************通用的一些设置********************************************************************************************
void MainWindow::jiemianqiehuan(int index,QLabel *tu1,QLabel *tu2,QLabel *zi1,QLabel *zi2){   //界面切换
    ui->stackedWidget->setCurrentIndex(index);

    //显示
    cv::Mat imgShow;
    cvtColor(currentImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    tu1->setPixmap(QPixmap::fromImage(qImg.scaled(tu1->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    zi1->setText("Original Image");

    //清空
    tu2->setText(" ");
    zi2->setText(" ");
}


//*************************************************************图像预处理********************************************************************************************
/*
 * 灰度处理
 */
void MainWindow::on_tool_huiduchuli_clicked(){
    page_idx=0;    //界面索引
    function_idx=0;//功能的索引

    jiemianqiehuan(0,ui->label_tu_01,ui->label_tu_02,ui->label_05,ui->label_06);
    status_label->setText(QString(""));
}


//灰度化
void MainWindow::on_rgbtogray_clicked(){
    if(ui->checkBox_Pipeline->isChecked()){
      if (currentImg.channels() != 3){
          QMessageBox::information(this,"tip","channels() != 3");
          return;
      }else{
          cvtColor(currentImg, grayImg, cv::COLOR_BGR2GRAY);  //opcncv灰度化
      }
    }else{
      if(zijishixian){  //自己实现的
          grayImg.create(srcImg.rows, srcImg.cols, CV_8UC1);
          QImage Qtemp;
          for(int i = 0 ; i < srcImg.rows ; i++){
              for(int j = 0 ; j < srcImg.cols ; j++){
                  grayImg.at<uchar>(i,j) = (int)0.11 * srcImg.at<cv::Vec3b>(i,j)[0]+ 0.59 * srcImg.at<cv::Vec3b>(i,j)[1]+ 0.3 * srcImg.at<cv::Vec3b>(i,j)[2];
              }
          }
      }else{
          cvtColor(srcImg, grayImg, cv::COLOR_BGR2GRAY);  //opcncv灰度化
      }
    }

    currentImg=grayImg;

    //显示
    cv::Mat imgShow;
    cvtColor(currentImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_02->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_02->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->label_06->setText("Grayscale Image");
}

//二值化
void MainWindow::on_rgbtoerzhi_clicked(){
    if(ui->checkBox_Pipeline->isChecked()){
        threshold(currentImg, erzhiImg, 128, 255,cv::THRESH_BINARY);
    }else{
        threshold(srcImg, erzhiImg, 128, 255,cv::THRESH_BINARY);
    }

    currentImg=erzhiImg;

    //显示
    cv::Mat imgShow;
    cvtColor(currentImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_02->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_02->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->label_06->setText("Binary Image");
}

//反相化
void MainWindow::on_fanxiang_clicked(){
    if(ui->checkBox_Pipeline->isChecked()){
        bitwise_not(currentImg, fanxiangImg);  //像素取反
    }else{
        bitwise_not(srcImg, fanxiangImg);      //像素取反
    }

    currentImg=fanxiangImg;

    //显示
    cv::Mat imgShow;
    cvtColor(currentImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_02->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_02->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->label_06->setText("Inverted Image");
}


//灰度直方图函数
cv::Mat gray_to_level(cv::Mat gray){
    QVector<int> pixel(256,0);

    for(int i = 0 ; i < gray.rows ; i++)
        for(int j = 0 ; j < gray.cols ; j++){
            pixel[gray.at<uchar>(i,j)]++;
        }

    cv::Mat gray_level;
    gray_level.create(350, 256, CV_8UC1);

    int max_rows = 0;
    for(int i = 0;i <= 255;i++){
        if(pixel[i] > max_rows){
            max_rows = pixel[i];
        }
    }

    for(int i = 0;i < 256;i++){
        for(int j = 0;j < 350 ; j++){
            gray_level.at<uchar>(j,i) = 255;
        }
    }

    for(int i = 0;i < 256;i++){
        for(int j = 0;j < 350 - int(320.*float(pixel[i])/float(max_rows)) ; j++){
            gray_level.at<uchar>(j,i) = 0;
        }
    }

    return gray_level;
}

//灰度直方图
void MainWindow::on_gray_leval_clicked(){
    cv::Mat gray_level;

    if(ui->checkBox_Pipeline->isChecked()){
         gray_level = gray_to_level(currentImg);
    }else{
         gray_level = gray_to_level(grayImg);
    }
    currentImg=gray_level;

    //显示
    cv::Mat imgShow;
    cvtColor(gray_level,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_02->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_02->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->label_06->setText("Gray Histogram");
}

//直方图均衡化
void MainWindow::on_gray_balance_clicked() {
    cv::Mat balance,gray2Img;

    gray2Img.create(srcImg.rows, srcImg.cols, CV_8UC1);
    balance.create(srcImg.rows, srcImg.cols, CV_8UC1);
    QImage Qtemp;

    QVector<int> pixel(256,0);
    QVector<float> pixel_gray(256,0.);
    float sum=0;

    for(int i = 0 ; i < grayImg.rows ; i++)
        for(int j = 0 ; j < grayImg.cols ; j++){
            pixel[grayImg.at<uchar>(i,j)]++;
        }

    for(int i = 0 ; i < pixel.size() ; i++){
        sum += pixel[i];
    }

    for(int i = 0 ; i < 256 ; i++){
        float num = 0;
        for(int j = 0 ; j <= i ; j++){
            num += pixel[j];
        }
        pixel_gray[i] = 255*num/sum;
    }

    for(int i = 0 ; i < srcImg.rows ; i++)
        for(int j = 0 ; j < srcImg.cols ; j++){
            balance.at<uchar>(i,j) = pixel_gray[grayImg.at<uchar>(i,j)];
        }

    gray2Img = balance;

    //显示
    cv::Mat imgShow;
    cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_02->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_02->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->label_06->setText("Histogram Equalization");
}





/*
 * 图像锐化
 */
void MainWindow::on_tool_tuxiangruihua_clicked(){
    page_idx=1;    //界面索引
    function_idx=1;//功能的索引

    jiemianqiehuan(1,ui->label_tu_11,ui->label_tu_12,ui->label_15,ui->label_16);
    status_label->setText(QString(""));
}


//梯度锐化
void MainWindow::on_grad_sharpen_clicked(){
    cv::Mat grad,gray2Img;
    gray2Img.create(srcImg.rows, srcImg.cols, CV_8UC1);
    QImage Qtemp,Qtemp2;
    grad.create(srcImg.rows, srcImg.cols, CV_8UC1);
    for(int i = 0 ; i < gray2Img.rows - 1 ; i++)
        for(int j = 0 ; j < gray2Img.cols - 1 ; j++){
            grad.at<uchar>(i,j) = cv::saturate_cast<uchar>(std::max(fabs(grayImg.at<uchar>(i+1, j)-grayImg.at<uchar>(i,j)),fabs(grayImg.at<uchar>(i, j+1)-grayImg.at<uchar>(i,j))));
            gray2Img.at<uchar>(i,j) = cv::saturate_cast<uchar>(grayImg.at<uchar>(i,j) - grad.at<uchar>(i,j));
        }

    //显示
    ui->label_16->setText("梯度锐化");
    cv::Mat imgShow;
    cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_11->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_11->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


    cv::Mat imgShow2;
    cvtColor(grad,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
    ui->label_tu_12->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_12->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}


//Laplace锐化
void MainWindow::on_laplace_sharpen_clicked(){
    cv::Mat gradimg,gray2Img;
    QImage Qtemp,Qtemp2;
    gray2Img.create(grayImg.rows, grayImg.cols, CV_8UC1);
    gradimg.create(grayImg.rows, grayImg.cols, CV_8UC1);
    for (int i = 1; i < srcImg.rows - 1; i++){
        for (int j = 1; j < srcImg.cols - 1; j++){
            gradimg.at<uchar>(i, j) = cv::saturate_cast<uchar>(- 4 * grayImg.at<uchar>(i, j) + grayImg.at<uchar>(i + 1 , j)
                                                          + grayImg.at<uchar>(i, j + 1) + grayImg.at<uchar>(i - 1, j)
                                                          + grayImg.at<uchar>(i, j - 1));
            gray2Img.at<uchar>(i, j) = cv::saturate_cast<uchar>(5 * grayImg.at<uchar>(i, j) - grayImg.at<uchar>(i + 1, j)
                                                        - grayImg.at<uchar>(i, j + 1) - grayImg.at<uchar>(i - 1, j)
                                                        - grayImg.at<uchar>(i, j - 1));
        }
    }

    //显示
    ui->label_16->setText("Laplace锐化");
    cv::Mat imgShow;
    cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_11->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_11->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));

    cv::Mat imgShow2;
    cvtColor(gradimg,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
    ui->label_tu_12->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_12->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}


/*
 * 边缘检测
 */
void MainWindow::on_tool_bianyuanjiance_clicked(){
    page_idx=2;    //界面索引
    function_idx=2;//功能的索引

    jiemianqiehuan(2,ui->label_tu_21,ui->label_tu_22,ui->label_25,ui->label_26);
    status_label->setText(QString(""));
}


//canny双阈值处理
void DoubleThreshold(cv::Mat &imageIput,double lowThreshold,double highThreshold){
    for(int i=0;i<imageIput.rows;i++){
        for(int j=0;j<imageIput.cols;j++){
            if(imageIput.at<uchar>(i,j)>highThreshold){
                imageIput.at<uchar>(i,j)=255;
            }
            if(imageIput.at<uchar>(i,j)<lowThreshold){
                imageIput.at<uchar>(i,j)=0;
            }
        }
    }
}

//canny双阈值连接
void DoubleThresholdLink(cv::Mat &imageInput,double lowThreshold,double highThreshold){
    for(int i=1;i<imageInput.rows-1;i++){
        for(int j=1;j<imageInput.cols-1;j++){
            if(imageInput.at<uchar>(i,j)>lowThreshold&&imageInput.at<uchar>(i,j)<255){
                if(imageInput.at<uchar>(i-1,j-1)==255||imageInput.at<uchar>(i-1,j)==255||imageInput.at<uchar>(i-1,j+1)==255||
                    imageInput.at<uchar>(i,j-1)==255||imageInput.at<uchar>(i,j)==255||imageInput.at<uchar>(i,j+1)==255||
                    imageInput.at<uchar>(i+1,j-1)==255||imageInput.at<uchar>(i+1,j)==255||imageInput.at<uchar>(i+1,j+1)==255){
                    imageInput.at<uchar>(i,j)=255;
                    DoubleThresholdLink(imageInput,lowThreshold,highThreshold); //递归调用
                }else{
                    imageInput.at<uchar>(i,j)=0;
                }
            }
        }
    }
}

void MainWindow::on_Canny_clicked(){
    cv::Mat gauss,f_x,f_y,gradimg,max_control,gray2Img;
    QImage Qtemp,Qtemp2;
    gauss.create(grayImg.rows, grayImg.cols, CV_8UC1);
    gradimg.create(grayImg.rows, grayImg.cols, CV_8UC1);
    gray2Img.create(grayImg.rows, grayImg.cols, CV_8UC1);
    f_x.create(grayImg.rows, grayImg.cols, CV_8UC1);
    f_y.create(grayImg.rows, grayImg.cols, CV_8UC1);
    QVector<double> direction((grayImg.rows-1)*(grayImg.rows-1),0);
    //高斯处理
    for (int i = 0; i < grayImg.rows - 1; i++){
        for (int j = 0; j < grayImg.cols - 1; j++){
            gauss.at<uchar>(i,j) = cv::saturate_cast<uchar>(fabs((0.751136*grayImg.at<uchar>(i - 1, j - 1) +0.123841* grayImg.at<uchar>(i - 1, j) +0.0751136* grayImg.at<uchar>(i - 1, j + 1)
                                                         +0.123841*grayImg.at<uchar>(i, j - 1) +0.20418*grayImg.at<uchar>(i, j) + 0.123841*grayImg.at<uchar>(i, j + 1)
                                                         + 0.0751136*grayImg.at<uchar>(i + 1, j - 1) + 0.123841*grayImg.at<uchar>(i + 1, j) + 0.0751136*grayImg.at<uchar>(i + 1, j + 1))));
        }
    }
    //sobel处理
    int k = 0;
    for (int i = 1; i < gauss.rows - 1; i++){
        for (int j = 1; j < gauss.cols - 1; j++){
            f_x.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(grayImg.at<uchar>(i + 1, j - 1) + 2*grayImg.at<uchar>(i + 1, j) + grayImg.at<uchar>(i + 1, j + 1)
                                                            - grayImg.at<uchar>(i - 1, j - 1) - 2*grayImg.at<uchar>(i - 1, j) - grayImg.at<uchar>(i - 1, j + 1)));
            f_y.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(grayImg.at<uchar>(i - 1, j + 1) + 2*grayImg.at<uchar>(i, j + 1) + grayImg.at<uchar>(i + 1, j + 1)
                                                            - grayImg.at<uchar>(i - 1, j - 1) - 2*grayImg.at<uchar>(i, j - 1) - grayImg.at<uchar>(i + 1, j - 1)));
            gradimg.at<uchar>(i, j) = sqrt(pow(f_x.at<uchar>(i, j),2)+pow(f_y.at<uchar>(i, j),2));

            if(f_x.at<uchar>(i, j)==0){
                direction[k]=atan(f_y.at<uchar>(i, j)/0.0000000001)*57.3;  //防止除数为0异常
            }else {
                direction[k]=atan(f_y.at<uchar>(i, j)/f_x.at<uchar>(i, j))*57.3;
            }
            direction[k]+=90;
            k++;
        }
    }

    //极大值抑制
//    double m,s;
//    Mat Mat_mean,Mat_vari;
//    meanStdDev(gradimg,Mat_mean,Mat_vari);
//    m = Mat_mean.at<double>(0,0);
//    s = Mat_vari.at<double>(0,0);
//    std::cout<<"m"<< m <<"    "<< "s" << s << std::endl;
//    m = m+s;
//    s = 0.4*m;
//    std::cout<<"m"<< m <<"    "<< "s" << s << std::endl;
    max_control=gradimg.clone();
    k = 0;
    for (int i = 1; i < gradimg.rows - 1; i++){
        for (int j = 1; j < gradimg.cols - 1; j++){
            int value00=gradimg.at<uchar>((i-1),j-1);
            int value01=gradimg.at<uchar>((i-1),j);
            int value02=gradimg.at<uchar>((i-1),j+1);
            int value10=gradimg.at<uchar>((i),j-1);
            int value11=gradimg.at<uchar>((i),j);
            int value12=gradimg.at<uchar>((i),j+1);
            int value20=gradimg.at<uchar>((i+1),j-1);
            int value21=gradimg.at<uchar>((i+1),j);
            int value22=gradimg.at<uchar>((i+1),j+1);

            if(direction[k]>0&&direction[k]<=45){
                if(value11<=(value12+(value02-value12)*tan(direction[i*max_control.rows+j]))||(value11<=(value10+(value20-value10)*tan(direction[i*max_control.rows+j])))){
                    max_control.at<uchar>(i,j)=0;
                }
            }

            if(direction[k]>45&&direction[k]<=90){
                if(value11<=(value01+(value02-value01)/tan(direction[i*max_control.cols+j]))||value11<=(value21+(value20-value21)/tan(direction[i*max_control.cols+j]))){
                    max_control.at<uchar>(i,j)=0;

                }
            }

            if(direction[k]>90&&direction[k]<=135){
                if(value11<=(value01+(value00-value01)/tan(180-direction[i*max_control.cols+j]))||value11<=(value21+(value22-value21)/tan(180-direction[i*max_control.cols+j]))){
                    max_control.at<uchar>(i,j)=0;
                }
            }
            if(direction[k]>135&&direction[k]<=180){
                if(value11<=(value10+(value00-value10)*tan(180-direction[i*max_control.cols+j]))||value11<=(value12+(value22-value11)*tan(180-direction[i*max_control.cols+j]))){
                    max_control.at<uchar>(i,j)=0;
                }
            }
            k++;
        }
    }
    DoubleThreshold(max_control,10,40);
    DoubleThresholdLink(max_control,10,40);

    for (int i = 0; i < grayImg.rows - 1; i++){
        for (int j = 0; j < grayImg.cols - 1; j++){
            gray2Img.at<uchar>(i, j) = cv::saturate_cast<uchar>(grayImg.at<uchar>(i, j) - max_control.at<uchar>(i, j));
        }
    }
}



//检测
void MainWindow::on_pushButton_bianyuan_clicked(){
    int index = ui->comboBox->currentIndex();
    QString str=ui->comboBox->currentText();
    if(index==0){ //candy
        cv::Mat img2 = cv::Mat(srcImg.size(), CV_8U, cv::Scalar(0));
        Canny(grayImg, img2, 50, 150);
        //显示
        cv::Mat imgShow;
        cvtColor(img2,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->label_tu_22->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_22->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    }else if(index==1){ //roberts
        cv::Mat gradimg,gray2Img;
        QImage Qtemp,Qtemp2;
        gray2Img.create(grayImg.rows, grayImg.cols, CV_8UC1);
        gradimg.create(grayImg.rows, grayImg.cols, CV_8UC1);
        for (int i = 0; i < srcImg.rows - 1; i++){
            for (int j = 0; j < srcImg.cols - 1; j++){
                gradimg.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(grayImg.at<uchar>(i, j) - grayImg.at<uchar>(i + 1, j + 1)) + fabs(grayImg.at<uchar>(i + 1, j) - grayImg.at<uchar>(i, j + 1)));
                gray2Img.at<uchar>(i, j) = cv::saturate_cast<uchar>(grayImg.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
            }
        }

        //显示
        cv::Mat imgShow;
        cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->label_tu_21->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_21->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


        cv::Mat imgShow2;
        cvtColor(gradimg,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
        ui->label_tu_22->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_22->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    }else if(index==2){ //sobel
        cv::Mat gradimg,gray2Img,f_x,f_y;
        QImage Qtemp,Qtemp2;
        gray2Img.create(grayImg.rows, grayImg.cols, CV_8UC1);
        gradimg.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_x.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_y.create(grayImg.rows, grayImg.cols, CV_8UC1);
        for (int i = 1; i < srcImg.rows - 1; i++){
            for (int j = 1; j < srcImg.cols - 1; j++){
                f_x.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(grayImg.at<uchar>(i + 1, j - 1) + 2*grayImg.at<uchar>(i + 1, j) + grayImg.at<uchar>(i + 1, j + 1)
                                                                - grayImg.at<uchar>(i - 1, j - 1) - 2*grayImg.at<uchar>(i - 1, j) - grayImg.at<uchar>(i - 1, j + 1)));
                f_y.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(grayImg.at<uchar>(i - 1, j + 1) + 2*grayImg.at<uchar>(i, j + 1) + grayImg.at<uchar>(i + 1, j + 1)
                                                                - grayImg.at<uchar>(i - 1, j - 1) - 2*grayImg.at<uchar>(i, j - 1) - grayImg.at<uchar>(i + 1, j - 1)));
                gradimg.at<uchar>(i, j) = f_x.at<uchar>(i, j) + f_y.at<uchar>(i, j);
                gray2Img.at<uchar>(i, j) = cv::saturate_cast<uchar>(grayImg.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
            }
        }
        //显示
        cv::Mat imgShow;
        cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->label_tu_21->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_21->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


        cv::Mat imgShow2;
        cvtColor(gradimg,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
        ui->label_tu_22->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_22->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    }else if(index==3){ //prewitt
        cv::Mat gradimg,gray2Img,f_x,f_y;
        QImage Qtemp,Qtemp2;
        gray2Img.create(grayImg.rows, grayImg.cols, CV_8UC1);
        gradimg.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_x.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_y.create(grayImg.rows, grayImg.cols, CV_8UC1);
        for (int i = 1; i < srcImg.rows - 1; i++){
            for (int j = 1; j < srcImg.cols - 1; j++){
                f_x.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(grayImg.at<uchar>(i + 1, j - 1) + grayImg.at<uchar>(i + 1, j) + grayImg.at<uchar>(i + 1, j + 1)
                                                                - grayImg.at<uchar>(i - 1, j - 1) - grayImg.at<uchar>(i - 1, j) - grayImg.at<uchar>(i - 1, j + 1)));
                f_y.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(grayImg.at<uchar>(i - 1, j + 1) + grayImg.at<uchar>(i, j + 1) + grayImg.at<uchar>(i + 1, j + 1)
                                                                - grayImg.at<uchar>(i - 1, j - 1) - grayImg.at<uchar>(i, j - 1) - grayImg.at<uchar>(i + 1, j - 1)));
                gradimg.at<uchar>(i, j) = std::max(f_x.at<uchar>(i, j),f_y.at<uchar>(i, j));
                gray2Img.at<uchar>(i, j) =cv::saturate_cast<uchar>(grayImg.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
            }
        }

        //显示
        cv::Mat imgShow;
        cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->label_tu_21->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_21->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


        cv::Mat imgShow2;
        cvtColor(gradimg,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
        ui->label_tu_22->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_22->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    }else if(index==4){ //laplace
        cv::Mat gradimg,gray2Img;
        QImage Qtemp,Qtemp2;
        gray2Img.create(grayImg.rows, grayImg.cols, CV_8UC1);
        gradimg.create(grayImg.rows, grayImg.cols, CV_8UC1);
        for (int i = 1; i < srcImg.rows - 1; i++){
            for (int j = 1; j < srcImg.cols - 1; j++){
                gradimg.at<uchar>(i, j) = cv::saturate_cast<uchar>(- 4 * grayImg.at<uchar>(i, j) + grayImg.at<uchar>(i + 1 , j)
                                                              + grayImg.at<uchar>(i, j + 1) + grayImg.at<uchar>(i - 1, j)
                                                              + grayImg.at<uchar>(i, j - 1));
                gray2Img.at<uchar>(i, j) = cv::saturate_cast<uchar>(5 * grayImg.at<uchar>(i, j) - grayImg.at<uchar>(i + 1, j)
                                                            - grayImg.at<uchar>(i, j + 1) - grayImg.at<uchar>(i - 1, j)
                                                            - grayImg.at<uchar>(i, j - 1));
            }
        }
        //显示
        cv::Mat imgShow;
        cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->label_tu_21->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_21->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


        cv::Mat imgShow2;
        cvtColor(gradimg,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
        ui->label_tu_22->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_22->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    }else{
        cv::Mat gradimg,gray2Img,f_1,f_2,f_3,f_4,f_5,f_6,f_7,f_8;
        QImage Qtemp,Qtemp2;
        gray2Img.create(grayImg.rows, grayImg.cols, CV_8UC1);
        gradimg.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_1.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_2.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_3.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_4.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_5.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_6.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_7.create(grayImg.rows, grayImg.cols, CV_8UC1);
        f_8.create(grayImg.rows, grayImg.cols, CV_8UC1);
        for (int i = 1; i < srcImg.rows - 1; i++){
            for (int j = 1; j < srcImg.cols - 1; j++){
                f_1.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(-5*grayImg.at<uchar>(i - 1, j - 1) -5* grayImg.at<uchar>(i - 1, j) -5* grayImg.at<uchar>(i - 1, j + 1)
                                                               +3*grayImg.at<uchar>(i, j - 1) +3*grayImg.at<uchar>(i, j + 1)
                                                               + 3*grayImg.at<uchar>(i + 1, j - 1) + 3*grayImg.at<uchar>(i + 1, j) + 3*grayImg.at<uchar>(i + 1, j + 1)));
                f_2.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(3*grayImg.at<uchar>(i - 1, j - 1) -5* grayImg.at<uchar>(i - 1, j) -5* grayImg.at<uchar>(i - 1, j + 1)
                                                               +3*grayImg.at<uchar>(i, j - 1) -5*grayImg.at<uchar>(i, j + 1)
                                                               + 3*grayImg.at<uchar>(i + 1, j - 1) + 3*grayImg.at<uchar>(i + 1, j) + 3*grayImg.at<uchar>(i + 1, j + 1)));
                f_3.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(3*grayImg.at<uchar>(i - 1, j - 1) +3* grayImg.at<uchar>(i - 1, j) -5* grayImg.at<uchar>(i - 1, j + 1)
                                                               +3*grayImg.at<uchar>(i, j - 1) -5*grayImg.at<uchar>(i, j + 1)
                                                               + 3*grayImg.at<uchar>(i + 1, j - 1) + 3*grayImg.at<uchar>(i + 1, j) -5*grayImg.at<uchar>(i + 1, j + 1)));
                f_4.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(3*grayImg.at<uchar>(i - 1, j - 1) +3* grayImg.at<uchar>(i - 1, j) +3* grayImg.at<uchar>(i - 1, j + 1)
                                                               +3*grayImg.at<uchar>(i, j - 1) -5*grayImg.at<uchar>(i, j + 1)
                                                               + 3*grayImg.at<uchar>(i + 1, j - 1) -5*grayImg.at<uchar>(i + 1, j) -5*grayImg.at<uchar>(i + 1, j + 1)));
                f_5.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(3*grayImg.at<uchar>(i - 1, j - 1) +3* grayImg.at<uchar>(i - 1, j) +3* grayImg.at<uchar>(i - 1, j + 1)
                                                               +3*grayImg.at<uchar>(i, j - 1) +3*grayImg.at<uchar>(i, j + 1)
                                                               -5*grayImg.at<uchar>(i + 1, j - 1) -5*grayImg.at<uchar>(i + 1, j) -5*grayImg.at<uchar>(i + 1, j + 1)));
                f_6.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(3*grayImg.at<uchar>(i - 1, j - 1) +3* grayImg.at<uchar>(i - 1, j) +3* grayImg.at<uchar>(i - 1, j + 1)
                                                               -5*grayImg.at<uchar>(i, j - 1) +3*grayImg.at<uchar>(i, j + 1)
                                                               -5*grayImg.at<uchar>(i + 1, j - 1) -5*grayImg.at<uchar>(i + 1, j) +3*grayImg.at<uchar>(i + 1, j + 1)));
                f_7.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(-5*grayImg.at<uchar>(i - 1, j - 1) +3* grayImg.at<uchar>(i - 1, j) +3* grayImg.at<uchar>(i - 1, j + 1)
                                                               -5*grayImg.at<uchar>(i, j - 1) +3*grayImg.at<uchar>(i, j + 1)
                                                               -5*grayImg.at<uchar>(i + 1, j - 1) +3*grayImg.at<uchar>(i + 1, j) +3*grayImg.at<uchar>(i + 1, j + 1)));
                f_8.at<uchar>(i, j) = cv::saturate_cast<uchar>(fabs(-5*grayImg.at<uchar>(i - 1, j - 1) -5* grayImg.at<uchar>(i - 1, j) +3* grayImg.at<uchar>(i - 1, j + 1)
                                                               -5*grayImg.at<uchar>(i, j - 1) +3*grayImg.at<uchar>(i, j + 1)
                                                               +3*grayImg.at<uchar>(i + 1, j - 1) +3*grayImg.at<uchar>(i + 1, j) +3*grayImg.at<uchar>(i + 1, j + 1)));
                QVector<int> goal = {f_1.at<uchar>(i, j),f_2.at<uchar>(i, j),f_3.at<uchar>(i, j),f_4.at<uchar>(i, j),f_5.at<uchar>(i, j),f_6.at<uchar>(i, j),f_7.at<uchar>(i, j),f_8.at<uchar>(i, j)};
                int max_8 = 0;
                for(int i = 0 ; i<8;i++){
                    if(goal[i] > max_8){
                        max_8 = goal[i];
                    }
                }

                gradimg.at<uchar>(i, j) = max_8;
                gray2Img.at<uchar>(i, j) = cv::saturate_cast<uchar>(grayImg.at<uchar>(i, j) - gradimg.at<uchar>(i, j));
            }
        }
        //显示
        cv::Mat imgShow;
        cvtColor(gray2Img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->label_tu_21->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_21->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


        cv::Mat imgShow2;
        cvtColor(gradimg,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
        ui->label_tu_22->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_22->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    }

    ui->label_26->setText(str);
}


/*
 * 滤波
 */
void MainWindow::on_tool_lvbo_clicked(){
    page_idx=3;    //界面索引
    function_idx=3;//功能的索引

    jiemianqiehuan(3,ui->label_tu_31,ui->label_tu_32,ui->label_35,ui->label_36);
    status_label->setText(QString(""));
}



//添加噪声
cv::Mat addSaltNoise(const cv::Mat srcImage, int n){
    cv::Mat dstImage = srcImage.clone();
    for (int k = 0; k < n; k++){
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;
        //图像通道判定
        if (dstImage.channels() == 1){
            dstImage.at<uchar>(i, j) = 255;		//盐噪声
        }else{
            dstImage.at<cv::Vec3b>(i, j)[0] = 255;
            dstImage.at<cv::Vec3b>(i, j)[1] = 255;
            dstImage.at<cv::Vec3b>(i, j)[2] = 255;
        }
    }
    for (int k = 0; k < n; k++){
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;
        //图像通道判定
        if (dstImage.channels() == 1){
            dstImage.at<uchar>(i, j) = 0;		//椒噪声
        }else{
            dstImage.at<cv::Vec3b>(i, j)[0] = 0;
            dstImage.at<cv::Vec3b>(i, j)[1] = 0;
            dstImage.at<cv::Vec3b>(i, j)[2] = 0;
        }
    }
    return dstImage;
}

double generateGaussianNoise(double mu, double sigma){
    //定义小值
    const double epsilon = std::numeric_limits<double>::min();
    static double z0, z1;
    static bool flag = false;
    flag = !flag;
    //flag为假构造高斯随机变量X
    if (!flag)
        return z1 * sigma + mu;
    double u1, u2;
    //构造随机变量
    do{
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    } while (u1 <= epsilon);
    //flag为真构造高斯随机变量
    z0 = sqrt(-2.0*log(u1))*cos(2 * CV_PI*u2);
    z1 = sqrt(-2.0*log(u1))*sin(2 * CV_PI*u2);
    return z0*sigma + mu;
}

//为图像添加高斯噪声
cv::Mat addGaussianNoise(cv::Mat &srcImag){
    cv::Mat dstImage = srcImag.clone();
    for (int i = 0; i < dstImage.rows; i++){
        for (int j = 0; j < dstImage.cols; j++){
            //添加高斯噪声
            dstImage.at<cv::Vec3b>(i, j)[0] = cv::saturate_cast<uchar>(dstImage.at<cv::Vec3b>(i, j)[0] + generateGaussianNoise(2, 0.8) * 32);
            dstImage.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(dstImage.at<cv::Vec3b>(i, j)[1] + generateGaussianNoise(2, 0.8) * 32);
            dstImage.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(dstImage.at<cv::Vec3b>(i, j)[2] + generateGaussianNoise(2, 0.8) * 32);
        }
    }
    return dstImage;
}



//椒盐噪声
void MainWindow::on_salt_noise_clicked(){
    cv::Mat salt;
    cv::Mat temp;
    salt.create(srcImg.rows, srcImg.cols, CV_8UC3);
    salt = addSaltNoise(srcImg,800);
    QImage Qtemp2;
    cvtColor(salt, temp, CV_BGR2RGB);//BGR convert to RGB
    noiseImg=temp.clone();

    //显示
    cv::Mat imgShow;
    cvtColor(temp,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_31->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_31->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));

}
//高斯噪声
void MainWindow::on_guass_noise_clicked(){
    cv::Mat salt;
    cv::Mat temp;
    salt.create(srcImg.rows, srcImg.cols, CV_8UC3);
    salt = addGaussianNoise(srcImg);
    QImage Qtemp2;
    cvtColor(salt, temp, CV_BGR2RGB);//BGR convert to RGB
    noiseImg=temp.clone();

    //显示
    cv::Mat imgShow;
    cvtColor(temp,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_31->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_31->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));

}


void MainWindow::on_average_filter_clicked(){
    cv::Mat filterImg;
    QImage Qtemp,Qtemp2;

    filterImg = noiseImg.clone();

    for(int i = 1 ; i < noiseImg.rows - 1 ; i++)
        for(int j = 1 ; j < noiseImg.cols - 1 ; j++){
            for(int k = 0 ; k < 3 ; k++){
                filterImg.at<cv::Vec3b>(i,j)[k] = cv::saturate_cast<uchar>((noiseImg.at<cv::Vec3b>(i - 1,j - 1)[k] + noiseImg.at<cv::Vec3b>(i - 1,j)[k] + noiseImg.at<cv::Vec3b>(i - 1,j + 1)[k]
                                                   +noiseImg.at<cv::Vec3b>(i,j - 1)[k] + noiseImg.at<cv::Vec3b>(i,j)[k] + noiseImg.at<cv::Vec3b>(i,j + 1)[k]
                                                   +noiseImg.at<cv::Vec3b>(i + 1,j - 1)[k] + noiseImg.at<cv::Vec3b>(i + 1,j)[k] + noiseImg.at<cv::Vec3b>(i + 1,j + 1)[k])/9);
            }
        }

    //显示
    cv::Mat imgShow;
    cvtColor(filterImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_32->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_32->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}

void MainWindow::on_middle_filter_clicked(){
    cv::Mat filterImg;
    QImage Qtemp,Qtemp2;
    QVector<double> middle(9,0);

    filterImg = noiseImg.clone();

    for(int i = 1 ; i < noiseImg.rows - 1 ; i++)
        for(int j = 1 ; j < noiseImg.cols - 1 ; j++){
            for(int k = 0 ; k < 3 ; k++){
                middle[0] = noiseImg.at<cv::Vec3b>(i - 1 , j - 1)[k];
                middle[1] = noiseImg.at<cv::Vec3b>(i - 1 , j)[k];
                middle[2] = noiseImg.at<cv::Vec3b>(i - 1 , j + 1)[k];
                middle[3] = noiseImg.at<cv::Vec3b>(i , j - 1)[k];
                middle[4] = noiseImg.at<cv::Vec3b>(i , j)[k];
                middle[5] = noiseImg.at<cv::Vec3b>(i , j + 1)[k];
                middle[6] = noiseImg.at<cv::Vec3b>(i + 1 , j - 1)[k];
                middle[7] = noiseImg.at<cv::Vec3b>(i + 1 , j)[k];
                middle[8] = noiseImg.at<cv::Vec3b>(i + 1 , j + 1)[k];

                std::sort(middle.begin(),middle.end());

                filterImg.at<cv::Vec3b>(i,j)[k] = cv::saturate_cast<uchar>(middle[5]);
            }
        }
    //显示
    cv::Mat imgShow;
    cvtColor(filterImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_32->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_32->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}

void MainWindow::on_window_filter_clicked(){
    cv::Mat filterImg;
    QImage Qtemp,Qtemp2;
    QVector<double> window(8,0),minImg(8,0);

    filterImg = noiseImg.clone();

    for(int i = 1 ; i < noiseImg.rows - 1 ; i++)
        for(int j = 1 ; j < noiseImg.cols - 1 ; j++){
            for(int k = 0 ; k < 3 ; k++){
                window[0] = (noiseImg.at<cv::Vec3b>(i - 1 , j - 1)[k] + noiseImg.at<cv::Vec3b>(i - 1 , j)[k] + noiseImg.at<cv::Vec3b>(i , j - 1)[k] + noiseImg.at<cv::Vec3b>(i , j)[k])/4;
                window[1] = (noiseImg.at<cv::Vec3b>(i - 1 , j)[k] + noiseImg.at<cv::Vec3b>(i - 1 , j + 1)[k] + noiseImg.at<cv::Vec3b>(i , j)[k] + noiseImg.at<cv::Vec3b>(i , j + 1)[k])/4;
                window[2] = (noiseImg.at<cv::Vec3b>(i , j)[k] + noiseImg.at<cv::Vec3b>(i , j + 1)[k] + noiseImg.at<cv::Vec3b>(i + 1 , j)[k] + noiseImg.at<cv::Vec3b>(i + 1 , j + 1)[k])/4;
                window[3] = (noiseImg.at<cv::Vec3b>(i , j - 1)[k] + noiseImg.at<cv::Vec3b>(i , j)[k] + noiseImg.at<cv::Vec3b>(i + 1 , j - 1)[k] + noiseImg.at<cv::Vec3b>(i + 1 , j)[k])/4;
                window[4] = (noiseImg.at<cv::Vec3b>(i - 1 , j - 1)[k] + noiseImg.at<cv::Vec3b>(i - 1 , j)[k] + noiseImg.at<cv::Vec3b>(i - 1 , j + 1)[k] + noiseImg.at<cv::Vec3b>(i, j - 1)[k] + noiseImg.at<cv::Vec3b>(i, j)[k] + noiseImg.at<cv::Vec3b>(i, j + 1)[k])/6;
                window[5] = (noiseImg.at<cv::Vec3b>(i + 1 , j - 1)[k] + noiseImg.at<cv::Vec3b>(i + 1 , j)[k] + noiseImg.at<cv::Vec3b>(i + 1 , j + 1)[k] + noiseImg.at<cv::Vec3b>(i, j - 1)[k] + noiseImg.at<cv::Vec3b>(i, j)[k] + noiseImg.at<cv::Vec3b>(i, j + 1)[k])/6;
                window[6] = (noiseImg.at<cv::Vec3b>(i - 1 , j)[k] + noiseImg.at<cv::Vec3b>(i - 1 , j + 1)[k] + noiseImg.at<cv::Vec3b>(i , j)[k] + noiseImg.at<cv::Vec3b>(i, j + 1)[k] + noiseImg.at<cv::Vec3b>(i + 1, j)[k] + noiseImg.at<cv::Vec3b>(i + 1, j + 1)[k])/6;
                window[7] = (noiseImg.at<cv::Vec3b>(i - 1 , j - 1)[k] + noiseImg.at<cv::Vec3b>(i - 1 , j)[k] + noiseImg.at<cv::Vec3b>(i , j)[k] + noiseImg.at<cv::Vec3b>(i, j - 1)[k] + noiseImg.at<cv::Vec3b>(i + 1, j)[k] + noiseImg.at<cv::Vec3b>(i + 1, j - 1)[k])/6;

                for(int n = 0 ; n < 8 ; n++){
                    minImg[n] = pow(window[n] - noiseImg.at<cv::Vec3b>(i , j)[k],2);
                }
                auto smallest = std::min_element(std::begin(minImg), std::end(minImg));
                int position = std::distance(std::begin(minImg), smallest);
                filterImg.at<cv::Vec3b>(i , j)[k] = cv::saturate_cast<uchar>(window[position]);
            }
        }

    //显示
    cv::Mat imgShow;
    cvtColor(filterImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_32->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_32->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}



void MainWindow::on_gauss_filter_clicked(){
    cv::Mat filterImg;
    QImage Qtemp,Qtemp2;

    filterImg = noiseImg.clone();

    for(int i = 1 ; i < noiseImg.rows - 1 ; i++)
        for(int j = 1 ; j < noiseImg.cols - 1 ; j++){
            for(int k = 0 ; k < 3 ; k++){
                filterImg.at<cv::Vec3b>(i,j)[k] = cv::saturate_cast<uchar>((noiseImg.at<cv::Vec3b>(i - 1,j - 1)[k] + 2*noiseImg.at<cv::Vec3b>(i - 1,j)[k] + noiseImg.at<cv::Vec3b>(i - 1,j + 1)[k]
                                                   +2*noiseImg.at<cv::Vec3b>(i,j - 1)[k] + 4*noiseImg.at<cv::Vec3b>(i,j)[k] + 2*noiseImg.at<cv::Vec3b>(i,j + 1)[k]
                                                   +noiseImg.at<cv::Vec3b>(i + 1,j - 1)[k] + 2*noiseImg.at<cv::Vec3b>(i + 1,j)[k] + noiseImg.at<cv::Vec3b>(i + 1,j + 1)[k])/16);
            }
        }


    //显示
    cv::Mat imgShow;
    cvtColor(filterImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_32->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_32->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}

void MainWindow::on_form_filter_clicked(){
    cv::Mat filterImg,temp,RGB;
    QImage Qtemp,Qtemp2;

    cv::Mat element=getStructuringElement(cv::MORPH_RECT,cv::Size(15,15));
    cvtColor(srcImg, RGB, CV_BGR2RGB);
    erode(RGB,temp,element);
    dilate(temp,filterImg,element);


    //显示
    cv::Mat imgShow;
    cvtColor(filterImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_32->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_32->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}




/*
 * 图像变换
 */
void MainWindow::on_tool_xuxiangbianhuan_clicked(){
    page_idx=4;    //界面索引
    function_idx=4;//功能的索引

    jiemianqiehuan(4,ui->label_tu_41,ui->label_tu_42,ui->label_45,ui->label_46);
    status_label->setText(QString(""));
}



//仿射变换
void MainWindow::on_affine_clicked(){
    QImage Qtemp,Qtemp2;
    cv::Point2f srcTri[3], dstTri[3];
    cv::Mat rot_mat(2, 3, CV_32FC1);
    cv::Mat warp_mat(2, 3, CV_32FC1);
    cv::Mat dst,RGB;
    cvtColor(srcImg, RGB, CV_BGR2RGB);

    dst = cv::Mat::zeros(RGB.rows, RGB.cols, RGB.type());

    srcTri[0] = cv::Point2f (0,0);
    srcTri[1] = cv::Point2f(RGB.cols - 1,0); //缩小一个像素
    srcTri[2]= cv::Point2f(0,RGB.rows - 1);

    dstTri[0] = cv::Point2f(RGB.cols * 0.0,RGB.rows * 0.33);
    dstTri[1] = cv::Point2f(RGB.cols * 0.85,RGB.rows * 0.25);
    dstTri[2] = cv::Point2f(RGB.cols* 0.15,RGB.rows* 0.7);

    warp_mat = getAffineTransform(srcTri, dstTri);
    warpAffine(RGB, dst, warp_mat,RGB.size());

    //显示
    cv::Mat imgShow;
    cvtColor(dst,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_42->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_42->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->label_46->setText("仿射变换");
}

//透视变换
void MainWindow::on_perspective_clicked(){
    QImage Qtemp,Qtemp2;
    cv::Point2f srcQuad[4],dstQuad[4];
    cv::Mat warp_matrix(3,3,CV_32FC1);
    cv::Mat dst,RGB;
    cvtColor(srcImg, RGB, CV_BGR2RGB);
    dst = cv::Mat::zeros(RGB.rows,RGB.cols,RGB.type());

    srcQuad[0]=cv::Point2f(0,0); //src top left
    srcQuad[1] =cv::Point2f(RGB.cols -1,0); //src top right
    srcQuad[2]=cv::Point2f(0, RGB.rows-1); //src bottom left
    srcQuad[3]=cv::Point2f(RGB.cols -1, RGB.rows-1); //src bot right

    dstQuad[0]=cv::Point2f(RGB.cols*0.05,RGB.rows*0.33); //dst top left
    dstQuad[1]=cv::Point2f(RGB.cols*0.9,RGB.rows*0.25); //dst top right
    dstQuad[2]=cv::Point2f(RGB.cols*0.2,RGB.rows*0.7); //dst bottom left
    dstQuad[3]=cv::Point2f(RGB.cols*0.8,RGB.rows*0.9); //dst bot right

    warp_matrix=getPerspectiveTransform(srcQuad,dstQuad);
    warpPerspective(RGB,dst,warp_matrix,RGB.size());

    //显示
    cv::Mat imgShow;
    cvtColor(dst,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_42->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_42->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->label_46->setText("透视变换");
}


/*
 * 傅里叶变换
 */
void MainWindow::on_tool_FFT_clicked(){
    page_idx=5;    //界面索引
    function_idx=5;//功能的索引

    jiemianqiehuan(5,ui->label_tu_51,ui->label_tu_52,ui->label_55,ui->label_56);
    status_label->setText(QString(""));
}



//频谱图
void My_DFT(cv::Mat input_image, cv::Mat& output_image, cv::Mat& transform_image){ //傅里叶变换得到频谱图和复数域结果
    //1.扩展图像矩阵，为2，3，5的倍数时运算速度快
    int m = cv::getOptimalDFTSize(input_image.rows);
    int n = cv::getOptimalDFTSize(input_image.cols);
    copyMakeBorder(input_image, input_image, 0, m - input_image.rows, 0, n - input_image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //2.创建一个双通道矩阵planes，用来储存复数的实部与虚部
    cv::Mat planes[] = { cv::Mat_<float>(input_image), cv::Mat::zeros(input_image.size(), CV_32F) };

    //3.从多个单通道数组中创建一个多通道数组:transform_image。函数Merge将几个数组合并为一个多通道阵列，即输出数组的每个元素将是输入数组元素的级联
    merge(planes, 2, transform_image);

    //4.进行傅立叶变换
    dft(transform_image, transform_image);

    //5.计算复数的幅值，保存在output_image（频谱图）
    split(transform_image, planes); // 将双通道分为两个单通道，一个表示实部，一个表示虚部
    magnitude(planes[0], planes[1], output_image); //计算复数的幅值，保存在output_image（频谱图）

    //6.前面得到的频谱图数级过大，不好显示，因此转换
    output_image += cv::Scalar(1);   // 取对数前将所有的像素都加1，防止log0
    log(output_image, output_image);   // 取对数
    normalize(output_image, output_image, 0, 1, cv::NORM_MINMAX); //归一化

    //7.剪切和重分布幅度图像限
    output_image = output_image(cv::Rect(0, 0, output_image.cols & -2, output_image.rows & -2));

      // 重新排列傅里叶图像中的象限，使原点位于图像中心
    int cx = output_image.cols / 2;
    int cy = output_image.rows / 2;
    cv::Mat q0(output_image, cv::Rect(0, 0, cx, cy));   // 左上区域
    cv::Mat q1(output_image, cv::Rect(cx, 0, cx, cy));  // 右上区域
    cv::Mat q2(output_image, cv::Rect(0, cy, cx, cy));  // 左下区域
    cv::Mat q3(output_image, cv::Rect(cx, cy, cx, cy)); // 右下区域

      //交换象限中心化
    cv::Mat tmp;
    q0.copyTo(tmp); q3.copyTo(q0); tmp.copyTo(q3);//左上与右下进行交换
    q1.copyTo(tmp); q2.copyTo(q1); tmp.copyTo(q2);//右上与左下进行交换
}


void MainWindow::on_pushButton_pinputu_clicked(){

    cv::Mat imgShow,image_output,image_transform;
    My_DFT(grayImg, image_output, image_transform);


    cvtColor(image_output,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    cv::imwrite("fft.png",imgShow*255);
    imgShow = cv::imread("fft.png");

    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_52->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_52->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}


//*************************************************************形态学********************************************************************************************
/*
 * 膨胀腐蚀开闭操作
 */
void MainWindow::on_tool_dilate_clicked(){
    page_idx=6;    //界面索引
    function_idx=6;//功能的索引

    jiemianqiehuan(6,ui->label_tu_61,ui->label_tu_62,ui->label_65,ui->label_66);
    status_label->setText(QString(""));
    ui->gn_6->setText("Dilate");  //设置字
}

void MainWindow::on_tool_erode_clicked(){
    page_idx=6;    //界面索引
    function_idx=7;//功能的索引

    jiemianqiehuan(6,ui->label_tu_61,ui->label_tu_62,ui->label_65,ui->label_66);
    status_label->setText(QString(""));
    ui->gn_6->setText("Erode");   //设置字
}

void MainWindow::on_tool_opening_clicked(){
    page_idx=6;    //界面索引
    function_idx=8;//功能的索引

    jiemianqiehuan(6,ui->label_tu_61,ui->label_tu_62,ui->label_65,ui->label_66);
    status_label->setText(QString(""));
    ui->gn_6->setText("Opening");   //设置字
}

void MainWindow::on_tool_closing_clicked(){
    page_idx=6;    //界面索引
    function_idx=9;//功能的索引

    jiemianqiehuan(6,ui->label_tu_61,ui->label_tu_62,ui->label_65,ui->label_66);
    status_label->setText(QString(""));
    ui->gn_6->setText("Closing");   //设置字
}


/*
 * 选择卷积核
 */
void MainWindow::on_pb_juanjihe_clicked(){
    matrixwindow.show();
}


//膨胀 腐蚀 开操作 闭操作
void MainWindow::on_gn_6_clicked(){
    cv::Mat OriginalImg;
    cv::Mat ResultImg;
    kernel=matrixwindow.getkernel();
    int itercishu=ui->spinBox_6->value();

    //Pipeline
    if(ui->checkBox_Pipeline->isChecked()){
        OriginalImg=currentImg;
    }else{
        OriginalImg=srcImg;
    }

    if(ui->gn_6->text()=="Dilate"){
        dilate(OriginalImg,ResultImg, kernel, cv::Point(-1,-1),itercishu);
        ui->label_66->setText("Dilate");
    }else if(ui->gn_6->text()=="Erode"){
        erode(OriginalImg,ResultImg, kernel, cv::Point(-1,-1),itercishu);
        ui->label_66->setText("Erode");
    }else if(ui->gn_6->text()=="Opening"){
        morphologyEx(OriginalImg,ResultImg, 2,kernel, cv::Point(-1,-1),itercishu);
        ui->label_66->setText("Opening");
    }else if(ui->gn_6->text()=="Closing"){
        morphologyEx(OriginalImg,ResultImg, 3,kernel, cv::Point(-1,-1),itercishu);
        ui->label_66->setText("Closing");
    }

    currentImg=ResultImg;

    //显示
    cv::Mat imgShow;
    cvtColor(ResultImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_62->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_62->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}



/*
 *击中击不中
 */
void MainWindow::on_pushButton_jizhongjibuzhong_clicked(){
    page_idx=7;    //界面索引
    function_idx=10;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("Hit-Miss");
    status_label->setText(QString(""));
}


/*
 *边界提取
 */
void MainWindow::on_pushButton_bianjietiqu_clicked(){
    page_idx=7;    //界面索引
    function_idx=11;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("Boundary");
    status_label->setText(QString(""));
}


/*
 *区域填充
 */
void MainWindow::on_pushButton_quyutianchong_clicked(){
    page_idx=7;    //界面索引
    function_idx=12;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("RegionFill");
    status_label->setText(QString(""));
}


/*
 *连通分量提取
 */
void MainWindow::on_pushButton_liantongtiqu_clicked(){
    page_idx=7;    //界面索引
    function_idx=13;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("Connected Components");
    status_label->setText(QString(""));
}


/*
 *凸壳
 */
void MainWindow::on_pushButton_tuke_clicked(){  
    page_idx=7;    //界面索引
    function_idx=14;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("Convex Hull");
    status_label->setText(QString(""));
}


/*
 *细化
 */
void MainWindow::on_pushButton_xihua_clicked(){
    page_idx=7;    //界面索引
    function_idx=15;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("Thin");
    status_label->setText(QString(""));
}


/*
 *粗化
 */
void MainWindow::on_pushButton_cuhua_clicked(){
    page_idx=7;    //界面索引
    function_idx=16;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("Coarse");
    status_label->setText(QString(""));
}


/*
 *骨架
 */
void MainWindow::on_pushButton_gujia_clicked(){
    page_idx=7;    //界面索引
    function_idx=17;//功能的索引

    jiemianqiehuan(7,ui->label_tu_71,ui->label_tu_72,ui->label_75,ui->label_76);
    ui->pushButton_gn7->setText("Skeleton");
    status_label->setText(QString(""));
}




//选择卷积核
void MainWindow::on_pb_juanjihe_7_clicked(){
    matrixwindow.show();
}


//击中击不中 边界提取 孔洞填充  连通分量提取
void MainWindow::on_pushButton_gn7_clicked(){
    kernel=matrixwindow.getkernel();

    cv::Mat OriginalImg;
    cv::Mat ResultImg;

    //Pipeline
    if(ui->checkBox_Pipeline->isChecked()){
        OriginalImg=currentImg;
    }else{
        OriginalImg=srcImg;
    }


    //功能
    if(ui->pushButton_gn7->text()=="Hit-Miss"){
        if (OriginalImg.type()!= CV_8UC1){
            QMessageBox::information(this,"tip","src.type() != CV_8UC1");
            return;
        }
        morphologyEx(OriginalImg,ResultImg, cv::MORPH_HITMISS,kernel, cv::Point(-1,-1));
        ui->label_76->setText("Hit-Miss");

    }else if(ui->pushButton_gn7->text()=="Boundary"){
        morphologyEx(OriginalImg,ResultImg, cv::MORPH_GRADIENT,kernel, cv::Point(-1,-1));
        ui->label_76->setText("Boundary");

    }else if(ui->pushButton_gn7->text()=="RegionFill"){
        int imgHeight = OriginalImg.rows;
        int imgWidth = OriginalImg.cols;//生成mask图
        cv::Mat mask = 255 - OriginalImg;//生成masker图
        cv::Mat masker = cv::Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8U);
        for (int row = 0; row < imgHeight; row++){
            if (row == 0 || row == imgHeight - 1){
                for (int col = 0; col < imgWidth; col++){
                    masker.at<uchar>(row, col) = 255;
                }
            }else {
                masker.at<uchar>(row, 0) = 255;
                masker.at<uchar>(row, imgWidth - 1) = 255;
            }
        }//iteration

        cv::Mat masker_pre;
        cv::Mat diletion = cv::Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8U);
        cv::Mat se = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
        while (true) {
            masker_pre = masker.clone();
            dilate(masker, diletion, se);
            for (int row = 0; row < imgHeight; row++){
                 for (int col = 0; col < imgWidth; col++){
                     masker.at<uchar>(row, col) = diletion.at<uchar>(row, col) < mask.at<uchar>(row, col)? diletion.at<uchar>(row, col): mask.at<uchar>(row, col);
                 }
            }

            cv::Mat diff = masker != masker_pre;
           bool eq = cv::countNonZero(diff) == 0;
           if (eq == true) {
                 break;
           }
        }
        ResultImg = 255 - masker;
        ui->label_76->setText("RegionFill");

    }else if(ui->pushButton_gn7->text()=="Connected Components"){
        cv::Mat image, image_gray, image_bw, image_bw2, image_bw3;

        if(OriginalImg.type() == CV_8UC3){ //如果是RGB图
            cvtColor(OriginalImg, image_gray, cv::COLOR_BGR2GRAY);  //转换为灰度图像
            threshold(image_gray, image_bw, 120, 255, 1); //转换为二值图，通过0，1调节二值图像背景颜色
        }else{
            threshold(OriginalImg, image_bw, 120, 255, 1);
        }

        //提取连通域
        int number = connectedComponents(image_bw,image_bw2, 8, CV_16U);
        cv::RNG rng(50); //RNG随机数生成器
        std::vector<cv::Vec3b>colors; //定义颜色数组
        for (int i = 0; i < number; ++i) {
             //使用均匀分布的随机确定颜色
             cv::Vec3b vec3 = cv::Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
             colors.push_back(vec3);
        }

        //以不同颜色标记出不同的连通域
        cv::Mat result = cv::Mat::zeros(image_bw.size(), CV_8UC3); //定义标记结果图像
        int w = result.cols;
        int h = result.rows;

        for (int row = 0; row < h; ++row) {
             for (int col = 0; col < w; ++col) {
                int label = image_bw2.at<uint16_t>(row, col);
                   if (label == 0) {//背景的黑色不改变
                        continue;
                   }
                    result.at<cv::Vec3b>(row, col) = colors[label];
             }
        }
        ResultImg=result;
        ui->label_76->setText("Connected Components");

     }else if (ui->pushButton_gn7->text()=="Convex Hull") {
        cv::Mat bin_output;
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierachy;

        if(OriginalImg.type() == CV_8UC3){ //如果是RGB图
            cvtColor(OriginalImg, OriginalImg, cv::COLOR_BGR2GRAY);
        }

        blur(OriginalImg, OriginalImg, cv::Size(3, 3), cv::Point(-1, -1), cv::BORDER_DEFAULT);
        threshold(OriginalImg, bin_output, 100, 255, cv::THRESH_BINARY);
        findContours(bin_output, contours, hierachy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

        std::vector<std::vector<cv::Point>> convexs(contours.size());
        for (size_t i = 0; i < contours.size(); i++){
            cv::convexHull(contours[i], convexs[i], false, true);
        }

        //绘制
        ResultImg = cv::Mat::zeros(OriginalImg.size(), CV_8UC3);
        std::vector<cv::Vec4i> empty(0);
        cv::RNG rng(12345);
        for (size_t k = 0; k < contours.size(); k++){
            cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            //cv::drawContours(dst, contours, k, color, 2, cv::LINE_8, hierachy, 0, cv::Point(0, 0));
            cv::drawContours(ResultImg, convexs, k, color, 2, cv::LINE_8, empty, 0, cv::Point(0, 0));
        }
        ui->label_76->setText("Convex Hull");

     }else if (ui->pushButton_gn7->text()=="Thin") {
        if (OriginalImg.type()!= CV_8UC1){
            QMessageBox::information(this,"tip","src.type() != CV_8UC1");
            return;
        }

        cv::Mat dst;
        threshold(OriginalImg, dst, 128, 255, cv::THRESH_BINARY);
        cv::Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
        cv::Mat skel(OriginalImg.size(), CV_8UC1, cv::Scalar(0));
        cv::Mat temp;
        cv::Mat eroded;
        int i=2;
        do {
            erode(dst, eroded, element);
            dilate(eroded, temp, element);
            subtract(dst, temp, temp);
            bitwise_or(skel, temp, skel);
            eroded.copyTo(dst);
            i--;
        } while (i!= 0);
        ResultImg =eroded;
        ui->label_76->setText("Thin");

    }else if (ui->pushButton_gn7->text()=="Coarse") {
        if (OriginalImg.type()!= CV_8UC1){
            QMessageBox::information(this,"tip","src.type() != CV_8UC1");
            return;
        }

        cv::Mat dst;
        threshold(OriginalImg, dst, 128, 255, cv::THRESH_BINARY);
        cv::Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
        cv::Mat temp;
        erode(dst, temp, element);
        dilate(temp, dst, element);
        ResultImg=temp;
        ui->label_76->setText("Coarse");

    }else if (ui->pushButton_gn7->text()=="Skeleton") {
        cv::Mat dst;
        if (OriginalImg.type()!= CV_8UC1){
            QMessageBox::information(this,"tip","src.type() != CV_8UC1");
            return;
        }

        threshold(OriginalImg, dst, 128, 255, cv::THRESH_BINARY);
        cv::Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
        cv::Mat skel(OriginalImg.size(), CV_8UC1, cv::Scalar(0));
        cv::Mat temp;
        do {
            erode(dst, temp, element);
            dilate(temp, temp, element);
            subtract(dst, temp, temp);
            bitwise_or(skel, temp, skel);
            temp.copyTo(dst);
        } while (countNonZero(dst) != 0);
        ResultImg = temp;
        ui->label_76->setText("Skeleton");
    }

    currentImg=ResultImg;
    //显示
    cv::Mat imgShow;
    cvtColor(ResultImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_72->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_72->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}


//*************************************************************图像分割********************************************************************************************
/*
 *阈值分割
 */
void MainWindow::on_pushButton_yuzhi_clicked(){
    page_idx=8;    //界面索引
    function_idx=18;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("Threshold");
    status_label->setText(QString(""));
}
/*
 *OSTU
 */
void MainWindow::on_pushButton_OSTU_clicked(){
    page_idx=8;    //界面索引
    function_idx=19;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("OSTU");
    status_label->setText(QString(""));
}
/*
 *Kittler
 */
void MainWindow::on_pushButton_Kittler_clicked(){
    page_idx=8;    //界面索引
    function_idx=20;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("Kittler");
    status_label->setText(QString(""));
}

/*
 *移动平均
 */
void MainWindow::on_pushButton_yidongpinjun_clicked(){
    page_idx=8;    //界面索引
    function_idx=21;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("Moving Average");
    status_label->setText(QString(""));
}

/*
 *Hough
 */
void MainWindow::on_pushButton_hough_clicked(){
    page_idx=8;    //界面索引
    function_idx=22;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("Hough");
    status_label->setText(QString(""));
}


/*
 *分水岭
 */
void MainWindow::on_pushButton_fenshuilin_clicked(){
    page_idx=8;    //界面索引
    function_idx=23;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("Watershed");
    status_label->setText(QString(""));
}


/*
 *区域分裂与合并
 */
void MainWindow::on_pushButton_fenkaihebing_clicked(){
    page_idx=8;    //界面索引
    function_idx=24;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("Separate and Merge");
    status_label->setText(QString(""));
}


/*
 *Kmeans
 */
void MainWindow::on_pushButton_Kmeans_clicked(){
    page_idx=8;    //界面索引
    function_idx=25;//功能的索引

    jiemianqiehuan(8,ui->label_tu_81,ui->label_tu_82,ui->label_85,ui->label_86);
    ui->pushButton_gn8->setText("Kmeans");
    status_label->setText(QString(""));
}



int OSTU(QVector<int> hist){
    float u0, u1, w0, w1; int count0, t, maxT; float devi, maxDevi = 0; //方差及最大方差 int i, sum = 0;
    int i, sum = 0;
    for (i = 0; i < 256; i++){ sum = sum + hist[i]; }

    for (t = 0; t < 255; t++){
        u0 = 0; count0 = 0;
        for (i = 0; i <= t; i++){ //阈值为t时，c0组的均值及产生的概率;
            u0 += i * hist[i];
            count0 += hist[i];
        }
        u0 = u0 / count0;
        w0 = (float)count0/sum;
        for (i = t + 1; i < 256; i++){ //阈值为t时，c1组的均值及产生的概率
            u1 += i * hist[i];
        }
        u1 = u1 / (sum - count0); w1 = 1 - w0;
        devi = w0 * w1 * (u1 - u0) * (u1 - u0); //两类间方差
        if (devi > maxDevi) {
            //记录最大的方差及最佳位置
            maxDevi = devi;
            maxT = t;
        }
    }
    return maxT;
}



//阈值分割  OSTU Kittler 移动平均 分水岭 区域分裂与合并  Kmeans
void MainWindow::on_pushButton_gn8_clicked(){
    cv::Mat OriginalImg;
    cv::Mat ResultImg;

    //Pipeline
    if(ui->checkBox_Pipeline->isChecked()){
        OriginalImg=currentImg;
    }else{
        OriginalImg=srcImg;
    }


    if(ui->pushButton_gn8->text()=="Threshold"){
        cv::Mat targetImg;

        targetImg.create(OriginalImg.rows, OriginalImg.cols, CV_8UC1);
        for(int i = 0; i < OriginalImg.rows ; i++){
            for(int j = 0; j < OriginalImg.cols ; j++){
                if(OriginalImg.at<uchar>(i, j)>128){
                    targetImg.at<uchar>(i,j) = 255;
                }else{
                    targetImg.at<uchar>(i,j) = 0;
                }
            }
        }
        ResultImg=targetImg;
        ui->label_86->setText("Threshold");

    }else if(ui->pushButton_gn8->text()=="OSTU"){
        QVector<int> hist(256,0);

        for(int i = 0 ; i < OriginalImg.rows ; i++)
            for(int j = 0 ; j < OriginalImg.cols ; j++){
                hist[OriginalImg.at<uchar>(i,j)]++;
            }
        int T;
        T = OSTU(hist);
        std::cout<<"OSTU:"<<T<<std::endl;

        QImage Qtemp;
        cv::Mat targetImg;
        targetImg.create(OriginalImg.rows, OriginalImg.cols, CV_8UC1);

        for(int i = 0; i < OriginalImg.rows ; i++){
            for(int j = 0; j < OriginalImg.cols ; j++){
                if(OriginalImg.at<uchar>(i, j)>T){
                    targetImg.at<uchar>(i,j) = 255;
                }else{
                    targetImg.at<uchar>(i,j) = 0;
                }
            }
        }
        ResultImg=targetImg;
        ui->label_86->setText("OSTU");

    }else if(ui->pushButton_gn8->text()=="Kittler"){
        QImage Qtemp;
        cv::Mat targetImg,temp;
        temp = OriginalImg.clone();
        targetImg.create(OriginalImg.rows, OriginalImg.cols, CV_8UC1);
        int Grads,sumGrads = 0,sumGrayGrads = 0,KT;

        for (int i=1;i<temp.rows-1;i++){
            uchar* previous=temp.ptr<uchar>(i-1); // previous row
            uchar* current=temp.ptr<uchar>(i); // current row
            uchar* next=temp.ptr<uchar>(i+1); // next row
            for(int j=1;j<temp.cols-1;j++){   //求水平或垂直方向的最大梯度
                Grads=MAX(abs(previous[j]-next[j]),abs(current[j-1]-current[j+1]));
                sumGrads  += Grads;
                sumGrayGrads += Grads*(current[j]); //梯度与当前点灰度的积
            }
        }
        KT=sumGrayGrads/sumGrads;
        std::cout<<"Kittler:"<<KT<<std::endl;

        for(int i = 0; i < OriginalImg.rows ; i++){
            for(int j = 0; j < OriginalImg.cols ; j++){
                if(OriginalImg.at<uchar>(i, j)>KT){
                    targetImg.at<uchar>(i,j) = 255;
                }
                else{targetImg.at<uchar>(i,j) = 0;}
            }
        }
        ResultImg=targetImg;
        ui->label_86->setText("Kittler");


    }else if(ui->pushButton_gn8->text()=="Moving Average"){
        if (OriginalImg.type()!= CV_8UC1){
            QMessageBox::information(this,"tip","src.type() != CV_8UC1");
            return;
        }
        adaptiveThreshold(OriginalImg, ResultImg,  255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY, 21, 10);//局部的均值作为局部的阈值，就是移动平均法
        ui->label_86->setText("Moving Average");

    }else if(ui->pushButton_gn8->text()=="Hough"){
        cv::Mat temp, gray, dst;
        medianBlur(OriginalImg, temp, 3); // 中值滤波去除噪声

        if (temp.type() == CV_8UC1){
            gray=temp;
        }else if(temp.type() == CV_8UC3){
            cvtColor(temp, gray, cv::COLOR_BGR2GRAY); // 灰度化
        }

        // 霍夫圆检测
        std::vector<cv::Vec3f> pcircles;
        HoughCircles(gray, pcircles, cv::HOUGH_GRADIENT, 1, 20, 100, 100, 1, 100);

        currentImg.copyTo(dst);
        for(size_t i = 0; i < pcircles.size(); i++){
            cv::Vec3f cc = pcircles[i]; // cc[0] -> x  cc[1] -> y  cc[2] -> r
            circle(dst, cv::Point(cc[0], cc[1]), cc[2], cv::Scalar(0, 0, 255), 2, cv::LINE_AA); // 可视化圆弧
            circle(dst, cv::Point(cc[0], cc[1]), 1, cv::Scalar(255, 0, 0), 2, cv::LINE_AA); // 可视化圆心
         }
        ResultImg=dst;
        ui->label_86->setText("Hough");

    }else if(ui->pushButton_gn8->text()=="Watershed"){

        cv::Mat src=OriginalImg;
        if (OriginalImg.type()!= CV_8UC3){
            QMessageBox::information(this,"tip","src.type() != CV_8UC3");
            return;
        }

        // 将白色背景转换为黑色背景
        for (int row = 0; row < src.rows; row++){
            for (int col = 0; col < src.cols; col++){
                // 当前位置为白色
                if(src.at<cv::Vec3b>(row, col) == cv::Vec3b(255, 255, 255)){
                    src.at<cv::Vec3b>(row, col)[0] = 0;
                    src.at<cv::Vec3b>(row, col)[1] = 0;
                    src.at<cv::Vec3b>(row, col)[2] = 0;
                }
            }
        }

        // 利用拉普拉斯算子锐化
        cv::Mat sharp_img, temp1, temp2;
        cv::Mat kernel = (cv::Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1); // 定义拉普拉斯算子
        cv::filter2D(src, temp1, CV_32F, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
        src.convertTo(temp2, CV_32F);
        sharp_img = temp2 - temp1;
        sharp_img.convertTo(sharp_img, CV_8UC3);

        // 二值化
        cv::Mat binary_img;
        cv::cvtColor(sharp_img, binary_img, cv::COLOR_BGR2GRAY);
        cv::threshold(binary_img, binary_img, 40, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        // 距离变换
        cv::Mat distImg;
        cv::distanceTransform(binary_img, distImg, cv::DIST_L1, 3, 5);
        cv::normalize(distImg, distImg, 0, 1, cv::NORM_MINMAX);


        // 再次二值化，腐蚀
        cv::threshold(distImg, distImg, 0.4, 1, cv::THRESH_BINARY);
        cv::Mat k1 = cv::Mat::zeros(13, 13, CV_8UC1);
        cv::erode(distImg, distImg, k1, cv::Point(-1, -1));


        // 生成标记
        cv::Mat dist_8u;
        distImg.convertTo(dist_8u, CV_8U);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(dist_8u, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

        cv::Mat markers = cv::Mat::zeros(src.size(), CV_32SC1);
        for(size_t i = 0; i < contours.size(); i++){
            cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar::all(static_cast<int>(i) + 1), -1);
        }
        cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(255, 255, 255), -1);


        // 分水岭变换
        cv::watershed(src, markers);
        cv::Mat mark = cv::Mat::zeros(markers.size(), CV_8UC1);
        markers.convertTo(mark, CV_8UC1);
        cv::bitwise_not(mark, mark, cv::Mat());


        // 随机产生颜色
        std::vector<cv::Vec3b> colors;
        for(size_t i = 0; i < contours.size(); i++){
            int r = cv::theRNG().uniform(0, 255);
            int g = cv::theRNG().uniform(0, 255);
            int b = cv::theRNG().uniform(0, 255);
            colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
        }

        // 填充颜色
        cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);
        for (int row = 0; row < markers.rows; row++){
            for (int col = 0; col < markers.cols; col++){
                int index = markers.at<int>(row, col);
                if (index > 0 && index <= static_cast<int>(contours.size())){
                    dst.at<cv::Vec3b>(row, col) = colors[index - 1];
                }
                else{
                    dst.at<cv::Vec3b>(row, col) = cv::Vec3b(0, 0, 0);
                }
            }
        }
        ResultImg=dst;
        ui->label_86->setText("Watershed");

    }else if(ui->pushButton_gn8->text()=="Separate and Merge"){
       if (OriginalImg.type()!= CV_8UC3){
           QMessageBox::information(this,"tip","src.type() != CV_8UC3");
           return;
       }

      SplitAndMerge a(OriginalImg);
      ResultImg= a.excute();
      ui->label_86->setText("Separate and Merge");

    }else if(ui->pushButton_gn8->text()=="Kmeans"){
        if (OriginalImg.type()!= CV_8UC3){
            QMessageBox::information(this,"tip","src.type() != CV_8UC3");
            return;
        }

        cv::Vec3b colorArr[6] = {cv::Vec3b(0,0,255),
                                 cv::Vec3b(0,255,0),
                                 cv::Vec3b(255,0,0),
                                 cv::Vec3b(0,255,255),
                                 cv::Vec3b(255,0,255),
                                 cv::Vec3b(255,255,0),
         };
        // 将图像转换为一维向量
        cv::Mat samples = OriginalImg.reshape(3, OriginalImg.rows* OriginalImg.cols);

        // 转换为32位浮点型数据
        cv::Mat samples32F;
        samples.convertTo(samples32F, CV_32F);
        // 设置聚类的个数
        int K = 4;
        // 定义输出标签矩阵
        cv::Mat labels;
        // 定义停止准则
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 100, 0.1);
        // 运行K-means算法进行聚类
        kmeans(samples32F, K, labels, criteria, 10, cv::KMEANS_RANDOM_CENTERS);
        // 将每个像素赋值为其所属聚类中心的颜色
        cv::Mat segmented = cv::Mat(OriginalImg.size(), OriginalImg.type());
        for (int i = 0; i < OriginalImg.rows; i++) {
          for (int j = 0; j < OriginalImg.cols; j++) {
            int label = labels.at<int>(i * OriginalImg.cols + j);
             segmented.at<cv::Vec3b>(i, j) = colorArr[label];
           }

        }
        cv::imwrite("k.png",segmented);
        ResultImg= cv::imread("k.png");
        ui->label_86->setText("Kmeans");
    }
    currentImg=ResultImg;


    //显示
    cv::Mat imgShow;
    cvtColor(ResultImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_82->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_82->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}







bool MainWindow::eventFilter(QObject* watched, QEvent* event){
    if (event->type() == QEvent::MouseButtonPress){
        QMouseEvent *mouseevent = static_cast<QMouseEvent*>(event);
        if(mouseevent->button() == Qt::LeftButton && watched == ui->graphicsView_LeftImage->viewport()){
            //左窗口测试选点(左键)
            std::vector<QPointF> ps = ui->graphicsView_LeftImage->GetPoints();
            if (ps.size() != 0){
                QPointF nowp = ps.at(ps.size() - 1);

                ui->textBrowser_log->moveCursor(QTextCursor::End);
                ui->textBrowser_log->append(">>" + QString("影像上一个点为第" +QString::number(ps.size()) +"个点,坐标为:(" +QString::number(nowp.x(), 'f', 3) +"," +QString::number(nowp.y(), 'f', 3) + ")"));
            }

        }
        else if(mouseevent->button() == Qt::LeftButton && watched == ui->graphicsView_9->viewport()){
            ;
        }

        else if(mouseevent->button() == Qt::RightButton && watched == ui->graphicsView_10->viewport()){
            ;
        }

        else if(mouseevent->button() == Qt::RightButton && watched == ui->graphicsView_RightImage->viewport()){
            //右键测试选区域(右键)
            std::vector<std::vector<QPointF>> areas = ui->graphicsView_RightImage->GetAllPoints();
            if (areas.size() != 0){
                std::vector<QPointF> nowarea = areas.at(areas.size() - 1);
                QString coorstr = "";
                for (unsigned long long i(0); i<nowarea.size(); ++i){
                    QString tstr = "(" +QString::number(nowarea[i].x(), 'f', 3) +", " +QString::number(nowarea[i].y(), 'f', 3) +")\n";
                    coorstr += tstr;
                }

                ui->textBrowser_log->moveCursor(QTextCursor::End);
                ui->textBrowser_log->append(">>" +QString("影像上一个区域为第" +QString::number(areas.size()) +"个区域,坐标集为:\n" + coorstr));

            }
        }
    }
    return QObject::eventFilter(watched, event);
}



//区域生长
void MainWindow::on_pushButton_quyushenzhang_clicked(){
    page_idx=9;    //界面索引
    function_idx=26;//功能的索引


    ui->stackedWidget->setCurrentIndex(9);
    ui->pushButton_gn9->setText("Region Growing");
    status_label->setText(QString(""));

    //图片
    cv::Mat imgShow;
    cvtColor(currentImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_9->SetImage(qImg);


    //设置事件过滤器,用于选点测试
    ui->graphicsView_9->viewport()->installEventFilter(this);

    //设置联动
//    ui->graphicsView_LeftImage->SetLinkFlag(1);
//    ui->graphicsView_RightImage->SetLinkFlag(1);
//    ui->graphicsView_LeftImage->SetLinkWidget(ui->graphicsView_RightImage);
//    ui->graphicsView_RightImage->SetLinkWidget(ui->graphicsView_LeftImage);


    //设置选点
    ui->graphicsView_9->SetChoosePoint(2);
   //ui->graphicsView_RightImage->SetChoosePoint(1);//右边窗口测试选区域
}


//漫水填充
void MainWindow::on_pushButton_floodFill_clicked(){
    page_idx=9;    //界面索引
    function_idx=27;//功能的索引

    ui->stackedWidget->setCurrentIndex(9);
    ui->pushButton_gn9->setText("Flood Fill");
    status_label->setText(QString(""));

    //图片
    cv::Mat imgShow;
    cvtColor(currentImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_9->SetImage(qImg);

    //设置事件过滤器,用于选点测试
    ui->graphicsView_9->viewport()->installEventFilter(this);

    //设置选点
    ui->graphicsView_9->SetChoosePoint(2);
}




OCVLiveWire lw;
cv::Mat img;

///////////////////////////////////////////////
static void onMouse( int event, int x, int y, int flags, void* ){

    bool isKeyShift = (flags&CV_EVENT_FLAG_SHIFTKEY);
    bool isKeyCtrl  = (flags&CV_EVENT_FLAG_CTRLKEY);

    if(event==CV_EVENT_LBUTTONDOWN) {
        lw.calcLWP(cv::Point(x,y), !isKeyShift);
        lw.incPath();
        lw.helpDrawImgPXY();
    } else {
        cv::Point pxy(x,y);
        if(lw.isStartedPath()) {
            lw.calcLWPath(pxy, !isKeyShift);
            cv::Mat imgOUT;
            img.copyTo(imgOUT);
            for(int ii=1; ii<lw.pathTot.size(); ii++) {
                cv::line(imgOUT,lw.pathTot[ii-1],lw.pathTot[ii],cv::Scalar(255,0,0),2);
            }
            std::vector<cv::Point> tpath;
            tpath.push_back(lw.getCPoint());
            tpath.insert(tpath.end(), lw.path.begin(), lw.path.end());
            for(int ii=1; ii<tpath.size(); ii++) {
                cv::line(imgOUT,tpath[ii-1],tpath[ii],cv::Scalar(0,255,0),2);
            }
            cv::circle(imgOUT,lw.getCPoint(),3,cv::Scalar(0,0,255),1);
            cv::imshow("win", imgOUT);

            cv::imwrite("lv.png",imgOUT);
        }
    }
}


//livewire
void MainWindow::on_pushButton_livewire_clicked(){
    page_idx=9;    //界面索引
    function_idx=28;//功能的索引

    ui->stackedWidget->setCurrentIndex(9);
    ui->pushButton_gn9->setText("livewire");
    status_label->setText(QString(""));

    ui->label_96->setText("livewire");


    cv::Mat OriginalImg;
    cv::Mat ResultImg;

    //Pipeline
    if(ui->checkBox_Pipeline->isChecked()){
        OriginalImg=currentImg;
    }else{
        OriginalImg=srcImg;
    }

    //图片
    cv::Mat imgShow;
    cvtColor(OriginalImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_9->SetImage(qImg);

    //设置事件过滤器,用于选点测试
    ui->graphicsView_9->viewport()->installEventFilter(this);

    //设置选点
    ui->graphicsView_9->SetChoosePoint(2);


    img =OriginalImg;
    lw.loadImage(img);

    cv::imshow("win", img);
    cv::setMouseCallback("win", onMouse, 0);

    while(true) {
        int key = cv::waitKey(0);
        if(key==27) {
            break;
        }
    }


}


cv::Mat regionGrow(const cv::Mat &src, const cv::Point2i seed, int throld){
    //convert src to gray for getting gray value of every pixel
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);

    // set every pixel to black
    cv::Mat result = cv::Mat::zeros(src.size(), CV_8UC1);
    if ((seed.x < 0) || (seed.y < 0))
        return result;
    result.at<uchar>(seed.y, seed.x) = 255;

    //count: sum of points that meet the conditions in every eight-neighbor
    int count = 1;
    //start growing
    while (count > 0) {
        count = 0;
        for (int row = 1; row < gray.rows - 1; row++) {
            for (int col = 1; col < gray.cols - 1; col++) {
                if (result.at<uchar>(row, col) != 255)
                    continue;
                //gray value of current seed
                int seed_gray = gray.at<uchar>(row, col);
                for (int m = row - 1; m <= row + 1; m++) {
                    for (int n = col - 1; n <= col + 1; n++) {
                        int value = gray.at<uchar>(m, n);
                        if ((result.at<uchar>(m, n) == 0) && (abs(value - seed_gray) <= throld)) {
                            result.at<uchar>(m, n) = 255;
                            count++;
                        }
                    }
                }
            }
        }
    }
    return result;
}

cv::Mat regionGrowFast(const cv::Mat &src, const cv::Point2i seed, int throld){
    //convert src to gray for getting gray value of every pixel
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);

    // set every pixel to black
    cv::Mat result = cv::Mat::zeros(src.size(), CV_8UC1);
    if ((seed.x < 0) || (seed.y < 0))
        return result;
    result.at<uchar>(seed.y, seed.x) = 255;

    //grow direction sequenc
    int grow_direction[8][2] = { {-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0} };
    //seeds collection
    std::vector<cv::Point2i> seeds;
    seeds.push_back(seed);

    //start growing
    while (!seeds.empty()) {
        //get a seed
        cv::Point2i current_seed = seeds.back();
        seeds.pop_back();
        //gray value of current seed
        int seed_gray = gray.at<uchar>(current_seed.y, current_seed.x);

        for (int i = 0; i < 8; ++i) {
            cv::Point2i neighbor_seed(current_seed.x + grow_direction[i][0], current_seed.y + grow_direction[i][1]);
            //check wether in image
            if (neighbor_seed.x < 0 || neighbor_seed.y < 0 || neighbor_seed.x >(gray.cols - 1) || (neighbor_seed.y > gray.rows - 1))
                continue;
            int value = gray.at<uchar>(neighbor_seed.y, neighbor_seed.x);
            if ((result.at<uchar>(neighbor_seed.y, neighbor_seed.x) == 0) && (abs(value - seed_gray) <= throld)) {
                result.at<uchar>(neighbor_seed.y, neighbor_seed.x) = 255;
                seeds.push_back(neighbor_seed);
            }
        }
    }
    return result;
}


void MainWindow::on_pushButton_gn9_clicked(){
    std::vector<QPointF> ps = ui->graphicsView_9->GetPoints();
    QPointF nowp = ps.at(ps.size() - 1);
    qDebug()<<nowp.x()<<"," <<nowp.y();
    cv::Point2i seed_point(nowp.x(), nowp.y());

    cv::Mat OriginalImg;
    cv::Mat ResultImg;

    //Pipeline
    if(ui->checkBox_Pipeline->isChecked()){
        OriginalImg=currentImg;
    }else{
        OriginalImg=srcImg;
    }

    if(ui->pushButton_gn9->text()=="Region Growing"){
        if (OriginalImg.type()!= CV_8UC3){
            QMessageBox::information(this,"tip","src.type() != CV_8UC3");
            return;
        }

        ResultImg= regionGrowFast(OriginalImg, seed_point, 4);
        ui->label_96->setText("Region Growing");
    }else if(ui->pushButton_gn9->text()=="Flood Fill"){
        // 定义填充颜色
        cv::Scalar fillColor(0, 0, 255); // BGR颜色，这里是红色

        if (OriginalImg.type()!= CV_8UC3){
            QMessageBox::information(this,"tip","src.type() != CV_8UC3");
            return;
        }
        ResultImg=OriginalImg;
        cv::floodFill(ResultImg, seed_point, fillColor);
        ui->label_96->setText("Flood Fill");

    }else if(ui->pushButton_gn9->text()=="livewire"){

        ResultImg = cv::imread("lv.png");
        ui->label_96->setText("livewire");
    }
    currentImg=ResultImg;

    cv::Mat imgShow;
    cvtColor(ResultImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_92->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_92->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}


//SVM
void MainWindow::on_svm_test_clicked(){
    page_idx=10;    //界面索引
    function_idx=29;//功能的索引


    ui->stackedWidget->setCurrentIndex(10);
    ui->pushButton_gn10->setText("SVM");
    status_label->setText(QString(""));

    //图片
    cv::Mat imgShow;
    cvtColor(currentImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_10->SetImage(qImg);

    //设置事件过滤器,用于选点测试
    ui->graphicsView_10->viewport()->installEventFilter(this);

    //设置选点
    ui->graphicsView_10->SetChoosePoint(1);

}


void MainWindow::on_pushButton_gn10_clicked(){
    std::vector<std::vector<QPointF>> areas = ui->graphicsView_10->GetAllPoints();
    cv::Mat OriginalImg;
    cv::Mat ResultImg;

    //Pipeline
    if(ui->checkBox_Pipeline->isChecked()){
        OriginalImg=currentImg;
    }else{
        OriginalImg=srcImg;
    }

    if (OriginalImg.type()!= CV_8UC3){
        QMessageBox::information(this,"tip","src.type() != CV_8UC3");
        return;
    }

    ResultImg= OriginalImg.clone();

    if (areas.size() >= 2){
        std::vector<QPointF> justarea = areas.at(areas.size()-2);
        std::vector<QPointF> nowarea = areas.at(areas.size()-1);

        // 选取目标区域和背景区域
        cv::Mat BackImg;
        cv::Mat ForeImg;

        for (unsigned long long i(0); i<nowarea.size(); ++i){
            BackImg.push_back(OriginalImg.at<cv::Vec3b>(nowarea[i].x(), nowarea[i].y()));
        }

        for (unsigned long long i(0); i<justarea.size(); ++i){
            ForeImg.push_back(OriginalImg.at<cv::Vec3b>(justarea[i].x(), justarea[i].y()));
        }

        // 初始化训练数据
        cv::Mat trainingDataMat = ForeImg.clone().reshape(1, ForeImg.cols*ForeImg.rows);
        //在这里直接存入背景像素点，或者像下边一个一个点存入也可以
        trainingDataMat.push_back(BackImg.clone().reshape(1,BackImg.cols*BackImg.rows));
        trainingDataMat.convertTo(trainingDataMat, CV_32FC1);

        // 初始化标签，分别给两种标签辅助，虽然这里memset已经全部初始化为1了，可是这里的1是浮点数
        int *labels = new int[ForeImg.cols*ForeImg.rows + BackImg.cols*BackImg.rows];
        memset(labels, 1, sizeof(int)*(ForeImg.cols*ForeImg.rows + BackImg.cols*BackImg.rows));
        for (int i = 0; i < ForeImg.rows; ++i)
            for (int j = 0; j < ForeImg.cols; ++j){
                labels[i*ForeImg.cols + j] = 1;
            }
        for (int h = 0; h<BackImg.rows; h++){
            for (int w = 0; w<BackImg.cols; w++){
                labels[ForeImg.cols*ForeImg.rows + h*BackImg.cols + w] = -1;
            }
        }
        cv::Mat labelsMat = cv::Mat(ForeImg.cols*ForeImg.rows + BackImg.cols*BackImg.rows, 1, CV_32SC1, labels);

        // 可以将数据写入文件来检查是否正确
        //FileStorage fs("data.xml", FileStorage::WRITE);
        //fs << "traindata" << trainingDataMat << "labels" << labelsMat;

        // Train the SVM
        cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
        svm->setType(cv::ml::SVM::C_SVC);
        svm->setKernel(cv::ml::SVM::RBF);
        svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, (int)1e5, 1e-6));
        svm->train(trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat);

        // 开始分类
        cv::Vec3b black(0, 0, 0), white(255, 255, 255);
        for (int i = 0; i < ResultImg.rows; ++i){
            uchar* p_sample = ResultImg.ptr<uchar>(i);
            for (int j = 0; j < ResultImg.cols; ++j){
                cv::Mat sampleMat(1, 3, CV_32FC1);
                sampleMat.at<float>(0, 0) = p_sample[3 * j + 0];
                sampleMat.at<float>(0, 1) = p_sample[3 * j + 1];
                sampleMat.at<float>(0, 2) = p_sample[3 * j + 2];

                float response = svm->predict(sampleMat);

                if (response == 1)
                    ResultImg.at<cv::Vec3b>(i, j) = white;
                else if (response == -1)
                    ResultImg.at<cv::Vec3b>(i, j) = black;
            }
        }
    }
    ui->label_106->setText("SVM");
    currentImg= ResultImg;

    cv::Mat imgShow;
    cvtColor(ResultImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_102->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_102->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}

//SVM字符测试
void on_word_test_clicked(){
    cv::Ptr<cv::ml::SVM> svm1 = cv::ml::SVM::load("models//SVM_HOG.xml");

    if (svm1->empty()){
        std::cout<< "load svm detector failed!!!\n"<< std::endl;
        return;
    }

    cv::Mat testimg;
    testimg = cv::imread("C://Users//31156//Desktop//Poff-AI//sample/9/0.png");
    cv::resize(testimg, testimg, cv::Size(28, 28), 1);
    imshow("src", testimg);
    //waitKey(0);

    cv::HOGDescriptor hog(cv::Size(14, 14), cv::Size(7, 7), cv::Size(1, 1), cv::Size(7, 7), 9);
    std::vector<float> imgdescriptor;
    hog.compute(testimg, imgdescriptor, cv::Size(5, 5));
    cv::Mat sampleMat;
    sampleMat.create(1, imgdescriptor.size(), CV_32FC1);

    for (size_t i = 0; i < imgdescriptor.size(); i++){
        sampleMat.at<float>(0, i) = imgdescriptor[i];//第num个样本的特征向量中的第i个元素
    }
    int ret = svm1->predict(sampleMat);
    std::cout << "ret= " << ret << std::endl;

    cv::waitKey(0);
    cv::destroyWindow("src");
    cv::waitKey(1);
}

void MainWindow::on_pushButton_snake_clicked(){
    cv::Mat edge,src;
    int x0, y0, x1, y1, step;

    if (false){
        src = cv::imread("../CvSnakeImage/canon.jpg", 0);
        cv::threshold(src, src, 90, 255, cv::THRESH_OTSU);
        x0 = 60; y0 = 90; x1 = 450; y1 = 640; step = 10;
    }else{
        src =currentImg;
        //src = cv::imread("‪models\\16_sam_mask.png", 0);
        x0 = 122; y0 = 68; x1 = 211; y1 = 154; step = 10;
    }

    #define edgepath "C:\\Users\\31156\\Desktop\\Poff-AI\\IADS\\models\\16_sam_mask.png"

    edge =cv::imread(edgepath);


   // 使用Canny算法检测边缘
   cv::Mat edges;
   double lowerThreshold = 50; // 下限阈值
   double upperThreshold = 150; // 上限阈值
   int apertureSize = 3; // 应用Sobel算子的窗口大小
   cv::Canny(edge, edges, lowerThreshold, upperThreshold, apertureSize);

   imshow("canny",edges);
   cvWaitKey(0); //等待按键


       // 查找边缘点
       std::vector<cv::Point> points;
       for (int y = 0; y < edges.rows; ++y) {
           for (int x = 0; x < edges.cols; ++x) {
               if (edges.at<uchar>(y, x) != 0) {
                   points.push_back(cv::Point(x, y));
               }

           }
           continue;
       }

       // 输出边缘点
       for (const auto& point : points) {
           std::cout << "Edge point: (" << point.x << ", " << point.y << ")" << std::endl;
       }



//    std::vector<cv::Point> points;
//    //up side
//    for (int i = 0; i <= (x1 - x0) / step; i++)
//    {
//        points.push_back({ x0 + i * step, y0 });
//    }
//    //right side
//    for (int i = 1; i <= (y1 - y0) / step; i++)
//    {
//        points.push_back({ x1, y0 + i * step});
//    }
//    //bottom side
//    for (int i = (x1 - x0) / step - 1; i > 0 ; i--)
//    {
//        points.push_back({ x0 + i * step, y1 });
//    }
//    //left side
//    for (int i = (y1 - y0) / step; i > 0; i--)
//    {
//        points.push_back({ x0, y0 + i * step });
//    }

    SnakeImage snake(src);
    snake.segment(points);
    cv::waitKey(0);
}

//***********************************************************************************特征检测****************************************************************************************************
//lbp
void elbp(cv::Mat& src, cv::Mat &dst, int radius, int neighbors){

    for (int n = 0; n<neighbors; n++){
        // 采样点的计算
        float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
        float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));
        // 上取整和下取整的值
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));
        // 小数部分
        float ty = y - fy;
        float tx = x - fx;
        // 设置插值权重
        float w1 = (1 - tx) * (1 - ty);
        float w2 = tx  * (1 - ty);
        float w3 = (1 - tx) * ty;
        float w4 = tx * ty;
        // 循环处理图像数据
        for (int i = radius; i < src.rows - radius; i++){
            for (int j = radius; j < src.cols - radius; j++){
                // 计算插值
                float t = static_cast<float>(w1*src.at<uchar>(i + fy, j + fx) + w2*src.at<uchar>(i + fy, j + cx) + w3*src.at<uchar>(i + cy, j + fx) + w4*src.at<uchar>(i + cy, j + cx));
                // 进行编码
                dst.at<uchar>(i - radius, j - radius) += ((t > src.at<uchar>(i, j)) || (abs(t - src.at<uchar>(i, j)) < std::numeric_limits<float>::epsilon())) << n;
            }
        }
    }
}

void elbp1(cv::Mat& src, cv::Mat &dst){
    // 循环处理图像数据
    for (int i = 1; i < src.rows - 1; i++){
        for (int j = 1; j < src.cols - 1; j++){
            uchar tt = 0;
            int tt1 = 0;
            uchar u = src.at<uchar>(i, j);
            if (src.at<uchar>(i - 1, j - 1)>u) { tt += 1 << tt1; }
            tt1++;
            if (src.at<uchar>(i - 1, j)>u) { tt += 1 << tt1; }
            tt1++;
            if (src.at<uchar>(i - 1, j + 1)>u) { tt += 1 << tt1; }
            tt1++;
            if (src.at<uchar>(i, j + 1)>u) { tt += 1 << tt1; }
            tt1++;
            if (src.at<uchar>(i + 1, j + 1)>u) { tt += 1 << tt1; }
            tt1++;
            if (src.at<uchar>(i + 1, j)>u) { tt += 1 << tt1; }
            tt1++;
            if (src.at<uchar>(i + 1, j - 1)>u) { tt += 1 << tt1; }
            tt1++;
            if (src.at<uchar>(i - 1, j)>u) { tt += 1 << tt1; }
            tt1++;

            dst.at<uchar>(i - 1, j - 1) = tt;
        }
    }
}

//lbp
void MainWindow::on_tool_lbp_clicked(){
    page_idx=16;    //界面索引
    function_idx=30;//功能的索引


    jiemianqiehuan(16,ui->label_tu_161,ui->label_tu_162,ui->label_165,ui->label_166);
    status_label->setText(QString(""));
    ui->pushButton_gn16->setText("lbp");

    ui->comboBox_16->clear(); //清除列表
    QStringList strList;
    strList<<"Normal"<<"Circle";
    ui->comboBox_16->addItems(strList);
}



//haar
double compute_sum_of_rect(cv::Mat src,cv::Rect r){
    int x=r.x;
    int y=r.y;
    int width=r.width;
    int height=r.height;
    double sum;
    //这里使用Mat::at函数需要注意第一参数为行数对应的y和高度height，第二个参数对应才是列数对应的x和宽度width
    sum=src.at<double>(y,x)+src.at<double>(y+height,x+width)-src.at<double>(y+height,x)-src.at<double>(y,x+width);
    return sum;
}

void MainWindow::on_tool_haar_clicked(){
    page_idx=16;    //界面索引
    function_idx=31;//功能的索引

    jiemianqiehuan(16,ui->label_tu_161,ui->label_tu_162,ui->label_165,ui->label_166);
    status_label->setText(QString(""));
    ui->pushButton_gn16->setText("Haar");

    ui->comboBox_16->clear(); //清除列表
    QStringList strList;
    strList<<"Haar水平"<<"Haar竖直";
    ui->comboBox_16->addItems(strList);
}


//GLCM
void MainWindow::on_tool_GLCM_clicked(){
    page_idx=16;    //界面索引
    function_idx=32;//功能的索引

    jiemianqiehuan(16,ui->label_tu_161,ui->label_tu_162,ui->label_165,ui->label_166);
    status_label->setText(QString(""));
    ui->pushButton_gn16->setText("GLCM");

    ui->comboBox_16->clear(); //清除列表
    QStringList strList;
    strList<<"GLCM0"<<"GLCM45"<<"GLCM90"<<"GLCM135";
    ui->comboBox_16->addItems(strList);

}
// 0°GLCM
void getGLCM0(cv::Mat& src, cv::Mat& dst, int gray_level){
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst.create(gray_level, gray_level, CV_32SC1);
    dst = cv::Scalar::all(0);
    for (int i = 0; i < height; i++) {
        uchar* srcdata = src.ptr<uchar>(i);
        for (int j = 0; j < width - 1; j++){
            //horizontal
            int rows = srcdata[j];
            int cols = srcdata[j + 1];
            dst.ptr<int>(rows)[cols]++;
        }
    }
}

// 90°GLCM
void getGLCM90(cv::Mat& src, cv::Mat& dst, int gray_level)
{
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst = cv::Mat(gray_level, gray_level, CV_32SC1, cv::Scalar(0));
    for (int i = 0; i < height - 1; i++){
        uchar* srcdata = src.ptr<uchar>(i);
        uchar* srcdata1 = src.ptr<uchar>(i + 1);
        for (int j = 0; j < width; j++){
            // vertical
            int rows = srcdata[j];
            int cols = srcdata1[j];
            dst.ptr<int>(rows)[cols]++;
        }
    }
}

// 45°GLCM
void getGLCM45(cv::Mat& src, cv::Mat& dst, int gray_level)
{
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst = cv::Mat(gray_level, gray_level, CV_32SC1, cv::Scalar(0));
    for (int i = 0; i < height - 1; i++){
        uchar* srcdata = src.ptr<uchar>(i);
        uchar* srcdata1 = src.ptr<uchar>(i + 1);
        for (int j = 0; j < width - 1; j++) {
            // +45°
            int rows = srcdata[j];
            int cols = srcdata1[j + 1];
            dst.ptr<int>(rows)[cols]++;
        }
    }
}

// 135°GLCM
void getGLCM135(cv::Mat& src, cv::Mat& dst, int gray_level){
    CV_Assert(1 == src.channels());
    int height = src.rows;
    int width = src.cols;
    dst = cv::Mat(gray_level, gray_level, CV_32SC1, cv::Scalar(0));
    for (int i = 0; i < height - 1; i++) {
        uchar* srcdata = src.ptr<uchar>(i);
        uchar* srcdata1 = src.ptr<uchar>(i + 1);
        for (int j = 1; j < width; j++) {
            // -45°
            int rows = srcdata[j];
            int cols = srcdata1[j - 1];
            dst.ptr<int>(rows)[cols]++;
        }
    }

}


//Garbor滤波
void MainWindow::on_tool_Garbor_clicked(){
    page_idx=16;    //界面索引
    function_idx=33;//功能的索引


    jiemianqiehuan(16,ui->label_tu_161,ui->label_tu_162,ui->label_165,ui->label_166);
    status_label->setText(QString(""));
    ui->pushButton_gn16->setText("Garbor");

    ui->comboBox_16->clear(); //清除列表
    QStringList strList;
    strList<<"gabor1"<<"gabor2"<<"gabor3"<<"gabor4";
    ui->comboBox_16->addItems(strList);

}



void MainWindow::on_pushButton_gn16_clicked(){

    int radius, neighbors;
    radius = 1;
    neighbors = 8;
    cv::Mat dst;


    if(ui->comboBox_16->currentText()=="Normal"){
        //创建一个LBP    注意为了溢出,行列都在原有图像上减去2个半径
        dst = cv::Mat(grayImg.rows - 2 * radius, grayImg.cols - 2 * radius, CV_8UC1, cv::Scalar(0));
        elbp1(grayImg, dst);
    }else if(ui->comboBox_16->currentText()=="Circle"){
        dst = cv::Mat(grayImg.rows - 2 * radius, grayImg.cols - 2 * radius, CV_8UC1, cv::Scalar(0));
        elbp(grayImg, dst, 1, 8);
    }else if(ui->comboBox_16->currentText()=="Haar水平"){
        cv::Mat src_img =currentImg;
        cv::Mat gray_img;
        cvtColor(src_img,gray_img,cv::COLOR_BGR2GRAY);

        cv::Mat sum_img = cv::Mat::zeros(gray_img.rows + 1, gray_img.cols + 1,CV_32FC1);
        integral(grayImg,sum_img,CV_64F);

        int step_x = 8, step_y = 8;
        double sum;
        cv::Rect rect1, rect2;
        cv::Mat dst_1 = cv::Mat::zeros(src_img.size(),CV_32FC1);
        for(int i = 0; i < src_img.rows; i = i + step_x){
            for(int j = 0; j < src_img.cols; j = j + step_y){
                rect1 = cv::Rect(j, i, 2, 4);
                rect2 = cv::Rect(j + 2, i, 2, 4);
                sum = compute_sum_of_rect(gray_img,rect1) - compute_sum_of_rect(gray_img,rect2);
                dst_1 .at<float>(i,j) = sum;
            }
        }
        convertScaleAbs(dst_1 ,dst);
    }else if(ui->comboBox_16->currentText()=="Haar竖直"){
        cv::Mat src_img =currentImg;
        cv::Mat gray_img;
        cvtColor(src_img,gray_img,cv::COLOR_BGR2GRAY);

        cv::Mat sum_img = cv::Mat::zeros(gray_img.rows + 1, gray_img.cols + 1,CV_32FC1);
        integral(grayImg,sum_img,CV_64F);

        int step_x = 8, step_y = 8;
        double sum;
        cv::Rect rect1, rect2;
        cv::Mat dst_1= cv::Mat::zeros(src_img.size(),CV_32FC1);
        for(int i = 0; i < src_img.rows; i = i + step_x){
            for(int j = 0; j < src_img.cols; j = j + step_y){
                rect1 = cv::Rect(j, i, 2, 4);
                rect2 = cv::Rect(j, i + 2, 2, 4);
                sum = compute_sum_of_rect(gray_img,rect1) - compute_sum_of_rect(gray_img,rect2);
                dst_1.at<float>(i,j) = sum;
            }
        }
        convertScaleAbs(dst_1,dst);
    }else if(ui->comboBox_16->currentText()=="GLCM0"){
        cv::Mat gray;
        cvtColor(currentImg, gray, CV_RGB2GRAY);
        getGLCM0(gray, dst, 256);
    }else if(ui->comboBox_16->currentText()=="GLCM45"){
        cv::Mat gray;
        cvtColor(currentImg, gray, CV_RGB2GRAY);
        getGLCM45(gray, dst, 256);
    }else if(ui->comboBox_16->currentText()=="GLCM90"){
        cv::Mat gray;
        cvtColor(currentImg, gray, CV_RGB2GRAY);
        getGLCM90(gray, dst, 256);
    }else if(ui->comboBox_16->currentText()=="GLCM135"){
        cv::Mat gray;
        cvtColor(currentImg, gray, CV_RGB2GRAY);
        getGLCM135(gray, dst, 256);
    }


    if(ui->pushButton_gn16->text()=="Garbor"){
        cv::Mat src=grayImg;
        cv::Mat src_f;
        src.convertTo(src_f, CV_32F);

        // 参数初始化
        int kernel_size = 3;
        double sigma = 1.0, lambd = CV_PI / 8, gamma = 0.5, psi = 0;
        std::vector<cv::Mat> destArray;
        double theta[4];
        cv::Mat temp;

        // theta 法线方向
        theta[0] = 0;
        theta[1] = CV_PI / 4;
        theta[2] = CV_PI / 2;
        theta[3] = CV_PI - CV_PI / 4;
        // gabor 纹理检测器，可以更多，
        // filters = number of thetas * number of lambd
        // 这里lambad只取一个值，所有4个filter
        for (int i = 0; i<4; i++){
            cv::Mat kernel1;
            cv::Mat dest;
            kernel1 = getGaborKernel(cv::Size(kernel_size, kernel_size), sigma, theta[i], lambd, gamma, psi, CV_32F);
            filter2D(src_f, dest, CV_32F, kernel1);
            destArray.push_back(dest);
        }
        // 显示与保存
        cv::Mat dst1, dst2, dst3, dst4;   // gabor1 gabor2 gabor3 gabor4
        convertScaleAbs(destArray[0], dst1);
        convertScaleAbs(destArray[1], dst2);
        convertScaleAbs(destArray[2], dst3);
        convertScaleAbs(destArray[3], dst4);

        // 合并结果
        add(destArray[0], destArray[1], destArray[0]);
        add(destArray[2], destArray[3], destArray[2]);
        add(destArray[0], destArray[2], destArray[0]);


        cv::Mat dst_res;
        convertScaleAbs(destArray[0], dst_res, 0.2, 0);

        // 二值化显示
        cv::Mat gray, binary;
        // cvtColor(dst_res, gray, COLOR_BGR2GRAY);
        threshold(dst_res, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
        //imshow("result", dst_res);
        //imshow("binary", binary);

        if(ui->comboBox_16->currentText()=="gabor1"){
            dst=dst1;
        }else if(ui->comboBox_16->currentText()=="gabor2"){
            dst=dst2;
        }else if(ui->comboBox_16->currentText()=="gabor3"){
            dst=dst3;
        }else if(ui->comboBox_16->currentText()=="gabor4"){
            dst=dst4;
        }
    }

    ui->label_166->setText(ui->comboBox_16->currentText());
    cv::Mat imgShow;
    cvtColor(dst,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_162->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_162->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}


//直方图检测
void MainWindow::on_tool_his_clicked(){
    page_idx=17;    //界面索引
    function_idx=34;//功能的索引


    jiemianqiehuan(17,ui->label_tu_171,ui->label_tu_172,ui->label_175,ui->label_176);
    status_label->setText(QString(""));
    ui->pushButton_gn17->setText("直方图检测");
}

//模版匹配
void MainWindow::on_tool_moban_clicked(){
    page_idx=17;    //界面索引
    function_idx=35;//功能的索引

    jiemianqiehuan(17,ui->label_tu_171,ui->label_tu_172,ui->label_175,ui->label_176);
    status_label->setText(QString(""));
    ui->pushButton_gn17->setText("模版匹配");
}

//SIFT
void MainWindow::on_tool_SIFT_clicked(){
    page_idx=17;    //界面索引
    function_idx=36;//功能的索引

    jiemianqiehuan(17,ui->label_tu_171,ui->label_tu_172,ui->label_175,ui->label_176);
    status_label->setText(QString(""));
    ui->pushButton_gn17->setText("SIFT");
}

//orb
void MainWindow::on_tool_orb_clicked(){
    page_idx=17;    //界面索引
    function_idx=37;//功能的索引

    jiemianqiehuan(17,ui->label_tu_171,ui->label_tu_172,ui->label_175,ui->label_176);
    status_label->setText(QString(""));
    ui->pushButton_gn17->setText("orb");
}

//SURF
void MainWindow::on_tool_SURF_clicked(){
    page_idx=17;    //界面索引
    function_idx=38;//功能的索引

    jiemianqiehuan(17,ui->label_tu_171,ui->label_tu_172,ui->label_175,ui->label_176);
    status_label->setText(QString(""));
    ui->pushButton_gn17->setText("SURF");
}

void MainWindow::on_pushButton_file17_clicked(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片",".",tr("Image Files (*.png *.jpeg *.jpg *.bmp)"));

    if(!FileName.isEmpty()){
        templateImg = cv::imread(FileName.toStdString());

        if(srcImg.empty()){
            QMessageBox::information(this,"tip","Could not open the image");
            templateImg = cv::imread("C://Users//31156//Desktop//Poff-AI//suda0.jpg");
            return ;
        }
    }
    //显示
    cv::Mat imgShow;
    cvtColor(templateImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_173->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_173->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}




void MainWindow::on_pushButton_gn17_clicked(){
    cv::Mat out;
    if(ui->pushButton_gn17->text()=="直方图检测"){
        QImage Qtemp,Qtemp1;

        cv::Mat temp0 = currentImg;
        cv::Mat temp1 = templateImg;
        cv::Mat Img0,Img1,Img2;

        cvtColor(temp0, Img0, cv::COLOR_BGR2HSV);
        cvtColor(temp1, Img1, cv::COLOR_BGR2HSV);
        cv::Mat box = Img1.clone();

        int h_bins = 50;
        int s_bins = 60;
        int histSize[] = { h_bins,s_bins };
        float h_ranges[] = { 0,180 };
        float s_ranges[] = { 0,256 };
        const float* ranges[] = { h_ranges, s_ranges };
        int channels[] = { 0,1 };

        double max = 0.;
        int x_ray,y_ray;


        for(int i = 0; i < Img0.rows-Img1.rows-1; i++){
            for(int j = 0; j < Img0.cols-Img1.cols-1; j++){
                for(int x = i; x < Img1.rows + i; x++){
                    for(int y = j; y < Img1.cols + j; y++){
                        box.at<cv::Vec3b>(x-i, y-j) = Img0.at<cv::Vec3b>(x, y);
                    }
                }
                cv::MatND hist_src0;
                cv::MatND hist_src1;

                calcHist(&box, 1, channels, cv::Mat(), hist_src0, 2, histSize, ranges, true, false);
                normalize(hist_src0, hist_src0, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

                calcHist(&Img1, 1, channels, cv::Mat(), hist_src1, 2, histSize, ranges, true, false);
                normalize(hist_src1, hist_src1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

                double src_src = compareHist(hist_src0, hist_src1, CV_COMP_CORREL);

                std::cout << "src compare with src correlation value : " << src_src << std::endl;

                if(src_src > max){
                    max = src_src;
                    x_ray = i;
                    y_ray = j;
                }
            }
        }
        std::cout << "************************************"  << std::endl;

        cv::Rect rect(x_ray, y_ray, Img1.rows, Img1.cols);
        rectangle(Img0, rect, cv::Scalar(255, 0, 0),1, cv::LINE_8,0);
        //imshow("check",Img0);
        //imshow("Img1",Img1);
        out=Img0;
    }else if(ui->pushButton_gn17->text()=="模版匹配"){
        QImage Qtemp,Qtemp1;
        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;

        cv::Mat Img0 = currentImg;
        cv::Mat Img1 = templateImg;

        cv::Mat result;

        matchTemplate(Img0, Img1, result, cv::TM_SQDIFF);

        normalize(result, result, 1, 0, cv::NORM_MINMAX);
        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
        rectangle(Img0,cv::Rect(minLoc.x,minLoc.y,Img1.cols,Img1.rows),1,8,0 );

        //cv::imwrite("moban.png",result*255);
        //out = cv::imread("moban.png");
        out=Img0;

    }else if(ui->pushButton_gn17->text()=="SIFT"){

        cv::Mat src1 = currentImg;
        cv::Mat src2 = templateImg;
        if (!src1.data || !src2.data){
           cout<<" --(!) Error reading images \n";
           return;
        }

        //sift feature detect
        cv::Ptr<cv::SIFT> siftdetector = cv::SIFT::create();
        std::vector<cv::KeyPoint> kp1, kp2;

        siftdetector->detect(src1, kp1);
        siftdetector->detect(src2, kp2);
        cv::Mat des1, des2;//descriptor
        siftdetector->compute(src1, kp1, des1);
        siftdetector->compute(src2, kp2, des2);
        cv::Mat res1, res2;

        drawKeypoints(src1, kp1, res1);//在内存中画出特征点
        drawKeypoints(src2, kp2, res2);

        //_cprintf("size of description of Img1: %d\n",kp1.size());
        //_cprintf("size of description of Img2: %d\n",kp2.size());

        cv::Mat transimg1, transimg2;
        transimg1 = res1.clone();
        transimg2 = res2.clone();

        char str1[20], str2[20];
        //sprintf(str1, "%d", kp1.size());
        //sprintf(str2, "%d", ckp2.size());

        const char* str = str1;
        putText(transimg1, str1, cv::Point(280, 230), 0, 1.0,cv::Scalar(255, 0, 0),2);//在图片中输出字符

        str = str2;
        putText(transimg2, str2, cv::Point(280, 230), 0, 1.0,cv::Scalar(255, 0, 0),2);//在图片中输出字符

                                                                                //imshow("Description 1",res1);
        //imshow("descriptor1", transimg1);
        //imshow("descriptor2", transimg2);

        cv::BFMatcher matcher(cv::NORM_L2, true);
        std::vector<cv::DMatch> matches;
        matcher.match(des1, des2, matches);
        cv::Mat img_match;
        drawMatches(src1, kp1, src2, kp2, matches, img_match);//,Scalar::all(-1),Scalar::all(-1),vector<char>(),drawmode);
        //_cprintf("number of matched points: %d\n",matches.size());
        //imshow("matches", img_match);
        out=img_match;


    }else if(ui->pushButton_gn17->text()=="orb"){

        cv::Mat obj = currentImg;  //载入目标图像
        cv::Mat scene = templateImg;//载入场景图像
        if (obj.empty() || scene.empty()){
            cout << "Can't open the picture!\n";
            return;
        }
        std::vector<cv::KeyPoint> obj_keypoints, scene_keypoints;
        cv::Mat obj_descriptors, scene_descriptors;
        cv::Ptr<cv::ORB> detector = cv::ORB::create();

        detector->detect(obj, obj_keypoints);
        detector->detect(scene, scene_keypoints);
        detector->compute(obj, obj_keypoints, obj_descriptors);
        detector->compute(scene, scene_keypoints, scene_descriptors);

        cv::BFMatcher matcher(cv::NORM_HAMMING, true); //汉明距离做为相似度度量
        std::vector<cv::DMatch> matches;
        matcher.match(obj_descriptors, scene_descriptors, matches);
        cv::Mat match_img;
        drawMatches(obj, obj_keypoints, scene, scene_keypoints, matches, match_img);
        //imshow("match_img", match_img);

        //保存匹配对序号
        std::vector<int> queryIdxs(matches.size()), trainIdxs(matches.size());
        for (size_t i = 0; i < matches.size(); i++){
            queryIdxs[i] = matches[i].queryIdx;
            trainIdxs[i] = matches[i].trainIdx;
        }

        cv::Mat H12;                    //变换矩阵

        std::vector<cv::Point2f> points1;
        cv::KeyPoint::convert(obj_keypoints, points1, queryIdxs);
        std::vector<cv::Point2f> points2;
        cv::KeyPoint::convert(scene_keypoints, points2, trainIdxs);
        int ransacReprojThreshold = 5;  //拒绝阈值


        H12 = findHomography(cv::Mat(points1), cv::Mat(points2), cv::RANSAC, ransacReprojThreshold);
        std::vector<char> matchesMask(matches.size(), 0);
        cv::Mat points1t;
        perspectiveTransform(cv::Mat(points1), points1t, H12);
        for (size_t i1 = 0; i1 < points1.size(); i1++)  //保存‘内点’
        {
            if (norm(points2[i1] - points1t.at<cv::Point2f>((int)i1, 0)) <= ransacReprojThreshold) //给内点做标记
            {
                matchesMask[i1] = 1;
            }
        }
        cv::Mat match_img2;   //滤除‘外点’后
        drawMatches(obj, obj_keypoints, scene, scene_keypoints, matches, match_img2, cv::Scalar(0, 0, 255), cv::Scalar::all(-1), matchesMask);

        //画出目标位置
        std::vector<cv::Point2f> obj_corners(4);
        obj_corners[0] = cv::Point(0, 0); obj_corners[1] = cv::Point(obj.cols, 0);
        obj_corners[2] = cv::Point(obj.cols, obj.rows); obj_corners[3] = cv::Point(0, obj.rows);
        std::vector<cv::Point2f> scene_corners(4);
        perspectiveTransform(obj_corners, scene_corners, H12);
        //line( match_img2, scene_corners[0] + Point2f(static_cast<float>(obj.cols), 0),scene_corners[1] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);
        //line( match_img2, scene_corners[1] + Point2f(static_cast<float>(obj.cols), 0),scene_corners[2] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);
        //line( match_img2, scene_corners[2] + Point2f(static_cast<float>(obj.cols), 0),scene_corners[3] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);
        //line( match_img2, scene_corners[3] + Point2f(static_cast<float>(obj.cols), 0),scene_corners[0] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);
        line(match_img2, cv::Point2f((scene_corners[0].x + static_cast<float>(obj.cols)), (scene_corners[0].y)), cv::Point2f((scene_corners[1].x + static_cast<float>(obj.cols)), (scene_corners[1].y)), cv::Scalar(0, 0, 255), 2);
        line(match_img2, cv::Point2f((scene_corners[1].x + static_cast<float>(obj.cols)), (scene_corners[1].y)), cv::Point2f((scene_corners[2].x + static_cast<float>(obj.cols)), (scene_corners[2].y)), cv::Scalar(0, 0, 255), 2);
        line(match_img2, cv::Point2f((scene_corners[2].x + static_cast<float>(obj.cols)), (scene_corners[2].y)), cv::Point2f((scene_corners[3].x + static_cast<float>(obj.cols)), (scene_corners[3].y)), cv::Scalar(0, 0, 255), 2);
        line(match_img2, cv::Point2f((scene_corners[3].x + static_cast<float>(obj.cols)), (scene_corners[3].y)), cv::Point2f((scene_corners[0].x + static_cast<float>(obj.cols)), (scene_corners[0].y)), cv::Scalar(0, 0, 255), 2);

        float A_th;
        A_th = atan(abs((scene_corners[3].y - scene_corners[0].y) / (scene_corners[3].x - scene_corners[0].x)));
        A_th = 90-180 * A_th / 3.14;

        //imshow("match_img2", match_img2);

        //line( scene, scene_corners[0],scene_corners[1],Scalar(0,0,255),2);
        //line( scene, scene_corners[1],scene_corners[2],Scalar(0,0,255),2);
        //line( scene, scene_corners[2],scene_corners[3],Scalar(0,0,255),2);
        //line( scene, scene_corners[3],scene_corners[0],Scalar(0,0,255),2);

        //imshow("scense",scene);

        cv::Mat rotimage;
        cv::Mat rotate = getRotationMatrix2D(cv::Point(scene.cols/2,scene.rows/2), A_th, 1);
        warpAffine(scene,rotimage,rotate, scene.size());
        //imshow("rotimage", rotimage);


        //方法三 透视变换
        cv::Point2f src_point[4];
        cv::Point2f dst_point[4];
        src_point[0].x = scene_corners[0].x;
        src_point[0].y = scene_corners[0].y;
        src_point[1].x = scene_corners[1].x;
        src_point[1].y = scene_corners[1].y;
        src_point[2].x = scene_corners[2].x;
        src_point[2].y = scene_corners[2].y;
        src_point[3].x = scene_corners[3].x;
        src_point[3].y = scene_corners[3].y;


        dst_point[0].x = 0;
        dst_point[0].y = 0;
        dst_point[1].x = obj.cols;
        dst_point[1].y = 0;
        dst_point[2].x = obj.cols;
        dst_point[2].y = obj.rows;
        dst_point[3].x = 0;
        dst_point[3].y = obj.rows;

        cv::Mat newM(3, 3, CV_32FC1);
        newM=getPerspectiveTransform(src_point, dst_point);

        cv::Mat dst = scene.clone();

        warpPerspective(scene, dst, newM, obj.size());

        //imshow("obj", obj);
        //imshow("dst", dst);

        cv::Mat resultimg=dst.clone();

        absdiff(obj, dst, resultimg);//当前帧跟前面帧相减

        //imshow("result", resultimg);

        //imshow("dst", dst);
        //imshow("src", obj);
        out=match_img2;
    }else if(ui->pushButton_gn17->text()=="SURF"){

           cv::Mat src1 = currentImg;
           cv::Mat src2 = templateImg;

    }

    ui->label_176->setText(ui->pushButton_gn17->text());
    cv::Mat imgShow;
    cvtColor(out,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_172->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_172->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


}

//颜色匹配
void MainWindow::on_tool_colorfit_clicked(){
    page_idx=18;    //界面索引
    function_idx=39;//功能的索引

    jiemianqiehuan(18,ui->label_tu_181,ui->label_tu_182,ui->label_185,ui->label_186);
    status_label->setText(QString(""));
    ui->pushButton_gn18->setText("颜色匹配");

}

void MainWindow::on_pushButton_gn18_clicked(){
    //cv::Mat img_color = cv::imread("C:\\Users\\31156\\Desktop\\Poff-AI\\color.jpg");
    cv::Mat img_color = srcImg;
    if (!img_color.data || img_color.channels() != 3)
        return;

    //灰度值归一化
    cv::Mat bgr_color;
    //HSV图像
    cv::Mat hsv_color;

    //彩色图像的灰度值归一化
    img_color.convertTo(bgr_color, CV_32FC3, 1.0 / 255, 0);
    //颜色空间转换
    cvtColor(bgr_color, hsv_color, cv::COLOR_BGR2HSV);


    //色度
    //int hsv_hmin = 0;
    //int hsv_hmin_Max = 360;
    //int hsv_hmax = 360;
    //int hsv_hmax_Max = 360;

    //饱和度
    //int hsv_smin = 0;
    int hsv_smin_Max = 255;
    //int hsv_smax = 255;
    int hsv_smax_Max = 255;

    //亮度
    //int hsv_vmin = 106;
    int hsv_vmin_Max = 255;
    //int hsv_vmax = 250;
    int hsv_vmax_Max = 255;



    //掩码
    cv::Mat mask;
    cv::inRange(hsv_color, cv::Scalar(ui->horizontalSlider_hmin->value(),ui->horizontalSlider_smin->value()/ float(hsv_smin_Max),
                                      ui->horizontalSlider_vmin->value() / float(hsv_vmin_Max)),
                cv::Scalar(ui->horizontalSlider_hmax->value(), ui->horizontalSlider_smax->value()/ float(hsv_smax_Max),
                           ui->horizontalSlider_vmax ->value()/ float(hsv_vmax_Max)), mask);


    //输出图像
    cv::Mat dst_color;

    //输出图像分配内存
    dst_color = cv::Mat::zeros(img_color.size(), CV_32FC3);

    //只保留
    for (int r = 0; r < bgr_color.rows; r++){
        for (int c = 0; c < bgr_color.cols; c++){
            if (mask.at<uchar>(r, c) == 255){
                dst_color.at<cv::Vec3f>(r, c) = bgr_color.at<cv::Vec3f>(r, c);
            }
        }
    }

    cv::imwrite("colorfit.png",dst_color*255);
    dst_color = cv::imread("colorfit.png");

    ui->label_186->setText(ui->pushButton_gn18->text());
    cv::Mat imgShow;
    cvtColor(dst_color,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->label_tu_182->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_182->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}



//************************************************************************************目标检测***************************************************************************
//视频
void MainWindow::on_shiping_clicked(){
    page_idx=27;    //界面索引
    function_idx=40;//功能的索引

    ui->stackedWidget->setCurrentIndex(27);
    ui->pushButton_bofang->setText("播放");
    status_label->setText(QString(""));
}
//face_Haar
void MainWindow::on_face_haar_clicked(){
    page_idx=27;    //界面索引
    function_idx=41;//功能的索引

    ui->stackedWidget->setCurrentIndex(27);
    ui->pushButton_bofang->setText("检测");
    status_label->setText(QString(""));
}


//Camshift
void MainWindow::on_tool_Camshift_clicked(){
    page_idx=27;    //界面索引
    function_idx=42;//功能的索引

    ui->stackedWidget->setCurrentIndex(27);
    ui->pushButton_bofang->setText("Camshift");
    status_label->setText(QString(""));
}

//Meanshift
void MainWindow::on_tool_meanshift_clicked(){
    page_idx=27;    //界面索引
    function_idx=43;//功能的索引

    ui->stackedWidget->setCurrentIndex(27);
    ui->pushButton_bofang->setText("Meanshift");
    status_label->setText(QString(""));

}



void Delay(int msec){
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}


void MainWindow::on_pushButton_bofang_clicked(){

    if(ui->pushButton_bofang->text()=="播放"){
        AVFormatContext *pFormatCtx;
        int videoindex;
        AVCodecContext *pCodecCtx;
        AVCodec *pCodec;
        AVFrame *pFrame , *pFrameRGB;
        unsigned char *out_buffer;
        AVPacket *packet;

        int ret,got_picture;
        struct SwsContext *img_convert_ctx;

        char filepath[] = "C:\\Users\\31156\\Desktop\\Poff-AI\\1.mp4";
        // 初始化编解码库
        av_register_all(); // 创建AVFormatContext对象，与码流相关的结构
        pFormatCtx = avformat_alloc_context();
        // 初始化pFormatCtx结构
        if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0){
            qDebug() << "Couldn't open input stream. . " << endl;
            return;
        }
        // 获取音频视频流数据信息
        if(avformat_find_stream_info(pFormatCtx,NULL)<0){
            qDebug() << "Couldn't find stream information. " << endl;
            return;
        }
        videoindex = -1;
        // nb_streams视音频流的个数，这里当查找到视频流时就中断了。
        for(unsigned int i=0;i<pFormatCtx->nb_streams;i++){
            if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
                videoindex=i;
                break;
            }
        }
        if(videoindex==-1){
            qDebug() << "Didn't find a video stream. " << endl;
            return;
        }
        // 获取视频流编码结构
        pCodecCtx = pFormatCtx->streams[videoindex]->codec;
        // 查找解码器
        pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
        if(pCodec==NULL){
            qDebug() << "codec not found." << endl;
            return;
        }
        // 用于初始化pCodecCtx结构
        if(avcodec_open2(pCodecCtx,pCodec,NULL)<0){
            qDebug() << "Could not open codec. " << endl;
            return;
        }
        // 创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配 。
        pFrame = av_frame_alloc();
        pFrameRGB = av_frame_alloc();
        // 创建动态内存，创建存储图像数据的空间
        // av_image_get_buffer_size 获取一帧图像需要的大小
        out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height,1));
        av_image_fill_arrays(pFrameRGB->data,pFrameRGB->linesize,out_buffer,AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height, 1);
        packet = (AVPacket*)av_malloc(sizeof(AVPacket));
        // Output Info
        qDebug() << "------------ File Information -------------" << endl;
        // 此函数打印输入或输出的详细信息
        av_dump_format(pFormatCtx, 0, filepath, 0);
        qDebug() << "----------------" << endl;
        // 初始化img_convert_ctx结构
        img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,pCodecCtx->pix_fmt,
                                         pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
        // av_read_frame读取一帧未解码的数据
        while(av_read_frame(pFormatCtx,packet)>=0){
            // 如果是视频数据
            if (packet->stream_index==videoindex){
                // 解码一帧视频数据
                ret = avcodec_decode_video2(pCodecCtx,pFrame, &got_picture, packet);
                if(ret<0){
                    qDebug() << "Decode error" << endl;
                    return;
                }
                if(got_picture){
                    sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data,  pFrame->linesize, 0, pCodecCtx->height,pFrameRGB->data, pFrameRGB->linesize);
                    QImage img((uchar*)pFrameRGB->data[0],pCodecCtx->width,pCodecCtx->height, QImage::Format_RGB32);
                    ui->label_shiping->setPixmap(QPixmap::fromImage(img.scaled(ui->label_shiping->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
                    Delay(40);
                }
            }
            av_free_packet(packet);
        }
        sws_freeContext(img_convert_ctx);
        av_frame_free(&pFrameRGB);
        av_frame_free(&pFrame);
        avcodec_close(pCodecCtx);
        avformat_close_input(&pFormatCtx);
    }else if(ui->pushButton_bofang->text()=="检测"){
        std::string label = "Face";
        cv::CascadeClassifier faceCascade;
        faceCascade.load("models\\haarcascade_frontalface_alt2.xml");  //加载分类器
        if (faceCascade.empty()){
            qDebug()<< "load faceCascade failed" << endl;
            return;
        }

        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);// 打开摄像头
        //先设置采集格式
        pCapture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        //设置高清采集分辨率
        pCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
        pCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 576);

        if (!pCapture.isOpened()){
            qDebug()<< "open camera failed" << endl;
            return;
        }

        cv::Mat img, imgGray;
        std::vector<cv::Rect> faces;

        while (true){
            pCapture >> img;// 读取图像至img
            if (img.empty()){
                continue;
            }
            if (img.channels() == 3){
                cvtColor(img, imgGray, CV_RGB2GRAY);
            }else{
                imgGray = img;
            }

            //double start = cv::getTickCount();
            faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, cv::Size(0, 0));// 检测人脸
            //double end = cv::getTickCount();
            //_cprintf("run time: %f ms\n", (end - start));

            if (faces.size()>0){
                for (size_t i = 0; i<faces.size(); i++){
                    rectangle(img, cv::Point(faces[i].x, faces[i].y), cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), cv::Scalar(0, 255, 0), 1, 8);
                    putText(img, label, cv::Point(faces[i].x, faces[i].y -5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255,0));
                }
            }

            //显示
            cv::Mat imgShow;
            if(!img.empty()){
                cvtColor(img,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
                QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
                ui->label_shiping->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_shiping->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
            }
            if (cv::waitKey(1)==27){
                break;
            }
        }
        pCapture.release();
        cv::destroyAllWindows(); //关闭所有窗口
    }
    else if(ui->pushButton_bofang->text()=="Camshift"){
        cv::Mat pframe, roi, hsv_roi, mask;

        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);
        //先设置采集格式
        pCapture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        //设置高清采集分辨率
        pCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
        pCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 576);

        if (!pCapture.isOpened()){
            qDebug() << "Couldn't find stream information. " << endl;
            return;
        }

        pCapture >> pframe;

        cv::Rect track_window(500, 300, 100, 50);
        if(!ui->label_shiping->getRoiRect().isEmpty()){
           cv::Rect track_window2(ui->label_shiping->getRoiRect().x(), ui->label_shiping->getRoiRect().y(),
                                   ui->label_shiping->getRoiRect().width(),
                                   ui->label_shiping->getRoiRect().height());

           track_window=track_window2;
        }


        // set up the ROI for tracking
        roi = pframe(track_window);
        cvtColor(roi, hsv_roi, cv::COLOR_BGR2HSV);
        cv::inRange(hsv_roi, cv::Scalar(0, 60, 32), cv::Scalar(180, 255, 255), mask);

        float range_[] = {0, 180};
        const float* range[] = {range_};
        cv::Mat roi_hist;
        int histSize[] = {180};
        int channels[] = {0};
        calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
        normalize(roi_hist, roi_hist, 0, 255, cv::NORM_MINMAX);

        cv::TermCriteria term_crit(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1);

        while (true) {
                cv::Mat hsv, dst;
                pCapture >> pframe;
                if (pframe.empty())
                    break;
                cvtColor(pframe, hsv, cv::COLOR_BGR2HSV);
                calcBackProject(&hsv, 1, channels, roi_hist, dst, range);

                cv::RotatedRect rot_rect = CamShift(dst, track_window, term_crit);
                // Draw it on image
                cv::Point2f points[4];
                rot_rect.points(points);
                for (int i = 0; i < 4; i++)
                    line(pframe, points[i], points[(i + 1) % 4], 255, 2);

                //显示
                cv::Mat imgShow;
                if(!pframe.empty()){
                    cvtColor(pframe,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
                    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
                    //ui->label_shiping->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_shiping->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
                    ui->label_shiping->setBackImage(qImg);
                }

                if (cv::waitKey(1)==27){
                    break;
                }
        }
        pCapture.release();
        cv::destroyAllWindows(); //关闭所有窗口
    }

    else if(ui->pushButton_bofang->text()=="Meanshift"){
        cv::Mat pframe, roi, hsv_roi, mask;

        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);
        //先设置采集格式
        pCapture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        //设置高清采集分辨率
        pCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
        pCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 576);

        if (!pCapture.isOpened()){
            qDebug() << "Couldn't find stream information. " << endl;
            return;
        }

        pCapture >> pframe;


        cv::Rect track_window(500, 300, 100, 50);
        if(!ui->label_shiping->getRoiRect().isEmpty()){
           cv::Rect track_window2(ui->label_shiping->getRoiRect().x(), ui->label_shiping->getRoiRect().y(),
                                   ui->label_shiping->getRoiRect().width(),
                                   ui->label_shiping->getRoiRect().height());

           track_window=track_window2;
        }

        roi = pframe(track_window);
        cvtColor(roi, hsv_roi, cv::COLOR_BGR2HSV);
        cv::inRange(hsv_roi, cv::Scalar(0, 60, 32), cv::Scalar(180, 255, 255), mask);

        float range_[] = {0, 180};
        const float* range[] = {range_};
        cv::Mat roi_hist;
        int histSize[] = {180};
        int channels[] = {0};
        calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
        normalize(roi_hist, roi_hist, 0, 255, cv::NORM_MINMAX);

        cv::TermCriteria term_crit(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1);

        while (true) {
                cv::Mat hsv, dst;
                pCapture >> pframe;
                if (pframe.empty())
                    break;
                cvtColor(pframe, hsv, cv::COLOR_BGR2HSV);
                calcBackProject(&hsv, 1, channels, roi_hist, dst, range);

                // apply meanshift to get the new location
                meanShift(dst, track_window, term_crit);

                // Draw it on image
                rectangle(pframe, track_window, 255, 2);

                //显示
                cv::Mat imgShow;
                if(!pframe.empty()){
                    cvtColor(pframe,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
                    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
                    //ui->label_shiping->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_shiping->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
                    ui->label_shiping->setBackImage(qImg);
                }

                if (cv::waitKey(1)==27){
                    break;
                }
        }
        pCapture.release();
        cv::destroyAllWindows(); //关闭所有窗口
    }
}

void MainWindow::on_pushButton_pause_clicked(){
    pCapture.release();
}


//帧间差分
void MainWindow::on_frame_diff_clicked(){
    page_idx=28;    //界面索引
    function_idx=44;//功能的索引

    ui->stackedWidget->setCurrentIndex(28);
    ui->pushButton_gn30->setText("帧间差分");
    status_label->setText(QString(""));
}
//高斯混合背景
void MainWindow::on_mix_guass_clicked(){
    page_idx=28;    //界面索引
    function_idx=45;//功能的索引

    ui->stackedWidget->setCurrentIndex(28);
    ui->pushButton_gn30->setText("高斯混合背景");
    status_label->setText(QString(""));
}

//隐身效果
void MainWindow::on_cloaking_clicked(){
    page_idx=28;    //界面索引
    function_idx=46;//功能的索引

    ui->stackedWidget->setCurrentIndex(28);
    ui->pushButton_gn30->setText("隐身效果");
    status_label->setText(QString(""));
}


void MainWindow::on_pushButton_gn30_clicked(){
    if(ui->pushButton_gn30->text()=="帧间差分"){
        cv::Mat pFrame1,pFrame2, pFrame3;  //当前帧
        int nFrmNum;
        cv::Mat pframe;
        //pCapture.open("C:\\Users\\31156\\Desktop\\Poff-AI\\1.mp4");

        //打开摄像机设备
        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);
        //先设置采集格式
        pCapture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        //设置高清采集分辨率
        pCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
        pCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);


        if (!pCapture.isOpened()){
            qDebug() << "Couldn't find stream information. " << endl;
            return;
        }
        pCapture >> pframe;

        cv::Mat pFrImg1,pFrImg2,pFrImg3;   //当前帧
        pFrImg1.create(pframe.size(), CV_8UC1);
        pFrImg2.create(pframe.size(), CV_8UC1);
        pFrImg3.create(pframe.size(), CV_8UC1);

        cv::Mat pFrMat1, pFrMat2, pFrMat3;

        nFrmNum = 0;
        while (1){
            nFrmNum++;

                pCapture >> pFrame1;
                if (pFrame1.data == NULL)
                    return;
                pCapture >> pFrame2;
                pCapture >> pFrame3;

                cvtColor(pFrame1, pFrImg1, CV_BGR2GRAY);
                cvtColor(pFrame2, pFrImg2, CV_BGR2GRAY);
                cvtColor(pFrame3, pFrImg3, CV_BGR2GRAY);

                absdiff(pFrImg1, pFrImg2, pFrMat1);//当前帧跟前面帧相减
                absdiff(pFrImg2, pFrImg3, pFrMat2);//当前帧与后面帧相减

                //二值化前景图
                threshold(pFrMat1, pFrMat1, 10, 255.0, CV_THRESH_BINARY);
                threshold(pFrMat2, pFrMat2, 10, 255.0, CV_THRESH_BINARY);

                cv::Mat element = getStructuringElement(0, cv::Size(3, 3));
                cv::Mat element1 = getStructuringElement(0, cv::Size(5, 5));
                //膨胀化前景图
                erode(pFrMat1, pFrMat1, element);
                erode(pFrMat2, pFrMat2, element);

                dilate(pFrMat1, pFrMat1, element1);
                dilate(pFrMat2, pFrMat2, element1);

                dilate(pFrMat1, pFrMat1, element1);
                dilate(pFrMat2, pFrMat2, element1);

                // imshow("diff1", pFrMat1);
                // imshow("diff2", pFrMat2);

                //显示
                cv::Mat imgShow;
                cvtColor(pFrMat2,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
                QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
                ui->label_tu_301->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_301->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));

                std::vector<std::vector<cv::Point>> contours;
                std::vector<cv::Vec4i> hierarchy;

                //当前帧与前面帧相减后提取的轮廓线
                findContours(pFrMat2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
                double Maxarea = 0;
                size_t numi=0;
                for (size_t i = 0; i < contours.size(); ++i){
                    double area = contourArea(contours[i], false);
                    if (area > Maxarea){
                        Maxarea = area;
                        numi = i;
                    }
                }
                if (numi!=0){
                    //绘制轮廓
                    drawContours(pFrame2, contours, numi, cv::Scalar(0, 0, 255), 2);
                }

                cv::Mat resultImage = cv::Mat::zeros(pFrMat2.size(), CV_8U);

                // imshow("src", pFrame2);

                //显示
                cv::Mat imgShow2;
                cvtColor(pFrame2,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
                QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
                ui->label_tu_302->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_302->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));

                cv::waitKey(10);
                if (cv::waitKey(25)>0) //按下任意键退出摄像头
                        break;
        }
        pCapture.release();
        cv::destroyAllWindows(); //关闭所有窗口
    }else if(ui->pushButton_gn30->text()=="高斯混合背景"){
        cv::Mat greyimg;
        cv::Mat foreground, foreground2;
        cv::Ptr<cv::BackgroundSubtractorKNN> ptrKNN = cv::createBackgroundSubtractorKNN(100, 400, true);
        cv::Ptr<cv::BackgroundSubtractorMOG2> mog2 = cv::createBackgroundSubtractorMOG2(100, 25, true);

        cv::Mat pframe;

        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);
        if (!pCapture.isOpened()){
            qDebug() << "Couldn't find stream information. " << endl;
            return;
        }

        while (1){
            pCapture >> pframe;
            if (pframe.data == NULL)
                return;
            cvtColor(pframe, greyimg, CV_BGR2GRAY);
            long long t = cv::getTickCount();
            ptrKNN->apply(pframe, foreground, 0.01);
            long long t1 = cv::getTickCount();
            mog2->apply(greyimg, foreground2, -1);
            long long t2 = cv::getTickCount();
            //_cprintf("t1 = %f t2 = %f\n", (t1 - t) / getTickFrequency(), (t2 - t1) / getTickFrequency());
            std::cout<<"t1 = "<<(t1 - t) / cv::getTickFrequency()<<"t2 = "<<(t2 - t1) / cv::getTickFrequency()<< std::endl;

            //imshow("Extracted Foreground", foreground);
            //imshow("Extracted Foreground2", foreground2);
            //imshow("video", pframe);

            //显示
            cv::Mat imgShow;
            cvtColor(foreground,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
            QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
            ui->label_tu_301->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_301->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
            //显示
            cv::Mat imgShow2;
            cvtColor(pframe,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
            QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
            ui->label_tu_302->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_302->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));

            cv::waitKey(10);
            if (cv::waitKey(25)>0) //按下任意键退出摄像头
                    break;
        }
        cv::destroyAllWindows();
    }else if(ui->pushButton_gn30->text()=="隐身效果"){

        //cap.open("C:\\Users\\31156\\Desktop\\Poff-AI\\Input-color0.mp4");
        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);

        if (!pCapture.isOpened()) {
            std::cout << "Error opening video stream or file" << std::endl;
            return;
        }

        cv::Mat background, background1;
        for (int i = 0; i<60; i++){
            pCapture >> background;
        }

        //flip(background,background,1);

        for (int i = 0; i<100; i++){
            pCapture >> background1;
        }
        while (1){
            long t = cv::getTickCount();

            cv::Mat frame;
            pCapture >> frame;
            if (frame.empty())
                break;

            cv::Mat hsv;
            //flip(frame,frame,1);
            cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

            cv::Mat mask1, mask2;
            cv::inRange(hsv, cv::Scalar(0, 120, 70), cv::Scalar(10, 255, 255), mask1);   //H为0-10的分量
            cv::inRange(hsv, cv::Scalar(170, 120, 70), cv::Scalar(180, 255, 255), mask2);//H为170-180的分量


            mask1 = mask1 + mask2;

            cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F);
            morphologyEx(mask1, mask1, cv::MORPH_OPEN, kernel);//开
            morphologyEx(mask1, mask1, cv::MORPH_DILATE, kernel);//膨胀

            bitwise_not(mask1, mask2);

            cv::Mat res1, res2, final_output;
            bitwise_and(frame, frame, res1, mask2);//not red
            //imshow("res1 !!!", res1);
            bitwise_and(background, background, res2, mask1);//red

            //long t1 = getTickCount();
            //imshow("res2 !!!",res2);
            //addWeighted(res1, 1, res2, 1, 0, final_output);
            add(res1, res2, final_output);

            //imshow("Magic !!!", final_output);
            //imshow( "Frame", frame );  // Display the resulting frame

            //显示
            cv::Mat imgShow;
            cvtColor(frame,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
            QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
            ui->label_tu_301->setPixmap(QPixmap::fromImage(qImg.scaled(ui->label_tu_301->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
            //显示
            cv::Mat imgShow2;
            cvtColor(final_output,imgShow2, cv::COLOR_BGR2RGB);//BGR convert to RGB
            QImage qImg2 = QImage((unsigned char*)(imgShow2.data), imgShow2.cols,imgShow2.rows, imgShow2.cols*imgShow2.channels(), QImage::Format_RGB888);
            ui->label_tu_302->setPixmap(QPixmap::fromImage(qImg2.scaled(ui->label_tu_302->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation)));


            // Press  ESC on keyboard to exit
            char c = (char)cv::waitKey(1);
            if (c == 27)
                break;
            // Also relese all the mat created in the code to avoid memory leakage.
            frame.release(), hsv.release(), mask1.release(), mask2.release(), res1.release(), res2.release(), final_output.release();

            long t1 = cv::getTickCount();

            std::cout << "t1 =  " << (t1 - t) / cv::getTickFrequency()*1000 << "ms\n"<< std::endl;

            if (cv::waitKey(1)!=-1)
                    break;
        }
        pCapture.release();
    }
}

//暂停
void MainWindow::on_pushButton_gn30_pause_clicked(){
    pCapture.release();
}




//*******************************************************************************************************************************************************************
void calRealPoint(std::vector<std::vector<cv::Point3f>>& obj, int boardWidth, int boardHeight, int imgNumber, int squareSize){
    std::vector<cv::Point3f> imgpoint;
    for (int rowIndex = 0; rowIndex < boardHeight; rowIndex++){
        for (int colIndex = 0; colIndex < boardWidth; colIndex++){
            imgpoint.push_back(cv::Point3f(rowIndex * squareSize, colIndex * squareSize, 0));
        }
    }
    for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++){
        obj.push_back(imgpoint);
    }
}

cv::Mat R, T, E, F;
cv::Mat Rl, Rr, Pl, Pr, Q;

//映射表
cv::Mat mapLx, mapLy, mapRx, mapRy;
cv::Mat cameraMatrixL = (cv::Mat_<double>(3, 3) << 530.1397548683084, 0, 338.2680507680664,0, 530.2291152852337, 232.4902023212199,0, 0, 1);

//获得的畸变参数
cv::Mat distCoeffL = (cv::Mat_<double>(5, 1) << -0.266294943795012, -0.0450330886310585, 0.0003024821418382528, -0.001243865371699451, 0.2973605735168139);
cv::Mat cameraMatrixR = (cv::Mat_<double>(3, 3) << 530.1397548683084, 0, 338.2680507680664,0, 530.2291152852337, 232.4902023212199,0, 0, 1);
cv::Mat distCoeffR = (cv::Mat_<double>(5, 1) << -0.266294943795012, -0.0450330886310585, 0.0003024821418382528, -0.001243865371699451, 0.2973605735168139);


void MainWindow::on_camera2_clicked(){
    //摄像头的分辨率
    const int imageWidth = 640;
    const int imageHeight = 480;
    //横向的角点数目
    const int boardWidth = 9;
    //纵向的角点数目
    const int boardHeight = 6;
    //总的角点数目
    //const int boardCorner = boardWidth * boardHeight;
    //相机标定时需要采用的图像帧数
    const int frameNumber = 14;
    //标定板黑白格子的大小 单位是mm
    const int squareSize = 10;
    //标定板的总内角点
    const cv::Size boardSize = cv::Size(boardWidth, boardHeight);
    cv::Size imageSize = cv::Size(imageWidth, imageHeight);


    //R旋转矢量 T平移矢量 E本征矩阵 F基础矩阵
    std::vector<cv::Mat> rvecs; //R
    std::vector<cv::Mat> tvecs; //T

    //左边摄像机所有照片角点的坐标集合
    std::vector<std::vector<cv::Point2f>> imagePointL;
    //右边摄像机所有照片角点的坐标集合
    std::vector<std::vector<cv::Point2f>> imagePointR;
    //各图像的角点的实际的物理坐标集合
    std::vector<std::vector<cv::Point3f>> objRealPoint;
    //左边摄像机某一照片角点坐标集合
    std::vector<cv::Point2f> cornerL;
    //右边摄像机某一照片角点坐标集合
    std::vector<cv::Point2f> cornerR;

    cv::Mat rgbImageL, grayImageL;
    cv::Mat rgbImageR, grayImageR;
    cv::Mat intrinsic;
    cv::Mat distortion_coeff;
    //校正旋转矩阵R，投影矩阵P，重投影矩阵Q
    //映射表
    cv::Mat mapLx, mapLy, mapRx, mapRy;
    cv::Rect validROIL, validROIR;
    //图像校正之后，会对图像进行裁剪，其中，validROI裁剪之后的区域

    cv::Mat img;
    int goodFrameCount = 1;
    while (goodFrameCount <= frameNumber){
        char filename[100];
        /*读取左边的图像*/
        sprintf(filename, "C:/Users/31156/Desktop/Poff-AI/IADS/camer_cab/left%02d.jpg", goodFrameCount);
        rgbImageL = cv::imread(filename, 1);
        imshow("chessboardL", rgbImageL);
        cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);

        /*读取右边的图像*/
        sprintf(filename, "C:/Users/31156/Desktop/Poff-AI/IADS/camer_cab/right%02d.jpg", goodFrameCount);
        rgbImageR = cv::imread(filename, 1);
        cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);

        bool isFindL, isFindR;
        isFindL = findChessboardCorners(rgbImageL, boardSize, cornerL);
        isFindR = findChessboardCorners(rgbImageR, boardSize, cornerR);
        if (isFindL == true && isFindR == true){
            cornerSubPix(grayImageL, cornerL, cv::Size(5, 5), cv::Size(-1, 1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
            drawChessboardCorners(rgbImageL, boardSize, cornerL, isFindL);
            imshow("chessboardL", rgbImageL);
            imagePointL.push_back(cornerL);

            cornerSubPix(grayImageR, cornerR, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
            drawChessboardCorners(rgbImageR, boardSize, cornerR, isFindR);
            imshow("chessboardR", rgbImageR);
            imagePointR.push_back(cornerR);
            //_cprintf("the image %d is good\n",goodFrameCount);
            goodFrameCount++;
        }else{
            std::cout<<"the image is bad please try again\n";
        }

        if (cv::waitKey(10) == 'q'){
            break;
        }
    }

    //计算实际的校正点的三维坐标，根据实际标定格子的大小来设置
    calRealPoint(objRealPoint, boardWidth, boardHeight, frameNumber, squareSize);

    //标定摄像头
    double rms = stereoCalibrate(objRealPoint, imagePointL, imagePointR,cameraMatrixL, distCoeffL,cameraMatrixR, distCoeffR,cv::Size(imageWidth, imageHeight), R, T, E, F, cv::CALIB_USE_INTRINSIC_GUESS,cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-5));
    std::cout<<"Stereo Calibration done with RMS error ="<<rms;

    stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl,Rr, Pl, Pr, Q, cv::CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);

    //摄像机校正映射
    initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pl, imageSize, CV_32FC1, mapLx, mapLy);
    initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);

    cv::Mat rectifyImageL, rectifyImageR;
    cvtColor(grayImageL, rectifyImageL, CV_GRAY2BGR);
    cvtColor(grayImageR, rectifyImageR, CV_GRAY2BGR);

    imshow("RecitifyL Before", rectifyImageL);
    imshow("RecitifyR Before", rectifyImageR);

    //经过remap之后，左右相机的图像已经共面并且行对准了
    cv::Mat rectifyImageL2, rectifyImageR2;
    remap(rectifyImageL, rectifyImageL2, mapLx, mapLy, cv::INTER_LINEAR);
    remap(rectifyImageR, rectifyImageR2, mapRx, mapRy, cv::INTER_LINEAR);


    imshow("rectifyImageL", rectifyImageL2);
    imshow("rectifyImageR", rectifyImageR2);

    /*保存数据*/
    /*输出数据*/
    cv::FileStorage fs("intrisics.yml", cv::FileStorage::WRITE);
    if (fs.isOpened()){
        fs << "cameraMatrixL" << cameraMatrixL << "cameraDistcoeffL" << distCoeffL << "cameraMatrixR" << cameraMatrixR << "cameraDistcoeffR" << distCoeffR;
        fs.release();
        std::cout << "cameraMatrixL=:" << cameraMatrixL << std::endl << "cameraDistcoeffL=:" << distCoeffL << std::endl << "cameraMatrixR=:" << cameraMatrixR << std::endl << "cameraDistcoeffR=:" << distCoeffR << std::endl;
    }else{
        cout << "Error: can not save the intrinsics!!!!" << std::endl;
    }

    fs.open("extrinsics.yml", cv::FileStorage::WRITE);
    if (fs.isOpened()){
        fs << "R" << R << "T" << T << "Rl" << Rl << "Rr" << Rr << "Pl" << Pl << "Pr" << Pr << "Q" << Q;
        std::cout << "R=" << R << std::endl << "T=" << T << std::endl << "Rl=" << Rl << std::endl << "Rr" << Rr << std::endl << "Pl" << Pl << std::endl << "Pr" << Pr << std::endl << "Q" << Q << std::endl;
        fs.release();
    }else{
        std::cout << "Error: can not save the extrinsic parameters\n";
    }



    //显示校正结果
    cv::Mat canvas;
    double sf;
    int w, h;
    sf = 600. / MAX(imageSize.width, imageSize.height);
    w = cvRound(imageSize.width * sf);
    h = cvRound(imageSize.height * sf);
    canvas.create(h, w * 2, CV_8UC3);

    //左图像画到画布上
    cv::Mat canvasPart = canvas(cv::Rect(0, 0, w, h));
    cv::resize(rectifyImageL2, canvasPart, canvasPart.size(), 0, 0, cv::INTER_AREA);
    cv::Rect vroiL(cvRound(validROIL.x*sf), cvRound(validROIL.y*sf),cvRound(validROIL.width*sf), cvRound(validROIL.height*sf));
    cv::rectangle(canvasPart, vroiL, cv::Scalar(0, 0, 255), 3, 8);

    //右图像画到画布上
    canvasPart = canvas(cv::Rect(w, 0, w, h));
    cv::resize(rectifyImageR2, canvasPart, canvasPart.size(), 0, 0,cv::INTER_LINEAR);
    cv::Rect vroiR(cvRound(validROIR.x*sf), cvRound(validROIR.y*sf),cvRound(validROIR.width*sf), cvRound(validROIR.height*sf));
    cv::rectangle(canvasPart, vroiR, cv::Scalar(0, 255, 0), 3, 8);


    //画上对应的线条
    for (int i = 0; i < canvas.rows; i += 16){
        line(canvas, cv::Point(0, i), cv::Point(canvas.cols, i), cv::Scalar(0, 255, 0), 1, 8);
    }

    imshow("rectified", canvas);
    cv::waitKey(0);

    cv::destroyAllWindows();
    cv::waitKey(1);
}



int getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor){
    cv::Mat disp8u;
    disp8u = disparity;
    // 转换为伪彩色图像 或 灰度图像
    if (isColor){
        if (disparityImage.empty() || disparityImage.type() != CV_8UC3 || disparityImage.size() != disparity.size()){
            disparityImage = cv::Mat::zeros(disparity.rows, disparity.cols, CV_8UC3);
        }
        for (int y = 0; y<disparity.rows; y++){
            for (int x = 0; x<disparity.cols; x++){
                uchar val = disp8u.at<uchar>(y, x);
                uchar r, g, b;

                if (val == 0){
                    r = g = b = 0;
                }else{
                    r = 255 - val;
                    g = val < 128 ? val * 2 : (uchar)((255 - val) * 2);
                    b = val;
                }
                disparityImage.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
        }
    }else{
        disp8u.copyTo(disparityImage);
    }
    return 1;
}






const int imageWidth = 1920;                             //摄像头的分辨率
const int imageHeight = 1080;
cv::Size imageSize = cv::Size(imageWidth, imageHeight);

cv::Mat rgbImageL, grayImageL;
cv::Mat rgbImageR, grayImageR;
cv::Mat rectifyImageL, rectifyImageR;

cv::Rect validROIL;//图像校正之后，会对图像进行裁剪，这里的validROI就是指裁剪之后的区域
cv::Rect validROIR;

cv::Mat xyz;              //三维坐标
int blockSize = 0, uniquenessRatio = 0, numDisparities = 0;
cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create(16, 9);

cv::Mat T_new = (cv::Mat_<double>(3, 1) << -3.3269653179960471e+01, 3.7375231026230421e-01,-1.2058042444883227e-02);//T平移向量
//Mat rec = (Mat_<double>(3, 1) << -0.00306, -0.03207, 0.00206);//rec旋转向量
cv::Mat R_new = (cv::Mat_<double>(3, 3) << 9.9998505024526163e-01, 3.5253250461816949e-03,
    4.1798767087380161e-03, -3.4957471578341281e-03,
    9.9996894942320580e-01, -7.0625732745616225e-03,
    -4.2046447876106169e-03, 7.0478558986986593e-03,
    9.9996632377767658e-01);//R 旋转矩阵



/*****立体匹配*****/
void stereo_match(int, void*){


    bm->setBlockSize(2 * blockSize + 5);     //SAD窗口大小，5~21之间为宜
    bm->setROI1(validROIL);
    bm->setROI2(validROIR);
    bm->setPreFilterCap(31);
    bm->setMinDisparity(0);  //最小视差，默认值为0, 可以是负值，int型
    bm->setNumDisparities(numDisparities * 16 + 16);//视差窗口，即最大视差值与最小视差值之差,窗口大小必须是16的整数倍，int型
    bm->setTextureThreshold(10);
    bm->setUniquenessRatio(uniquenessRatio);//uniquenessRatio主要可以防止误匹配
    bm->setSpeckleWindowSize(100);
    bm->setSpeckleRange(32);
    bm->setDisp12MaxDiff(-1);
    cv::Mat disp, disp8, disparityImage;
    bm->compute(rectifyImageL, rectifyImageR, disp);//输入图像必须为灰度图
    disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16)*16.));//计算出的视差是CV_16S格式
    reprojectImageTo3D(disp, xyz, Q, true); //在实际求距离时，ReprojectTo3D出来的X / W, Y / W, Z / W都要乘以16(也就是W除以16)，才能得到正确的三维坐标信息。
    xyz = xyz * 16;
    getDisparityImage(disp8, disparityImage, true);
    imshow("disparity", disparityImage);
}

//立体匹配
void MainWindow::on_camera2_2_clicked(){
    // TODO: 在此添加控件通知处理程序代码


    //立体校正
    stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R_new, T_new, Rl, Rr, Pl, Pr, Q, cv::CALIB_ZERO_DISPARITY,0, imageSize, &validROIL, &validROIR);
    initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pr, imageSize, CV_32FC1, mapLx, mapLy);
    initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);

    rgbImageL = cv::imread("C:/Users/31156/Desktop/Poff-AI/IADS/camer_cab/left01.jpg", CV_LOAD_IMAGE_COLOR);
    cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);
    rgbImageR = cv::imread("C:/Users/31156/Desktop/Poff-AI/IADS/camer_cab/right01.jpg", CV_LOAD_IMAGE_COLOR);
    cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);

    imshow("ImageL Before Rectify", grayImageL);
    imshow("ImageR Before Rectify", grayImageR);

    /*
    经过remap之后，左右相机的图像已经共面并且行对准了
    */
    remap(grayImageL, rectifyImageL, mapLx, mapLy, cv::INTER_LINEAR);
    remap(grayImageR, rectifyImageR, mapRx, mapRy, cv::INTER_LINEAR);

    /*
    把校正结果显示出来
    */
    cv::Mat rgbRectifyImageL, rgbRectifyImageR;
    cvtColor(rectifyImageL, rgbRectifyImageL, CV_GRAY2BGR);  //伪彩色图
    cvtColor(rectifyImageR, rgbRectifyImageR, CV_GRAY2BGR);
    //单独显示
    //rectangle(rgbRectifyImageL, validROIL, Scalar(0, 0, 255), 3, 8);
    //rectangle(rgbRectifyImageR, validROIR, Scalar(0, 0, 255), 3, 8);
    imshow("ImageL After Rectify", rgbRectifyImageL);
    imshow("ImageR After Rectify", rgbRectifyImageR);

    //显示在同一张图上
    cv::Mat canvas;
    double sf;
    int w, h;
    sf = 600. / MAX(imageSize.width, imageSize.height);
    w = cvRound(imageSize.width * sf);
    h = cvRound(imageSize.height * sf);
    canvas.create(h, w * 2, CV_8UC3);   //注意通道

                                        //左图像画到画布上
    cv::Mat canvasPart = canvas(cv::Rect(w * 0, 0, w, h));                                //得到画布的一部分
    cv::resize(rgbRectifyImageL, canvasPart, canvasPart.size(), 0, 0, cv::INTER_AREA);     //把图像缩放到跟canvasPart一样大小
    cv::Rect vroiL(cvRound(validROIL.x*sf), cvRound(validROIL.y*sf),                //获得被截取的区域
        cvRound(validROIL.width*sf), cvRound(validROIL.height*sf));
    //rectangle(canvasPart, vroiL, Scalar(0, 0, 255), 3, 8);                      //画上一个矩形
	std::cout << "Painted ImageL" << std::endl;

    //右图像画到画布上
    canvasPart = canvas(cv::Rect(w, 0, w, h));                                      //获得画布的另一部分
    cv::resize(rgbRectifyImageR, canvasPart, canvasPart.size(), 0, 0, cv::INTER_LINEAR);
    cv::Rect vroiR(cvRound(validROIR.x * sf), cvRound(validROIR.y*sf),
        cvRound(validROIR.width * sf), cvRound(validROIR.height * sf));
    //rectangle(canvasPart, vroiR, Scalar(0, 0, 255), 3, 8);
	std::cout << "Painted ImageR" << std::endl;

    //画上对应的线条
    for (int i = 0; i < canvas.rows; i += 16)
        line(canvas, cv::Point(0, i), cv::Point(canvas.cols, i), cv::Scalar(0, 255, 0), 1, 8);
    imshow("rectified", canvas);

    /*
    立体匹配
    */
    cv::namedWindow("disparity", CV_WINDOW_AUTOSIZE);
    // 创建SAD窗口 Trackbar
    cv::createTrackbar("BlockSize:\n", "disparity", &blockSize, 8, stereo_match);
    // 创建视差唯一性百分比窗口 Trackbar
    cv::createTrackbar("UniquenessRatio:\n", "disparity", &uniquenessRatio, 50, stereo_match);
    // 创建视差窗口 Trackbar
    cv::createTrackbar("NumDisparities:\n", "disparity", &numDisparities, 16, stereo_match);
    stereo_match(0, 0);

    cv::waitKey(0);

    cv::destroyAllWindows();
    cv::waitKey(1);

}




//*****************************************************************************************************************三维重建**************************************************************************************
//MPR浏览
void MainWindow::on_tool_MPR_clicked(){
    page_idx=30;    //界面索引
    function_idx=48;//功能的索引

    ui->stackedWidget->setCurrentIndex(30);
    status_label->setText(QString(""));
}

//体绘制
void MainWindow::on_tool_tihuizhi_clicked(){
    page_idx=31;    //界面索引
    function_idx=49;//功能的索引

    ui->pb_sanweihuizhi->setText("Volume Rendering");
    ui->stackedWidget->setCurrentIndex(31);
    status_label->setText(QString(""));

}

//面绘制
void MainWindow::on_tool_mianhuizhi_clicked(){
    page_idx=31;    //界面索引
    function_idx=50;//功能的索引

    ui->pb_sanweihuizhi->setText("Surface Rendering");
    ui->stackedWidget->setCurrentIndex(31);
    status_label->setText(QString(""));

}

//切平面
void MainWindow::on_tool_cut_clicked(){
    page_idx=31;    //界面索引
    function_idx=51;//功能的索引

    ui->pb_sanweihuizhi->setText("Tangent Plane");
    ui->stackedWidget->setCurrentIndex(31);
    status_label->setText(QString(""));
}

//挑选
void MainWindow::on_tool_tiaoxuan_clicked()
{
    page_idx=31;    //界面索引
    function_idx=52;//功能的索引

    ui->pb_sanweihuizhi->setText("Slice selection");
    ui->stackedWidget->setCurrentIndex(31);
    status_label->setText(QString(""));
}

//ROI
void MainWindow::on_tool_ROI_clicked(){
    page_idx=31;    //界面索引
    function_idx=53;//功能的索引

    ui->pb_sanweihuizhi->setText("ROI");
    ui->stackedWidget->setCurrentIndex(31);


    status_label->setText(QString("按键盘R可以进行矩形区域裁剪,再按R返回正常状态"));
    //ui->statusbar->showMessage(QString("按键盘R可以进行矩形区域裁剪,再按R返回正常状态"));
}
//ROI_BOX
void MainWindow::on_tool_ROI_BOX_clicked(){
    page_idx=31;    //界面索引
    function_idx=54;//功能的索引

    ui->pb_sanweihuizhi->setText("ROI_BOX");
    ui->stackedWidget->setCurrentIndex(31);
    status_label->setText(QString(""));
}




void MainWindow::on_pushButton_clicked(){
    // 点击 "选择Dicom文件夹"按钮后，执行该函数
    QSettings s; // 为了记住上一次的路径
    QString dirStr = s.value("OPEN_FILEPATH", ".").toString();// 不存在的话为当前应用程序路径
    dirStr = QFileDialog::getExistingDirectory(this, QString("选择Dicom序列所在的文件夹"), dirStr);

    if (dirStr.isEmpty())
        return;
    s.setValue("OPEN_FILEPATH", dirStr); // 记住该路径，以备下次使用

    if (isOpeing){
        QMessageBox::warning(this, "TIP", QString("正在检查数据，请等待完成打开其他数据！"));
        return;
    }


    isOpeing = true;
    ReadWorker *worker = new ReadWorker(dirStr);
    QThread *thread = new QThread();

    connect(thread, SIGNAL(started()), worker, SLOT(checkDicomsFileDir())); // 线程开始后执行worker->checkDicomsFileDir()
    connect(worker, SIGNAL(progress(int)), this, SLOT(setProgressBarValue(int)));// worker 发送信号，执行this->setProgressBarValue
    connect(worker, SIGNAL(finish()), this, SLOT(ReadCheckCompleted()));// 检查完成，执行this->ReadCheckCompleted()
    connect(worker, SIGNAL(finish()), worker, SLOT(deleteLater()));// 执行完成，析构worker
    connect(worker, SIGNAL(destroyed(QObject*)), thread, SLOT(quit()));// 析构worker 完成， 推出线程
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); // 推出线程， 析构线程
    worker->moveToThread(thread); // 把worker 移动到线程
    thread->start(); // 开始线程


    //读取二维切片数据序列
    vtkSmartPointer<vtkDICOMImageReader>reader =vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDataByteOrderToLittleEndian();
    reader->SetDirectoryName(dirStr.toStdString().c_str());//设置读取路径
    reader->SetDataSpacing(1.0, 1.0, 1.0);//设置每个体素的大小
    reader->Update();


    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volumeMapper->SetInputData(reader->GetOutput());;
    volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance()/2);	//设置光线采样距离
    //volumeMapper->SetAutoAdjustSampleDistances(0);//设置图像采样步长
    //volumeMapper->SetImageSampleDistance(4);


    vtkSmartPointer<vtkVolumeProperty> volumeProperty =vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();  //打开或者关闭阴影测试
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);  //漫反射
    volumeProperty->SetSpecular(0.2); //镜面反射

    //设置不透明度
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(70, 0.00);
    compositeOpacity->AddPoint(90, 0.40);
    compositeOpacity->AddPoint(180, 0.60);

    //compositeOpacity->AddPoint(-1000, 0.00);
    //compositeOpacity->AddPoint(100, 0.00);
    //compositeOpacity->AddPoint(300, 0.879);
    //compositeOpacity->AddPoint(478, 0.879);
    //compositeOpacity->AddPoint(1500, 0.901);
    volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数

    //设置梯度不透明属性
    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeGradientOpacity->AddPoint(10, 0.0);
    volumeGradientOpacity->AddPoint(90, 0.5);
    volumeGradientOpacity->AddPoint(100, 1.0);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比
    //设置颜色属性
    vtkSmartPointer<vtkColorTransferFunction> color =vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(-1000, 0.00, 0.00, 0.00);
    color->AddRGBPoint(100, 220/255., 21/255., 3/255.);
    color->AddRGBPoint(300, 250/255., 1.00, 189/255.);
    color->AddRGBPoint(478, 224/255., 1, 249/255.);
    color->AddRGBPoint(1500, 1, 1, 1);

    volumeProperty->SetColor(color);


    /********************************************************************/
    vtkSmartPointer<vtkVolume> volume =vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);


    //定义绘制器
    vtkSmartPointer<vtkRenderer> aRenderer =vtkSmartPointer<vtkRenderer>::New();
    aRenderer->SetBackground(0, 0, 0);
    aRenderer->AddVolume(volume);

    //创建一个camera
    vtkSmartPointer< vtkCamera > aCamera =vtkSmartPointer< vtkCamera >::New();
    aCamera->SetViewUp(0, 0, -1);
    aCamera->SetPosition(0, 1, 0);
    aCamera->SetFocalPoint(0, 0, 0);

    aRenderer->AddActor(volume);
    aRenderer->SetActiveCamera(aCamera);
    aRenderer->ResetCamera();
    aCamera->Dolly(1.5);
    aRenderer->SetBackground(0, 0, 0);
    aRenderer->ResetCameraClippingRange();


    ui->openGLWidget_Dicom->GetRenderWindow()->AddRenderer((aRenderer));
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(ui->openGLWidget_Dicom->GetRenderWindow());
    ui->openGLWidget_Dicom->show();
    ui->openGLWidget_Dicom->GetRenderWindow()->Render();
    renderWindowInteractor->Initialize();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
    style = vtkInteractorStyleTrackballCamera::New();
    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->Start();

    if (renderWindowInteractor)
        renderWindowInteractor->ExitCallback();

}



void MainWindow::ReadCheckCompleted(){
    ui->comboBox_which->clear();
     // worker->checkDicomsFileDir()线程执行完成后，执行该函数
    // 序列的个数
    int seriesCount = ReadWorker::UID_Files.count();

    ui->textBrowser->append(QString("共有%1个序列: ").arg(seriesCount));

    foreach(QString uid, ReadWorker::UID_Files.keys()){
        ui->comboBox_which->addItem(uid);
        ui->textBrowser->append(QString("序列%1: 共%2张图像").arg(uid).arg(ReadWorker::UID_Files[uid].count()));
    }

    isOpeing = false;

    on_comboBox_which_currentIndexChanged(ui->comboBox_which->currentText());
}


void MainWindow::on_comboBox_which_currentIndexChanged(QString seriesUID){
    if (isOpeing)
         return;
    isOpeing = true;
    SeriesBase::readSeriesUid(seriesUID); // 读取序列
    isOpeing = false;
    if (!SeriesBase::isNormal())
        return;

     ui->textBrowser->clear();
     ui->textBrowser->append(QString("共有%1个序列>>").arg(ReadWorker::UID_Files.count()));
     ui->textBrowser->append(QString("当前序列(%1):").arg(seriesUID));

     int w, h, s;
     SeriesBase::getSeriesSize(w, h, s);
     ui->textBrowser->append(QString("宽：%1").arg(w));
     ui->textBrowser->append(QString("高：%1").arg(h));
     ui->textBrowser->append(QString("图像数：%1").arg(s));
     xyS->update(); xzS->update(); yzS->update();
     ui->graphicsView_XY->setSeries(xyS);

     ui->graphicsView_YZ->setSeries(yzS);
     ui->graphicsView_XZ->setSeries(xzS);
}

void MainWindow::on_comboBox_2_currentIndexChanged(int color){
    // 伪彩下拉菜单选择彩色后，设置序列彩色，并更新视图
    SeriesBase::setColor(color);
    ui->graphicsView_XY->updateView();
    ui->graphicsView_YZ->updateView();
    ui->graphicsView_XZ->updateView();
}

void MainWindow::on_lineEdit_ww_textEdited(QString ww){
    // 窗宽编辑框数字编辑完成后，设置序列窗宽位，并更新视图
        double ww_d = 0.0, wl = 0.0;
        SeriesBase::getWindow(wl, ww_d);
        ww_d = ww.toDouble();
        SeriesBase::setWindow(wl, ww_d);
        ui->graphicsView_XY->updateView();
        ui->graphicsView_YZ->updateView();
        ui->graphicsView_XZ->updateView();
}

void MainWindow::on_lineEdit_wl_textChanged(QString wl){
    // 窗位编辑框数字编辑完成后，设置序列窗宽位，并更新视图
        double ww = 0.0, wl_d = 0.0;
        SeriesBase::getWindow(wl_d, ww);
        wl_d = wl.toDouble();
        SeriesBase::setWindow(wl_d, ww);
        ui->graphicsView_XY->updateView();
        ui->graphicsView_YZ->updateView();
        ui->graphicsView_XZ->updateView();
}

void MainWindow::on_windowChanged(){
    // 更新窗宽、窗位的编辑框数字
    double ww = 0.0, wl = 0.0;
    SeriesBase::getWindow(wl, ww);
    set_lineEdit_wl_text(wl);
    set_lineEdit_ww_text(ww);

    ui->graphicsView_XY->updateView();
    ui->graphicsView_YZ->updateView();
    ui->graphicsView_XZ->updateView();
}


void MainWindow::set_lineEdit_ww_text(double winWidth){
    ui->lineEdit_ww->setText(QString::number(winWidth));
}

void MainWindow::set_lineEdit_wl_text(double winCenter){
    ui->lineEdit_wl->setText(QString::number(winCenter));
}

void MainWindow::setProgressBarValue(int progress){
    ui->progressBar->setValue(progress);
}

void MainWindow::setPixelInfo(QString pixelInfo){
    ui->textBrowser->setText(pixelInfo);
}


void  MainWindow::on_clickedView(SeriesBase::ViewType vt){
    switch (vt){
    case SeriesBase::XY:
        ui->graphicsView_XY->setHighlight(true);
        ui->graphicsView_XZ->setHighlight(false);
        ui->graphicsView_YZ->setHighlight(false);
        break;
    case SeriesBase::XZ:
        ui->graphicsView_XY->setHighlight(false);
        ui->graphicsView_XZ->setHighlight(true);
        ui->graphicsView_YZ->setHighlight(false);
        break;
    case SeriesBase::YZ:
        ui->graphicsView_XY->setHighlight(false);
        ui->graphicsView_XZ->setHighlight(false);
        ui->graphicsView_YZ->setHighlight(true);
        break;
    default:
        break;
    }
}





class vtkWidgetCall : public vtkCommand{
public:

    static vtkWidgetCall *New(){
        return new vtkWidgetCall;
    }
public:

    vtkWidgetCall(){
         clippingPlane = vtkSmartPointer<vtkPlane>::New();

    }

    virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData){
        vtkPlaneWidget *pWidget = vtkPlaneWidget::SafeDownCast(caller);
        pWidget->GetPlane(clippingPlane);
        m_pMapper->AddClippingPlane(clippingPlane);
        m_pMapper->Update();

    }
public:
    vtkSmartPointer<vtkPlane> clippingPlane;
    vtkRenderWindow * m_pRenderWindow;
    vtkDataSetMapper* m_pMapper;
};




//vtkClipClosedSurface
namespace {
    // Define interaction style
    class HighlightInteractorStyle : public vtkInteractorStyleRubberBandPick{
    public:
        static HighlightInteractorStyle* New();
        vtkTypeMacro(HighlightInteractorStyle, vtkInteractorStyleRubberBandPick);

        HighlightInteractorStyle() : vtkInteractorStyleRubberBandPick(){
            this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
            this->SelectedActor = vtkSmartPointer<vtkActor>::New();
            this->SelectedActor->SetMapper(SelectedMapper);
        }

        virtual void OnLeftButtonUp() override{
            // Forward events
            vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

            if (this->CurrentMode == VTKISRBP_SELECT){
                vtkNew<vtkNamedColors> colors;

                vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

                vtkNew<vtkPlaneCollection> planeCollection;
                int pCount = frustum->GetNumberOfPlanes() - 2;
                for (int i = 0; i < pCount; i++){
                    double normal[3];
                    frustum->GetPlane(i)->GetNormal(normal);
                    double origin[3];
                    frustum->GetPlane(i)->GetOrigin(origin);

                    vtkNew<vtkPlane> plane;
                    plane->SetOrigin(origin[0], origin[1], origin[2]);
                    plane->SetNormal(-normal[0], -normal[1], -normal[2]);
                    planeCollection->AddItem(plane);
                }

                vtkNew<vtkClipClosedSurface> clipper;
                clipper->SetInputData(this->PolyData);
                clipper->SetClippingPlanes(planeCollection);
                clipper->Update();

                if (m_SrcActor != nullptr)
                    m_SrcActor->VisibilityOff();

                this->SelectedMapper->SetInputData(clipper->GetOutput());

                this->SelectedActor->GetProperty()->SetColor(1.0000, 1, 1);
                this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(SelectedActor);
                this->GetInteractor()->GetRenderWindow()->Render();
                this->HighlightProp(NULL);
            }
        }

        void SetPolyData(vtkSmartPointer<vtkPolyData> polyData){
            this->PolyData = polyData;
        }
        vtkActor* m_SrcActor = nullptr;
    private:
        vtkSmartPointer<vtkPolyData> PolyData;
        vtkSmartPointer<vtkActor> SelectedActor;
        vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
    };
    vtkStandardNewMacro(HighlightInteractorStyle);

} // namespace



class vtkBoxCallback : public vtkCommand{
public:
    static vtkBoxCallback* New(){
        return new vtkBoxCallback;
    }

    void SetMapper(vtkSmartPointer<vtkPolyDataMapper> mapper){
        this->Mapper = mapper;
    }
    virtual void Execute(vtkObject *caller, unsigned long, void*) override{
        vtkSmartPointer<vtkBoxWidget> boxWidget =reinterpret_cast<vtkBoxWidget*>(caller);
        // 创建一个vtkPlanes对象
        vtkSmartPointer<vtkPlanes> planes = vtkSmartPointer<vtkPlanes>::New();

        // 将框的平面添加到vtkPlanes对象中
        boxWidget->GetPlanes(planes);

        // 使用裁剪框来提取框内的区域
        vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
        clipper->SetInputData(Mapper->GetInput());
        clipper->SetClipFunction(planes);
        clipper->InsideOutOn();     // 关闭 InsideOut，以保留框内的区域
        clipper->GenerateClipScalarsOff();
        clipper->GenerateClippedOutputOn();
        clipper->Update();

        Mapper->SetInputData(clipper->GetOutput());
    }

private:
    vtkSmartPointer<vtkPolyDataMapper> Mapper;
};




void MainWindow::on_pb_sanweihuizhi_clicked(){
    std::string filename = "out_Smooth.stl";//设置输出文件路径

    QDir dir(dirchoose);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
         QFileInfo fileInfo = list.at(i);
         QString file_suffix = fileInfo.suffix(); //只含最后一个后缀
         if(file_suffix != "dcm"){
             dirchoose ="C:\\Users\\31156\\Desktop\\Poff-AI\\tuxiang\\dicom_path\\all\\guzhe\\001";
             break;
         }
         std::cout << qPrintable(QString("%1").arg(fileInfo.fileName()));
         std::cout << std::endl;
    }


    //读取二维切片数据序列
    vtkSmartPointer<vtkDICOMImageReader>reader =vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDataByteOrderToLittleEndian();
    reader->SetDirectoryName(dirchoose.toStdString().c_str());//设置读取路径
    reader->SetDataSpacing(1.0, 1.0, 1.0);//设置每个体素的大小
    reader->Update();

    if(ui->pb_sanweihuizhi->text()=="Surface Rendering"){

         //抽取等值面为骨头的信息
         //MC算法
         vtkSmartPointer< vtkMarchingCubes > boneExtractor =vtkSmartPointer< vtkMarchingCubes >::New();
         //boneExtractor->SetInputConnection(reader->GetOutputPort());
         boneExtractor->SetInputData(reader->GetOutput());
         boneExtractor->ComputeNormalsOn();
         boneExtractor->SetValue(0, ui->spinBox_surfacethreshold->value()); //设置提取的等值信息
         boneExtractor->Update();


         //利用ContourFilter提取等值面
         /*vtkSmartPointer< vtkContourFilter > boneExtractor =vtkSmartPointer< vtkContourFilter >::New();
         boneExtractor->SetInputConnection(reader->GetOutputPort());
         boneExtractor->SetValue(0, 200); //设置提取的等值信息
         boneExtractor->Update();*/


         //DC算法   耗时长，模型有明显缝隙
         /*vtkSmartPointer< vtkRecursiveDividingCubes > boneExtractor =vtkSmartPointer< vtkRecursiveDividingCubes >::New();
         boneExtractor->SetInputConnection(reader->GetOutputPort());
         boneExtractor->SetValue(500);
         boneExtractor->SetDistance(1);
         boneExtractor->SetIncrement(2);
         boneExtractor->Update();*/


         //剔除旧的或废除的数据单元，提高绘制速度(可略去这一步)
         vtkSmartPointer< vtkStripper > boneStripper =vtkSmartPointer< vtkStripper >::New(); //三角带连接
         boneStripper->SetInputConnection(boneExtractor->GetOutputPort());
         boneStripper->Update();


         //平滑滤波
         vtkSmartPointer<vtkSmoothPolyDataFilter> pSmoothPolyDataFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
         pSmoothPolyDataFilter->SetInputConnection(boneStripper->GetOutputPort());
         pSmoothPolyDataFilter->SetRelaxationFactor(0.05);
         vtkSmartPointer<vtkPolyDataNormals> pPolyDataNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
         pPolyDataNormals->SetInputConnection(pSmoothPolyDataFilter->GetOutputPort());



         //将模型输出到入STL文件
         /*
         vtkSmartPointer<vtkSTLWriter> stlWriter =vtkSmartPointer<vtkSTLWriter>::New();
         stlWriter->SetFileName(filename.c_str());
         stlWriter->SetInputConnection(pPolyDataNormals->GetOutputPort());
         stlWriter->Write();
         */


         //建立映射
         vtkSmartPointer< vtkPolyDataMapper > boneMapper =vtkSmartPointer< vtkPolyDataMapper >::New();
         boneMapper->SetInputData(boneExtractor->GetOutput());
         boneMapper->ScalarVisibilityOff();
         //建立角色
         vtkSmartPointer<vtkActor> bone =vtkSmartPointer< vtkActor >::New();
         bone->SetMapper(boneMapper);
         bone->GetProperty()->SetDiffuseColor(1.0, 1.0, 1.0);
         bone->GetProperty()->SetSpecular(.3);
         bone->GetProperty()->SetSpecularPower(20);

         //定义绘制器
         vtkSmartPointer<vtkRenderer> aRenderer =vtkSmartPointer<vtkRenderer>::New();
         aRenderer->AddActor(bone);
         aRenderer->SetBackground(0, 0, 0);


         //定义绘制窗口
         //vtkSmartPointer< vtkRenderWindow > renWin =vtkSmartPointer< vtkRenderWindow >::New();
         //renWin->AddRenderer(aRenderer);

         //定义窗口交互器
         //vtkSmartPointer< vtkRenderWindowInteractor > iren =vtkSmartPointer< vtkRenderWindowInteractor >::New();
         //iren->SetRenderWindow(renWin);

         //创建一个camera
         vtkSmartPointer< vtkCamera > aCamera =vtkSmartPointer< vtkCamera >::New();
         aCamera->SetViewUp(0, 0, -1);
         aCamera->SetPosition(0, 1, 0);
         aCamera->SetFocalPoint(0, 0, 0);
         aRenderer->SetActiveCamera(aCamera);
         aRenderer->ResetCamera();
         aCamera->Dolly(1.5);
         aRenderer->ResetCameraClippingRange();


         //将3D模型渲染到绘制窗口
         //iren->Initialize();
         //iren->Start();

         ui->openGLWidget->GetRenderWindow()->AddRenderer((aRenderer));


         vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
         renderWindowInteractor->SetRenderWindow(ui->openGLWidget->GetRenderWindow());
         ui->openGLWidget->show();


         vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
         style = vtkInteractorStyleTrackballCamera::New();
         renderWindowInteractor->SetInteractorStyle(style);

         ui->openGLWidget->GetRenderWindow()->Render();
         renderWindowInteractor->Initialize();
         renderWindowInteractor->Start();

         if (renderWindowInteractor)
             renderWindowInteractor->ExitCallback();

     }else if(ui->pb_sanweihuizhi->text()=="Volume Rendering"){
         vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
         volumeMapper->SetInputData(reader->GetOutput());;
         volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance()/2);	//设置光线采样距离
         //volumeMapper->SetAutoAdjustSampleDistances(0);//设置图像采样步长
         //volumeMapper->SetImageSampleDistance(4);


         vtkSmartPointer<vtkVolumeProperty> volumeProperty =vtkSmartPointer<vtkVolumeProperty>::New();
         volumeProperty->SetInterpolationTypeToLinear();
         volumeProperty->ShadeOn();  //打开或者关闭阴影测试
         volumeProperty->SetAmbient(0.4);
         volumeProperty->SetDiffuse(0.6);  //漫反射
         volumeProperty->SetSpecular(0.2); //镜面反射

         //设置不透明度
         vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();
         compositeOpacity->AddPoint(70, 0.00);
         compositeOpacity->AddPoint(90, 0.40);
         compositeOpacity->AddPoint(180, 0.60);
         //compositeOpacity->AddPoint(-1000, 0.00);
         //compositeOpacity->AddPoint(100, 0.00);
         //compositeOpacity->AddPoint(300, 0.879);
         //compositeOpacity->AddPoint(478, 0.879);
         //compositeOpacity->AddPoint(1500, 0.901);
         volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数

         //设置梯度不透明属性
         vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();
         volumeGradientOpacity->AddPoint(10, 0.0);
         volumeGradientOpacity->AddPoint(90, 0.5);
         volumeGradientOpacity->AddPoint(100, 1.0);
         volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比
         //设置颜色属性
         vtkSmartPointer<vtkColorTransferFunction> color =vtkSmartPointer<vtkColorTransferFunction>::New();
         color->AddRGBPoint(-1000, 0.00, 0.00, 0.00);
         color->AddRGBPoint(100, 220/255., 21/255., 3/255.);
         color->AddRGBPoint(300, 250/255., 1.00, 189/255.);
         color->AddRGBPoint(478, 224/255., 1, 249/255.);
         color->AddRGBPoint(1500, 1, 1, 1);
         volumeProperty->SetColor(color);


         /*************************实例化体绘制Actor*****************/
         vtkSmartPointer<vtkVolume> volume =vtkSmartPointer<vtkVolume>::New();
         volume->SetMapper(volumeMapper);
         volume->SetProperty(volumeProperty);


         //定义绘制器
         vtkSmartPointer<vtkRenderer> aRenderer =vtkSmartPointer<vtkRenderer>::New();
         aRenderer->SetBackground(0, 0, 0);
         aRenderer->AddVolume(volume);

         //创建一个camera
         vtkSmartPointer< vtkCamera > aCamera =vtkSmartPointer< vtkCamera >::New();
         aCamera->SetViewUp(0, 0, -1);
         aCamera->SetPosition(0, 1, 0);
         aCamera->SetFocalPoint(0, 0, 0);
         aRenderer->AddActor(volume);
         aRenderer->SetActiveCamera(aCamera);
         aRenderer->ResetCamera();
         aCamera->Dolly(1.5);
         aRenderer->SetBackground(0, 0, 0);
         aRenderer->ResetCameraClippingRange();

         vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
         ui->openGLWidget->GetRenderWindow()->AddRenderer((aRenderer));
         renderWindowInteractor->SetRenderWindow(ui->openGLWidget->GetRenderWindow());

         ui->openGLWidget->show();
         ui->openGLWidget->GetRenderWindow()->Render();
         renderWindowInteractor->Initialize();

         vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
         style = vtkInteractorStyleTrackballCamera::New();

         renderWindowInteractor->SetInteractorStyle(style);
         renderWindowInteractor->Start();

         if (renderWindowInteractor)
             renderWindowInteractor->ExitCallback();
     }else if(ui->pb_sanweihuizhi->text()=="Tangent Plane"){
        vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
        volumeMapper->SetInputData(reader->GetOutput());;
        volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance()/2);	//设置光线采样距离

        vtkSmartPointer<vtkVolumeProperty> volumeProperty =vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->SetInterpolationTypeToLinear();
        volumeProperty->ShadeOn();  //打开或者关闭阴影测试
        volumeProperty->SetAmbient(0.4);
        volumeProperty->SetDiffuse(0.6);  //漫反射
        volumeProperty->SetSpecular(0.2); //镜面反射

        //设置不透明度
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(70, 0.00);
        compositeOpacity->AddPoint(90, 0.40);
        compositeOpacity->AddPoint(180, 0.60);
        volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数

        //设置梯度不透明属性
        vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();
        volumeGradientOpacity->AddPoint(10, 0.0);
        volumeGradientOpacity->AddPoint(90, 0.5);
        volumeGradientOpacity->AddPoint(100, 1.0);
        volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比
        //设置颜色属性
        vtkSmartPointer<vtkColorTransferFunction> color =vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(-1000, 0.00, 0.00, 0.00);
        color->AddRGBPoint(100, 220/255., 21/255., 3/255.);
        color->AddRGBPoint(300, 250/255., 1.00, 189/255.);
        color->AddRGBPoint(478, 224/255., 1, 249/255.);
        color->AddRGBPoint(1500, 1, 1, 1);
        volumeProperty->SetColor(color);


        /*************************实例化体绘制Actor*****************/
        vtkSmartPointer<vtkVolume> volume =vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(volumeMapper);
        volume->SetProperty(volumeProperty);


        //定义绘制器
        vtkSmartPointer<vtkRenderer> aRenderer =vtkSmartPointer<vtkRenderer>::New();
        aRenderer->SetBackground(0, 0, 0);
        aRenderer->AddVolume(volume);
        ui->openGLWidget->GetRenderWindow()->AddRenderer((aRenderer));

        vtkSmartPointer<vtkRenderer> renderer2 =vtkSmartPointer<vtkRenderer>::New();
        aRenderer->SetViewport(0, 0, 0.5, 1);//设置渲染窗口位置
        renderer2->SetViewport(0.5, 0, 1, 1);
        ui->openGLWidget->GetRenderWindow()->AddRenderer((renderer2));
        vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
        renderWindowInteractor->SetRenderWindow(ui->openGLWidget->GetRenderWindow());


        //创建一个camera
        vtkSmartPointer< vtkCamera > aCamera =vtkSmartPointer< vtkCamera >::New();
        aCamera->SetViewUp(0, 0, -1);
        aCamera->SetPosition(0, 1, 0);
        aCamera->SetFocalPoint(0, 0, 0);
        aRenderer->AddActor(volume);
        aRenderer->SetActiveCamera(aCamera);
        aRenderer->ResetCamera();
        aCamera->Dolly(1.5);
        aRenderer->SetBackground(0, 0, 0);
        aRenderer->ResetCameraClippingRange();

        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
        style = vtkInteractorStyleTrackballCamera::New();
        renderWindowInteractor->SetInteractorStyle(style);



        //=====================ImagePlaneWidget==========================
        //实例化ImagePlaneWidget
        vtkSmartPointer<vtkImagePlaneWidget> pwidget =vtkSmartPointer<vtkImagePlaneWidget>::New();
        pwidget->SetInteractor(renderWindowInteractor);

        pwidget->SetInputConnection(reader->GetOutputPort());
        pwidget->RestrictPlaneToVolumeOn();//将widget限制在volume中
        pwidget->GetPlaneProperty()->SetColor(0.9, 0.4, 0.4);
        pwidget->TextureInterpolateOn();
        pwidget->SetDefaultRenderer(aRenderer);
        pwidget->DisplayTextOn();//显示鼠标选中的坐标、灰度值的文本信息
        pwidget->SetResliceInterpolateToLinear();//切面线性插值
        pwidget->SetPlaneOrientationToZAxes();//设置widget朝向为Z轴
        pwidget->SetSliceIndex(14);
        pwidget->On();
        pwidget->InteractionOn();


        //============================切面渲染==============================
        vtkSmartPointer<vtkPlaneSource> slicePlane =vtkSmartPointer<vtkPlaneSource>::New();
        vtkSmartPointer<vtkPolyDataMapper> planeMapper =vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> planeActor =vtkSmartPointer<vtkActor>::New();
        planeMapper->SetInputConnection(slicePlane->GetOutputPort());
        planeActor->SetMapper(planeMapper);
        planeActor->SetTexture(pwidget->GetTexture());
        renderer2->AddActor(planeActor);


        //===============
        ui->openGLWidget->show();
        ui->openGLWidget->GetRenderWindow()->Render();
        renderWindowInteractor->Initialize();
        renderWindowInteractor->Start();

        if (renderWindowInteractor)
            renderWindowInteractor->ExitCallback();
    }else if(ui->pb_sanweihuizhi->text()=="Slice selection"){

        vtkRenderer *aRender = vtkRenderer::New();

        vtkRenderWindowInteractor *renderWindowInteractor = vtkRenderWindowInteractor::New();
        ui->openGLWidget->GetRenderWindow()->AddRenderer(aRender);

        renderWindowInteractor->SetRenderWindow(ui->openGLWidget->GetRenderWindow());


        vtkSmartPointer<vtkMarchingCubes> boneExtractor =vtkSmartPointer<vtkMarchingCubes>::New();
        boneExtractor->SetInputData(reader->GetOutput());
        boneExtractor->SetValue(0, ui->spinBox_surfacethreshold->value());



        //建立映射
        vtkSmartPointer<vtkDataSetMapper> boneMapper =vtkSmartPointer<vtkDataSetMapper>::New();
        boneMapper->SetInputConnection(boneExtractor->GetOutputPort());
        boneMapper->ScalarVisibilityOff();


        //建立角色
        vtkSmartPointer<vtkActor> bone =vtkSmartPointer<vtkActor>::New();
        bone->SetMapper(boneMapper);
        bone->GetProperty()->SetDiffuseColor(1.0, 1.0, 1.0);
        bone->GetProperty()->SetSpecular(.3);
        bone->GetProperty()->SetSpecularPower(20);


        vtkSmartPointer<vtkProperty> backProp =vtkSmartPointer<vtkProperty>::New();
        backProp->SetDiffuseColor(0.8900, 0.8100, 0.3400);
        bone->SetBackfaceProperty(backProp);

        //创建数据的初始视图很方便。的FocalPoint而位置构成一个矢量方向。稍后(ResetCamera()方法)
        //这个矢量是用来定位摄像机来查看数据的

        //创建一个camera
        vtkSmartPointer<vtkCamera> aCamera =vtkSmartPointer<vtkCamera>::New();
        aCamera->SetViewUp(0, 0, -1);
        aCamera->SetPosition(0, 1, 0);
        aCamera->SetFocalPoint(0, 0, 0);
        aCamera->ComputeViewPlaneNormal();
        aCamera->Azimuth(30.0);
        aCamera->Elevation(30.0);
        aRender->AddActor(bone);
        aRender->SetActiveCamera(aCamera);
        aRender->ResetCamera();
        aCamera->Dolly(1.5);


        aRender->SetBackground(0, 0, 0);
        aRender->ResetCameraClippingRange();

        aRender->ResetCamera();
        ui->openGLWidget->GetRenderWindow()->Modified();
        ui->openGLWidget->GetRenderWindow()->Render();


        /*Clipping Plane Widget*/
        vtkSmartPointer<vtkPlaneWidget> m_pPlaneWidget = vtkSmartPointer<vtkPlaneWidget>::New();
        m_pPlaneWidget->SetInteractor(renderWindowInteractor);//与交互器关联
        m_pPlaneWidget->SetInputData(reader->GetOutput());//设置数据集，用于初始化平面，可以不设置
        m_pPlaneWidget->SetResolution(50);//即：设置网格数
        m_pPlaneWidget->GetPlaneProperty()->SetColor(.2, .8, 0.1);//设置颜色
        m_pPlaneWidget->GetPlaneProperty()->SetOpacity(0.5);//设置透明度
        m_pPlaneWidget->GetHandleProperty()->SetColor(0, .4, .7);//设置平面顶点颜色
        m_pPlaneWidget->GetHandleProperty()->SetLineWidth(1.5);//设置平面线宽
        m_pPlaneWidget->NormalToZAxisOn();//初始法线方向平行于Z轴
        m_pPlaneWidget->SetRepresentationToWireframe();//平面显示为网格属性
        m_pPlaneWidget->SetCenter(bone->GetCenter());//设置平面坐标
        m_pPlaneWidget->SetPlaceFactor(1.0);
        m_pPlaneWidget->PlaceWidget();//放置平面
        m_pPlaneWidget->On();//显示平面

        vtkWidgetCall *pCall = vtkWidgetCall::New();
        pCall->m_pMapper = boneMapper;
        pCall->m_pRenderWindow = ui->openGLWidget->GetRenderWindow();
        m_pPlaneWidget->AddObserver(vtkCommand::EndInteractionEvent, pCall);


        renderWindowInteractor->Initialize();

        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
        style = vtkInteractorStyleTrackballCamera::New();
        renderWindowInteractor->SetInteractorStyle(style);
        renderWindowInteractor->Start();

        if (renderWindowInteractor)
            renderWindowInteractor->ExitCallback();

    }else if(ui->pb_sanweihuizhi->text()=="ROI"){
        //MC算法
        vtkSmartPointer< vtkMarchingCubes > boneExtractor =vtkSmartPointer< vtkMarchingCubes >::New();
        boneExtractor->SetInputConnection(reader->GetOutputPort());
        boneExtractor->SetValue(0, ui->spinBox_surfacethreshold->value()); //设置提取的等值信息
        boneExtractor->Update();

        //建立映射
        vtkSmartPointer< vtkPolyDataMapper > boneMapper =vtkSmartPointer< vtkPolyDataMapper >::New();
        boneMapper->SetInputData(boneExtractor->GetOutput());
        boneMapper->ScalarVisibilityOff();
        //建立角色
        vtkSmartPointer<vtkActor> bone =vtkSmartPointer< vtkActor >::New();
        bone->SetMapper(boneMapper);
        bone->GetProperty()->SetDiffuseColor(1.0, 1.0, 1.0);
        bone->GetProperty()->SetSpecular(.3);
        bone->GetProperty()->SetSpecularPower(20);

        //定义绘制器
        vtkSmartPointer<vtkRenderer> aRenderer =vtkSmartPointer<vtkRenderer>::New();
        aRenderer ->UseHiddenLineRemovalOn();


        //创建一个camera
        vtkSmartPointer< vtkCamera > aCamera =vtkSmartPointer< vtkCamera >::New();
        aCamera->SetViewUp(0, 0, -1);
        aCamera->SetPosition(0, 1, 0);
        aCamera->SetFocalPoint(0, 0, 0);
        aRenderer->AddActor(bone);
        aRenderer->SetActiveCamera(aCamera);
        aRenderer->ResetCamera();
        aCamera->Dolly(1.5);
        aRenderer->SetBackground(0, 0, 0);
        aRenderer->ResetCameraClippingRange();

        ui->openGLWidget->GetRenderWindow()->AddRenderer((aRenderer));

        vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
        vtkNew<vtkAreaPicker> areaPicker;
        renderWindowInteractor->SetPicker(areaPicker);
        renderWindowInteractor->SetRenderWindow(ui->openGLWidget->GetRenderWindow());

        renderWindowInteractor->Initialize();
        vtkNew<HighlightInteractorStyle> style;
        style->m_SrcActor = bone;
        style->SetPolyData(boneExtractor->GetOutput());
        style->SetCurrentRenderer(aRenderer);

        renderWindowInteractor->SetInteractorStyle(style);
        ui->openGLWidget->show();
        ui->openGLWidget->GetRenderWindow()->Render();
        renderWindowInteractor->Start();

        if (renderWindowInteractor)
            renderWindowInteractor->ExitCallback();

    }else if(ui->pb_sanweihuizhi->text()=="ROI_BOX"){
        //MC算法
        vtkSmartPointer< vtkMarchingCubes > boneExtractor =vtkSmartPointer< vtkMarchingCubes >::New();
        boneExtractor->SetInputConnection(reader->GetOutputPort());
        boneExtractor->SetValue(0, ui->spinBox_surfacethreshold->value()); //设置提取的等值信息
        boneExtractor->Update();

        vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
        clipper->SetInputConnection(boneExtractor->GetOutputPort());

        //建立映射
        vtkSmartPointer< vtkPolyDataMapper > boneMapper =vtkSmartPointer< vtkPolyDataMapper >::New();
        boneMapper->SetInputData(boneExtractor->GetOutput());
        boneMapper->ScalarVisibilityOff();

        //建立角色
        vtkSmartPointer<vtkActor> bone =vtkSmartPointer< vtkActor >::New();
        bone->SetMapper(boneMapper);
        bone->GetProperty()->SetDiffuseColor(1.0, 1.0, 1.0);
        bone->GetProperty()->SetSpecular(.3);
        bone->GetProperty()->SetSpecularPower(20);

        //定义绘制器
        vtkSmartPointer<vtkRenderer> aRenderer =vtkSmartPointer<vtkRenderer>::New();
        aRenderer ->UseHiddenLineRemovalOn();

        //创建一个camera
        vtkSmartPointer< vtkCamera > aCamera =vtkSmartPointer< vtkCamera >::New();
        aCamera->SetViewUp(0, 0, -1);
        aCamera->SetPosition(0, 1, 0);
        aCamera->SetFocalPoint(0, 0, 0);
        aRenderer->AddActor(bone);
        aRenderer->SetActiveCamera(aCamera);
        aRenderer->ResetCamera();
        aCamera->Dolly(1.5);
        aRenderer->SetBackground(0, 0, 0);
        aRenderer->ResetCameraClippingRange();

        vtkNew<vtkInteractorStyleTrackballCamera> style;
        ui->openGLWidget->GetRenderWindow()->AddRenderer((aRenderer));

        vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
        renderWindowInteractor->SetInteractorStyle(style);
        renderWindowInteractor->SetRenderWindow(ui->openGLWidget->GetRenderWindow());

        // 创建包围盒并设置其位置和大小
        vtkSmartPointer<vtkBoxWidget> boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
        boxWidget->SetProp3D(bone);
        boxWidget->SetInteractor(renderWindowInteractor);
        boxWidget->SetPlaceFactor(1); // 根据需要调整
        boxWidget->PlaceWidget();
        boxWidget->On();// 开启交互


        vtkSmartPointer<vtkBoxCallback> boxCallback = vtkSmartPointer<vtkBoxCallback>::New();
        boxCallback->SetMapper(boneMapper);
        boxWidget->AddObserver(vtkCommand::InteractionEvent, boxCallback);

        renderWindowInteractor->Initialize();
        ui->openGLWidget->show();
        ui->openGLWidget->GetRenderWindow()->Render();
        renderWindowInteractor->Start();

        if (renderWindowInteractor)
            renderWindowInteractor->ExitCallback();

    }

}


//********************************************************模型部署********************************************************************************
//将mat过来的数据U8类型的转为Blob,因为Blob是任意类型的因此也声明为模板
template <typename T> void matU8ToBlob(const cv::Mat& orig_image, InferenceEngine::Blob::Ptr& blob, int batchIndex = 0){
    InferenceEngine::SizeVector blobSize = blob->getTensorDesc().getDims();
    const size_t width = blobSize[3];
    const size_t height = blobSize[2];
    const size_t channels = blobSize[1];
    InferenceEngine::MemoryBlob::Ptr mblob = InferenceEngine::as<InferenceEngine::MemoryBlob>(blob);
    if (!mblob) {
        THROW_IE_EXCEPTION << "We expect blob to be inherited from MemoryBlob in matU8ToBlob,"<<"but by fact we were not able to cast inputBlob to MemoryBlob";
    }

    // locked memory holder should be alive all time while access to its buffer happens
    auto mblobHolder = mblob->wmap();

    T* blob_data = mblobHolder.as<T*>();

    cv::Mat resized_image(orig_image);
    if (static_cast<int>(width) != orig_image.size().width ||static_cast<int>(height) != orig_image.size().height) {
        cv::resize(orig_image, resized_image, cv::Size(width, height));
    }

    auto batchOffset = batchIndex * width * height * channels;

    for (size_t c = 0; c < channels; c++) {
        for (size_t h = 0; h < height; h++) {
            for (size_t w = 0; w < width; w++) {
                blob_data[batchOffset + c * width * height + h * width + w] =resized_image.at<cv::Vec3b>(h, w)[c];
            }
        }
    }
}

//模型输入推理设置函数封装,因为要修改infer_request,所有还是需要传智能指针,将输入的src转换为对应格式设置到infer_request的Blob中
void frameToBlob(cv::Mat& src, std::shared_ptr<InferenceEngine::InferRequest>& infer_request, std::string& input_name){
    auto input = infer_request->GetBlob(input_name); //通过传入的推理引擎,获取input的Blob(进行输入的格式设置的类对象)
    matU8ToBlob<uchar>(src, input);
    return;
}


//分类
void MainWindow::on_tool_classification_clicked(){
    page_idx=35;    //界面索引
    function_idx=55;//功能的索引


    ui->stackedWidget->setCurrentIndex(35);
    ui->pb_gnAI->setText("classification");
    status_label->setText(QString(""));

    //设置默认的模型、参数和label
    xmlFilename = "models/resnet18.xml";
    binFilename = "models/resnet18.bin";
    labelFilename= "models/imagenet_classes.txt";

    ui->xml_lineEdit->setText("resnet18.xml");
    ui->bin_lineEdit->setText("resnet18.bin");
    ui->label_textEdit->setText("imagenet_classes.txt");


    //显示
    cv::Mat imgShow;
    cvtColor(srcImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_45->SetImage(qImg);
//    QGraphicsPixmapItem* graphpixmap = new QGraphicsPixmapItem();
//    graphpixmap->setPixmap(QPixmap::fromImage(qImg));

//    // 将QGraphicsPixmapItem类添加到QGraphicsScene
//    graphscene->addItem(graphpixmap);  // 对象元素-->场景
//    // 将QGraphicsScene添加到QGraphicsView
//    ui->graphicsView_45->setScene(graphscene);  // 场景-->视图
//    // 设置QGraphicsView更新
//    ui->graphicsView_45->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}


//语义分割
void MainWindow::on_tool_semanticseg_clicked(){
    page_idx=35;    //界面索引
    function_idx=56;//功能的索引

    ui->stackedWidget->setCurrentIndex(35);
    ui->pb_gnAI->setText("SemanticSeg");
    status_label->setText(QString(""));

    //设置默认的模型、参数和label
    xmlFilename = "models/road-segmentation-adas-0001.xml";
    binFilename = "models/road-segmentation-adas-0001.bin";

    ui->xml_lineEdit->setText("road-segmentation-adas-0001.xml");
    ui->bin_lineEdit->setText("road-segmentation-adas-0001.bin");
    ui->label_textEdit->setText(" ");


    //显示
    cv::Mat imgShow;
    cvtColor(srcImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_45->SetImage(qImg);
}

//实例分割
void MainWindow::on_tool_instanceseg_clicked(){
    page_idx=35;    //界面索引
    function_idx=57;//功能的索引

    ui->stackedWidget->setCurrentIndex(35);
    ui->pb_gnAI->setText("InstanceSeg");
    status_label->setText(QString(""));



    //设置默认的模型、参数和label
    xmlFilename = "models/instance-segmentation-security-0050.xml";
    binFilename = "models/instance-segmentation-security-0050.bin";
    labelFilename = "models/coco.txt";


    ui->xml_lineEdit->setText("instance-segmentation-security-0050.xml");
    ui->bin_lineEdit->setText("instance-segmentation-security-0050.bin");
    ui->label_textEdit->setText("coco.txt");


    //显示
    cv::Mat imgShow;
    cvtColor(srcImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_45->SetImage(qImg);
}


//FastSAM
void MainWindow::on_tool_FastSAM_clicked(){
    page_idx=35;    //界面索引
    function_idx=58;//功能的索引

    ui->stackedWidget->setCurrentIndex(35);
    ui->pb_gnAI->setText("FastSAM");
    status_label->setText(QString(""));

    //设置默认的模型、参数和label
    xmlFilename = "models/FastSAM-s.xml";
    binFilename = "models/FastSAM-s.bin";
    ui->xml_lineEdit->setText("FastSAM-s.xml");
    ui->bin_lineEdit->setText("FastSAM-s.bin");
    ui->label_textEdit->setText("");

    //显示
    cv::Mat imgShow;
    cvtColor(srcImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_45->SetImage(qImg);
}


//opencv_infer
void opencv_infer(){
    // opencv 推理
    cv::dnn::Net net = cv::dnn::readNetFromONNX("models/resnet18_Opset16.onnx");  // 加载训练好的识别模型
    cv::Mat image = cv::imread("models/coco.jpg");  // 读取图片
    cv::Mat blob = cv::dnn::blobFromImage(image);  // 由图片加载数据 这里还可以进行缩放、归一化等预处理
    net.setInput(blob);  // 设置模型输入
    cv::Mat predict = net.forward(); // 推理出结果

    double minValue, maxValue;    // 最大值，最小值
    cv::Point  minIdx, maxIdx;    // 最小值坐标，最大值坐标
    cv::minMaxLoc(predict, &minValue, &maxValue, &minIdx, &maxIdx);
    std::cout << "res: " << predict << std::endl;
    std::cout << "maxValue: " << maxValue << "maxIdx: " << maxIdx.x << std::endl;
    std::cout << "minValue: " << minValue << "minIdx: " << minIdx.x << std::endl;
    int classId = maxIdx.x;
    std::cout <<classId;
}


void MainWindow::on_pushButton_xml_clicked(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片","models/","*.xml");

    if(!FileName.isEmpty()){
        QFileInfo fileinfo =QFileInfo(FileName);
        ui->xml_lineEdit->setText(fileinfo.fileName());

        xmlFilename = FileName.toStdString();
    }else{
        QMessageBox::warning(this, "提示", "文件路径错误。");
        return;
    }
}

void MainWindow::on_pushButton_bin_clicked(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片","models/","*.bin");

    if(!FileName.isEmpty()){
        QFileInfo fileinfo =QFileInfo(FileName);
        ui->bin_lineEdit->setText(fileinfo.fileName());

        binFilename = FileName.toStdString();
    }else{
        QMessageBox::warning(this, "提示", "文件路径错误。");
        return;
    }

}

void MainWindow::on_pushButton_label_clicked(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片","models/","*.txt");

    if(!FileName.isEmpty()){
        QFileInfo fileinfo =QFileInfo(FileName);
        ui->label_textEdit->setText(fileinfo.fileName());
        labelFilename = FileName.toStdString();
    }else{
        QMessageBox::warning(this, "提示", "文件路径错误。");
        return;
    }

}

void MainWindow::on_pb_gnAI_clicked(){
    if(ui->pb_gnAI->text()=="classification"){
        //1、初始化Core ie
        InferenceEngine::Core ie;//IE的核心类 其实就是IE

        //2、ie.ReadNetwork读取CNN网络
        InferenceEngine::CNNNetwork network = ie.ReadNetwork(xmlFilename, binFilename);//读取到加载CNN网络就会自动解析网络结构然后就可以获取输入输出了

        //3、获取输入与输出格式并设置精度
        InferenceEngine::InputsDataMap inputs = network.getInputsInfo();  //InputsDataMap本质是一个vector的数组
        InferenceEngine::OutputsDataMap outputs = network.getOutputsInfo();
        std::string input_name = "";
        std::string output_name = "";
        for (auto item : inputs){
            input_name = item.first;
            auto input_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
            input_data->setPrecision(InferenceEngine::Precision::FP32); //因为这个模型是来自pytorch数字集所以它的精度是FP32的全精度的 所以这个时候需要设置FP32
            input_data->setLayout(InferenceEngine::Layout::NCHW);  //这个也是根据模型设置的输入数据模式
            input_data->getPreProcess().setColorFormat(InferenceEngine::ColorFormat::RGB);   //也是根据模型设置输入的ColorFormat
        }
        for (auto item : outputs){
            output_name = item.first;
            auto output_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
            output_data->setPrecision(InferenceEngine::Precision::FP32); //因为这个模型是来自pytorch数字集所以它的精度是FP32的全精度的 所以我们这个时候需要设置FP32
        }
        //4、获取可执行网络并链接硬件
        auto executable_network = ie.LoadNetwork(network, "GPU");//会加载网络到CPU这个硬件，也可以设置成GPU

        //5、创建推理请求 创建之后就可以尝试进行推理了，但是实际在推理之前还有很多事情要做，如格式设置
        auto infer_request = executable_network.CreateInferRequest();
        //6、获取输入的Blob 格式转换类对象
        auto input = infer_request.GetBlob(input_name);          //获取input的Blob(进行输入的格式设置的类对象)
        size_t num_channels = input->getTensorDesc().getDims()[1];
        size_t h = input->getTensorDesc().getDims()[2];
        size_t w = input->getTensorDesc().getDims()[3];
        size_t image_size = h * w;
        //7、输入图像数据预处理(包括BGR->RGB，大小 浮点数 计算转换 图像顺序转换HWC->NCHW)
        cv::Mat srcOriginal = srcImg;//需要解析的原图
        cv::Mat src;
        cv::cvtColor(srcOriginal, src, cv::COLOR_BGR2RGB);// 因为resnet18模型接收的顺序是RGB的,而opencv的顺序是BGR的,需要进行一下转换再进行resnet18模型的图像预处理
        cv::Mat blob_image;//转换为网络可以解析图片格式
        cv::resize(src, blob_image, cv::Size(w, h));//转换大小
        blob_image.convertTo(blob_image, CV_32F);//转换为浮点数
        blob_image = blob_image / 255.0;//转换到0-1之间
        cv::subtract(blob_image, cv::Scalar(0.485, 0.456, 0.406), blob_image);//每个通道的值都减去均值
        cv::divide(blob_image, cv::Scalar(0.229, 0.224, 0.225), blob_image);// 每个通道的值都除以方差
        //8、将设置好的数据设置到输入的Blob中 -> 实际上GetBlob()的时候就已经开辟好了存储输入图像数据内存空间
        float* data = static_cast<float*>(input->buffer());//这就是直接将数据转换后填充到input那指定空间里面去了
        //opencv返回的mat图像的顺序是HWC,要将他转换为NCHW就是要将HWC类型的矩阵转换为NCHW类型,就是矩阵填充的问题
        for (size_t row = 0; row < h; row++) {
            for (size_t col = 0; col < w; col++) {
                for (size_t ch = 0; ch < num_channels; ch++) {
                    //blob_image是opencv过来的HWC格式  -》 转换为NCHW就是每一个通道变成一张图,按照通道顺序来的。第几个通道的就是第几张图的这种存放
                    data[image_size * ch + row * w + col] = blob_image.at<cv::Vec3f>(row, col)[ch];
                }
            }
        }
        //8、执行推理
        infer_request.Infer();
        //9、获取output
        auto output = infer_request.GetBlob(output_name);
        const float* probs = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(output->buffer());//将输出数据转换 Precision精度
        //10、最后需要获取输出的维度信息,解析数据,并输出最大的那个就是resnet18图像识别出来的结果
        const InferenceEngine::SizeVector outputDims = output->getTensorDesc().getDims();
        std::cout << outputDims[0] << "X" << outputDims[1] << std::endl;
        float max = probs[0];
        int max_index = 0;
        for (size_t i = 1; i < outputDims[1]; i++){
            if (max < probs[i]){
                max = probs[i];
                max_index = i;
            }
        }

        //解析imagenet_classes数集训练模型.txt输出图像识别分类结构到图片上
        std::vector<std::string> labels;
        std::ifstream fp(labelFilename);
        if (!fp.is_open()){
            printf("could not open file...\n");
            return;
        }
        std::string name;
        while (!fp.eof()){
            std::getline(fp, name);
            if (name.length()){
                labels.push_back(name);
                ui->label_textEdit->append(QString::fromStdString(name));
            }

        }
        fp.close();

        //图片上写文字
        cv::putText(srcOriginal, labels[max_index], cv::Point(50,50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2, 8);

        //显示
        cv::Mat imgShow;
        cvtColor(srcOriginal,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->graphicsView_45->SetImage(qImg);
    }else if(ui->pb_gnAI->text()=="SemanticSeg"){
        //1、初始化Core ie
        InferenceEngine::Core ie;

        //2、ie.ReadNetwork 读取CNN网络
        InferenceEngine::CNNNetwork network = ie.ReadNetwork(xmlFilename, binFilename);//读取到加载CNN网络就会自动解析网络结构然后就可以获取输入输出了

        //3、获取输入与输出格式并设置精度
        InferenceEngine::InputsDataMap inputs = network.getInputsInfo();//InputsDataMap本质是一个vector的数组，如果你有多个输入的话则对应每个
        InferenceEngine::OutputsDataMap outputs = network.getOutputsInfo();
        std::string input_name = "";
        std::string output_name = "";

        for (auto item : inputs){
           input_name = item.first;
           std::cout<<input_name;
           auto input_data = item.second;
           input_data->setPrecision(InferenceEngine::Precision::U8);//输入图片数据格式为unsigned char 就是8位的精度
           input_data->setLayout(InferenceEngine::Layout::NCHW);
           input_data->getPreProcess().setColorFormat(InferenceEngine::ColorFormat::BGR);//该模型规定输入图片为BGR
        }

        for (auto item : outputs){
           output_name = item.first;
           auto output_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
           output_data->setPrecision(InferenceEngine::Precision::FP32);//输出还是浮点数输出的
        }
        //4、获取可执行网络并链接硬件
        auto executable_network = ie.LoadNetwork(network, "CPU");//会加载网络到CPU这个硬件，也可以设置成GPU

        //5、创建推理请求 创建之后就可以尝试进行推理了，但是实际在推理之前还有很多事情要做，如格式设置
        auto curr_infer_request = executable_network.CreateInferRequestPtr();

        //6、输入图像数据预处理(包括BGR->RGB，大小 浮点数 计算转换 图像顺序转换HWC->NCHW)
        cv::Mat curr_frame = srcImg;

        //设置推理输入
        frameToBlob(curr_frame, curr_infer_request, input_name);


        std::vector<cv::Vec3b> color_tab;
        color_tab.push_back(cv::Vec3b(0, 0, 0));//对应BG
        color_tab.push_back(cv::Vec3b(255, 0, 0));//对应road
        color_tab.push_back(cv::Vec3b(0, 0, 255));//对应curb
        color_tab.push_back(cv::Vec3b(0, 255, 255));//对应mark

        //执行推理
        curr_infer_request->Infer();

        //获取output
        auto output = curr_infer_request->GetBlob(output_name);


        const float* detection_out = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(output->buffer());//检测的输出结果

        //最后需要获取输出的维度信息,解析数据,detection_out 是4*w*h个值的集合,我们要分析出h*w每个对应像素的四个值中的最大值保持下来
        const InferenceEngine::SizeVector outputDims = output->getTensorDesc().getDims();
        const int out_c = outputDims[1];//每个像素点的四个类型的值
        const int out_h = outputDims[2];
        const int out_w = outputDims[3];
        cv::Mat result = cv::Mat::zeros(cv::Size(out_w, out_h), CV_8UC3);//存放结果
        int step = out_h * out_w;
        for (int row = 0; row < out_h; row++){
           for (int col = 0; col < out_w; col++){
               int max_index = 0;
               float max_pord = detection_out[row * out_w + col];//该像素的第0个类别的权值
               for (int cn = 1; cn < out_c; cn++){
                   float pord = detection_out[cn * step + row * out_w + col];//该像素的第i个类别的权值
                   if (pord > max_pord){
                       max_index = cn;
                       max_pord = pord;
                   }
               }
               result.at<cv::Vec3b>(row, col) = color_tab[max_index];//设置像素点的颜色值
           }
        }
        cv::resize(result, result, cv::Size(curr_frame.cols, curr_frame.rows));//尺寸转换便于叠加
        cv::addWeighted(curr_frame, 0.5, result, 0.5, 0, curr_frame);//叠加

        //显示
        cv::Mat imgShow;
        cvtColor(curr_frame,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->graphicsView_45->SetImage(qImg);
    }else if(ui->pb_gnAI->text()=="InstanceSeg"){
        //读取coco_labels
        std::vector<std::string> coco_labels;
        std::ifstream fp(labelFilename);
        if (!fp.is_open()){
            printf("could not open file...\n");
            exit(-1);
        }
        std::string name;
        while (!fp.eof()){
            std::getline(fp, name);
            if (name.length()){
                coco_labels.push_back(name);
            }
        }
        fp.close();

        cv::RNG rng(12345);

        InferenceEngine::Core ie;//1、创建IE核心

        //2、ie.ReadNetwork加载网络和权重
        InferenceEngine::CNNNetwork network = ie.ReadNetwork(xmlFilename, binFilename);

        //3、获取输入与输出格式并设置精度
        InferenceEngine::InputsDataMap inputs = network.getInputsInfo();
        InferenceEngine::OutputsDataMap outputs = network.getOutputsInfo();


        std::string input_name = "";
        std::string info_name = "";
        int in_index = 0;
        for (auto item : inputs){
            if (in_index == 0){
                input_name = item.first;
                auto input_data = item.second;
                input_data->setPrecision(InferenceEngine::Precision::U8);
                input_data->setLayout(InferenceEngine::Layout::NCHW);
                input_data->getPreProcess().setColorFormat(InferenceEngine::ColorFormat::BGR);
                std::cout << "input name = " << input_name << std::endl;
            }else{
                info_name = item.first;
                auto input_data = item.second;
                input_data->setPrecision(InferenceEngine::Precision::FP32);
                std::cout << "info_name name = " << info_name << std::endl;
            }
            in_index++;
        }
        for (auto item : outputs){
            std::string output_name = item.first;
            auto output_data = item.second;
            output_data->setPrecision(InferenceEngine::Precision::FP32);//������Ǹ����������
            std::cout << "output name = " << output_name << std::endl;
        }

        //获取可执行网络并链接硬件
        auto executable_network = ie.LoadNetwork(network, "CPU");

        //创建推理请求 创建之后就可以尝试进行推理了，但是实际在推理之前还有很多事情要做，如格式设置
        auto curr_infer_request = executable_network.CreateInferRequestPtr();

        cv::Mat curr_frame =srcImg;
        std::vector<cv::Rect> vc_RoiRect;
        frameToBlob(curr_frame, curr_infer_request, input_name);//���õ�һ������
        auto input2 = curr_infer_request->GetBlob(info_name);
        auto imInfoDim = inputs.find(info_name)->second->getTensorDesc().getDims()[1];
        InferenceEngine::MemoryBlob::Ptr minput2 = InferenceEngine::as<InferenceEngine::MemoryBlob>(input2);
        auto minput2Holder = minput2->wmap();
        float* p = minput2Holder.as<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>();
        p[0] = static_cast<float>(inputs[input_name]->getTensorDesc().getDims()[2]);
        p[1] = static_cast<float>(inputs[input_name]->getTensorDesc().getDims()[3]);
        p[2] = 1.0f;
        std::cout << p[0] << "  " << p[1] << std::endl;

        //执行推理
        curr_infer_request->Infer();

        //获取output
        auto scores = curr_infer_request->GetBlob("scores");
        auto boxes = curr_infer_request->GetBlob("boxes");
        auto clazzes = curr_infer_request->GetBlob("classes");
        auto raw_masks = curr_infer_request->GetBlob("raw_masks");
        const float* scores_out = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(scores->buffer()); //检测的输出结果
        const float* boxes_out = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(boxes->buffer()); // 这个矩形输出的坐标大小是基于480*480的
        const float* clazzes_data = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(clazzes->buffer());
        const auto raw_masks_data = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(raw_masks->buffer());//输出的是 raw_masks 结构
        //最后需要获取输出的维度信息 解析数据
        const InferenceEngine::SizeVector scores_outputDims = scores->getTensorDesc().getDims();
        const InferenceEngine::SizeVector boxes_outputDims = boxes->getTensorDesc().getDims();
        const InferenceEngine::SizeVector mask_outputDims = raw_masks->getTensorDesc().getDims();

        size_t max_scoresSize = scores_outputDims[0];//100
        size_t max_boxesSize = boxes_outputDims[1];//4
        size_t mask_h = mask_outputDims[2];//raw_masks 宽高
        size_t mask_w = mask_outputDims[3];
        size_t box_stride = mask_h * mask_w * mask_outputDims[1];//表示每一个实例的之间的步距 81*14*14，这100个实例之间每个实例都隔这么多字节

        int im_w = curr_frame.cols;
        int im_h = curr_frame.rows;
        float w_rate = static_cast<float>(im_w) / 480.0;
        float h_rate = static_cast<float>(im_h) / 480.0;

        for (size_t  i = 0; i < max_scoresSize; i++){
            float confidence = scores_out[i];
            float xmin = boxes_out[i * max_boxesSize] * w_rate;  //这个矩形输出的坐标大小是基于480*480的
            float ymin = boxes_out[i * max_boxesSize + 1] * h_rate;
            float xmax = boxes_out[i * max_boxesSize + 2] * w_rate;
            float ymax = boxes_out[i * max_boxesSize + 3] * h_rate;
            if (confidence > 0.5){
                cv::Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));//得到一个随机颜色
                cv::Rect box;
                float x1 = std::min(std::max(0.0f, xmin), static_cast<float>(im_w));
                float y1 = std::min(std::max(0.0f, ymin), static_cast<float>(im_h));
                float x2 = std::min(std::max(0.0f, xmax), static_cast<float>(im_w));
                float y2 = std::min(std::max(0.0f, ymax), static_cast<float>(im_h));
                box.x = static_cast<int>(x1);
                box.y = static_cast<int>(y1);
                box.width = static_cast<int>(x2 - x1);
                box.height = static_cast<int>(y2 - y1);
                int label = static_cast<int>(clazzes_data[i]);
                // 解析mask   第i个实例(box_stride*i) + 81个类别的第几个类别 每个类别的步距是宽*高(mask_h * mask_w * label)
                float* mask_arr = raw_masks_data + box_stride * i + mask_h * mask_w * label;//Ѱ�ҵ����ʵ�����������Ӧ�ĵ�ַ(��С��14*14)
                cv::Mat mask_mat(mask_h, mask_w, CV_32FC1, mask_arr);//将数据创建到mat中大小为14*14的
                cv::Mat roi_img = curr_frame(box);
                cv::Mat resized_mask_mat(box.height, box.width, CV_32FC1);
                cv::resize(mask_mat, resized_mask_mat, cv::Size(box.width, box.height));//进行尺寸转换

                cv::Mat uchar_resized_mask(box.height, box.width, CV_8UC3, color);

                roi_img.copyTo(uchar_resized_mask, resized_mask_mat <= 0.5);           //将roi_img像素点对应resized_mask_mat为1的复制到uchar_resized_mask上去
                cv::addWeighted(uchar_resized_mask, 0.7, roi_img, 0.3, 0.0f, roi_img);//进行mat叠加,uchar_resized_mask占比0.7,roi_img占比0.3 输出到roi_img
                cv::putText(curr_frame, coco_labels[label].c_str(), box.tl() + (box.br() - box.tl()) / 2, cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 255), 1, 8);
            }
        }
        //显示
        cv::Mat imgShow;
        cvtColor(curr_frame,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
        ui->graphicsView_45->SetImage(qImg);
    }else if(ui->pb_gnAI->text()=="FastSAM"){
        FastSAM fastsam;
        if(fastsam.Initialize(xmlFilename,binFilename, 0.4, 0.3, true)) {
            auto start = std::chrono::steady_clock::now();

            //std::string image_path ="models/coco.jpg";
            //fastsam.Infer(image_path)
            cv::Mat out =fastsam.Infer(srcImg);

            auto end = std::chrono::steady_clock::now();
            auto tt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();;
            qDebug()<< "infer time:" << tt << " ms.\n";

            //显示
            cv::Mat imgShow;
            cvtColor(out,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
            QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
            ui->graphicsView_45->SetImage(qImg);
        }
    }
}



//目标检测
void MainWindow::on_tool_detection_clicked(){
    page_idx=36;    //界面索引
    function_idx=59;//功能的索引


    ui->stackedWidget->setCurrentIndex(36);
    ui->pb_gnAI_46->setText("Detection");
    status_label->setText(QString(""));

    //设置默认的模型、参数和label
    xmlFilename = "models/face-detection-0202.xml";
    binFilename = "models/face-detection-0202.bin";

    ui->xml_lineEdit_2->setText("face-detection-0202.xml");
    ui->bin_lineEdit_2->setText("face-detection-0202.bin");
    ui->label_textEdit_2->setText("");

}

//检测分类
void MainWindow::on_tool_detection_classification_clicked(){
    page_idx=36;    //界面索引
    function_idx=60;//功能的索引

    ui->stackedWidget->setCurrentIndex(36);
    ui->pb_gnAI_46->setText("Detection_Classification");
    status_label->setText(QString(""));

}


void MainWindow::on_pushButton_xml_2_clicked(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片","models/","*.xml");

    if(!FileName.isEmpty()){
        QFileInfo fileinfo =QFileInfo(FileName);
        ui->xml_lineEdit_2->setText(fileinfo.fileName());

        xmlFilename = FileName.toStdString();
    }else{
        QMessageBox::warning(this, "提示", "文件路径错误。");
        return;
    }
}

void MainWindow::on_pushButton_bin_2_clicked(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片","models/","*.bin");

    if(!FileName.isEmpty()){
        QFileInfo fileinfo =QFileInfo(FileName);
        ui->bin_lineEdit_2->setText(fileinfo.fileName());

        binFilename = FileName.toStdString();
    }else{
        QMessageBox::warning(this, "提示", "文件路径错误。");
        return;
    }
}

void MainWindow::on_pushButton_label_2_clicked(){
    QString FileName = QFileDialog::getOpenFileName(this, "选择图片","models/","*.txt");

    if(!FileName.isEmpty()){
        QFileInfo fileinfo =QFileInfo(FileName);
        ui->label_textEdit_2->setText(fileinfo.fileName());
        labelFilename = FileName.toStdString();
    }else{
        QMessageBox::warning(this, "提示", "文件路径错误。");
        return;
    }
}





//加载人脸表情识别的模型 传出 推理以及输入输出名称
void load_face_emotion_model(std::shared_ptr<InferenceEngine::InferRequest>& face_emotion_request, std::string& face_emotion_input_name, std::string& face_emotion_output_name){
    //1、初始化Core ie
    InferenceEngine::Core ie;//IE的核心类 其实就是IE
    //2、ie.ReadNetwork 读取CNN网络 需要模型里面的两个文件 resnet18.bin resnet18.xml
    std::string xmlFilename = "models/emotions-recognition-retail-0003.xml";
    std::string binFilename = "models/emotions-recognition-retail-0003.bin";
    InferenceEngine::CNNNetwork network = ie.ReadNetwork(xmlFilename, binFilename);//读取到加载CNN网络就会自动解析网络结构然后就可以获取输入输出了
    //3、获取输入与输出格式并设置精度
    InferenceEngine::InputsDataMap inputs = network.getInputsInfo();//InputsDataMap本质是一个vector的数组，如果你有多个输入的话则对应每个
    InferenceEngine::OutputsDataMap outputs = network.getOutputsInfo();
    std::string input_name = "";
    std::string output_name = "";

    for (auto item : inputs){  //这个模型有两个输入,会循环两次
        face_emotion_input_name = item.first;
        auto input_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
        input_data->setPrecision(InferenceEngine::Precision::U8);//输入图片数据格式为unsigned char 就是8位的精度
        input_data->setLayout(InferenceEngine::Layout::NCHW);
        input_data->getPreProcess().setColorFormat(InferenceEngine::ColorFormat::BGR);//该模型规定输入图片为BGR
    }
    for (auto item : outputs){
        face_emotion_output_name = item.first;
        auto output_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
        output_data->setPrecision(InferenceEngine::Precision::FP32);//输出还是浮点数输出的
    }
    //4、获取可执行网络并链接硬件
    auto executable_network = ie.LoadNetwork(network, "CPU");//会加载网络到CPU这个硬件，也可以设置成GPU
    //5、创建推理请求 创建之后就可以尝试进行推理了，但是实际在推理之前还有很多事情要做，如格式设置
    face_emotion_request = executable_network.CreateInferRequestPtr();
}






void MainWindow::on_pb_gnAI_46_clicked(){
    if(ui->pb_gnAI_46->text()=="Detection"){
        InferenceEngine::Core ie;
        InferenceEngine::CNNNetwork network = ie.ReadNetwork(xmlFilename, binFilename);//读取到加载CNN网络就会自动解析网络结构然后就可以获取输入输出了
        InferenceEngine::InputsDataMap inputs = network.getInputsInfo();//InputsDataMap本质是一个vector的数组，如果你有多个输入的话则对应每个
        InferenceEngine::OutputsDataMap outputs = network.getOutputsInfo();
        std::string input_name = "";
        std::string output_name = "";
        for (auto item : inputs){
            input_name = item.first;
            auto input_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
            input_data->setPrecision(InferenceEngine::Precision::U8);//输入图片数据格式为unsigned char 就是8位的精度
            input_data->setLayout(InferenceEngine::Layout::NCHW);
            input_data->getPreProcess().setColorFormat(InferenceEngine::ColorFormat::BGR);//该模型规定输入图片为BGR
        }
        for (auto item : outputs){
            output_name = item.first;
            auto output_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
            output_data->setPrecision(InferenceEngine::Precision::FP32);//输出还是浮点数输出的
        }
        //4、获取可执行网络并链接硬件
        auto executable_network = ie.LoadNetwork(network, "CPU");//会加载网络到CPU这个硬件，也可以设置成GPU

        //5、创建推理请求 创建之后就可以尝试进行推理了，但是实际在推理之前还有很多事情要做，如格式设置
        auto infer_request = executable_network.CreateInferRequest();

        //6、创建视频流/加载视频文件
        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);
        //先设置采集格式
        pCapture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        //设置高清采集分辨率
        pCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
        pCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

        cv::Mat src;
        while (true){
            bool ret = pCapture.read(src);
            if (ret == false)
                break;


            //7、循环执行推理
            //通过传入的推理引擎；获取输入的Blob 格式转换类对象
            auto input = infer_request.GetBlob(input_name);//获取input的Blob(进行输入的格式设置的类对象)
            size_t num_channels = input->getTensorDesc().getDims()[1];
            size_t h = input->getTensorDesc().getDims()[2];
            size_t w = input->getTensorDesc().getDims()[3];
            size_t image_size = h * w;

            //对输入图像转换
            int im_h = src.rows;
            int im_w = src.cols;
            cv::Mat blob_image;//转换为网络可以解析图片格式
            cv::resize(src, blob_image, cv::Size(w, h));//转换大小

            //3、将设置好的数据设置到输入的Blob中 -> 实际上GetBlob()的时候就已经开辟好了存储输入图像数据内存空间
            unsigned char* data = static_cast<unsigned char*>(input->buffer());//这就是直接将数据转换后填充到input那指定空间里面去了
            for (size_t row = 0; row < h; row++) {
                for (size_t col = 0; col < w; col++) {
                    for (size_t ch = 0; ch < num_channels; ch++) {
                        //blob_image 是opencv过来的HWC格式  -》 转换为NCHW就是每一个通道变成一张图 按照通道顺序来的。第几个通道的就是第几张图的这种存放
                        data[image_size * ch + row * w + col] = blob_image.at<cv::Vec3b>(row, col)[ch];
                    }
                }
            }

            //执行推理
            infer_request.Infer();

            //获取output
            auto output = infer_request.GetBlob(output_name);
            const float* detection_out = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(output->buffer());//检测的输出结果

            //最后需要获取输出的维度信息 解析数据
            const InferenceEngine::SizeVector outputDims = output->getTensorDesc().getDims();
            //std::cout << outputDims[2] << "X" << outputDims[3] << std::endl;
            size_t max_num = outputDims[2];//是输出的那个N
            size_t object_size = outputDims[3];//是输出的那个7
            for (int n = 0; n < max_num; n++){
                float lable = detection_out[n * object_size + 1];// +1表示输出的是7个里面的第二个lableID
                float confidence = detection_out[n * object_size + 2];
                float xmin = detection_out[n * object_size + 3] * im_w;  //输出得到的都是0-1的浮点数坐标  要乘以原宽高才是实际坐标
                float ymin = detection_out[n * object_size + 4] * im_h;
                float xmax = detection_out[n * object_size + 5] * im_w;
                float ymax = detection_out[n * object_size + 6] * im_h;
                if (confidence > 0.5){
                    //printf("lable id = %d\n", static_cast<int>(lable));
                    cv::Rect box;
                    box.x = static_cast<int>(xmin);
                    box.y = static_cast<int>(ymin);
                    box.width = static_cast<int>(xmax - xmin);
                    box.height = static_cast<int>(ymax - ymin);
                    cv::rectangle(src, box, cv::Scalar(0, 255, 0), 2, 8, 0);
                    cv::putText(src, cv::format("%.2f", confidence), box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1, 8);
                }
            }

            //显示
            cv::Mat imgShow;
            cvtColor(src,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
            QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
            ui->graphicsView_46->SetImage(qImg);



            char c = cv::waitKey(1);
            if (c == 27)//按下esc
                break;
        }
    }
    else if(ui->pb_gnAI_46->text()=="Detection_Classification"){
        //加载人脸表情识别的模型
        std::shared_ptr<InferenceEngine::InferRequest> face_emotion_request;
        std::string face_emotion_input_name;
        std::string face_emotion_output_name;
        load_face_emotion_model(face_emotion_request, face_emotion_input_name, face_emotion_output_name);

        //1、初始化Core ie
        InferenceEngine::Core ie;//IE的核心类 其实就是IE

        //2、ie.ReadNetwork 读取CNN网络 需要模型里面的两个文件 resnet18.bin resnet18.xml
        std::string xmlFilename = "models/face-detection-0202.xml";
        std::string binFilename = "models/face-detection-0202.bin";
        InferenceEngine::CNNNetwork network = ie.ReadNetwork(xmlFilename, binFilename);//读取到加载CNN网络就会自动解析网络结构然后就可以获取输入输出了

        //3、获取输入与输出格式并设置精度
        InferenceEngine::InputsDataMap inputs = network.getInputsInfo();     //InputsDataMap本质是一个vector的数组，如果你有多个输入的话则对应每个
        InferenceEngine::OutputsDataMap outputs = network.getOutputsInfo();
        std::string input_name = "";
        std::string output_name = "";
        for (auto item : inputs){
            input_name = item.first;
            auto input_data = item.second;                           //是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
            input_data->setPrecision(InferenceEngine::Precision::U8);//输入图片数据格式为unsigned char,就是8位的精度
            input_data->setLayout(InferenceEngine::Layout::NCHW);    //这个也是根据模型设置的输入数据模式
            input_data->getPreProcess().setColorFormat(InferenceEngine::ColorFormat::BGR);//根据模型设置输入的ColorFormat,该模型规定输入图片为BGR
        }
        for (auto item : outputs){
            output_name = item.first;
            auto output_data = item.second;//是一个数据结构 暂采用C++11的auto自动推断 因为只有一个输入则暂时这样定义在里面设置它的精度
            output_data->setPrecision(InferenceEngine::Precision::FP32);//输出还是浮点数输出的
        }
        //4、获取可执行网络并链接硬件
        auto executable_network = ie.LoadNetwork(network, "GPU");//会加载网络到CPU这个硬件，也可以设置成GPU

        //5、创建推理请求 创建之后就可以尝试进行推理了，但是实际在推理之前还有很多事情要做，如格式设置
        auto curr_infer_request = executable_network.CreateInferRequestPtr();
        auto next_infer_request = executable_network.CreateInferRequestPtr();

        //6、输入图像数据预处理(包括BGR->RGB，大小 浮点数 计算转换 图像顺序转换HWC->NCHW)
        pCapture = cv::VideoCapture(0,cv::CAP_DSHOW);
        //先设置采集格式
        pCapture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        //设置高清采集分辨率
        pCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
        pCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

        cv::Mat curr_frame;
        cv::Mat next_frame;
        pCapture.read(curr_frame);
        frameToBlob(curr_frame, curr_infer_request, input_name);
        bool first_frame = true;
        bool last_frame = false;
        while (true){
            bool ret = pCapture.read(next_frame);
            if (ret == false){ //最后一帧
                last_frame = true;//不在输入设置了
            }
            if (!last_frame){//不是最后一帧就设置下一帧
                frameToBlob(next_frame, next_infer_request, input_name);
            }
            if (first_frame) {
                curr_infer_request->StartAsync();//开始进行推理
                next_infer_request->StartAsync();
                first_frame = false;
            }else{
                if (!last_frame){ //每次开始都是下一帧准备异步  下面有交换
                    next_infer_request->StartAsync();
                }
            }

            //表情的几个值
            static const char* const items[] = {"neutral", "happy", "sad", "surprise", "anger"};

            //7、循环执行推理  每次推理都是当前帧 等待推理成功就进行解析
            if (InferenceEngine::OK == curr_infer_request->Wait(InferenceEngine::IInferRequest::WaitMode::RESULT_READY)){
                //人脸检测的推理,返回人脸矩形框
                cv::Rect face_box;
                //获取output
                auto output = curr_infer_request->GetBlob(output_name);
                const float* detection_out = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(output->buffer());//检测的输出结果
                //最后需要获取输出的维度信息 解析数据
                const InferenceEngine::SizeVector outputDims = output->getTensorDesc().getDims();
                //std::cout << outputDims[2] << "X" << outputDims[3] << std::endl;
                auto max_num = outputDims[2];//是输出的那个N
                auto object_size = outputDims[3];//是输出的那个7
                int im_h = curr_frame.rows;
                int im_w = curr_frame.cols;
                for (unsigned long long n = 0; n < max_num; n++){
                    float lable = detection_out[n * object_size + 1];// +1表示输出的是7个里面的第二个lableID
                    float confidence = detection_out[n * object_size + 2];
                    float xmin = detection_out[n * object_size + 3] * im_w;  //输出得到的都是0-1的浮点数坐标  要乘以原宽高才是实际坐标
                    float ymin = detection_out[n * object_size + 4] * im_h;
                    float xmax = detection_out[n * object_size + 5] * im_w;
                    float ymax = detection_out[n * object_size + 6] * im_h;
                    if (confidence > 0.9){
                        printf("lable id = %d\n", static_cast<int>(lable));
                        //做一下保护 保证矩形在图像类  做大小判断而不是坐标判断
                        xmin = std::min(std::max(0.0f, xmin), static_cast<float>(im_w));
                        ymin = std::min(std::max(0.0f, ymin), static_cast<float>(im_h));
                        xmax = std::min(std::max(0.0f, xmax), static_cast<float>(im_w));
                        ymax = std::min(std::max(0.0f, ymax), static_cast<float>(im_h));
                        face_box.x = static_cast<int>(xmin);
                        face_box.y = static_cast<int>(ymin);
                        face_box.width = static_cast<int>(xmax - xmin);
                        face_box.height = static_cast<int>(ymax - ymin);
                        //std::cout << box.x << " " << box.y << " " << box.width << " " << box.height;
                        cv::rectangle(curr_frame, face_box, cv::Scalar(0, 255, 0), 2, 8, 0);
                        //cv::putText(src, cv::format("%.2f", confidence), box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 1, 8);
                    }
                }


                //对人脸位置矩形框进行表情识别,设置输入,进行推理,解析输出
                if(face_box.width > 64 && face_box.height > 64 && face_box.x > 0 && face_box.y > 0){ //要做保护 不然容易奔溃
                    cv::Mat faceROI = curr_frame(face_box);  //设置输入
                    frameToBlob(faceROI, face_emotion_request, face_emotion_input_name);

                    face_emotion_request->Infer();  //执行推理

                    //输出string结果返回
                    auto output = face_emotion_request->GetBlob(face_emotion_output_name);
                    const float* probs = static_cast<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>(output->buffer());//将输出数据转换 Precision精度

                    const InferenceEngine::SizeVector outputDims = output->getTensorDesc().getDims();
                    float max = probs[0];
                    int max_index = 0;
                    for (size_t i = 1; i < outputDims[1]; i++){
                        if (max < probs[i]){
                            max = probs[i];
                            max_index = i;
                        }
                    }

                    cv::putText(curr_frame, items[max_index], face_box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2, 8);
                }
            }

            //显示
            cv::Mat imgShow;
            cvtColor(curr_frame,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
            QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
            ui->graphicsView_46->SetImage(qImg);

            char c = cv::waitKey(1);
            if (c == 27)//按下esc
                break;

            if (last_frame){
                break;
            }
            // 异步交互
            next_frame.copyTo(curr_frame);
            curr_infer_request.swap(next_infer_request);//指针域里面的函数
        }
    }
}


void MainWindow::on_pushButton_gn46_pause_clicked(){
    pCapture.release();
}




void MainWindow::on_tool_MedSAM_clicked(){
    page_idx=36;    //界面索引
    function_idx=60;//功能的索引

    ui->stackedWidget->setCurrentIndex(32);
    ui->pb_gnAI_42->setText("MedSAM");
    status_label->setText(QString(""));


    //显示
    cv::Mat imgShow;
    cvtColor(srcImg,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
    ui->graphicsView_LeftImage->SetImage(qImg);


}

void MainWindow::on_pb_gnAI_42_clicked(){

    // opencv 推理
//    cv::dnn::Net net = cv::dnn::readNetFromONNX("models/sam_onnx_example.onnx");  // 加载训练好的识别模型
//    cv::Mat image = cv::imread("models/coco.jpg");  // 读取图片

//    cv::Mat blob = cv::dnn::blobFromImage(image);  // 由图片加载数据 这里还可以进行缩放、归一化等预处理
//    net.setInput(blob);  // 设置模型输入

//    cv::Mat predict = net.forward(); // 推理出结果

//    double minValue, maxValue;    // 最大值，最小值
//    cv::Point  minIdx, maxIdx;    // 最小值坐标，最大值坐标
//    cv::minMaxLoc(predict, &minValue, &maxValue, &minIdx, &maxIdx);
//    std::cout << "res: " << predict << std::endl;
//    std::cout << "maxValue: " << maxValue << "maxIdx: " << maxIdx.x << std::endl;
//    std::cout << "minValue: " << minValue << "minIdx: " << minIdx.x << std::endl;
//    int classId = maxIdx.x;
//    std::cout <<classId;

       // 这里假设Python安装在默认路径，并且python命令可用
       const char* command = "python  MedSAM_Inference.py  -i C:\\Users\\31156\\Desktop\\Poff-AI\\IADS\\models\\coco.jpg -o C:\\Users\\31156\\Desktop\\Poff-AI\\IADS\\models | cd  C:\\Users\\31156\\Desktop\\SAM-Seg-VIS-main\\MedSAM-main";
       // 执行命令
       int result = system(command);


       // 检查命令是否成功执行
       if (result < 0) {
           // 处理错误
       }


//    //显示
//    cv::Mat imgShow;
//    cvtColor(curr_frame,imgShow, cv::COLOR_BGR2RGB);//BGR convert to RGB
//    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,imgShow.rows, imgShow.cols*imgShow.channels(), QImage::Format_RGB888);
//    ui->graphicsView_RightImage->SetImage(qImg);


}




void MainWindow::on_pb_diffusion_clicked(){

    vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();

    reader->SetFileDimensionality(3);
    reader->SetDataExtent(0, 512, 0, 512, 0, 160);
    reader->SetFilePrefix("C:\\Users\\31156\\Desktop\\Poff-AI\\tuxiang\\111\\0\\");
    reader->SetFilePattern("%s%d.jpg");
    reader->Update();
    reader->SetDataByteOrderToLittleEndian();



    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volumeMapper->SetInputData(reader->GetOutput());;
    volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance()/2);	//设置光线采样距离
    //volumeMapper->SetAutoAdjustSampleDistances(0);//设置图像采样步长
    //volumeMapper->SetImageSampleDistance(4);


    vtkSmartPointer<vtkVolumeProperty> volumeProperty =vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();  //打开或者关闭阴影测试
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);  //漫反射
    volumeProperty->SetSpecular(0.2); //镜面反射

    //设置不透明度
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();

    compositeOpacity->AddPoint(20, 0.0);
    compositeOpacity->AddPoint(50, 0.0);
    compositeOpacity->AddPoint(70, 0.1);
    compositeOpacity->AddPoint(90, 0.3);
    compositeOpacity->AddPoint(180, 0.6);
    compositeOpacity->AddPoint(255, 1.0);

    volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数

    //设置梯度不透明属性
//    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =vtkSmartPointer<vtkPiecewiseFunction>::New();
//    volumeGradientOpacity->AddPoint(0, 0.0);
//    volumeGradientOpacity->AddPoint(20, 0.2);
//    volumeGradientOpacity->AddPoint(90, 0.5);
//    volumeGradientOpacity->AddPoint(100, 1);
//    volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比


    //设置颜色属性
    vtkSmartPointer<vtkColorTransferFunction> color =vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0, 0.1, 0.1, 0.1);
    color->AddRGBPoint(30, 1.0, 0.0, 0.0);
    color->AddRGBPoint(64, 1.0, 0.52, 0.3);
    color->AddRGBPoint(180, 1.0, 1.0, 1.0);
    color->AddRGBPoint(255, 1.0, 1.0, 1.0);



//    color->AddRGBPoint(-1000, 0.00, 0.00, 0.00);
//    color->AddRGBPoint(100, 220/255., 21/255., 3/255.);
//    color->AddRGBPoint(300, 250/255., 1.00, 189/255.);
//    color->AddRGBPoint(478, 224/255., 1, 249/255.);
//    color->AddRGBPoint(1500, 1, 1, 1);
    volumeProperty->SetColor(color);


    /*************************实例化体绘制Actor*****************/
    vtkSmartPointer<vtkVolume> volume =vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    //定义绘制器
    vtkSmartPointer<vtkRenderer> aRenderer =vtkSmartPointer<vtkRenderer>::New();
    aRenderer->SetBackground(0, 0, 0);
    aRenderer->AddVolume(volume);

    //创建一个camera
    vtkSmartPointer< vtkCamera > aCamera =vtkSmartPointer< vtkCamera >::New();
    aCamera->SetViewUp(0, 0, -1);
    aCamera->SetPosition(0, 1, 0);
    aCamera->SetFocalPoint(0, 0, 0);
    aRenderer->AddActor(volume);
    aRenderer->SetActiveCamera(aCamera);
    aRenderer->ResetCamera();
    aCamera->Dolly(1.5);
    aRenderer->SetBackground(0, 0, 0);
    aRenderer->ResetCameraClippingRange();



    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    ui->openGLWidget_diffusion->GetRenderWindow()->AddRenderer((aRenderer));
    renderWindowInteractor->SetRenderWindow(ui->openGLWidget_diffusion->GetRenderWindow());

    ui->openGLWidget_diffusion->show();
    ui->openGLWidget_diffusion->GetRenderWindow()->Render();
    renderWindowInteractor->Initialize();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
    style = vtkInteractorStyleTrackballCamera::New();

    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->Start();

    if (renderWindowInteractor)
        renderWindowInteractor->ExitCallback();
}

void MainWindow::on_tool_ROI_Diffusion_clicked(){
    page_idx=33;    //界面索引
    function_idx=60;//功能的索引

    ui->stackedWidget->setCurrentIndex(33);
}
