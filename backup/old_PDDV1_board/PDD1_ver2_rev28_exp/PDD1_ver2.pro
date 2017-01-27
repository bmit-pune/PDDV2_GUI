#-------------------------------------------------
#
# Project created by QtCreator 2014-12-24T17:37:42
#
#-------------------------------------------------

QT       += core gui widgets printsupport

QMAKE_CXXFLAGS += -fpermissive  -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=softfp

#LIBS +=  -L /home/nishad/Downloads/fft/cross_compile/install/lib -lfftw3f -lm

LIBS +=  -lm

INCLUDEPATH += /home/nishad/Downloads/fft/cross_compile/install/include

TARGET = PDD1_ver2
TEMPLATE = app


SOURCES += main.cpp\
    qcustomplot.cpp \
    mainwindow.cpp \
    keyboard.cpp \
    set_index.cpp \
    update_data.cpp \
    read_battery_soc.cpp


HEADERS  += mainwindow.h \
    qcustomplot.h \
    global_indexes.h \
    all_headers.h \
    read_batt.h



FORMS    += mainwindow.ui



OTHER_FILES += \

