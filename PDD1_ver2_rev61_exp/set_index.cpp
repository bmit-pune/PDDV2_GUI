#include"all_headers.h"

using namespace std;

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
     if(!(* calib_in_progress)) {

             batt_status->start(QThread::LowestPriority);
             ui->batteryBar->setRange(0,100);
     }
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
     hint_timer.start(4500);
 }

 void MainWindow::hide_hint(){
     ui->hint_label->setText("");
     ui->hint_label->hide();
     hint_timer.stop();
 }

 void MainWindow::switch_off_leds_except(int switch_on_led)
 {
 #ifdef BOARDGUI

     for (int i = 2000; i < 2010; i++) {
         switch (i) {
         case SCR_MNUMBER:
             GPIO.gpio_set_level(SCR_PORT, SCR_PIN, LEDOFF);
             break;
         case S_P_MNUMBER:
             GPIO.gpio_set_level(S_P_PORT, S_P_PIN, LEDOFF);
             break;
         case SCR_CAP_MNUMBER:
             GPIO.gpio_set_level(SCR_CAP_PORT, SCR_CAP_PIN, LEDOFF);
             break;
         case LOC_MNUMBER:
             GPIO.gpio_set_level(LOC_PORT, LOC_PIN, LEDOFF);
             break;
         case NOISE_MNUMBER:
             GPIO.gpio_set_level(NOISE_PORT, NOISE_PIN, LEDOFF);
             break;
         case DATA_STR_MNUMBER:
             GPIO.gpio_set_level(DATA_STR_PORT, DATA_STR_PIN, LEDOFF);
             break;
         case OWTS_MNUMBER:
             GPIO.gpio_set_level(OWTS_PORT, OWTS_PIN, LEDOFF);
             break;
         case GPS_MNUMBER:
             GPIO.gpio_set_level(GPS_PORT, GPS_PIN, LEDOFF);
             break;
         case DATA_RETV_MNUMBER:
             GPIO.gpio_set_level(DATA_RETV_PORT, DATA_RETV_PIN, LEDOFF);
             break;
         case SLEEP_MNUMBER:
             GPIO.gpio_set_level(SLEEP_PORT, SLEEP_PIN, LEDOFF);
             break;
         }
     }

     switch (switch_on_led) {
     case SCR_MNUMBER:
         GPIO.gpio_set_level(SCR_PORT, SCR_PIN, LEDON);
         break;
     case S_P_MNUMBER:
         GPIO.gpio_set_level(S_P_PORT, S_P_PIN, LEDON);
         break;
     case SCR_CAP_MNUMBER:
         GPIO.gpio_set_level(SCR_CAP_PORT, SCR_CAP_PIN, LEDON);
         break;
     case LOC_MNUMBER:
         GPIO.gpio_set_level(LOC_PORT, LOC_PIN, LEDON);
         break;
     case NOISE_MNUMBER:
         GPIO.gpio_set_level(NOISE_PORT, NOISE_PIN, LEDON);
         break;
     case DATA_STR_MNUMBER:
         GPIO.gpio_set_level(DATA_STR_PORT, DATA_STR_PIN, LEDON);
         break;
     case OWTS_MNUMBER:
         GPIO.gpio_set_level(OWTS_PORT, OWTS_PIN, LEDON);
         break;
     case GPS_MNUMBER:
         GPIO.gpio_set_level(GPS_PORT, GPS_PIN, LEDON);
         break;
     case DATA_RETV_MNUMBER:
         GPIO.gpio_set_level(DATA_RETV_PORT, DATA_RETV_PIN, LEDON);
         break;
     case SLEEP_MNUMBER:
         GPIO.gpio_set_level(SLEEP_PORT, SLEEP_PIN, LEDON);
         break;
     case NONE:;
     default:;
     }
     if(GPS_FLAG==1)
         switch_on_leds(GPS_MNUMBER); /*just an exception*/
     if(mode==SPECTRUM &&  NOISE_FLAG==1)
         switch_on_leds(NOISE_MNUMBER);/*just an exception*/
 #endif
 }

 void MainWindow::switch_on_leds(int switch_on_led)
 {

 #ifdef BOARDGUI

     switch (switch_on_led) {
     case SCR_MNUMBER:
         GPIO.gpio_set_level(SCR_PORT, SCR_PIN, LEDON);
         break;
     case S_P_MNUMBER:
         GPIO.gpio_set_level(S_P_PORT, S_P_PIN, LEDON);
         break;
     case SCR_CAP_MNUMBER:
         GPIO.gpio_set_level(SCR_CAP_PORT, SCR_CAP_PIN, LEDON);
         break;
     case LOC_MNUMBER:
         GPIO.gpio_set_level(LOC_PORT, LOC_PIN, LEDON);
         break;
     case NOISE_MNUMBER:
         GPIO.gpio_set_level(NOISE_PORT, NOISE_PIN, LEDON);
         break;
     case DATA_STR_MNUMBER:
         GPIO.gpio_set_level(DATA_STR_PORT, DATA_STR_PIN, LEDON);
         break;
     case OWTS_MNUMBER:
         GPIO.gpio_set_level(OWTS_PORT, OWTS_PIN, LEDON);
         break;
     case GPS_MNUMBER:
         GPIO.gpio_set_level(GPS_PORT, GPS_PIN, LEDON);
         break;
     case DATA_RETV_MNUMBER:
         GPIO.gpio_set_level(DATA_RETV_PORT, DATA_RETV_PIN, LEDON);
         break;
     case SLEEP_MNUMBER:
         GPIO.gpio_set_level(SLEEP_PORT, SLEEP_PIN, LEDON);
         break;
     case NONE:;
     default:;
     }
 #endif
 }


 void MainWindow::swich_off_leds_except_active_mode(){
 if(mode==SCREENING || mode==SPECTRUM){
     switch_off_leds_except(SCR_MNUMBER);
 }
 else if(mode==LOC_MON){
     switch_off_leds_except(LOC_MNUMBER);
 }
 else
     switch_off_leds_except(OWTS_MNUMBER);
 }

 void MainWindow::beep(DURATION duration){

     GPIO.gpio_set_level(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN,LEDON);

     if(duration==SHORT)
         buzzer_timer.start(400);
     else if(duration==LONG)
         buzzer_timer.start(1000);



     //printf("beep\n\nbeep\n\nbeep\n\n");

 }
 void MainWindow::beep_off(){
     buzzer_timer.stop();
     GPIO.gpio_set_level(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN,LEDOFF);
 }
