#ifndef MATRIX_H
#define MATRIX_H

#include <QWidget>
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/ml.hpp>
#include <QFileDialog>
#include <QVector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <qmath.h>
#include <vector>
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/calib3d.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <stdio.h>
#include <fstream>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include <QMessageBox>


namespace Ui {
class matrix;
}

class matrix : public QWidget
{
    Q_OBJECT

public:
    explicit matrix(QWidget *parent = nullptr);
    ~matrix();
    cv::Mat kernel2 = cv::Mat::zeros(7, 7, CV_8U);

    cv::Mat getkernel(){
        return kernel2;
    }
    void loadStyleSheet(const QString &styleSheetFile);

private slots:
    void on_pb00_clicked();

    void on_pb01_clicked();

    void on_pb02_clicked();

    void on_pb03_clicked();

    void on_pb04_clicked();

    void on_pb05_clicked();

    void on_pb06_clicked();

    void on_pb16_clicked();

    void on_pb10_clicked();

    void on_pb11_clicked();

    void on_pb12_clicked();

    void on_pb13_clicked();

    void on_pb14_clicked();

    void on_pb15_clicked();

    void on_pb20_clicked();

    void on_pb21_clicked();

    void on_pb22_clicked();

    void on_pb23_clicked();

    void on_pb24_clicked();

    void on_pb25_clicked();

    void on_pb26_clicked();

    void on_pb30_clicked();

    void on_pb31_clicked();

    void on_pb32_clicked();

    void on_pb33_clicked();

    void on_pb34_clicked();

    void on_pb35_clicked();

    void on_pb36_clicked();

    void on_pb40_clicked();

    void on_pb41_clicked();

    void on_pb42_clicked();

    void on_pb43_clicked();

    void on_pb44_clicked();

    void on_pb45_clicked();

    void on_pb46_clicked();

    void on_pb50_clicked();

    void on_pb51_clicked();

    void on_pb52_clicked();

    void on_pb53_clicked();

    void on_pb54_clicked();

    void on_pb55_clicked();

    void on_pb56_clicked();

    void on_pb60_clicked();

    void on_pb61_clicked();

    void on_pb62_clicked();

    void on_pb63_clicked();

    void on_pb64_clicked();

    void on_pb65_clicked();

    void on_pb66_clicked();



private:
    Ui::matrix *ui;
};

#endif // MATRIX_H
