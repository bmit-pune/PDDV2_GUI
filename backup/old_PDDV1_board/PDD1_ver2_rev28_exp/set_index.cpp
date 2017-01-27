#include"all_headers.h"

using namespace std;
 void MainWindow::set_index(QLabel * indexes,QLabel *threshold)
{
}

 void MainWindow::update_date_time()
 {
ui->date_time->setText(QDateTime::currentDateTime().toString());
 }
 void MainWindow::set_range()
 {

 //Index=Freq * N/fs
 //Here Index=?,Freq=10-350Mhz ,N=64K points,fs=3GhzMhz,(Max freq can be shown=fs/2)

     /*key1_noise.clear();
     y0_noise.clear();*/

     switch(RANGE_INDEX)
{
    /* case 0: RANGE_LOW=10*MEGA_HERTZ* FFT_VALS_DIVIDE_BY_FS;RANGE_HIGH=350*MEGA_HERTZ*FFT_VALS_DIVIDE_BY_FS;
     ui->range->setText("10-350 Mhz");
     printf("RANGE_LOW:%d",RANGE_LOW);printf("RANGE_HIGH:%d",RANGE_HIGH);
     break;

     case 1:RANGE_LOW=10*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);RANGE_HIGH=80*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);
     ui->range->setText("10-80 Mhz");
     break;

     case 2:RANGE_LOW=80*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);RANGE_HIGH=150*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);
     ui->range->setText("80-150 Mhz");
     break;

     case 3:RANGE_LOW=150*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);RANGE_HIGH=210*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);
     ui->range->setText("150-210 Mhz");
     break;

     case 4:RANGE_LOW=210*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);RANGE_HIGH=270*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);
     ui->range->setText("210-270 Mhz");
     break;

     case 5:RANGE_LOW=270*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);RANGE_HIGH=350*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);
     ui->range->setText("270 -350 Mhz");
     break;

     default:RANGE_LOW=10*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);RANGE_HIGH=350*MEGA_HERTZ*(FFT_VALS_DIVIDE_BY_FS);
     ui->range->setText("10-350 Mhz");
     RANGE_INDEX=0;
     break;*/

     default:RANGE_LOW=boundry_lower;RANGE_HIGH=boundry_upper-1;break;



}
 }

 void MainWindow::read_battery()
 {
     ui->batteryBar->setRange(0,100);
     dataTimer.stop();//stop map update when reading batt

     batt_status->read_battery_soc();//read battery

     if(PAUSE_FLAG!=1)
     dataTimer.start(5000);


 }


