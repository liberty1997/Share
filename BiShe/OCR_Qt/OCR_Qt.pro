TEMPLATE = app
TARGET = OCR_Qt

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += \
    /usr/include/tesseract \
    /usr/include/opencv4 \
    /usr/include/opencv4/opencv2

LIBS += -L/usr/lib/ -llept -ltesseract \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_imgcodecs \
    -lopencv_photo

SOURCES += \
    main.cpp \
    window.cpp

HEADERS += \
    untitled.h \
    window.h

FORMS += \
    untitled.ui
