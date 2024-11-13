#pragma once
#include <QGraphicsView>

#include "Dicomview/seriesbase.h"

class QGraphicsScene;
class QGraphicsPixmapItem;
class QGraphicsWidget;
class DicomView : public QGraphicsView
{
    Q_OBJECT

public:
    enum viewStatus {
        Browse,
        Zoom,//缩放
        Pan, // 移动
        Magnifier, // 放大镜
        Drawing,// 画
    };

    DicomView(QWidget *parent=0);
    ~DicomView();

    void setSeries(SeriesBase*series);
    void resizePixmapItem(); // 窗口改变时
    void repositionAuxItems(); // 窗口改变时
    void updateView();
    void setHighlight(bool yes); // 边框

    public slots:
    void setCurFrameItem(int);

signals:
    void windowChanged();// 窗宽位改变信号
    void clicked(SeriesBase::ViewType);// 点击

protected:
    void mousePressEvent(QMouseEvent *event); // 鼠标按住时触发
    void mouseMoveEvent(QMouseEvent *event); // 鼠标移动时触发
    void mouseReleaseEvent(QMouseEvent *event); // 鼠标释放后触发
    void wheelEvent(QWheelEvent *e); // 滚轮出发
    void resizeEvent(QResizeEvent *event); // 窗口调整时运行
    QSize sizeHint() const { return hintSize; }

private:
    QGraphicsScene *scene;// 场景
    QGraphicsPixmapItem *pixmapItem;//图像项 场景中的图像
    QGraphicsSimpleTextItem *posValueItem;// 文本项 显示当前鼠标值
    QGraphicsSimpleTextItem *curFrameItem; //  文本项 显示当前帧
    QGraphicsWidget *slider;// 控制条

    SeriesBase *dicoms; // 该视图显示的数据
    QSize hintSize;
    double factor;
    double fixFactor;// xspace/yspace 宽高的比例
    QPoint prevMousePos; //
    viewStatus status;
};
