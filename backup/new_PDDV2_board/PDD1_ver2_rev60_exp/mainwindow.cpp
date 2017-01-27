#include "all_headers.h"
#include "global_indexes.h"

unsigned short int array[NO_OF_RAW_VALS];
float arr_float[NO_OF_FFT_VALS];
float arr_float_noise[NO_OF_FFT_VALS];
float arr_float_mV[NO_OF_RAW_VALS];
MODE mode;


using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    calib_in_progress = (int *)mmap(NULL, sizeof *calib_in_progress, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *calib_in_progress = 0;



    if (fork() == 0)
    {
      *calib_in_progress = 1;
       batt_status->batt_calibrate();
      *calib_in_progress = 0;
       exit(EXIT_SUCCESS);
    }
    else{


    ui->setupUi(this);
    GPIO.gpio_set_level(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN,LEDOFF);
    beep(SHORT);





    ui->centralWidget->setStyleSheet("color: blue;"
                                     "background-color: white;"
                                     "selection-color: black;"
                                     "selection-background-color: blue;");

    setupSimpleDemo(ui->customPlot, ui->customPlot_avg, ui->customPlot_max);



    update_data();

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSimpleDemo(QCustomPlot* customPlot, QCustomPlot* customPlot_avg, QCustomPlot* customPlot_max)

{
#ifdef DEBUG
    cout << "Setup_simple_demo_started" << endl;
#endif

    customPlot->setBackground(Qt::white);


    QPen pen1(QBrush(QColor(10, 10, 250, 255)), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen1.setWidth(2);

    QPen pen2(QBrush(QColor(250, 10, 10, 100)), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen1.setWidth(2);

    QPen pen3(QBrush(QColor(250, 10, 10, 255)), 3, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    pen1.setWidth(2);

    //FOR SIGNAL
    customPlot->addGraph();
    customPlot->graph(0)->setPen(pen1); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(102, 153, 102, 0))); // first graph will be filled with translucent blue

    //FOR NOISE
    customPlot->addGraph();
    customPlot->graph(1)->setPen(pen2);
    customPlot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 100)));

    customPlot->yAxis->setAutoTicks(true);
    customPlot->yAxis->setAutoTickLabels(true);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis->setAutoTickStep(true);
    customPlot->xAxis->setTickLabelColor("black");
    customPlot->yAxis->setTickLabelColor("black");
    customPlot->xAxis2->setTickLabelColor("black");
    customPlot->yAxis2->setTickLabelColor("black");
    customPlot->setAutoFillBackground(true);

    customPlot->graph(0)->setAdaptiveSampling(true); //for large data
    customPlot->graph(1)->setAdaptiveSampling(true);//for large data

    customPlot->xAxis->setTickStep(0); //distance between steps on x-axis
    ui->statusBar->setHidden(true);

    customPlot->xAxis->grid()->setVisible(false);

    customPlot->yAxis->setLabel("Power");
    ui->customPlot->xAxis->setLabel("Frequency");
    ui->batteryBar->setMaximum(0); //Indicate busy before calibration
    ui->batteryBar->setMinimum(0);

    /*----------------------LOC_MON------------------------*/
    customPlot_avg->addGraph();
    customPlot_avg->graph(0)->setPen(pen1);
    customPlot_avg->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    customPlot_avg->yAxis->setAutoTicks(true);
    customPlot_avg->yAxis->setAutoTickLabels(true);
    customPlot_avg->xAxis2->setVisible(true);
    customPlot_avg->xAxis2->setTickLabels(false);
    customPlot_avg->yAxis2->setVisible(true);
    customPlot_avg->xAxis2->setVisible(true);
    customPlot_avg->yAxis2->setTickLabels(false);
    customPlot_avg->xAxis->setAutoTickStep(true);
    customPlot_avg->xAxis->setTickLabelColor("black");
    customPlot_avg->yAxis->setTickLabelColor("black");
    customPlot_avg->xAxis2->setTickLabelColor("black");
    customPlot_avg->yAxis2->setTickLabelColor("black");
    customPlot_avg->setAutoFillBackground(true);
    customPlot_avg->xAxis->setTickStep(0); //distance between steps on x-axis
    customPlot_avg->xAxis->setLabel("Location");
    customPlot_avg->yAxis->setLabel("Avg");

    customPlot_max->addGraph();
    customPlot_max->graph(0)->setPen(pen3);
    customPlot_max->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    customPlot_max->yAxis->setAutoTicks(true);
    customPlot_max->yAxis->setAutoTickLabels(true);
    customPlot_max->xAxis2->setVisible(true);
    customPlot_max->xAxis2->setTickLabels(false);
    customPlot_max->yAxis2->setVisible(true);
    customPlot_max->xAxis2->setVisible(true);
    customPlot_max->yAxis2->setTickLabels(false);
    customPlot_max->xAxis->setAutoTickStep(true);
    customPlot_max->xAxis->setTickLabelColor("black");
    customPlot_max->yAxis->setTickLabelColor("black");
    customPlot_max->xAxis2->setTickLabelColor("black");
    customPlot_max->yAxis2->setTickLabelColor("black");
    customPlot_max->setAutoFillBackground(true);
    customPlot_max->xAxis->setTickStep(0); //distance between steps on x-axis
    customPlot_max->yAxis->setLabel("Max");
    customPlot_max->xAxis->setLabel("Location");

    loc.location = 0;


    /*---------------------------------------------------*/
    /*------------------OWTS-----------------------------*/

    for (int i = 0; i < MAX_LOC_MON_LOCATIONS; i++) {
        owts.severity_label[i] = new QWidget();
        owts.severity_label[i]->setParent(ui->customPlot);
        owts.severity_label[i]->hide();
    }

    owts.gradient = new QLinearGradient();
    owts.palette = new QPalette();

    /*---------------------------------------------------*/

    //gps initilization...
    gps_counter = 0;
    __latitude = "";
    __longitude = "";
    gps_date = "";
    gps_time = "";
    rtc_flag = 0;

    /*---------------------------------------------------*/
    //Other Initilizations
    SLEEP_FLAG = 0;
    GPS_FLAG=0;
    SLEEP_FLAG=0;
    S_P_FLAG=0;
    NOISE_FLAG=0;
    GRAPH_MIN_MAX_CHANGED=0;
    graph_max_earlier=0;
    graph_min_earlier=0;
    RECORD_ONCE=0;
    GPIO.gpio_set_level(GPS_GPIO_PORT,GPS_GPIO_PIN,LEDOFF);
    GPIO.gpio_set_level(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN,LEDOFF);

    ui->hint_label->hide();

    /*---------------------------------------------------*/

    switch_off_leds_except(SCR_MNUMBER); //switch off all LEDs

    //read battery info after every 2 minutes
    connect(&batt1Timer, SIGNAL(timeout()), this, SLOT(read_battery()));
    batt1Timer.start(120000); // Interval 0 means to refresh as fast as possible
    batt_status = new read_batt();
    //batt_status->read_battery_soc();
    DATAUpdate = new dataUpdate();
    //to update progress bar from battery routine
    connect(batt_status, SIGNAL(batt1Signal(int)), this, SLOT(batteryLevelChanged(int)));

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(update_date_time()));
    connect(&hint_timer,SIGNAL(timeout()),this, SLOT(hide_hint()));
    connect(&buzzer_timer,SIGNAL(timeout()),this, SLOT(beep_off()));

    dataTimer.start(5000);

    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(on_mouseWheel(QWheelEvent*)));
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_mousePress(QMouseEvent*)));

    connect(&dialog, SIGNAL(on_dialog_exit_signal()), this, SLOT(start_LOC_MON_slot()));
    connect(&Menu, SIGNAL(on_menu_exit_signal()) , this, SLOT(start_timer()));


#ifdef DEBUG
    cout << "Setup_simple_demo_completed" << endl;
#endif

    read_battery(); //display batt status at beginning
    set_mode(SPECTRUM);
    this->grabKeyboard();


}

void MainWindow::realtimeDataSlot()
{
#ifdef DEBUG
    cout << "real_time_data_slot_started" << endl;
#endif
    //both x-axes n y-axes scale simultaneously
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    Present_Zoom_Level = 0;

    update_data();

    if (mode == SCREENING || mode == SPECTRUM)
        this->update_display_array(0, NO_SCROLL);

    if (LOC_MON_running == 1)
        this->start_LOC_MON_slot();

    gps_counter++;
    if (gps_counter >= 23 && GPS_FLAG==1) { //enter every 23rd iteration
        printf("\n\nread_gps_direct() started\n\n");
        read_gps_direct(); //read /home/linaro/gps1.txt if gps on
        gps_counter = 0;
    }

#ifdef DEBUG
    cout << "real_time_data_slot_completed" << endl;
#endif
}

void MainWindow::start_timer(){
dataTimer.start(5000);
this->grabKeyboard();
qDebug("Timer started:249\n");
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
