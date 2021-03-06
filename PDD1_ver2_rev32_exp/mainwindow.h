#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<all_headers.h>
#include"read_batt.h"

enum SCROLL {IN,OUT,NO_SCROLL};
enum MODE   {SCREENING,SPECTRUM};


//#define boundry_lower 0
//#define boundry_upper NO_OF_PLOTTING_VALS

struct LUT
{
    int zoom_level[200];
    int skip_value[200];
    int L[200];

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     int DATUM;
     QVector<double>key1,y0;
     /*QVector<double>key1_noise,y0_noise;*/

     QPoint point;

     QMessageBox msgBox;
     int samples_received,Present_Zoom_Level,Next_Zoom_Level,Previous_Zoom_Level,PZL_lower,PZL_upper,Total_Zoom_Levels;
     //SCROLL scroll;
     LUT lut;
     MODE mode;
     read_batt *batt_status;
     int RANGE_LOW,RANGE_HIGH,RANGE_INDEX;
     int PAUSE_FLAG;
     int NO_OF_PLOTTING_VALS,boundry_lower,boundry_upper;




     void setupSimpleDemo(QCustomPlot *customPlot);
     void set_index(QLabel *indexes,QLabel *threshold);
     void set_range();
     void set_mode(MODE);
     void update_display_array(int clicked_pixel_point,SCROLL scroll);
     void take_screenshot(std::string location, std::string filename);
     static inline void copy_fft(uint32_t *src_addr,unsigned int *dest_addr) __attribute__((always_inline,unused));
     static inline void copy_raw(uint32_t *src_addr,char *dest_addr) __attribute__((always_inline,unused));



public slots:

     void update_data();
     void realtimeDataSlot();
     void update_date_time();
     void read_battery();
     void on_mouseWheel(QWheelEvent*e);



protected:
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *event);



private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    QTimer batt1Timer;

};

#endif // MAINWINDOW_H
