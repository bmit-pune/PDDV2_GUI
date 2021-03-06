#include"all_headers.h"
#include"global_indexes.h"


unsigned int array[NO_OF_PLOTTING_VALS];
float * arr_float;
//unsigned int* arr_int;
/*fftwf_complex in[FFT_VALS];
fftwf_complex out[FFT_VALS];
extern fftwf_complex out_noise_capture[FFT_VALS];*/


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Present_Zoom_Level=0;
    Total_Zoom_Levels=0;
    PZL_lower=boundry_lower;
    PZL_upper=boundry_upper;
    RANGE_INDEX=0;
    PAUSE_FLAG=0;
    set_range();//Initilize range
    /*memset(out_noise_capture, 0, sizeof(out_noise_capture) );*/

    ui->centralWidget->setStyleSheet("color: blue;"
                        "background-color: white;"
                        "selection-color: black;"
                        "selection-background-color: blue;");


     setupSimpleDemo(ui->customPlot);

     ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                       QCP::iSelectLegend | QCP::iSelectPlottables);

     update_data();

     //Lut initilize

     for(int i=0;((int)NO_OF_PLOTTING_VALS/(pow(2,i)))>=pixel_points;i++)
     {
        lut.zoom_level[i]=i;
        lut.L[i]=(int)NO_OF_PLOTTING_VALS/(pow(2,i));
        lut.skip_value[i]=lut.L[i]/pixel_points;
        cout<<lut.zoom_level[i]<<" "<<lut.L[i]<<" "<<lut.skip_value[i]<<endl;
        Total_Zoom_Levels++;

     }


cout<<"LUT_initialization_completed"<<endl;

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
   /* customPlot->addGraph();
    customPlot->graph(1)->setPen(pen2);
    customPlot->graph(1)->setBrush(QBrush(QColor(255, 0,0,100)));*/




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

    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_mousePress(QMouseEvent*)));
    connect(ui->customPlot,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(on_mouseWheel(QWheelEvent*)));

    cout<<"Setup_simple_demo_completed"<<endl;
}



void MainWindow::realtimeDataSlot()
{

cout<<"real_time_data_slot_started"<<endl;
    //both x-axes n y-axes scale simultaneously
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    Present_Zoom_Level=0;
    update_data();
    this->update_display_array(0,NO_SCROLL);
cout<<"real_time_data_slot_completed"<<endl;


}

void MainWindow::update_display_array(int clicked_pixel_point, SCROLL scroll)
{

    cout<<"update_display_array_started"<<endl;
    //dataTimer.a();
    /*qDebug("zoom level:%d",Present_Zoom_Level);*/
    if(scroll==IN && Present_Zoom_Level==Total_Zoom_Levels-1)
    {/*qDebug("reached max zoom in");*/return;}

    if(scroll==OUT && Present_Zoom_Level==0)
    {/*qDebug("reached min zoom out");*/return;}

    //int point=display_array_index[clicked_pixel_point];
    /*qDebug("point=%d",point);*/
    int min,max;

    switch(scroll)
    {
    case IN: /*qDebug("in**");*/
  //  min=point-(int)(this->lut.L[Present_Zoom_Level+1]/2);

    if(min < boundry_lower )
    {/*qDebug("inside IN if");*/min=boundry_lower;max=min+this->lut.L[Present_Zoom_Level+1];Present_Zoom_Level++;break;}

  //  max=point+(int)(this->lut.L[Present_Zoom_Level+1]/2);
    if(max>boundry_upper)
    {/*qDebug("inside max IN if");*/max=boundry_upper;min=max-this->lut.L[Present_Zoom_Level+1];}
    Present_Zoom_Level++;
    break;

    case OUT:/*qDebug("out**");*/
    //min=point-(this->lut.L[Present_Zoom_Level]);
    /*qDebug("min=%d",min);*/
   // if(min < boundry_lower )
    //    {/*qDebug("inside min oUT if");*/min=boundry_lower;max=(int)(this->lut.L[Present_Zoom_Level-1]);Present_Zoom_Level--;/*qDebug("MIn:%d",min);qDebug("Max:%d",max);*/break;}

  //  max=point+(this->lut.L[Present_Zoom_Level]);
    if(max>boundry_upper)
    {/*qDebug("inside max oUT if");*/max=boundry_upper;min=max-(int)(this->lut.L[Present_Zoom_Level-1]);}
    Present_Zoom_Level--;
    break;


    case NO_SCROLL:/*qDebug("no-scroll**");*/
    min=boundry_lower;
    max=boundry_upper;
    break;



};




qDebug("min=%d",min);
qDebug("max=%d",max);

qDebug(QTime::currentTime().toString().toLatin1());

/*----------------------------------------------------------------------*/
int i=0;


/*for (i = 0; i < FFT_VALS ; i++)
{
in[i][REAL]=float(array[i]);
in[i][IMAG]=0;

}*/



/*fftwf_plan plan = fftwf_plan_dft_1d(FFT_VALS,
in,
out,
FFTW_FORWARD,
FFTW_ESTIMATE);


fftwf_execute ( plan );

printf ( "\n" );
printf ( "  Output FFT Coefficients:\n" );
printf ( "\n" );*/


for ( i = RANGE_LOW; i < 4*1024  ; i++ )
{



    if(i<=50)
      continue;
    //cout<<"Index is:"<<i<<endl;

    y0.push_back(arr_float[i]);
    key1.push_back(i);

 /* y0.push_back(sqrt(pow(out[i][REAL],2)+pow(out[i][IMAG],2)));
  key1.push_back(i*(FS_DIVIDE_BY_FFT_VALS));//index*(fs/N)

  y0_noise.push_back(sqrt(pow(out_noise_capture[i][REAL],2)+pow(out_noise_capture[i][IMAG],2)));
  key1_noise.push_back(i*FS_DIVIDE_BY_FFT_VALS);//index*(fs/N)|fs=3GHz|N=64K|

  if(i==RANGE_LOW)
      printf("pused low:%d",i*(FS_DIVIDE_BY_FFT_VALS));

  if(i==RANGE_HIGH-1)
      printf("pused high:%d",i*(FS_DIVIDE_BY_FFT_VALS));
*/



}

cout<<"array_get_loop_completed"<<endl;
/*fftwf_destroy_plan( plan );*/

ui->customPlot->graph(0)->setData(key1,y0);//signal
/*ui->customPlot->graph(1)->setData(key1_noise,y0_noise);//noise*/

cout<<"set_data_completed"<<endl;

ui->customPlot->yAxis->rescale(true);
ui->customPlot->xAxis->rescale(true);
ui->customPlot->replot();
s
key1.clear();
y0.clear();
cout<<"update_display_array_completed"<<endl;
}

