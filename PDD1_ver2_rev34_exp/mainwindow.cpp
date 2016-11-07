#include"all_headers.h"
#include"global_indexes.h"

char array[NO_OF_RAW_VALS];
float arr_float[NO_OF_FFT_VALS];
float arr_float_noise[NO_OF_FFT_VALS];

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_mode(SPECTRUM);


    ui->centralWidget->setStyleSheet("color: blue;"
                        "background-color: white;"
                        "selection-color: black;"
                        "selection-background-color: blue;");


     setupSimpleDemo(ui->customPlot);

     /*ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                       QCP::iSelectLegend | QCP::iSelectPlottables);*/

     update_data();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupSimpleDemo(QCustomPlot *customPlot)

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

    #ifdef DEBUG
    cout<<"Setup_simple_demo_completed"<<endl;
    #endif

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
    this->update_display_array(0,NO_SCROLL);

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

printf("This stage skip value is %d\n",this->lut.skip_value[Present_Zoom_Level]);

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
    key1.push_back(xval++);
    key1_noise.push_back(xval++);
    }

}

ui->customPlot->graph(0)->setData(key1,y0);//signal
ui->customPlot->graph(1)->setData(key1_noise,y0_noise);//noise


if(mode==SCREENING ) {
ui->customPlot->yAxis->rescale(true);
ui->customPlot->xAxis->rescale(true);
}

if(mode==SPECTRUM) {
ui->customPlot->yAxis->setRangeLower(0);
ui->customPlot->yAxis->setRangeUpper(620);
ui->customPlot->xAxis->rescale(true);
}

ui->customPlot->replot();

key1.clear();
y0.clear();
key1_noise.clear();
y0_noise.clear();

}

