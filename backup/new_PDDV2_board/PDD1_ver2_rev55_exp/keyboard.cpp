#include <all_headers.h>

using namespace std;
void MainWindow::keyPressEvent(QKeyEvent* e)

{

    if(!(dialog.isHidden()) || SLEEP_FLAG==1 && (e->key() != Qt::Key_E) )
        return; /*No action when in sleep or user is selecting params in LOC_MON*/

    else if ((e->key() == Qt::Key_L) && (e->isAutoRepeat() == false)) //LOC_MON
    {

        if(LOC_MON_running==1){
            user_hint(" Aldready running");
            return;
        }
        if (mode != LOC_MON && mode != OWTS_MODE){
            LOC_MON_first_transition = 1; //first time moved to LOC_MON from other mode(except OWTS & LOC_MON itself)
            user_hint("Switching mode...");}
        ui->range->setText("LOCATION MONITORING");
        dataTimer.stop();

#ifdef DEBUG
        qDebug() << "Location Monitoring Mode activated\n";
#endif
        /*dialog.Set_Threshold=0;
        dialog.Set_Time=0;
        dialog.show();*/
        set_mode(LOC_MON);
        swich_off_leds_except_active_mode();
    }

    else if ((e->key() == Qt::Key_M) && (e->isAutoRepeat() == false)) //Change Mode
    {
        user_hint("Switching mode...");
        dataTimer.stop();
        if (mode == SCREENING)
            set_mode(SPECTRUM);
        else if (mode == SPECTRUM)
            set_mode(SCREENING);
        else if (mode == LOC_MON)
            set_mode(SPECTRUM);
        else if (mode == OWTS_MODE)
            set_mode(SCREENING);
        dataTimer.start(5000);
        swich_off_leds_except_active_mode();
    }

    else if ((e->key() == Qt::Key_S) && (e->isAutoRepeat() == false)) //SCREENSHOT
    {
        switch_off_leds_except(SCR_CAP_MNUMBER);
        std::ostringstream oss;
        std::string date = QDate::currentDate().toString().replace(" ","_").toStdString();
        exec("mkdir -p /home/root/data");
        exec("mkdir -p /home/root/data/screenshots");
        exec("mkdir -p /home/root/data/screenshots/" + date);
        std::string temp = QTime::currentTime().toString().replace(":", "_", Qt::CaseInsensitive).toStdString();
        take_screenshot("/home/root/data/screenshots/" + date + "/", temp);
        user_hint(" "+QString::fromUtf8(temp.c_str())  + ".png saved." );
        swich_off_leds_except_active_mode();

    }

    else if ((e->key() == Qt::Key_N) && (e->isAutoRepeat() == false)) //NOISE CAPTURE
    {
        if (!(mode == SPECTRUM)){
            user_hint("Noise Capture-Only in Screening-FFT mode");
            return;}
        if(NOISE_FLAG==0){
        switch_off_leds_except(NOISE_MNUMBER);
        dataTimer.stop();
        key1_noise.clear();
        y0_noise.clear();
        QThread::msleep(2000); //Wait till capture completes
        update_data(); //Capture new set of data
        if (mode == SPECTRUM) {
            for (int i = 0; i < NO_OF_PLOTTING_VALS; i++) {
                arr_float_noise[i] = arr_float[i];
            }
        }
        user_hint("Noise captured.");
        dataTimer.start(5000);
        swich_off_leds_except_active_mode();
        NOISE_FLAG=1;
        }
        else if(NOISE_FLAG==1) {
            swich_off_leds_except_active_mode();
            user_hint("Noise cleared.");
            key1_noise.clear();
            y0_noise.clear();
            memset(arr_float_noise,0,sizeof(arr_float_noise));
            NOISE_FLAG=0;
        }
    }



    else if ((e->key() == Qt::Key_O) && (e->isAutoRepeat() == false)) //owts
    {
        /*----------check if OWTS is clicked before LOC MON---------*/
        short int check=0;
        for(int i=0;i<MAX_LOC_MON_LOCATIONS;i++){
            if(loc.monitored_locations[i]!=0)
                check++;
        }
        if (check==0){
        user_hint("Atleast one location needs to be monitored.");
        swich_off_leds_except_active_mode();
        return;
        }
        /*---------------------------------------------------------*/

        if (LOC_MON_running == 1){
            user_hint("Works only when Location Monitoring halts.");
            return;}
        dataTimer.stop();

#ifdef DEBUG
        qDebug() << "OWTS Mode activated\n";
#endif

        set_mode(OWTS_MODE);
        swich_off_leds_except_active_mode();
    }

  /*  if ((e->key() == Qt::Key_R) && (e->isAutoRepeat() == false)) { //change range of FFT
    }

    if ((e->key() == Qt::Key_I) && (e->isAutoRepeat() == false)) //RANGE INCREASE
    {
        RANGE_INDEX++;
        set_range();
        qDebug() << "key pressed in main\n\n";
    }
    if ((e->key() == Qt::Key_D) && (e->isAutoRepeat() == false)) //RANGE DECREASE
    {
        RANGE_INDEX--;
        set_range();
    }*/

    else if ((e->key() == Qt::Key_T) && (e->isAutoRepeat() == false)) //STORE FILE
    {
        if(!(mode==SCREENING || mode==SPECTRUM)){
            user_hint("Applicable only in Screening mode");
            return;}
        dataTimer.stop();
        QThread::msleep(200);
        switch_off_leds_except(DATA_STR_MNUMBER);
        std::string date = QDate::currentDate().toString().replace(" ","_").toStdString();
        exec("mkdir -p /home/root/data");
        exec("mkdir -p /home/root/data/data_files/");
        exec("mkdir -p /home/root/data/data_files/" + date);
        std::string temp;
        if (mode==SCREENING)
            temp = "Time_dimain_" + QTime::currentTime().toString().replace(":", "_", Qt::CaseInsensitive).toStdString();
        else if(mode==SPECTRUM)
            temp = "FFT_" + QTime::currentTime().toString().replace(":", "_", Qt::CaseInsensitive).toStdString();

        std::string filename = "/home/root/data/data_files/" + date + "/"+ temp + ".csv";
        FILE* pFile;
        pFile = fopen(filename.c_str(), "w");


            if (mode == SCREENING){
                for(int i=0;i<NO_OF_RAW_VALS;i++){
                    fprintf(pFile, "%d ,", array[i]);
                }
            }

            if (mode == SPECTRUM){
                for(int i=0;i<NO_OF_FFT_VALS;i++){
                    fprintf(pFile, "%f ,", arr_float[i]);
                }
            }
        fclose(pFile);
        //msgBox.setText(" file stored");
        user_hint("File Stored !!!");
        //qDebug() << filename;
        //msgBox.exec();
        swich_off_leds_except_active_mode();
        if(S_P_FLAG!=1)
            dataTimer.start(5000);
    }

    else if ((e->key() == Qt::Key_R) && (e->isAutoRepeat() == false)){ //Data retv

    exec("mkdir -p /mnt/mountpoint");
    if(mount("/dev/sda1","/mnt/mountpoint","vfat",MS_SYNCHRONOUS,NULL == -1)){
        user_hint("Detect ERROR-please use vfat formatted pendrive.");
        return;}

    exec("mkdir /mnt/mountpoint/PDDV2_data");
    user_hint("PDDV2_data folder created-wait...");
    exec("cp -r /home/root/data /mnt/mountpoint/PDDV2_data");
    umount("/mnt/mountpoint");
    user_hint("Copied-You can safely eject pendrive.");
    }


    else if ((e->key() == Qt::Key_P) && (e->isAutoRepeat() == false)) //PAUSE
    {
#ifdef DEBUG
        qDebug() << "Pause key pressed\n";
#endif

        if (S_P_FLAG == 0) {
            if (mode == SCREENING || mode == SPECTRUM)
                switch_off_leds_except(S_P_MNUMBER);
            else if (mode == LOC_MON || mode == OWTS_MODE){
                user_hint(" Applicable only in Screening mode.");
                return;}
            dataTimer.stop();
            S_P_FLAG = 1;
        }
        else if (S_P_FLAG == 1) {
            dataTimer.start(5000);
            S_P_FLAG = 0;
            swich_off_leds_except_active_mode();
        }
    }
    else if ((e->key() == Qt::Key_G) && (e->isAutoRepeat() == false)){ //GPS
    if(GPS_FLAG==0){
        #ifdef BOARDGUI
        GPIO.gpio_set_level(GPS_GPIO_PORT,GPS_GPIO_PIN,LEDON);
        #endif
        switch_on_leds(GPS_MNUMBER);
        GPS_FLAG=1;
    }
    else if(GPS_FLAG==1){
    GPS_FLAG=0;
    #ifdef BOARDGUI
    GPIO.gpio_set_level(GPS_GPIO_PORT,GPS_GPIO_PIN,LEDOFF);
    #endif
    swich_off_leds_except_active_mode();
    }
    }

    else if ((e->key() == Qt::Key_E) && (e->isAutoRepeat() == false)) {   //SLEEP
        if (SLEEP_FLAG == 0) {
            switch_off_leds_except(SLEEP_MNUMBER);
            exec("echo 0 > /sys/class/backlight/backlight.34/brightness");
            dataTimer.stop();
            SLEEP_FLAG = 1;
        }
        else if (SLEEP_FLAG == 1) {

            exec("echo 7 > /sys/class/backlight/backlight.34/brightness");
            dataTimer.start(5000);
            SLEEP_FLAG = 0;
            swich_off_leds_except_active_mode();
        }
    }

    else if ((e->key() == Qt::Key_W) && (e->isAutoRepeat() == true)){ //Shutdown
    //exec("echo 0 > /sys/class/backlight/backlight.34/brightness");
    switch_off_leds_except(NONE);
    exec("poweroff");
    }
}

//connect to Qcustomplot wheel event
void MainWindow::wheelEvent(QWheelEvent* event)
{
    on_mouseWheel(event);
}

void MainWindow::on_mouseWheel(QWheelEvent* e)
{
    if (mode != SCREENING){
        user_hint("Applicable only in Screening-Time Domain");
        return;}

    SCROLL scroll;

    point = e->pos();
    int x = point.x();
    int y = point.y();
    int pixel_abs = x - 77;
    qDebug("\n\n#######pixel_abs=%d\n\n", x);
    int clicked_pixel_point;
    if (mode == SCREENING)
        clicked_pixel_point = pixel_abs * (NO_OF_RAW_VALS / 630);
/*  else if(mode==SPECTRUM)
            clicked_pixel_point=pixel_abs* (((NO_OF_FFT_VALS/2)-11)/630); */
#ifdef DEBUG
    printf("clicked_pixel_point is:%d\n", clicked_pixel_point);
#endif

    //qDebug("clicked_pixel_point=%d",clicked_pixel_point);

    if (e->angleDelta().y() < 0)
        update_display_array(clicked_pixel_point, scroll = OUT);
    else
        update_display_array(clicked_pixel_point, scroll = IN);
}

void MainWindow::on_mousePress(QMouseEvent* event)
{
    if (mode != SPECTRUM)
    {user_hint("Mouse Click zoom-only in Screening-FFT mode");
        return;}

    QPoint point;
    point = event->pos();
    int x = point.x();
    int pixel_abs = x - 61;
    qDebug("\n\n#######pixel_abs=%d\n\n", x);
    int clicked_pixel_point;
    if (event->button() == Qt::LeftButton) {
        /*if(mode==SCREENING)
                clicked_pixel_point=pixel_abs* (NO_OF_RAW_VALS/373.72);
            else */ if (mode == SPECTRUM)
            clicked_pixel_point = pixel_abs * (((NO_OF_FFT_VALS / 2) - 10) / 644);
    }

    if (CLICKZOOM_FLAG == 0) {
        update_display_array(clicked_pixel_point, CLICKZOOM);
        CLICKZOOM_FLAG = 1;
    }
    else if (CLICKZOOM_FLAG == 1) {
        update_display_array(clicked_pixel_point, NO_SCROLL);
        CLICKZOOM_FLAG = 0;
    }
    qDebug() << clicked_pixel_point << "is clicked\n";
}

void MainWindow::take_screenshot(std::string location, std::string filename)
{ //store screenshot to received location


        /*screenshot effect*/
        exec("echo 2 > /sys/class/backlight/backlight.34/brightness");
        QThread::msleep(450);
        exec("echo 7 > /sys/class/backlight/backlight.34/brightness");


    std::string temp = location + filename + ".png";
    QString path = QString::fromUtf8(temp.c_str());
    QScreen* screen = QGuiApplication::primaryScreen();
    screen->grabWindow(QApplication::desktop()->winId(), 0, 0,
              QApplication::desktop()->screenGeometry().width(),
              QApplication::desktop()->screenGeometry().height())
        .save(path, 0, -1);



    //msgBox.setText("Screenshot Captured!!!");
    //msgBox.exec();

    //QCPAbstractPlottable *temp3;
    //QPointF point= temp3->coordsToPixels(160,160);
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
