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


     /*dataTimer.stop();//stop map update when reading batt*/

     if(!(* calib_in_progress)) {

         printf("Inside read_battery_soc condition \n");
             /*if(batt_status->read_battery_soc() != CALIBRATION_IN_PROCESS)//read battery*/
             batt_status->start(QThread::LowestPriority);
             ui->batteryBar->setRange(0,100);
             /*else {
                 ui->batteryBar->setMaximum(0);
                 ui->batteryBar->setMinimum(0);

             }*/

     }
     /*if(S_P_FLAG!=1){dataTimer.start(5000);}//resume map updat*/
 }
 void MainWindow::batteryLevelChanged(int nValue){
     ui->batteryBar->setValue(nValue);
     QString battStyleSheet = " QProgressBar::chunk {"
     " background-color: ";

     if(30 >= nValue)
     {
         battStyleSheet.append("red;");
     }
     else if(80 >= nValue)
     {
         battStyleSheet.append("yellow;");
     }
     else
     {
         battStyleSheet.append("green;");
     }
     battStyleSheet.append("     width: 10px;"\
                         "     margin: 0.5px;"\
                         " }");
     ui->batteryBar->setStyleSheet(battStyleSheet);

 }


 void MainWindow::user_hint(QString hint){

     ui->hint_label->setFont(QFont("Times", 12, QFont::Bold));
     if(mode==SCREENING || mode==SPECTRUM || mode==OWTS_MODE)
        ui->hint_label->setParent(ui->customPlot);
     else
        ui->hint_label->setParent(ui->customPlot_avg);
     ui->hint_label->setText("Hint:"+hint);
     ui->hint_label->setAlignment(Qt::AlignCenter);
     ui->hint_label->setStyleSheet("QLabel { background-color : rgba(211,211,211,80%); color : black; }");
     ui->hint_label->show();
     hint_timer.start(6000);
 }

 void MainWindow::hide_hint(){
     ui->hint_label->setText("");
     ui->hint_label->hide();
     hint_timer.stop();
 }
