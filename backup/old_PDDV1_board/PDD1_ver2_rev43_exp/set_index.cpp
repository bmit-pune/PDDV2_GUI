#include"all_headers.h"

using namespace std;
 void MainWindow::set_index(QLabel * indexes,QLabel *threshold)
{
}

 void MainWindow::update_date_time()
 {
     if(mode!=LOC_MON)
    ui->date_time->setText(QDateTime::currentDateTime().toString());

 }


 void MainWindow::set_range()
 {
     switch(RANGE_INDEX) {
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


