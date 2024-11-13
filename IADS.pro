QT += core gui sql network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    Algorithm/ocvlivewire.cpp \
    Algorithm/snakeimage.cpp \
    Algorithm/splitandmerge.cpp \
    Dicomview/colorindextable.cpp \
    Dicomview/image.cpp \
    Dicomview/mygobal.cpp \
    Dicomview/readworker.cpp \
    Dicomview/seriesbase.cpp \
    Dicomview/xyseries.cpp \
    Dicomview/xzseries.cpp \
    Dicomview/yzseries.cpp \
    ROI/drawroi.cpp \
    ROI/graphicsviewtool.cpp \
    ROI/roigraphicsview.cpp \
    models/fastsam.cpp \
    dicomview.cpp \
    ftpwindow.cpp \
    loginregister.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    myqgraphicsview.cpp \
    personalcenter.cpp \
    roilabel.cpp



HEADERS += \
    Algorithm/ocvlivewire.h \
    Algorithm/snakeimage.h \
    Algorithm/splitandmerge.h \
    Dicomview/colorindextable.h \
    Dicomview/image.h \
    Dicomview/mygobal.h \
    Dicomview/readworker.h \
    Dicomview/seriesbase.h \
    Dicomview/xyseries.h \
    Dicomview/xzseries.h \
    Dicomview/yzseries.h \
    ROI/drawroi.h \
    ROI/graphicsviewtool.h \
    ROI/roigraphicsview.h \
    models/fastsam.h \
    dicomview.h \
    ftpwindow.h \
    loginregister.h \
    mainwindow.h \
    matrix.h \
    myqgraphicsview.h \
    personalcenter.h \
    roilabel.h


FORMS += \
    ftpwindow.ui \
    loginregister.ui \
    mainwindow.ui \
    matrix.ui \
    personalcenter.ui

TRANSLATIONS += \
    IADS_zh_CN.ts \
    IADS_en.ts


#设置opencv
INCLUDEPATH += C:\openlib\opencv-4.9.0\opencv\build\include
INCLUDEPATH +=C:\openlib\opencv-4.9.0\opencv\build\include\opencv2
LIBS +=C:\openlib\opencv-4.9.0\opencv\build\x64\vc16\lib\opencv_world490.lib


#设置 ffmpeg
FFMPEG_HOME=C:\openlib\ffmpeg-n4.4.1-win64-gpl-shared-4.4
INCLUDEPATH += $$FFMPEG_HOME/include
LIBS +=-L$$FFMPEG_HOME\lib \
         -lavcodec \
         -lavdevice \
         -lavfilter \
         -lavformat \
         -lavutil \
         -lpostproc \
         -lswresample \
         -lswscale


#设置DCMTK
INCLUDEPATH += G:\cpplibrary\DCMTK\dcmtk-build\include
LIBS += -L$$quote("G:/cpplibrary/DCMTK/dcmtk-build/lib/")
LIBS += cmr.lib\
        dcmdata.lib\
        dcmdsig.lib\
        dcmect.lib\
        dcmfg.lib\
        dcmimage.lib\
        dcmimgle.lib\
        dcmiod.lib\
        dcmjpeg.lib\
        dcmjpls.lib\
        dcmnet.lib\
        dcmpmap.lib\
        dcmpstat.lib\
        dcmqrdb.lib\
        dcmrt.lib\
        dcmseg.lib\
        dcmsr.lib\
        dcmtkcharls.lib\
        dcmtls.lib\
        dcmtract.lib\
        dcmwlm.lib\
        i2d.lib\
        ijg12.lib\
        ijg16.lib\
        ijg8.lib\
        oflog.lib\
        ofstd.lib



#设置openvino
INCLUDEPATH += C:\openlib\openvino_2023.3.0\runtime\include
INCLUDEPATH += C:\openlib\openvino_2023.3.0\runtime\include\openvino
INCLUDEPATH += C:\openlib\openvino_2023.3.0\runtime\include\ngraph
INCLUDEPATH += C:\openlib\openvino_2023.3.0\runtime\include\ie

LIBS += -L$$quote(C:\openlib\openvino_2023.3.0\runtime\3rdparty\tbb\lib) \
        -ltbb \
        -ltbb12 \
        -ltbbbind_2_5 \
        -ltbbmalloc \
        -ltbbmalloc_proxy

LIBS += -L$$quote(C:\openlib\openvino_2023.3.0\runtime\lib\intel64\Release) \
        -lopenvino \
        -lopenvino_pytorch_frontend \
        -lopenvino_c \
        -lopenvino_onnx_frontend

#设置onnxruntime
INCLUDEPATH +=C:\openlib\onnxruntime-win-x64-1.13.1\include
LIBS += -L$$quote(C:\openlib\onnxruntime-win-x64-1.13.1\lib) \
        -lonnxruntime \
        -lonnxruntime_providers_shared

#设置 vtk
VTKPATH = "C:\Program Files\VTK"
INCLUDEPATH +=$${VTKPATH}\include\vtk-8.2
LIBS +=$${VTKPATH}\lib\vtkChartsCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonColor-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonComputationalGeometry-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonDataModel-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonExecutionModel-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonMath-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonMisc-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonSystem-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkCommonTransforms-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkDICOMParser-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkDomainsChemistry-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkDomainsChemistryOpenGL2-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkdoubleconversion-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkexodusII-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkexpat-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersAMR-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersExtraction-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersFlowPaths-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersGeneral-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersGeneric-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersGeometry-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersHybrid-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersHyperTree-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersImaging-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersModeling-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersParallel-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersParallelImaging-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersPoints-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersProgrammable-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersSelection-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersSMP-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersSources-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersStatistics-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersTexture-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersTopology-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkFiltersVerdict-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkfreetype-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkGeovisCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkgl2ps-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkglew-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkGUISupportQt-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkGUISupportQtSQL-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkhdf5-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkhdf5_hl-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingColor-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingFourier-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingGeneral-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingHybrid-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingMath-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingMorphological-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingSources-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingStatistics-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkImagingStencil-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkInfovisCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkInfovisLayout-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkInteractionImage-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkInteractionStyle-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkInteractionWidgets-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOAMR-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOAsynchronous-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOCityGML-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOEnSight-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOExodus-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOExport-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOExportOpenGL2-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOExportPDF-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOGeometry-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOImage-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOImport-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOInfovis-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOLegacy-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOLSDyna-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOMINC-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOMovie-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIONetCDF-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOParallel-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOParallelXML-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOPLY-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOSegY-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOSQL-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOTecplotTable-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOVeraOut-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOVideo-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOXML-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkIOXMLParser-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkjpeg-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkjsoncpp-8.2.lib
LIBS +=$${VTKPATH}\lib\vtklibharu-8.2.lib
LIBS +=$${VTKPATH}\lib\vtklibxml2-8.2.lib
LIBS +=$${VTKPATH}\lib\vtklz4-8.2.lib
LIBS +=$${VTKPATH}\lib\vtklzma-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkmetaio-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkNetCDF-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkogg-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkParallelCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkpng-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkproj-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkpugixml-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingAnnotation-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingContext2D-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingContextOpenGL2-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingFreeType-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingGL2PSOpenGL2-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingImage-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingLabel-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingLOD-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingOpenGL2-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingQt-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingVolume-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkRenderingVolumeOpenGL2-8.2.lib
LIBS +=$${VTKPATH}\lib\vtksqlite-8.2.lib
LIBS +=$${VTKPATH}\lib\vtksys-8.2.lib
LIBS +=$${VTKPATH}\lib\vtktheora-8.2.lib
LIBS +=$${VTKPATH}\lib\vtktiff-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkverdict-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkViewsContext2D-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkViewsCore-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkViewsInfovis-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkViewsQt-8.2.lib
LIBS +=$${VTKPATH}\lib\vtkzlib-8.2.lib



#设置CUDA
INCLUDEPATH += C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\include
LIBS += -L$$quote(C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\lib\x64) \
        -lcublas \
        -lcublasLt \
        -lcuda \
        -lcudadevrt \
        -lcudart \
        -lcudart_static \
        -lcufft \
        -lcufftw \
        -lcurand \
        -lcusolver \
        -lcusolverMg \
        -lcusparse \
        -lnppc \
        -lnppial \
        -lnppicc \
        -lnppidei \
        -lnppif \
        -lnppig \
        -lnppim \
        -lnppist \
        -lnppisu \
        -lnppitc \
        -lnpps \
        -lnvblas \
        -lnvjpeg \
        -lnvml \
        -lnvptxcompiler_static \
        -lnvrtc \
        -lOpenCL


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qss.qrc \
    res.qrc

DISTFILES += \
    IADS_en.ts



# CUDA 设置
#CUDA_SOURCES += cuda_functions.cu
#CUDA_DIR = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3"
#SYSTEM_NAME = x64
#SYSTEM_TYPE = 64
#CUDA_ARCH  = sm_61
#NVCC_OPTIONS = --use_fast_math

## 头文件路径
#INCLUDEPATH += "$$CUDA_DIR\include"

## 导入库文件路径
#QMAKE_LIBDIR += "$$CUDA_DIR/lib/x64"
#CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')


## 依赖库
#CUDA_LIB_NAMES += \
#    cuda \
#    cudadevrt \
#    cudart \

#CONFIG(debug, debug|release) {
#    CUDA_LIB_NAMES += ucrtd \
#}else{
#    CUDA_LIB_NAMES += ucrt \
#}

#for(lib, CUDA_LIB_NAMES) {
#    CUDA_LIBS += $$lib.lib
#}
#for(lib, CUDA_LIB_NAMES) {
#    NVCC_LIBS += -l$$lib
#}


#LIBS += $$NVCC_LIBS
#MSVCRT_LINK_FLAG_DEBUG   = "/MDd"
#MSVCRT_LINK_FLAG_RELEASE = "/MD"

## 这部分内容，实质上就是在Qt中调用环境中的命令行工具来先编译生成bilinear.obj
#CONFIG(debug, debug|release) {
#    # Debug 模式
#    OBJECTS_DIR = debug/obj
#    CUDA_OBJECTS_DIR = debug
#    cuda_d.input = CUDA_SOURCES
#    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}.obj
#    cuda_d.commands = $$CUDA_DIR/bin/nvcc.exe -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$LIBS \
#                      --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH \
#                      --compile -cudart static -g -DWIN32 -D_MBCS \
#                      -Xcompiler "/wd4819,/EHsc,/W3,/nologo,/Od,/Zi,/RTC1" \
#                      -Xcompiler $$MSVCRT_LINK_FLAG_DEBUG \
#                      -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
#    cuda_d.dependency_type = TYPE_C
#    QMAKE_EXTRA_COMPILERS += cuda_d
#}
#else {
#    # Release 模式
#    OBJECTS_DIR = release/obj
#    CUDA_OBJECTS_DIR = release
#    cuda.input = CUDA_SOURCES
#    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}.obj
#    cuda.commands = $$CUDA_DIR/bin/nvcc.exe $$NVCC_OPTIONS $$CUDA_INC $$LIBS \
#                    --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH \
#                    --compile -cudart static -D_MBCS \
#                    -Xcompiler "/wd4819,/EHsc,/W3,/nologo,/O3,/Zi" \
#                    -Xcompiler $$MSVCRT_LINK_FLAG_RELEASE \
#                    -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
#    cuda.dependency_type = TYPE_C
#    QMAKE_EXTRA_COMPILERS += cuda
#}

#DISTFILES += cuda_functions.cu







