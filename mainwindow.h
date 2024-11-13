#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <iostream>
#include <algorithm>
#include <limits>
#include <qmath.h>
#include <vector>
#include <string>
#include <array>
#include <stdio.h>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/calib3d.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include <opencv2/highgui/highgui_c.h>


#include <QtWinExtras/QtWinExtras>   //winAPI，需要使用它来获取opencv窗口句柄
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMovie>
#include <QVector>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QTimer>
#include <QTime>
#include <QThread>
#include <QSettings>
#include <QFileSystemModel>


#include <openvino/openvino.hpp>
#include "inference_engine.hpp"
#include <fstream>


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
#include <libswresample/swresample.h>
}

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include "vtkDICOMImageReader.h"
#include "vtkMarchingCubes.h"
#include "vtkStripper.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkBoxWidget.h"
#include "vtkSmartPointer.h"
#include "vtkTriangleFilter.h"
#include "vtkMassProperties.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkRecursiveDividingCubes.h"
#include "vtkSTLWriter.h"
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCallbackCommand.h"
#include "vtkObject.h"
#include <vtkboxwidget.h>
#include "vtkPolyData.h"
#include "vtkMapper.h"
#include <vtkBoxWidget2.h>
#include <vtkBoxRepresentation.h>
#include <vtkCommand.h>
#include <vtkTransform.h>
#include <vtkJPEGReader.h>


#include <vtkBoxWidget2.h>
#include <vtkPoints.h>
#include <vtkCamera.h>
#include <vtkCutter.h>
#include <vtkPlanes.h>
#include <vtkSphere.h>
#include <vtkStripper.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkProperty2D.h>
#include <vtkClipPolyData.h>
#include <vtkContourFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkBoxClipDataSet.h>
#include <vtkSeedRepresentation.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPointHandleRepresentation2D.h>
#include "vtkSmartVolumeMapper.h"
#include <vtkImagePlaneWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkImagePlaneWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageActor.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkProperty.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkTexture.h>
#include <vtkBoxRepresentation.h>

#include "vtkPlane.h"

#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkImageMapToColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkCutter.h"
#include "vtkPlane.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageCast.h"
#include "vtkPointData.h"
#include "vtkImageShiftScale.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkImageStencil.h"
#include "vtkLookupTable.h"
#include "vtkImagePlaneWidget.h"
#include "vtkCellArray.h"
#include "vtkType.h"
#include "vtkPlaneWidget.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkProbeFilter.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkClipPolyData.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkCamera.h"
#include "vtkMarchingCubes.h"
#include "vtkDataSetMapper.h"


#include <vtkActor.h>
#include <vtkAreaPicker.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h>
#include <vtkIdTypeArray.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVersion.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include "vtkClipClosedSurface.h"
#include "vtkPlaneCollection.h"
#include "vtkAxesActor.h"
#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1


#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType)

#include "matrix.h"
#include "loginregister.h"
#include "personalcenter.h"
#include "ftpwindow.h"
#include "Dicomview/readworker.h"
#include "Dicomview/mygobal.h"
#include "Dicomview/seriesbase.h"
#include "Dicomview/xyseries.h"
#include "Dicomview/xzseries.h"
#include "Dicomview/yzseries.h"
#include "Algorithm/splitandmerge.h"
#include "Algorithm/ocvlivewire.h"
#include "Algorithm/snakeimage.h"
#include "models/fastsam.h"


#define defaultimgpath "C:\\Users\\31156\\Desktop\\Poff-AI\\suda.jpg"
#define defaultdirpath "C:\\Users\\31156\\Desktop\\Poff-AI\\tuxiang\\dicom_path\\all\\guzhe\\001"


//让编译器编译生成程序的执行字符集为utf-8
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class SeriesBase;

class MyFileSytemModel : public QFileSystemModel{
public:
    //第1列显示名称，其他几列都屏蔽掉
    int columnCount(const QModelIndex &parent) const{
        return 1;
    }
};


class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cv::Mat srcImg, grayImg, erzhiImg,fanxiangImg,noiseImg,currentImg,templateImg;
    QString dirchoose=defaultdirpath; //选择的文件夹
    cv::VideoCapture pCapture; //摄像头

    size_t page_idx=0;    //界面索引
    size_t function_idx=0;//功能的索引

    //AI模型的文件
    std::string xmlFilename;
    std::string binFilename;
    std::string labelFilename;

    //窗口
    ftpwindow ftp_window;
    matrix matrixwindow; //卷积核窗口
    loginregister loginregister_window;
    personalcenter personalcente_window;

    bool zijishixian;
    cv::Mat kernel; //卷积核

    void on_Canny_clicked();
    void loadStyleSheet(const QString &styleSheetFile);

    void Imgchange(cv::Mat &img);

    void jiemianqiehuan(int index,QLabel *tu1,QLabel *tu2,QLabel *zi1,QLabel *zi2);

    bool eventFilter(QObject* watched, QEvent* event);//事件过滤

    bool isOpeing; // 是否正在读取文件
    SeriesBase *xyS;// 添加  xy视图的序列
    SeriesBase *xzS;// 添加
    SeriesBase *yzS;// 添加

    QLabel* status_label;
    std::string winName;

public slots:
    void on_windowChanged();
    void on_clickedView(SeriesBase::ViewType);// 在视图上点击鼠标（左右都可以）后执

private slots:

    void ReadCheckCompleted(); // worker完成后执行该函数

    void on_gray_leval_clicked();

    void on_gray_balance_clicked();

    void on_grad_sharpen_clicked();

    void on_laplace_sharpen_clicked();

    void on_salt_noise_clicked();

    void on_guass_noise_clicked();
    void on_average_filter_clicked();

    void on_middle_filter_clicked();

    void on_window_filter_clicked();

    void on_gauss_filter_clicked();

    void on_form_filter_clicked();

    void on_affine_clicked();

    void on_perspective_clicked();

    void on_frame_diff_clicked();

    void on_mix_guass_clicked();

    void on_cloaking_clicked();

    void on_face_haar_clicked();
    void on_camera2_clicked();
    void on_camera2_2_clicked();
    void on_rgbtoerzhi_clicked();
    void on_rgbtogray_clicked();
    void on_fanxiang_clicked();
    void on_pushButton_bianyuan_clicked();

    void on_pb_juanjihe_clicked();

    void on_pushButton_centre_clicked();

    void on_pushButton_jizhongjibuzhong_clicked();


    void on_pb_juanjihe_7_clicked();

    void on_pushButton_gn7_clicked();

    void on_pushButton_bianjietiqu_clicked();

    void on_pushButton_quyutianchong_clicked();

    void on_pushButton_liantongtiqu_clicked();

    void on_pushButton_tuke_clicked();

    void on_pushButton_xihua_clicked();

    void on_pushButton_cuhua_clicked();

    void on_pushButton_gujia_clicked();

    void on_pushButton_yuzhi_clicked();



    void on_pushButton_gn8_clicked();

    void on_pushButton_OSTU_clicked();

    void on_pushButton_Kittler_clicked();

    void on_pushButton_hough_clicked();

    void on_pushButton_fenshuilin_clicked();

    void on_pushButton_fenkaihebing_clicked();

    void on_actionopenfile_triggered();

    void on_actionsaveafile_triggered();

    void on_actionftp_triggered();

    void on_shiping_clicked();

    void on_pushButton_bofang_clicked();

    void on_toolBox_currentChanged(int index);

    void on_tool_huiduchuli_clicked();

    void on_tool_tuxiangruihua_clicked();

    void on_tool_bianyuanjiance_clicked();

    void on_tool_lvbo_clicked();

    void on_tool_xuxiangbianhuan_clicked();

    void on_pushButton_pinputu_clicked();

    void on_pb_sanweihuizhi_clicked();

    void on_actionioendic_triggered();

    void on_tool_tihuizhi_clicked();

    void on_tool_mianhuizhi_clicked();

    void on_pushButton_clicked();
    void on_comboBox_which_currentIndexChanged(QString seriesUID);
    void on_comboBox_2_currentIndexChanged(int color);
    void on_lineEdit_ww_textEdited(QString ww);
    void on_lineEdit_wl_textChanged(QString wl);

    void set_lineEdit_ww_text(double);
    void set_lineEdit_wl_text(double);

    void setProgressBarValue(int);
    void setPixelInfo(QString);

    void on_tool_MPR_clicked();

    void on_tool_cut_clicked();

    void on_tool_ROI_clicked();

    void on_tool_tiaoxuan_clicked();

    void on_pushButton_quyushenzhang_clicked();

    void on_pushButton_yidongpinjun_clicked();

    void on_pushButton_livewire_clicked();

    void on_pushButton_gn30_clicked();

    void on_pushButton_gn9_clicked();

    void on_pushButton_floodFill_clicked();

    void on_pushButton_Kmeans_clicked();

    void on_pushButton_gn10_clicked();

    void on_tool_lbp_clicked();

    void on_pushButton_gn16_clicked();

    void on_tool_haar_clicked();

    void on_tool_GLCM_clicked();

    void on_tool_Garbor_clicked();

    void on_pushButton_file17_clicked();

    void on_tool_his_clicked();

    void on_pushButton_gn17_clicked();

    void on_tool_moban_clicked();

    void on_tool_SIFT_clicked();

    void on_tool_orb_clicked();

    void on_tool_SURF_clicked();

    void on_svm_test_clicked();

    void on_tool_meanshift_clicked();

    void on_tool_FFT_clicked();

    void on_tool_FastSAM_clicked();

    void on_tool_ROI_BOX_clicked();

    void on_pushButton_gn30_pause_clicked();

    void on_pushButton_pause_clicked();

    void on_pb_gnAI_clicked();

    void on_tool_instanceseg_clicked();

    void on_tool_semanticseg_clicked();

    void on_pb_gnAI_46_clicked();

    void on_pushButton_gn46_pause_clicked();

    void on_tool_classification_clicked();

    void on_tool_detection_clicked();

    void on_tool_detection_classification_clicked();

    void on_pushButton_xml_clicked();

    void on_pushButton_bin_clicked();

    void on_pushButton_label_clicked();

    void on_pushButton_xml_2_clicked();

    void on_pushButton_bin_2_clicked();

    void on_pushButton_label_2_clicked();

    void on_tool_Camshift_clicked();

    void on_pushButton_gn18_clicked();

    void on_tool_colorfit_clicked();

    void on_treeView_File_clicked(const QModelIndex &index);

    void on_tool_dilate_clicked();

    void on_tool_erode_clicked();

    void on_tool_opening_clicked();

    void on_tool_closing_clicked();

    void on_gn_6_clicked();

    void on_ImglistWidget_clicked(const QModelIndex &index);

    void on_tool_MedSAM_clicked();

    void on_pb_gnAI_42_clicked();

    void on_pushButton_snake_clicked();

    void on_pb_diffusion_clicked();

    void on_tool_ROI_Diffusion_clicked();

private:
    Ui::MainWindow *ui;

    //Qt自带的类-文件系统模型
    MyFileSytemModel *file_model;
};
#endif // MAINWINDOW_H



