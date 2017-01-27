#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<all_headers.h>
#include <QLinearGradient>
#include "dataupdate.h"


enum SCROLL {IN,OUT,NO_SCROLL,CLICKZOOM};
enum MODE   {SCREENING,SPECTRUM,LOC_MON,OWTS_MODE};
extern MODE mode;

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

    double loc_avg[10];
    double loc_max[10];

    double  avg_temp;
    double  max_temp;
    unsigned long long minutes_temp;

    double avg_one_loc_minutes[60];
    double max_one_loc_minutes[60];

    short int monitored_locations[10]; //Log which locations have been monitored


};

struct OWTS
{
#define OWTS_steps 5

unsigned long owts_array[OWTS_steps][MAX_LOC_MON_LOCATIONS];
QWidget *    severity_label[MAX_LOC_MON_LOCATIONS];

signed int adjx;
signed int adjy;

signed int sev_label_adjx;
signed int sev_label_adjy;

unsigned long severity_arr_loc0[5];
unsigned long severity_arr_loc1[5];
unsigned long severity_arr_loc2[5];
unsigned long severity_arr_loc3[5];
unsigned long severity_arr_loc4[5];
unsigned long severity_arr_loc5[5];
unsigned long severity_arr_loc6[5];
unsigned long severity_arr_loc7[5];
unsigned long severity_arr_loc8[5];
unsigned long severity_arr_loc9[5];

QLinearGradient *gradient;
QPalette *palette;

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
     menu Menu;


     int samples_received,Present_Zoom_Level,Next_Zoom_Level,Previous_Zoom_Level,PZL_lower,PZL_upper,Total_Zoom_Levels;

     //SCROLL scroll;
     LUT lut;
     LOC_MON_AVG_MAX loc;
     //MODE mode;
     struct OWTS owts;
     dataUpdate * DATAUpdate;
     read_batt *batt_status;

     int RANGE_LOW,RANGE_HIGH,RANGE_INDEX,gps_counter,rtc_flag;
     int CLICKZOOM_FLAG,clickzoom_min,clickzoom_max,LOC_MON_running,LOC_MON_first_transition;
     int NO_OF_PLOTTING_VALS,boundry_lower,boundry_upper;
     unsigned int FPGA_DATUM;
     unsigned int FFT_RANGE;
     QString __latitude,__longitude;
     std::string gps_date,gps_time;
     gpio GPIO;
     short int SCR_FLAG,S_P_FLAG,SCR_CAP_FLAG,LOC_FLAG,NOISE_FLAG,DATA_STR_FLAG,OWTS_FLAG,GPS_FLAG,DATA_RETV_FLAG,SLEEP_FLAG,SHUTDOWN_FLAG;
     int * calib_in_progress;
     int graph_min, graph_max,graph_min_earlier,graph_max_earlier,GRAPH_MIN_MAX_CHANGED,RECORD_ONCE;



    //functions
     void setupSimpleDemo(QCustomPlot *,QCustomPlot *,QCustomPlot *);
     void set_index(QLabel *indexes,QLabel *threshold);
     int read_gps_direct();
     void rtc_program();
     std::string exec(std::string string_cmd);
     void set_range();
     void set_mode(MODE);
     void update_display_array(int clicked_pixel_point,SCROLL scroll);
     void take_screenshot(std::string location, std::string filename);
     static inline void copy_fft(uint32_t *src_addr,unsigned int *dest_addr) __attribute__((always_inline,unused));
     static inline void copy_raw(uint32_t *src_addr,unsigned short int *dest_addr) __attribute__((always_inline,unused));
     inline float sample_to_millivolt(unsigned int);
     void OWTS_display();
     int * index_sort_owts(unsigned long temp[]);
     void set_OWTS_label(int location,int* recv_index);
     void OWTS_hide();
     void LOC_MON_OWTS_data_clean();
     void LOC_MON_OWTS_overwrite_loc_data_clean(int Set_Location);
     void switch_off_leds_except(int);
     void switch_on_leds(int);
     void swich_off_leds_except_active_mode();
     void user_hint(QString hint);
     double set_precision(double number,int precision);



public slots:

     void update_data();
     void realtimeDataSlot();
     void update_date_time();
     void read_battery();
     void batteryLevelChanged(int);
     void on_mouseWheel(QWheelEvent*e);
     void on_mousePress(QMouseEvent *event);
     void start_LOC_MON_slot();
     void hide_hint();
     void start_timer();


protected:
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *event);

private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    QTimer batt1Timer;
    QTimer hint_timer;

};

#endif // MAINWINDOW_H
