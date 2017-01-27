#include"all_headers.h"
#include"global_indexes.h"

unsigned char array[NO_OF_RAW_VALS];
float arr_float[NO_OF_FFT_VALS];
float arr_float_noise[NO_OF_FFT_VALS];
float arr_float_mV[NO_OF_RAW_VALS];


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralWidget->setStyleSheet("color: blue;"
                        "background-color: white;"
                        "selection-color: black;"
                        "selection-background-color: blue;");


     setupSimpleDemo(ui->customPlot,ui->customPlot_avg,ui->customPlot_max);


     /*ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                       QCP::iSelectLegend | QCP::iSelectPlottables);*/

     update_data();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupSimpleDemo(QCustomPlot *customPlot,QCustomPlot *customPlot_avg,QCustomPlot *customPlot_max)

{
    cout<<"Setup_simple_demo_started"<<endl;

    customPlot->setBackground(Qt::white);

    QPen pen1(QBrush(QColor(10,10,250,255)), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen1.setWidth(2);

    QPen pen2(QBrush(QColor(250, 10,10,100)), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen1.setWidth(2);


    //FOR SIGNAL
    customPlot->addGraph();
    customPlot->graph(0)->setPen(pen1); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(102, 153,102,0))); // first graph will be filled with translucent blue

    //FOR NOISE
    customPlot->addGraph();
    customPlot->graph(1)->setPen(pen2);
    customPlot->graph(1)->setBrush(QBrush(QColor(255, 0,0,100)));

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

    customPlot->graph(0)->setAdaptiveSampling(true);//for large data
    /*customPlot->graph(1)->setAdaptiveSampling(true);//for large data*/

    customPlot->xAxis->setTickStep(0);//distance between steps on x-axis
    ui->statusBar->setHidden(true);

    customPlot->xAxis->grid()->setVisible(false);

    customPlot->yAxis->setLabel("Power");
    ui->customPlot->xAxis->setLabel("Frequency");
    ui->batteryBar->setMaximum(0);//Indicate busy before calibration
    ui->batteryBar->setMinimum(0);

    /*----------------------LOC_MON------------------------*/
    customPlot_avg->addGraph();
    customPlot_avg->graph(0)->setPen(pen1);
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
    customPlot_avg->xAxis->setTickStep(0);//distance between steps on x-axis
    customPlot_avg->xAxis->setLabel("Location");
    customPlot_avg->yAxis->setLabel("Avg");


    customPlot_max->addGraph();
    customPlot_max->graph(0)->setPen(pen1);
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
    customPlot_max->xAxis->setTickStep(0);//distance between steps on x-axis
    customPlot_max->yAxis->setLabel("Max");
    customPlot_max->xAxis->setLabel("Location");

    loc.location=0;

    /*---------------------------------------------------*/


    /*------------------OWTS-----------------------------*/
    for(int i=0;i<OWTS_steps;i++)
    {
        for(int j=0;j<MAX_LOC_MON_LOCATIONS;j++)
        {
            owts.label[i][j]=new QLabel();
            owts.label[i][j]->setParent(ui->customPlot);
            owts.label[i][j]->hide();
        }

    }
    /*---------------------------------------------------*/





    //read battery info after every 2 minutes
    connect(&batt1Timer, SIGNAL(timeout()), this, SLOT(read_battery()));
    batt1Timer.start(120000); // Interval 0 means to refresh as fast as possible
    batt_status=new read_batt();
    //to update progress bar from battery routine
    connect(batt_status, SIGNAL(batt1Signal(int)), ui->batteryBar,SLOT(setValue(int)));



    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(update_date_time()));
    dataTimer.start(5000);


    connect(ui->customPlot,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(on_mouseWheel(QWheelEvent*)));
    connect(ui->customPlot,SIGNAL(mousePress(QMouseEvent *)),this,SLOT(on_mousePress(QMouseEvent *)));

    connect(&dialog,SIGNAL(on_exit_signal()),this,SLOT(start_LOC_MON_slot()));

    #ifdef DEBUG
    cout<<"Setup_simple_demo_completed"<<endl;
    #endif

    set_mode(SPECTRUM);

}



void MainWindow::realtimeDataSlot()
{
    #ifdef DEBUG
    cout<<"real_time_data_slot_started"<<endl;
    #endif
    //both x-axes n y-axes scale simultaneously
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    Present_Zoom_Level=0;
    update_data();

    if(mode==SCREENING || mode==SPECTRUM)
    this->update_display_array(0,NO_SCROLL);

    if(LOC_MON_running==1)
        this->start_LOC_MON_slot();

    #ifdef DEBUG
    cout<<"real_time_data_slot_completed"<<endl;
    #endif

}

void MainWindow::update_display_array(int clicked_pixel_point, SCROLL scroll)
{

    #ifdef DEBUG
    cout<<"update_display_array_started"<<endl;
    qDebug("zoom level:%d",Present_Zoom_Level);
    #endif

    if(scroll==IN && Present_Zoom_Level==Total_Zoom_Levels-1)
    {qDebug("reached max zoom in");return;}

    if(scroll==OUT && Present_Zoom_Level==0)
    {qDebug("reached min zoom out");return;}

    //int point=display_array_index[clicked_pixel_point];
    /*qDebug("point=%d",point);*/
    int min,max;

    switch(scroll)
    {
    case IN: /*qDebug("in**");*/
    min=clicked_pixel_point-(int)(this->lut.L[Present_Zoom_Level+1]/2);

    if(min < boundry_lower )
    {/*qDebug("inside IN if");*/min=boundry_lower;max=min+this->lut.L[Present_Zoom_Level+1];Present_Zoom_Level++;break;}

    max=clicked_pixel_point+(int)(this->lut.L[Present_Zoom_Level+1]/2);
    if(max>boundry_upper)
    {/*qDebug("inside max IN if");*/max=boundry_upper;min=max-this->lut.L[Present_Zoom_Level+1];}
    Present_Zoom_Level++;
    break;

    case OUT:/*qDebug("out**");*/
    min=clicked_pixel_point-(this->lut.L[Present_Zoom_Level]);

    if(min < boundry_lower )
       {/*qDebug("inside min oUT if");*/min=boundry_lower;max=(int)(this->lut.L[Present_Zoom_Level-1]);Present_Zoom_Level--;/*qDebug("MIn:%d",min);qDebug("Max:%d",max);*/break;}

     max=clicked_pixel_point+(this->lut.L[Present_Zoom_Level]);
    if(max>boundry_upper)
    {/*qDebug("inside max oUT if");*/max=boundry_upper;min=max-(int)(this->lut.L[Present_Zoom_Level-1]);}
    Present_Zoom_Level--;
    break;


    case NO_SCROLL:/*qDebug("no-scroll**");*/
        if(CLICKZOOM_FLAG==1) {
            min=clickzoom_min;
            max=clickzoom_max;
        }

         else{
            min=boundry_lower;
            max=boundry_upper;
          }
    break;

    case CLICKZOOM:
        min=clicked_pixel_point-256;max=clicked_pixel_point+256;
        if(min<boundry_lower)
        {min=0;max=511;}
        if(max>boundry_upper)
        {max=boundry_upper;min=max-512;}
        clickzoom_min=min;clickzoom_max=max;

        break;

};

#ifdef DEBUG
qDebug("final:min=%d",min);
qDebug("final:max=%d\n",max);
#endif

/*----------------------------------------------------------------------*/
int i=0;
#ifdef DEBUG
printf("This stage skip value is %d\n",this->lut.skip_value[Present_Zoom_Level]);
#endif
int xval=0;

for ( i = min; i < max  ; i++ ) {
    if(i<=10 && mode==SPECTRUM)
      continue;//Skip first 10 Vals
     if(i> 2*KILO_SAMPLES && mode==SPECTRUM)
        continue;

    if(mode==SCREENING && (i%this->lut.skip_value[Present_Zoom_Level])==0) {
        y0.push_back((int)array[i]);
        key1.push_back(xval++);
    }

    if(mode==SPECTRUM /*&& (i%this->lut.skip_value[Present_Zoom_Level])==0*/ ) {
    y0.push_back((float)arr_float[i]);
    y0_noise.push_back((float)arr_float_noise[i]);
    key1.push_back(0.091 * xval ); //fs/(n/2) * sample index fs-375mHz, n=4096
    key1_noise.push_back(0.091 * xval);
    xval++;
    }

}

if(mode==SCREENING || mode==SPECTRUM) {
ui->customPlot->graph(0)->setData(key1,y0);//signal
ui->customPlot->graph(1)->setData(key1_noise,y0_noise);//noise
}

if(mode==SCREENING ) {
ui->customPlot->yAxis->rescale(true);
ui->customPlot->xAxis->rescale(true);
}

if(mode==SPECTRUM) {
ui->customPlot->yAxis->setRangeLower(0);
ui->customPlot->yAxis->setRangeUpper(620);
ui->customPlot->xAxis->rescale(true);
}

if(mode==SCREENING || mode==SPECTRUM) {
ui->customPlot->replot();
key1.clear();
y0.clear();
key1_noise.clear();
y0_noise.clear();
}




}

void MainWindow::start_LOC_MON_slot(){
#ifdef DEBUG
qDebug("LOC_MON %d",rand()%10);
#endif
    if(!dataTimer.isActive() && mode==LOC_MON){  //Only run first time
        LOC_MON_running=1;
        dataTimer.start(5000);
        loc.max_temp=0; //let 0 be max for very first time
        loc.avg_temp=0;
        loc.minutes_temp=0;
        loc.iteration=0;

        memset(loc.loc_avg,0,sizeof(loc.loc_avg));
        memset(loc.loc_max,0,sizeof(loc.loc_max));
        memset(loc.avg_one_loc_minutes,0,sizeof(loc.avg_one_loc_minutes));
        memset(loc.max_one_loc_minutes,0,sizeof(loc.max_one_loc_minutes));
        memset(owts.owts_array,0,sizeof(owts.owts_array));

        ui->range->setText("LOC_MON");


   }


    if(loc.iteration< (dialog.Set_Time*12))
    {

        qDebug("inside if loc.iteration=%d\n",loc.iteration);
        ui->date_time->setText(QString::number((loc.iteration + 1)*5)+" seconds elapsed");
        loc.max_temp=0;
        loc.avg_temp=0;
        int count=0;

        qDebug("started for\n");

        for(int i=0;(i<NO_OF_RAW_VALS) ;i++)
        {
            if (!((dialog.Set_Threshold>=arr_float_mV[i]) && arr_float_mV[i]>0))
                continue;

            if(loc.max_temp<arr_float_mV[i])  //for max
                loc.max_temp=arr_float_mV[i];

            loc.avg_temp+=(int)arr_float_mV[i];    //for avg
            count++;

            if(arr_float_mV[i]>10 && arr_float_mV[i]<20) //for owts
                owts.owts_array[0][loc.location]++;
            else if(arr_float_mV[i]>20 && arr_float_mV[i]<30)
                owts.owts_array[1][loc.location]++;
            else if(arr_float_mV[i]>30 && arr_float_mV[i]<40)
                owts.owts_array[2][loc.location]++;
            else if(arr_float_mV[i]>40 && arr_float_mV[i]<50)
                owts.owts_array[3][loc.location]++;
            else if(arr_float_mV[i]>50 && arr_float_mV[i]<200)
                owts.owts_array[4][loc.location]++;

            //qDebug ("mv val is:%f\n",arr_float_mV[i]);


        }

        qDebug("for ends...\n");

        qDebug("first:%d\n",loc.max_one_loc_minutes[loc.iteration]=loc.max_temp);

        qDebug("loc.avg_temp:%lld\n",loc.avg_temp);
        qDebug("count is:%d\n",count);
        if(count==0 && loc.avg_temp==0)
            loc.avg_one_loc_minutes[loc.iteration]=0;
        else
        qDebug("second:%d",loc.avg_one_loc_minutes[loc.iteration]=loc.avg_temp/count);
        loc.iteration++;

        qDebug("incremented loc.iteration=%d\n",loc.iteration);

    }


    else
    {
        qDebug("inside else\n");
        loc.max_temp=0;
        loc.avg_temp=0;
        int count=0;
        for(int i=0;i<dialog.Set_Time*12;i++)
        {
           if(loc.max_temp < loc.max_one_loc_minutes[i]) //for max
                loc.max_temp=loc.max_one_loc_minutes[i];

           loc.avg_temp+=loc.max_one_loc_minutes[i]; //for avg
           count++;
        }

        qDebug("***avg::%d",loc.loc_avg[loc.location]=loc.avg_temp/count); //to be plotted
        qDebug("***max::%d",loc.loc_max[loc.location]=loc.max_temp);

        for(int i=0;i<MAX_LOC_MON_LOCATIONS;i++)
        {
            key1_locavg.push_back(i);
            y0_locavg.push_back(loc.loc_avg[i]);
            qDebug("loc->%d::avg->%d*",i,loc.loc_avg[i]);
            ui->customPlot_avg->graph(0)->setData(key1_locavg,y0_locavg);//signal

            key1_locmax.push_back(i);
            y0_locmax.push_back(loc.loc_max[i]);
            qDebug("loc->%d::avg->%d*",i,loc.loc_max[i]);
            ui->customPlot_max->graph(0)->setData(key1_locmax,y0_locmax);

        }

        ui->customPlot_avg->yAxis->rescale(true);
        ui->customPlot_avg->xAxis->rescale(true);
        ui->customPlot_avg->replot();

        ui->customPlot_max->yAxis->rescale(true);
        ui->customPlot_max->xAxis->rescale(true);
        ui->customPlot_max->replot();

        key1_locavg.clear();
        key1_locmax.clear();

        y0_locavg.clear();
        y0_locmax.clear();



        dataTimer.stop();
        LOC_MON_running=0;
        loc.max_temp=0; //let 0 be max for very first time
        loc.avg_temp=0;
        loc.minutes_temp=0;
        loc.iteration=0;
        loc.location++;
    }

}


void MainWindow::OWTS_display()
{


#define LABELWIDTH 20
#define LABELHEIGHT 20
    owts.adjx=28;
    owts.adjy=-35;
    key1.clear();
    y0.clear();
    for(int i=0;i<10;i++)
    {
        key1.push_back(i);
        y0.push_back(10);
    }

    ui->customPlot->xAxis->setTickStep(1);
    ui->customPlot->yAxis->setTickStep(1);
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->graph(0)->setData(key1,y0);

    ui->customPlot->xAxis->setRangeLower(0);
    ui->customPlot->xAxis->setRangeUpper(11);
    ui->customPlot->xAxis->setLabel("Location");

    ui->customPlot->yAxis->setRangeLower(0);
    ui->customPlot->yAxis->setRangeUpper(6);
    ui->customPlot->yAxis->setLabel("mV");
    ui->customPlot->replot();

    ui->range->setText("OWTS");

    //QLabel *    label[OWTS_steps][MAX_LOC_MON_LOCATIONS];

    #ifdef PCGUI
    QPixmap circle("/home/nishad/pdd_workspace/oct-16/build/new/circle.png");
    QPixmap square("/home/nishad/pdd_workspace/oct-16/build/new/square.png");
    QPixmap cross("/home/nishad/pdd_workspace/oct-16/build/new/cross.png");
    QPixmap triangle("/home/nishad/pdd_workspace/oct-16/build/new/triangle.png");
    QPixmap unknown("/home/nishad/pdd_workspace/oct-16/build/shapes/unknown.png");
    #endif

    #ifdef BOARDGUI
    QPixmap circle("/home/linaro/oct-16/new/circle.png");
    QPixmap square("/home/linaro/oct-16/new/square.png");
    QPixmap cross("/home/linaro/oct-16/new/cross.png");
    QPixmap triangle("/home/linaro/oct-16/new/triangle.png");
    QPixmap unknown("/home/linaro/oct-16/new/unknown.png");
    #endif




    for(int i=0;i<OWTS_steps;i++)
    {
        for(int j=0;j<MAX_LOC_MON_LOCATIONS;j++)
        {
            //owts.owts_array[i][j]=(unsigned long)1 + ( std::rand() % ( 240 - 1 + 1 ) );

            qDebug("%ld",owts.owts_array[i][j]);


            if(owts.owts_array[i][j]>0 && owts.owts_array[i][j]<80) {
              owts.label[i][j]->setPixmap(cross);
              owts.label[i][j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j)+owts.adjx,ui->customPlot->yAxis->coordToPixel(i)+owts.adjy,LABELWIDTH,LABELHEIGHT);
              owts.label[i][j]->show();
            }

            else if(owts.owts_array[i][j]>80 && owts.owts_array[i][j]<160) {
              owts.label[i][j]->setPixmap(circle);
              owts.label[i][j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j)+owts.adjx,ui->customPlot->yAxis->coordToPixel(i)+owts.adjy,LABELWIDTH,LABELHEIGHT);
              owts.label[i][j]->show();
            }


            else if(owts.owts_array[i][j]>160 && owts.owts_array[i][j]<240) {
              owts.label[i][j]->setPixmap(square);
              owts.label[i][j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j)+owts.adjx,ui->customPlot->yAxis->coordToPixel(i)+owts.adjy,LABELWIDTH,LABELHEIGHT);
              owts.label[i][j]->show();
            }

            else if(owts.owts_array[i][j]>240) {
              owts.label[i][j]->setPixmap(triangle);
              owts.label[i][j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j)+owts.adjx,ui->customPlot->yAxis->coordToPixel(i)+owts.adjy,LABELWIDTH,LABELHEIGHT);
              owts.label[i][j]->show();
            }

            else
            {
                owts.label[i][j]->setPixmap(unknown);
                owts.label[i][j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j)+owts.adjx,ui->customPlot->yAxis->coordToPixel(i)+owts.adjy,LABELWIDTH,LABELHEIGHT);
                owts.label[i][j]->show();
            }


        }

    }


}

