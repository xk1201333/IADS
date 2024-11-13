#include "DicomView.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QMouseEvent>
#include <QDebug>
#include <qslider.h>

DicomView::DicomView(QWidget *parent): QGraphicsView(parent){
    scene = new QGraphicsScene(this);
    pixmapItem = new QGraphicsPixmapItem;
    posValueItem = new QGraphicsSimpleTextItem;
    curFrameItem = new QGraphicsSimpleTextItem;
    dicoms = 0;
    status = Browse;

    setFocusPolicy(Qt::StrongFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    setBackgroundBrush(QBrush(Qt::black));
    setAcceptDrops(true);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Plain);
    setAlignment(Qt::AlignCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setHighlight(false);

    scene->setSceneRect(-5000, -5000, 10000, 10000);
    setScene(scene);

    // 图片切换风格
    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    pixmapItem->setAcceptHoverEvents(true);
    posValueItem->setBrush(Qt::magenta);
    curFrameItem->setBrush(Qt::magenta);

    scene->addItem(pixmapItem);
    scene->addItem(posValueItem);
    scene->addItem(curFrameItem);

    slider = new QGraphicsWidget;

    QGraphicsLinearLayout *verticalLayout = new QGraphicsLinearLayout;
    verticalLayout->setOrientation(Qt::Vertical);
    verticalLayout->setSpacing(0);

    slider->setLayout(verticalLayout);

    scene->addItem(slider);
}

DicomView::~DicomView(){
}

void DicomView::setSeries(SeriesBase * series){
    if (!series)
        return;
    if (!series->isNormal())
        return;
    if (dicoms == 0){
        // 把dicoms->slider 添加到 slider
        QGraphicsProxyWidget*pw = scene->addWidget(series->slider);
        QGraphicsLinearLayout *verticalLayout = (QGraphicsLinearLayout *)slider->layout();
        verticalLayout->addItem(pw);
    }
    this->dicoms = series;
    dicoms->setDefaultWindow();

    emit windowChanged();

    connect(dicoms->slider, SIGNAL(valueChanged(int)), this, SLOT(setCurFrameItem(int)));
    dicoms->gotoFrame(dicoms->getFrameCount() / 2);


    double xSpacing = 0, ySpacing = 0;
    if (dicoms->getPixSpacing(xSpacing, ySpacing)) {
        if (xSpacing > 0.000001 && ySpacing > 0.000001) {
            double psX = xSpacing;
            double psY = ySpacing;
            fixFactor = psY / psX;
        }
    }

    pixmapItem->setPos(0, 0);
    pixmapItem->setRotation(0);
    pixmapItem->resetTransform();

    updateView();

    scene->update(scene->sceneRect());
}

void DicomView::resizePixmapItem(){
    if (!pixmapItem->pixmap().isNull()) {
        QRectF pixmapRect = pixmapItem->boundingRect();
        QRectF viewRect = this->rect();

        //if (!manualZoom) {
            if (pixmapRect.width()*viewRect.height() < pixmapRect.height()*fixFactor*viewRect.width())
                factor = viewRect.height() / (pixmapRect.height()*fixFactor);
            else
                factor = viewRect.width() / pixmapRect.width();
        //}

        pixmapItem->setTransform(QTransform(factor, 0, 0, factor*fixFactor, 0, 0));
        // if (!manualPan)
            centerOn(pixmapItem);
    }
}

void DicomView::repositionAuxItems(){
    QPointF sceneTL = mapToScene(rect().topLeft());
    QPointF sceneBR = mapToScene(rect().bottomRight());

    curFrameItem->setPos(sceneBR.x() - 68, sceneTL.y()+18);

    slider->setPos(sceneBR.x() - 58, sceneTL.y() + 58);

    QSizeF slider_size(58, sceneBR.y() - sceneTL.y() - 116);
    slider->resize(slider_size);
    posValueItem->setPos(sceneTL.x()+ 10, sceneBR.y()- 58);
}

void DicomView::updateView(){

    QPixmap pixmap;
    if (dicoms&&dicoms->isNormal()) {
        dicoms->getPixmap(pixmap);
        pixmapItem->setPixmap(pixmap);
        pixmapItem->setTransformOriginPoint(pixmapItem->boundingRect().center());

    }else {
        pixmapItem->setPixmap(pixmap);
        curFrameItem->setText("");
        posValueItem->setText("");
    }


    resizePixmapItem();
    repositionAuxItems();
}

void DicomView::setHighlight(bool yes){
    QPalette p = palette();
    if (yes){
        p.setColor(QPalette::Text, Qt::magenta);
    }else{
        p.setColor(QPalette::Text, Qt::gray);
    }
    setPalette(p);
}

void DicomView::setCurFrameItem(int){
    if (!dicoms || !dicoms->isNormal())
        return;
    curFrameItem->setText(tr("%1-%2").arg(dicoms->getCurFrame()).arg(dicoms->getFrameCount()));
}

void DicomView::mousePressEvent(QMouseEvent * event){
    if (dicoms&&dicoms->isNormal()){
        emit clicked(dicoms->getViewType());
        QPointF sp = mapToScene(event->pos());
        QPointF ip = pixmapItem->mapFromScene(sp);
        prevMousePos = event->pos();

        if (event->button() == Qt::LeftButton) {
            setDragMode(QGraphicsView::NoDrag);
            switch (status) {
            case Browse:
                if (!(event->modifiers()&Qt::ControlModifier)) {
                    setDragMode(QGraphicsView::RubberBandDrag);
                    setCursor(Qt::ArrowCursor); // 划框
                }
                break;
            }
        }
        QGraphicsView::mousePressEvent(event);
    }
}

void DicomView::mouseMoveEvent(QMouseEvent * event){
    if (dicoms&&dicoms->isNormal()) {
        QPointF sp = mapToScene(event->pos());
        QPointF ip = pixmapItem->mapFromScene(sp);
        if (pixmapItem->contains(ip)) {
            QPoint pos = ip.toPoint();
            QString s = dicoms->getPixelValue(pos.x(), pos.y());
            posValueItem->setText(tr("(%1, %2):").arg(pos.x()).arg(pos.y())+ s);
        }
    }

    // 按住右键调整窗宽窗位
    if (event->buttons()&Qt::RightButton) {
        // 调整窗宽位
        setDragMode(QGraphicsView::NoDrag);
        setCursor(Qt::ArrowCursor);
        QPoint delta = event->pos() - prevMousePos;
        dicoms->setWindowDelta(-delta.y() * 16, delta.x() * 16);
        emit windowChanged();
        updateView();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void DicomView::mouseReleaseEvent(QMouseEvent * event){
    if (dicoms&&dicoms->isNormal()) {
        if (event->button() == Qt::LeftButton) {
            switch (status) {
            case Browse:
                // 划框时结束时调整窗宽窗位
                if (scene->selectedItems().size() == 0){
                    if (rubberBandRect().isValid()){
                        if (dicoms->getViewType() == SeriesBase::XY){
                            dicoms->setRoiWindow(pixmapItem->mapFromScene(mapToScene(rubberBandRect())).boundingRect());
                            emit windowChanged();
                            updateView();
                        }

                    }
                }
                break;
            }
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void DicomView::wheelEvent(QWheelEvent * e){
    QPoint delta = e->angleDelta();
    if (dicoms == 0 || !dicoms->isNormal()) return;

    if (delta.y() > 0) dicoms->prevFrame();
    else if (delta.y() < 0) dicoms->nextFrame();
    updateView();
}

void DicomView::resizeEvent(QResizeEvent * event){
    resizePixmapItem();
    repositionAuxItems();
}
