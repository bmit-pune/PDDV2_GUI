#include "all_headers.h"
#include "global_indexes.h"

unsigned short int array[NO_OF_RAW_VALS];
volatile float arr_float[NO_OF_FFT_VALS];
float arr_float_noise[NO_OF_FFT_VALS];
float arr_float_mV[NO_OF_RAW_VALS];
MODE mode;
//int LOC_MON_first_transition=0;

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



    ui->centralWidget->setStyleSheet("color: blue;"
                                     "background-color: white;"
                                     "selection-color: black;"
                                     "selection-background-color: blue;");
    setupSimpleDemo(ui->customPlot, ui->customPlot_avg, ui->customPlot_max);

    /*ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                       QCP::iSelectLegend | QCP::iSelectPlottables);*/
    update_data();

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSimpleDemo(QCustomPlot* customPlot, QCustomPlot* customPlot_avg, QCustomPlot* customPlot_max)

{
    cout << "Setup_simple_demo_started" << endl;

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
    for (int i = 0; i < OWTS_steps; i++) {
        for (int j = 0; j < MAX_LOC_MON_LOCATIONS; j++) {
            owts.label[i][j] = new QLabel();
            owts.label[i][j]->setParent(ui->customPlot);
            owts.label[i][j]->hide();
        }
    }

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

    ui->hint_label->hide();

    /*---------------------------------------------------*/

    switch_off_leds_except(SCR_MNUMBER); //switch off all LEDs

    //read battery info after every 2 minutes
    connect(&batt1Timer, SIGNAL(timeout()), this, SLOT(read_battery()));
    batt1Timer.start(120000); // Interval 0 means to refresh as fast as possible
    batt_status = new read_batt();
    DATAUpdate = new dataUpdate();
    //to update progress bar from battery routine
    connect(batt_status, SIGNAL(batt1Signal(int)), ui->batteryBar, SLOT(batteryLevelChanged(int nValue)));

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(update_date_time()));
    connect(&hint_timer,SIGNAL(timeout()),this, SLOT(hide_hint()));
    dataTimer.start(5000);

    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(on_mouseWheel(QWheelEvent*)));
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_mousePress(QMouseEvent*)));

    connect(&dialog, SIGNAL(on_exit_signal()), this, SLOT(start_LOC_MON_slot()));

#ifdef DEBUG
    cout << "Setup_simple_demo_completed" << endl;
#endif

    read_battery(); //display batt status at beginning
    set_mode(SPECTRUM);


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
        read_gps_direct(); //read /home/linaro/gps1.txt if gps on
        gps_counter = 0;
    }

#ifdef DEBUG
    cout << "real_time_data_slot_completed" << endl;
#endif
}

void MainWindow::update_display_array(int clicked_pixel_point, SCROLL scroll)
{

   /* if(RECORD_ONCE==1)
        return;
    RECORD_ONCE=1;*/

#ifdef DEBUG
    cout << "update_display_array_started" << endl;
    qDebug("zoom level:%d", Present_Zoom_Level);
#endif

    if (scroll == IN && Present_Zoom_Level == Total_Zoom_Levels - 1) {
        qDebug("reached max zoom in");
        return;
    }

    if (scroll == OUT && Present_Zoom_Level == 0) {
        qDebug("reached min zoom out");
        return;
    }

    //int point=display_array_index[clicked_pixel_point];
    /*qDebug("point=%d",point);*/
    //int graph_min, graph_max;

    switch (scroll) {
    case IN: /*qDebug("in**");*/
        graph_min = clicked_pixel_point - (int)(this->lut.L[Present_Zoom_Level + 1] / 2);

        if (graph_min < boundry_lower) { /*qDebug("inside IN if");*/
            graph_min = boundry_lower;
            graph_max = graph_min + this->lut.L[Present_Zoom_Level + 1];
            Present_Zoom_Level++;
            break;
        }

        graph_max = clicked_pixel_point + (int)(this->lut.L[Present_Zoom_Level + 1] / 2);
        if (graph_max > boundry_upper) { /*qDebug("inside max IN if");*/
            graph_max = boundry_upper;
            graph_min = graph_max - this->lut.L[Present_Zoom_Level + 1];
        }
        Present_Zoom_Level++;
        break;

    case OUT: /*qDebug("out**");*/
        graph_min = clicked_pixel_point - (this->lut.L[Present_Zoom_Level]);

        if (graph_min < boundry_lower) { /*qDebug("inside min oUT if");*/
            graph_min = boundry_lower;
            graph_max = (int)(this->lut.L[Present_Zoom_Level - 1]);
            Present_Zoom_Level--; /*qDebug("MIn:%d",min);qDebug("Max:%d",max);*/
            break;
        }

        graph_max = clicked_pixel_point + (this->lut.L[Present_Zoom_Level]);
        if (graph_max > boundry_upper) { /*qDebug("inside max oUT if");*/
            graph_max = boundry_upper;
            graph_min = graph_max - (int)(this->lut.L[Present_Zoom_Level - 1]);
        }
        Present_Zoom_Level--;
        break;

    case NO_SCROLL: /*qDebug("no-scroll**");*/
        if (CLICKZOOM_FLAG == 1) {
            graph_min = clickzoom_min;
            graph_max = clickzoom_max;
        }

        else {
            graph_min = boundry_lower;
            graph_max = boundry_upper;
        }
        break;

    case CLICKZOOM:
        graph_min = clicked_pixel_point - 256;
        graph_max = clicked_pixel_point + 256;
        if (graph_min < boundry_lower) {
            graph_min = 0;
            graph_max = 511;
        }
        if (graph_max > boundry_upper) {
            graph_max = boundry_upper;
            graph_min = graph_max - 512;
        }
        clickzoom_min = graph_min;
        clickzoom_max = graph_max;

        break;
    };

#ifdef DEBUG
    qDebug("final:min=%d", graph_min);
    qDebug("final:max=%d\n", graph_max);
#endif

    /*----------------------------------------------------------------------*/
    int i = 0;
#ifdef DEBUG
    printf("This stage skip value is %d\n", this->lut.skip_value[Present_Zoom_Level]);
#endif

    if((graph_min_earlier != graph_min )|| (graph_max_earlier != graph_max)){
    GRAPH_MIN_MAX_CHANGED=1;
    graph_min_earlier=graph_min;
    graph_max_earlier=graph_max;
    key1.clear();
    key1_noise.clear();
    }


    int xval = 0;

    for (i = graph_min; i < graph_max; i++) {


        if (i <= 10 && mode == SPECTRUM)
            continue; //Skip first 10 Vals
        if (i > 2 * KILO_SAMPLES && mode == SPECTRUM)
            continue;

        if (mode == SCREENING && (i % this->lut.skip_value[Present_Zoom_Level]) == 0) {
            y0.push_back((unsigned short int)array[i]);

                if(GRAPH_MIN_MAX_CHANGED==1){
                    key1.push_back(xval++);
                }

        }

        if (mode == SPECTRUM /*&& (i%this->lut.skip_value[Present_Zoom_Level])==0*/) {
            y0.push_back((float)arr_float[i]);

            if(GRAPH_MIN_MAX_CHANGED==1){
                key1.push_back(61035.155 * xval); //fs/(n/2) * sample index fs-375mHz, n=4096
            }

            if(NOISE_FLAG==1){
            y0_noise.push_back((float)arr_float_noise[i]);
                if(GRAPH_MIN_MAX_CHANGED==1){
                    key1_noise.push_back(61035.155 * xval);
                }
            }

            xval++;

        }
    }

    if(GRAPH_MIN_MAX_CHANGED==1)
        GRAPH_MIN_MAX_CHANGED=0;



        ui->customPlot->graph(0)->setData(key1, y0); //signal
        if(NOISE_FLAG==1){
        ui->customPlot->graph(1)->setData(key1_noise, y0_noise); //noise
        }
        ui->customPlot->yAxis->rescale(true);
        ui->customPlot->xAxis->rescale(true);
        ui->customPlot->replot();
        y0.clear();
        y0_noise.clear();
}

void MainWindow::start_LOC_MON_slot()
{
    this->grabKeyboard();
    unsigned int count = 0;

    if (!dataTimer.isActive() && mode == LOC_MON) { //setup and start process of monitoring

        if (LOC_MON_first_transition == 1) { //Only run first time
            LOC_MON_OWTS_data_clean();
            LOC_MON_first_transition = 0;
            ui->customPlot_avg->graph(0)->setData(key1_locavg, y0_locavg); //plot avg data,clear graph
            ui->customPlot_max->graph(0)->setData(key1_locmax, y0_locmax); //plot max data,clear graph
        }

        if (loc.iteration == 0) {
            LOC_MON_OWTS_overwrite_loc_data_clean(dialog.Set_Location);
        }

        LOC_MON_running = 1;
        dataTimer.start(5000);

        /*ui->range->setText("LOCATION MONITORING");
        ui->customPlot_avg->xAxis->setRangeLower(-1);
        ui->customPlot_avg->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
        ui->customPlot_avg->xAxis->setAutoTickStep(false);
        ui->customPlot_avg->xAxis->setTickStep(1);
        ui->customPlot_avg->yAxis->setAutoSubTicks(false);
        ui->customPlot_avg->yAxis->setSubTickCount(10);

        ui->customPlot_max->xAxis->setRangeLower(-1);
        ui->customPlot_max->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
        ui->customPlot_max->xAxis->setAutoTickStep(false);
        ui->customPlot_max->xAxis->setTickStep(1);


        ui->customPlot_max->xAxis->setAutoTickLabels(false);
        ui->customPlot_avg->xAxis->setAutoTickLabels(false);
        QVector<QString> labels;
            labels <<""<<"Loc1"<<"Loc2"<<"Loc3"<<"Loc4"<<"Loc5"<<"Loc6"<<"Loc7"<<"Loc8"<<"Loc9"<<"Loc10";
        ui->customPlot_max->xAxis->setTickVectorLabels(labels);
        ui->customPlot_avg->xAxis->setTickVectorLabels(labels);

        ui->customPlot_avg->xAxis->setTickLabelRotation(-30);
        ui->customPlot_max->xAxis->setTickLabelRotation(-30);*/

        ui->customPlot_avg->replot();
        ui->customPlot_max->replot();
        update_data();
    }

    if (loc.iteration < (dialog.Set_Time * 12)) {

#ifdef DEBUG
        qDebug("inside if loc.iteration=%d\n", loc.iteration);
#endif
        ui->date_time->setText(QString::number((loc.iteration + 1) * 5) + " seconds elapsed");
        loc.max_temp = 0;
        loc.avg_temp = 0;
        count = 0;

        for (int i = 0; (i < NO_OF_RAW_VALS); i++) {
            if (((dialog.Set_Threshold >= arr_float_mV[i]) && arr_float_mV[i] > 0)) {
                //if(loc.max_temp<arr_float_mV[i])  //for max
                //    loc.max_temp=arr_float_mV[i];

                loc.avg_temp += (int)arr_float_mV[i]; //for avg
                count++;

                //owts.owts_array[0][dialog.Set_Location]
                if (arr_float_mV[i] >= 0 && arr_float_mV[i] < 3) //for owts
                    owts.owts_array[0][dialog.Set_Location]++;
                else if (arr_float_mV[i] > 3 && arr_float_mV[i] < 6)
                    owts.owts_array[1][dialog.Set_Location]++;
                else if (arr_float_mV[i] > 6 && arr_float_mV[i] < 9)
                    owts.owts_array[2][dialog.Set_Location]++;
                else if (arr_float_mV[i] > 9 && arr_float_mV[i] < 12)
                    owts.owts_array[3][dialog.Set_Location]++;
                else if (arr_float_mV[i] > 12 && arr_float_mV[i] < 15)
                    owts.owts_array[4][dialog.Set_Location]++;
            }
            if (loc.max_temp < arr_float_mV[i]) //for max
                loc.max_temp = arr_float_mV[i];
        }

#ifdef DEBUG
        qDebug("########Final max_temp:%f\n", loc.max_temp);
#endif
        qDebug("########Final max:%f -----> iteraton=%d\n", loc.max_one_loc_minutes[loc.iteration] = loc.max_temp, loc.iteration);
#ifdef DEBUG
        qDebug("loc.avg_temp:%f\n", loc.avg_temp);
        qDebug("count is:%d\n", count);
#endif
        if (count == 0 && loc.avg_temp == 0)
            loc.avg_one_loc_minutes[loc.iteration] = 0;
        else
            qDebug("########Final Avg:%f -----> iteraton=%d\n", loc.avg_one_loc_minutes[loc.iteration] = loc.avg_temp / count, loc.iteration);
        loc.iteration++;
    }

    else {
        ui->date_time->setText("Complete,Change Location...@ " + QDateTime::currentDateTime().toString());

        loc.max_temp = 0;
        loc.avg_temp = 0;
        count = 0;

        for (int i = 1; i < ((dialog.Set_Time * 12)); i++) {
            if (loc.max_temp < loc.max_one_loc_minutes[i]) //for max
                loc.max_temp = loc.max_one_loc_minutes[i];
#ifdef DEBUG
            qDebug("loc.max_temp=%f\n", loc.max_temp);
#endif

            loc.avg_temp += loc.avg_one_loc_minutes[i]; //for avg
#ifdef DEBUG
            qDebug("loc.avg_temp=%f\n", loc.avg_one_loc_minutes[i]);
#endif
            count++;
        }

        qDebug("***avg::%f", loc.loc_avg[dialog.Set_Location] = loc.avg_temp / count); //to be plotted
        qDebug("***max::%f", loc.loc_max[dialog.Set_Location] = loc.max_temp);

        /*for(int i=0;i<10;i++)
        {
            qDebug("Location:%d->%d\n",i,loc.monitored_locations[i]);
        }*/

        for (int i = 0; i <= MAX_LOC_MON_LOCATIONS - 1; i++) {
            if (loc.monitored_locations[i] == 1) {
                key1_locavg.push_back(i);
                y0_locavg.push_back(loc.loc_avg[i]);
                qDebug("loc->%d::avg->%f*", i, loc.loc_avg[i]);
                //ui->customPlot_avg->graph(0)->setData(key1_locavg,y0_locavg);//plot avg data

                key1_locmax.push_back(i);
                y0_locmax.push_back(loc.loc_max[i]);
                qDebug("        max->%f*", loc.loc_max[i]);
                //ui->customPlot_max->graph(0)->setData(key1_locmax,y0_locmax);//plot max data
            }
        }
        ui->customPlot_avg->graph(0)->setData(key1_locavg, y0_locavg); //plot avg data
        ui->customPlot_max->graph(0)->setData(key1_locmax, y0_locmax); //plot max data

        ui->customPlot_avg->yAxis->rescale(true);
        ui->customPlot_avg->replot();
        ui->customPlot_max->yAxis->rescale(true);
        ui->customPlot_max->replot();

        key1_locavg.clear();
        key1_locmax.clear();

        y0_locavg.clear();
        y0_locmax.clear();

        dataTimer.stop();
        LOC_MON_running = 0;
        loc.max_temp = 0; //let 0 be max for very first time
        loc.avg_temp = 0;
        loc.minutes_temp = 0;
        loc.iteration = 0;
        //loc.location++;
    }
}

void MainWindow::OWTS_display()
{

#define LABELWIDTH 35
#define LABELHEIGHT 25

#define SEV_LABELWIDTH 55
#define SEV_LABELHEIGHT 335
    owts.adjx = 48;
    owts.adjy = -68;

    owts.sev_label_adjx = 48;
    owts.sev_label_adjy = -25;

    key1.clear();
    y0.clear();
    for (int i = 0; i < 10; i++) {
        key1.push_back(i);
        y0.push_back(10);
    }

    ui->customPlot->xAxis->setTickStep(1);
    ui->customPlot->yAxis->setTickStep(1);
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->graph(0)->setData(key1, y0);

    ui->customPlot->xAxis->setRangeLower(0);
    ui->customPlot->xAxis->setRangeUpper(11);
    ui->customPlot->xAxis->setLabel("Location");

    ui->customPlot->yAxis->setRangeLower(0);
    ui->customPlot->yAxis->setRangeUpper(5);
    ui->customPlot->yAxis->setLabel("mV");
    ui->customPlot->replot();

    ui->range->setText("OWTS- HISTOGRAM");

    //QLabel *    label[OWTS_steps][MAX_LOC_MON_LOCATIONS];

    QFont my_font("Helvetica [Cronyx]", 7, QFont::Cursive);
    my_font.setWeight(75);

    int count_0, count_1, count_2, count_3, count_4, count_5, count_6, count_7, count_8, count_9;
    count_0 = count_1 = count_2 = count_3 = count_4 = count_5 = count_6 = count_7 = count_8 = count_9 = 0;

    for (int i = 0; i < OWTS_steps; i++) {
        for (int j = 0; j < MAX_LOC_MON_LOCATIONS; j++) {
            //owts.owts_array[i][j]=(unsigned long)1 + ( std::rand() % ( 240 - 1 + 1 ) );

            qDebug("%ld", owts.owts_array[i][j]);

            owts.label[i][j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j) + owts.adjx, ui->customPlot->yAxis->coordToPixel(i) + owts.adjy, LABELWIDTH, LABELHEIGHT);

            if (i == 4) {
                owts.severity_label[j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j) + owts.sev_label_adjx, ui->customPlot->yAxis->coordToPixel(i) + owts.sev_label_adjy, SEV_LABELWIDTH, SEV_LABELHEIGHT);
                /*owts.severity_label[j]->setStyleSheet("color: blue;"
                                                       "background-color: blue;"
                                                       "selection-color: blue;"
                                                       "selection-background-color: blue;");*/
                //owts.severity_label[j]->show();
            }

            switch (j) {
            case 0:
                owts.severity_arr_loc0[count_0] = owts.owts_array[i][j];
                count_0++;
                break;
            case 1:
                owts.severity_arr_loc1[count_1] = owts.owts_array[i][j];
                count_1++;
                break;
            case 2:
                owts.severity_arr_loc2[count_2] = owts.owts_array[i][j];
                count_2++;
                break;
            case 3:
                owts.severity_arr_loc3[count_3] = owts.owts_array[i][j];
                count_3++;
                break;
            case 4:
                owts.severity_arr_loc4[count_4] = owts.owts_array[i][j];
                count_4++;
                break;
            case 5:
                owts.severity_arr_loc5[count_5] = owts.owts_array[i][j];
                count_5++;
                break;
            case 6:
                owts.severity_arr_loc6[count_6] = owts.owts_array[i][j];
                count_6++;
                break;
            case 7:
                owts.severity_arr_loc7[count_7] = owts.owts_array[i][j];
                count_7++;
                break;
            case 8:
                owts.severity_arr_loc8[count_8] = owts.owts_array[i][j];
                count_8++;
                break;
            case 9:
                owts.severity_arr_loc9[count_9] = owts.owts_array[i][j];
                count_9++;
                break;
            }
            owts.label[i][j]->setFont(my_font);
            owts.label[i][j]->setWordWrap(true);
            owts.label[i][j]->setText(QString::number(owts.owts_array[i][j], 'g', 4));
        }
    }

    static int* recv_index = (int*)malloc(5 * sizeof(int));

    recv_index = index_sort_owts(owts.severity_arr_loc0);
    set_OWTS_label(0, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc1);
    set_OWTS_label(1, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc2);
    set_OWTS_label(2, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc3);
    set_OWTS_label(3, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc4);
    set_OWTS_label(4, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc5);
    set_OWTS_label(5, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc6);
    set_OWTS_label(6, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc7);
    set_OWTS_label(7, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc8);
    set_OWTS_label(8, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc9);
    set_OWTS_label(9, recv_index);

    int once = 0;
here1:
    QPalette index_palette;
    QLinearGradient index_gradient(ui->index_label->rect().bottomLeft(), ui->index_label->rect().topLeft());

    index_gradient.setColorAt(0.2, QColor(0, 204, 204));
    index_gradient.setColorAt(0.4, QColor(153, 204, 0));
    index_gradient.setColorAt(0.6, Qt::yellow);
    index_gradient.setColorAt(0.8, QColor(255, 111, 0));
    index_gradient.setColorAt(1.0, Qt::red);
    index_palette.setBrush(QPalette::Background, QBrush(index_gradient));

    ui->index_label->setParent(this);
    ui->index_label->setAutoFillBackground(true);
    ui->index_label->setPalette(index_palette);
    ui->index_label->setText("    High\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n    Low");
    ui->index_label->show();

    ui->index_label->setGeometry(720, 60, SEV_LABELWIDTH, SEV_LABELHEIGHT);
}

void MainWindow::set_OWTS_label(int location, int* recv_index)
{

    QPalette sample_palette;
    QLinearGradient gradient(owts.severity_label[location]->rect().bottomLeft(), owts.severity_label[location]->rect().topLeft());
/*gradient.setColorAt(0,	Qt::blue);
    gradient.setColorAt(0.4,	Qt::cyan);
    gradient.setColorAt(1,	Qt::green);
    sample_palette.setBrush(QPalette::Background, QBrush(gradient));
        for(int i=0;i<10;i++){
        owts.severity_label[i]->setAutoFillBackground(true);
        owts.severity_label[i]->setPalette(sample_palette);
        owts.severity_label[i]->setParent(this);
        owts.severity_label[i]->show();
        }*/

#ifdef PCGUI
    QPixmap circle("/home/nishad/pdd_workspace/oct-16/build/new/circle.png");
    QPixmap square("/home/nishad/pdd_workspace/oct-16/build/new/square.png");
    QPixmap cross("/home/nishad/pdd_workspace/oct-16/build/new/cross.png");
    QPixmap triangle("/home/nishad/pdd_workspace/oct-16/build/new/triangle.png");
    QPixmap tick("/home/nishad/pdd_workspace/oct-16/build/new/yellow_tick.jpg");
#endif

#ifdef BOARDGUI
    QPixmap circle("/opt/new/circle.png");
    QPixmap square("/opt/new/square.png");
    QPixmap cross("/opt/new/cross.png");
    QPixmap triangle("/opt/new/triangle.png");
    QPixmap tick("/opt/new/yellow_tick.jpg");
#endif

    float gradient_position;
    for (int i = 0; i < 5; i++) {
        //gradient_position=(*(recv_index+i)) ;

        switch ((*(recv_index + i))) {
        case 0:
            gradient_position = 0.2;
            break;
        case 1:
            gradient_position = 0.4;
            break;
        case 2:
            gradient_position = 0.6;
            break;
        case 3:
            gradient_position = 0.8;
            break;
        case 4:
            gradient_position = 1.0;
            break;
        }

        switch (i) {
        case 0:
            qDebug("gradient_position-->%f\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                //owts.label[gradient_position][location]->setText("____");
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                //owts.label[gradient_position][location]->setPixmap(cross);
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            break;
        case 1:
            qDebug("gradient_position-->%d\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                //owts.label[gradient_position][location]->setText("____");
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                //owts.label[gradient_position][location]->setPixmap(circle);
                gradient.setColorAt((gradient_position), QColor(153, 204, 0));
            break;
        case 2:
            qDebug("gradient_position-->%d\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                //owts.label[gradient_position][location]->setText("____");
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                //owts.label[gradient_position][location]->setPixmap(tick);
                gradient.setColorAt((gradient_position), Qt::yellow);
            break;
        case 3:
            qDebug("gradient_position-->%d\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                //owts.label[gradient_position][location]->setText("____");
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                //owts.label[gradient_position][location]->setPixmap(square);
                gradient.setColorAt((gradient_position), QColor(255, 111, 0));
            break;
        case 4:
            qDebug("gradient_position-->%d\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                //owts.label[gradient_position][location]->setText("____");
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                //owts.label[gradient_position][location]->setPixmap(triangle);
                gradient.setColorAt((gradient_position), Qt::red);
            // ui->index_label->setText("High Count:\nLoc1:\n"+QString::number(owts.owts_array[*(recv_index+i)][0])+"Loc2:\n"+QString::number(owts.owts_array[*(recv_index+i)][1])+"Loc3:\n");
            break;
        }
        //owts.label[gradient_position][location]->show();
    }

    sample_palette.setBrush(QPalette::Background, QBrush(gradient));

    owts.severity_label[location]->setParent(this);
    owts.severity_label[location]->setAutoFillBackground(true);
    owts.severity_label[location]->setPalette(sample_palette);

    if (loc.monitored_locations[location] == 1) //Show bar only if location is aldready monitored
        owts.severity_label[location]->show();

    /* QPalette sample_palette;

    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::blue);

    QLinearGradient gradient(owts.severity_label[0]->rect().topLeft(),owts.severity_label[0]->rect().bottomLeft());
        gradient.setColorAt(0,	Qt::blue);
        gradient.setColorAt(0.4,	Qt::cyan);
        gradient.setColorAt(1,	Qt::green);

        sample_palette.setBrush(QPalette::Background, QBrush(gradient));

    for(int i=0;i<10;i++){
    owts.severity_label[i]->setAutoFillBackground(true);
    owts.severity_label[i]->setPalette(sample_palette);
    owts.severity_label[i]->setParent(this);
    owts.severity_label[i]->show();
    }*/
}

int* MainWindow::index_sort_owts(unsigned long temp[])
{
    std::pair<unsigned long, int> pair[5];
    static int array_sort_index[5];

    for (int i = 0; i < 5; i++) {
        pair[i].first = temp[i];
        pair[i].second = i;
    }

#ifdef DEBUG
    qDebug("~~~~~~~~~~~~~~~~~~Before sorting~~~~~~~~~~~~~~~~~~");
    for (int i = 0; i < 5; i++) {
        qDebug("array[%d]=%ld\n", pair[i].second, pair[i].first);
    }
#endif

    //qsort(pair,pair+5,comp_func);

    std::sort(pair, pair + 5);

#ifdef DEBUG
    qDebug("~~~~~~~~~~~~~~~~~~After sorting~~~~~~~~~~~~~~~~~~");
    for (int i = 0; i < 5; i++) {
        qDebug("array[%d]=%ld\n", pair[i].second, pair[i].first);
    }
    qDebug("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

    for (int i = 0; i < 5; i++)
        array_sort_index[i] = pair[i].second;

    return array_sort_index;
}

void MainWindow::LOC_MON_OWTS_data_clean()
{
    qDebug("####only first time condition executed...\n");
    loc.max_temp = 0; //let 0 be max for very first time
    loc.avg_temp = 0;
    loc.minutes_temp = 0;
    loc.iteration = 0;

    memset(loc.loc_avg, 0, sizeof(loc.loc_avg));
    memset(loc.loc_max, 0, sizeof(loc.loc_max));
    memset(loc.avg_one_loc_minutes, 0, sizeof(loc.avg_one_loc_minutes));
    memset(loc.monitored_locations, 0, sizeof(loc.monitored_locations));

    memset(owts.owts_array, 0, sizeof(owts.owts_array));
    memset(owts.severity_arr_loc0, 0, sizeof(owts.severity_arr_loc0));
    memset(owts.severity_arr_loc1, 0, sizeof(owts.severity_arr_loc1));
    memset(owts.severity_arr_loc2, 0, sizeof(owts.severity_arr_loc2));
    memset(owts.severity_arr_loc3, 0, sizeof(owts.severity_arr_loc3));
    memset(owts.severity_arr_loc4, 0, sizeof(owts.severity_arr_loc4));
    memset(owts.severity_arr_loc5, 0, sizeof(owts.severity_arr_loc5));
    memset(owts.severity_arr_loc6, 0, sizeof(owts.severity_arr_loc6));
    memset(owts.severity_arr_loc7, 0, sizeof(owts.severity_arr_loc7));
    memset(owts.severity_arr_loc8, 0, sizeof(owts.severity_arr_loc8));
    memset(owts.severity_arr_loc9, 0, sizeof(owts.severity_arr_loc9));

    if (mode == LOC_MON)
        ui->range->setText("LOCATION MONITORING");

    ui->customPlot_avg->xAxis->setRangeLower(-1);
    ui->customPlot_avg->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
    ui->customPlot_avg->xAxis->setAutoTickStep(false);
    ui->customPlot_avg->xAxis->setTickStep(1);
    ui->customPlot_avg->yAxis->setAutoSubTicks(false);

    ui->customPlot_max->xAxis->setRangeLower(-1);
    ui->customPlot_max->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
    ui->customPlot_max->xAxis->setAutoTickStep(false);
    ui->customPlot_max->xAxis->setTickStep(1);

    ui->customPlot_max->xAxis->setAutoTickLabels(false);
    ui->customPlot_avg->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    QVector<QString> labels;
    labels << ""
           << "Loc1"
           << "Loc2"
           << "Loc3"
           << "Loc4"
           << "Loc5"
           << "Loc6"
           << "Loc7"
           << "Loc8"
           << "Loc9"
           << "Loc10";
    ui->customPlot_max->xAxis->setTickVectorLabels(labels);
    ui->customPlot_avg->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickVectorLabels(labels);

    ui->customPlot_avg->xAxis->setTickLabelRotation(-30);
    ui->customPlot_max->xAxis->setTickLabelRotation(-30);
    ui->customPlot->xAxis->setTickLabelRotation(-30);
}

void MainWindow::LOC_MON_OWTS_overwrite_loc_data_clean(int Set_Location)
{
    owts.owts_array[0][Set_Location] = owts.owts_array[1][Set_Location] = owts.owts_array[2][Set_Location] = owts.owts_array[3][Set_Location] = owts.owts_array[4][Set_Location] = 0;
    loc.loc_avg[Set_Location] = 0;
    loc.loc_max[Set_Location] = 0;
    loc.monitored_locations[dialog.Set_Location] = 1; //Log the location which we are monitoring

    qDebug("Location set is %d\n", dialog.Set_Location + 1);
    ui->index_label->setText("Monitor\nLocation:\n" + QString::number(dialog.Set_Location + 1) + "\n\nSet_Max:\n" + QString::number(dialog.Set_Threshold)+" mV"+"\n\nSet Time:\n" + QString::number(dialog.Set_Time)+" Mins" + "\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
