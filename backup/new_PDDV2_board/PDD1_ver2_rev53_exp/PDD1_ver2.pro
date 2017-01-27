#-------------------------------------------------
#
# Project created by QtCreator 2014-12-24T17:37:42
#
#-------------------------------------------------

QT       += core gui widgets printsupport



#QMAKE_CXXFLAGS += -fpermissive -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9


LIBS +=  -lm

TARGET = PDD1_ver2
TEMPLATE = app


SOURCES += main.cpp\
    qcustomplot.cpp \
    mainwindow.cpp \
    keyboard.cpp \
    set_index.cpp \
    update_data.cpp \
    read_battery_soc.cpp \
    mode_select.cpp \
    ThresholdTime.cpp


HEADERS  += mainwindow.h \
    qcustomplot.h \
    global_indexes.h \
    all_headers.h \
    read_batt.h \
    ThresholdTime.h

FORMS    += mainwindow.ui \
    ThresholdTime.ui

OTHER_FILES += \
