#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<all_headers.h>




enum SCROLL {IN,OUT,NO_SCROLL,CLICKZOOM};
enum MODE   {SCREENING,SPECTRUM,LOC_MON,OWTS};


//#define boundry_lower 0
//#define boundry_upper NO_OF_PLOTTING_VALS

struct LUT
{
    int zoom_level[200];
    int skip_value[200];
    int L[200];

};


struct LOC_MON_AVG_MAX
{
    int location;
    int iteration;

    long long loc_avg[10];
    long long loc_max[10];

    long long  avg_temp;
    long long  max_temp;
    long long minutes_temp;

    long long avg_one_loc_minutes[60];
    long long max_one_loc_minutes[60];

};

struct OWTS
{
#define OWTS_steps 5

unsigned long owts_array[OWTS_steps][MAX_LOC_MON_LOCATIONS];
QLabel *    label[OWTS_steps][MAX_LOC_MON_LOCATIONS];

signed int adjx;
signed int adjy;


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
     QVector<double>key1_noise,y0_noise;
     QVector<double>key1_locmax,y0_locmax;
     QVector<double>key1_locavg,y0_locavg;


     QPoint point;
     QMessageBox msgBox;
     ThresholdTime dialog;
     int samples_received,Present_Zoom_Level,Next_Zoom_Level,Previous_Zoom_Level,PZL_lower,PZL_upper,Total_Zoom_Levels;
     //SCROLL scroll;
     LUT lut;
     LOC_MON_AVG_MAX loc;
     MODE mode;
     struct OWTS owts;

     read_batt *batt_status;
     int RANGE_LOW,RANGE_HIGH,RANGE_INDEX;
     int PAUSE_FLAG,CLICKZOOM_FLAG,clickzoom_min,clickzoom_max,LOC_MON_running;
     int NO_OF_PLOTTING_VALS,boundry_lower,boundry_upper;




    //functions
     void setupSimpleDemo(QCustomPlot *,QCustomPlot *,QCustomPlot *);
     void set_index(QLabel *indexes,QLabel *threshold);
     void set_range();
     void set_mode(MODE);
     void update_display_array(int clicked_pixel_point,SCROLL scroll);
     void take_screenshot(std::string location, std::string filename);
     static inline void copy_fft(uint32_t *src_addr,unsigned int *dest_addr) __attribute__((always_inline,unused));
     static inline void copy_raw(uint32_t *src_addr,char *dest_addr) __attribute__((always_inline,unused));
     inline float sample_to_millivolt(unsigned int);
     void OWTS_display();
     void OWTS_hide();


public slots:

     void update_data();
     void realtimeDataSlot();
     void update_date_time();
     void read_battery();
     void on_mouseWheel(QWheelEvent*e);
     void on_mousePress(QMouseEvent *event);
     void start_LOC_MON_slot();




protected:
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *event);





private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    QTimer batt1Timer;

};

#endif // MAINWINDOW_H
