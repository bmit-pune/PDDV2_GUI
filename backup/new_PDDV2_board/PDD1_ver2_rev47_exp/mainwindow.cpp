#include"all_headers.h"
#include"global_indexes.h"

unsigned short int array[NO_OF_RAW_VALS];
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

    QPen pen3(QBrush(QColor(250, 10,10,255)), 3, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
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
    customPlot_avg->xAxis->setTickStep(0);//distance between steps on x-axis
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
            y0.push_back((unsigned short int)array[i]);
            key1.push_back(xval++);
    }

    if(mode==SPECTRUM /*&& (i%this->lut.skip_value[Present_Zoom_Level])==0*/ ) {
    y0.push_back((float)arr_float[i]);
    y0_noise.push_back((float)arr_float_noise[i]);
    key1.push_back(61035.155 * xval ); //fs/(n/2) * sample index fs-375mHz, n=4096
    key1_noise.push_back(61035.155 * xval);
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
ui->customPlot->yAxis->rescale(true);
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

    unsigned int count=0;

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

        memset(owts.severity_arr_loc0,0,sizeof(owts.severity_arr_loc0));
        memset(owts.severity_arr_loc1,0,sizeof(owts.severity_arr_loc1));
        memset(owts.severity_arr_loc2,0,sizeof(owts.severity_arr_loc2));
        memset(owts.severity_arr_loc3,0,sizeof(owts.severity_arr_loc3));
        memset(owts.severity_arr_loc4,0,sizeof(owts.severity_arr_loc4));
        memset(owts.severity_arr_loc5,0,sizeof(owts.severity_arr_loc5));
        memset(owts.severity_arr_loc6,0,sizeof(owts.severity_arr_loc6));
        memset(owts.severity_arr_loc7,0,sizeof(owts.severity_arr_loc7));
        memset(owts.severity_arr_loc8,0,sizeof(owts.severity_arr_loc8));
        memset(owts.severity_arr_loc9,0,sizeof(owts.severity_arr_loc9));


        ui->range->setText("LOC_MON");

        ui->customPlot_avg->xAxis->setRangeLower(-1);
        ui->customPlot_avg->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
        ui->customPlot_max->xAxis->setRangeLower(-1);
        ui->customPlot_max->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
        ui->customPlot_avg->replot();
        ui->customPlot_max->replot();

        update_data();

   }

    if(loc.iteration< (dialog.Set_Time*12))
    {

        qDebug("inside if loc.iteration=%d\n",loc.iteration);
        ui->date_time->setText(QString::number((loc.iteration + 1)*5)+" seconds elapsed");
        loc.max_temp=0;
        loc.avg_temp=0;
        count=0;




        for(int i=0;(i<NO_OF_RAW_VALS) ;i++)
        {
            if (((dialog.Set_Threshold>=arr_float_mV[i]) && arr_float_mV[i]>0))
            {
                if(loc.max_temp<arr_float_mV[i])  //for max
                    loc.max_temp=arr_float_mV[i];


                loc.avg_temp+=(int)arr_float_mV[i];//for avg
                count++;


            if(arr_float_mV[i]>= 0 && arr_float_mV[i]< 3) //for owts
                owts.owts_array[0][loc.location]++;
            else if(arr_float_mV[i]>3 && arr_float_mV[i]<6)
                owts.owts_array[1][loc.location]++;
            else if(arr_float_mV[i]>6 && arr_float_mV[i]<9)
                owts.owts_array[2][loc.location]++;
            else if(arr_float_mV[i]>9 && arr_float_mV[i]<12)
                owts.owts_array[3][loc.location]++;
            else if(arr_float_mV[i]>12 && arr_float_mV[i]<15)
                owts.owts_array[4][loc.location]++;
            }

        }

        qDebug("########Final max_temp:%f\n",loc.max_temp);
        qDebug("########Final max:%f -----> iteraton=%d\n",loc.max_one_loc_minutes[loc.iteration]=loc.max_temp,loc.iteration);
        #ifdef DEBUG
        qDebug("loc.avg_temp:%f\n",loc.avg_temp);
        qDebug("count is:%d\n",count);
        #endif
        if(count==0 && loc.avg_temp==0)
            loc.avg_one_loc_minutes[loc.iteration]=0;
        else
        qDebug("########Final Avg:%f -----> iteraton=%d\n",loc.avg_one_loc_minutes[loc.iteration]=loc.avg_temp/count,loc.iteration);
        loc.iteration++;


    }


    else
    {
        ui->date_time->setText("Complete,Change Location...");

        loc.max_temp=0;
        loc.avg_temp=0;
        count=0;

        for(int i=1;i< ((dialog.Set_Time*12));i++)
        {
           if(loc.max_temp < loc.max_one_loc_minutes[i]) //for max
                loc.max_temp=loc.max_one_loc_minutes[i];
           #ifdef DEBUG
           qDebug("loc.max_temp=%f\n",loc.max_temp);
           #endif

           loc.avg_temp+=loc.avg_one_loc_minutes[i]; //for avg
           #ifdef DEBUG
           qDebug("loc.avg_temp=%f\n",loc.avg_one_loc_minutes[i]);
           #endif
           count++;
        }

        qDebug("***avg::%f",loc.loc_avg[loc.location]=loc.avg_temp/count); //to be plotted
        qDebug("***max::%f",loc.loc_max[loc.location]=loc.max_temp);

        for(int i=0;i<=loc.location;i++)
        {
            key1_locavg.push_back(i);
            y0_locavg.push_back(loc.loc_avg[i]);
            qDebug("loc->%d::avg->%f*",i,loc.loc_avg[i]);
            ui->customPlot_avg->graph(0)->setData(key1_locavg,y0_locavg);//plot avg data

            key1_locmax.push_back(i);
            y0_locmax.push_back(loc.loc_max[i]);
            qDebug("        max->%f*",loc.loc_max[i]);
            ui->customPlot_max->graph(0)->setData(key1_locmax,y0_locmax);//plot max data

        }


        ui->customPlot_avg->yAxis->rescale(true);
        ui->customPlot_avg->xAxis->setRangeLower(-1);
        ui->customPlot_avg->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
        ui->customPlot_avg->replot();


        ui->customPlot_max->yAxis->rescale(true);
        ui->customPlot_max->xAxis->setRangeLower(-1);
        ui->customPlot_max->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
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


#define LABELWIDTH 35
#define LABELHEIGHT 25
    owts.adjx=48;
    owts.adjy=-68;
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


        QFont my_font( "Helvetica [Cronyx]", 7, QFont::Cursive);
        my_font.setWeight(75);


        int count_0,count_1,count_2,count_3,count_4,count_5,count_6,count_7,count_8,count_9;
        count_0=count_1=count_2=count_3=count_4=count_5=count_6=count_7=count_8=count_9=0;


    for(int i=0;i<OWTS_steps;i++)
    {
        for(int j=0;j<MAX_LOC_MON_LOCATIONS;j++)
        {
            //owts.owts_array[i][j]=(unsigned long)1 + ( std::rand() % ( 240 - 1 + 1 ) );

            qDebug("%ld",owts.owts_array[i][j]);

            owts.label[i][j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j)+owts.adjx,ui->customPlot->yAxis->coordToPixel(i)+owts.adjy,LABELWIDTH,LABELHEIGHT);

            switch(j)
            {
            case 0:
                owts.severity_arr_loc0[count_0]=owts.owts_array[i][j];count_0++;
                break;
            case 1:
                owts.severity_arr_loc1[count_1]=owts.owts_array[i][j];count_1++;
                break;
            case 2:
                owts.severity_arr_loc2[count_2]=owts.owts_array[i][j];count_2++;
                break;
            case 3:
                owts.severity_arr_loc3[count_3]=owts.owts_array[i][j];count_3++;
                break;
            case 4:
                owts.severity_arr_loc4[count_4]=owts.owts_array[i][j];count_4++;
                break;
            case 5:
                owts.severity_arr_loc5[count_5]=owts.owts_array[i][j];count_5++;
                break;
            case 6:
                owts.severity_arr_loc6[count_6]=owts.owts_array[i][j];count_6++;
                break;
            case 7:
                owts.severity_arr_loc7[count_7]=owts.owts_array[i][j];count_7++;
                break;
            case 8:
                owts.severity_arr_loc8[count_8]=owts.owts_array[i][j];count_8++;
                break;
            case 9:
                owts.severity_arr_loc9[count_9]=owts.owts_array[i][j];count_9++;
                break;

            }


             owts.label[i][j]->setFont(my_font);
            //owts.label[i][j]->setStyleSheet("QLabel {background-color : white; color : black;}");
           //owts.label[i][j]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
            owts.label[i][j]->setWordWrap(true);
            owts.label[i][j]->setText(QString::number(owts.owts_array[i][j],'g',4));

            /*if(owts.owts_array[i][j]>0 && owts.owts_array[i][j]<30) {
              owts.label[i][j]->setPixmap(cross);
            }

            else if(owts.owts_array[i][j]>30 && owts.owts_array[i][j]<55) {
              owts.label[i][j]->setPixmap(circle);
            }


            else if(owts.owts_array[i][j]>55 && owts.owts_array[i][j]<1300) {
              owts.label[i][j]->setPixmap(square);
            }

            else if(owts.owts_array[i][j]>1300) {
              owts.label[i][j]->setPixmap(triangle);
            }

            else
            {
                owts.label[i][j]->setPixmap(unknown);
            }*/



            //owts.label[i][j]->show();
        }

    }

   // std::pair<unsigned long, int> * pair =(std::pair<unsigned long, int> *) malloc(5* sizeof(std::pair<unsigned long, int>));

    static int *recv_index=(int *)malloc(5*sizeof(int));


    recv_index=index_sort_owts(owts.severity_arr_loc0);
    set_OWTS_label(0,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc1);
    set_OWTS_label(1,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc2);
    set_OWTS_label(2,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc3);
    set_OWTS_label(3,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc4);
    set_OWTS_label(4,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc5);
    set_OWTS_label(5,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc6);
    set_OWTS_label(6,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc7);
    set_OWTS_label(7,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc8);
    set_OWTS_label(8,recv_index);

    recv_index=index_sort_owts(owts.severity_arr_loc9);
    set_OWTS_label(9,recv_index);


}

void MainWindow::set_OWTS_label(int location,int* recv_index){

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
    for(int i=0;i<5;i++){
        switch(i){
        case 0:
              if(owts.owts_array[*(recv_index + i)][location]==0)
                owts.label[*(recv_index + i)][location]->setText("____");
              else
                owts.label[*(recv_index + i)][location]->setPixmap(cross);
            break;
        case 1:
            if(owts.owts_array[*(recv_index + i)][location]==0)
              owts.label[*(recv_index + i)][location]->setText("____");
            else
              owts.label[*(recv_index + i)][location]->setPixmap(circle);
          break;
        case 2:
            if(owts.owts_array[*(recv_index + i)][location]==0)
              owts.label[*(recv_index + i)][location]->setText("____");
            else
              owts.label[*(recv_index + i)][location]->setPixmap(tick);
          break;
        case 3:
            if(owts.owts_array[*(recv_index + i)][location]==0)
              owts.label[*(recv_index + i)][location]->setText("____");
            else
              owts.label[*(recv_index + i)][location]->setPixmap(square);
          break;
        case 4:
            if(owts.owts_array[*(recv_index + i)][location]==0)
              owts.label[*(recv_index + i)][location]->setText("____");
            else
              owts.label[*(recv_index + i)][location]->setPixmap(triangle);
          break;
        }
        owts.label[*(recv_index + i)][location]->show();
    }


}


int* MainWindow::index_sort_owts(unsigned long temp[])
{
    std::pair<unsigned long, int> pair[5];
    static int array_sort_index[5];

    for(int i=0;i<5;i++){
        pair[i].first=temp[i];
        pair[i].second=i;
    }

#ifdef DEBUG
     qDebug("~~~~~~~~~~~~~~~~~~Before sorting~~~~~~~~~~~~~~~~~~");


    for(int i=0;i<5;i++){
        qDebug("array[%d]=%ld\n",pair[i].second,pair[i].first);
    }
#endif

 //qsort(pair,pair+5,comp_func);

 std::sort(pair,pair+5);

#ifdef DEBUG
 qDebug("~~~~~~~~~~~~~~~~~~After sorting~~~~~~~~~~~~~~~~~~");


 for(int i=0;i<5;i++){
     qDebug("array[%d]=%ld\n",pair[i].second,pair[i].first);
 }


 qDebug("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

 for(int i=0;i<5;i++)
     array_sort_index[i]=pair[i].second;

 return array_sort_index;

 }



