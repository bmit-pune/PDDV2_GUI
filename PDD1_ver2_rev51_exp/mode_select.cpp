#include<all_headers.h>

using namespace std;

void MainWindow::set_mode(MODE setmode)
{

mode=setmode;




if(mode==SCREENING) {

    ui->customPlot_avg->hide();
    ui->customPlot_max->hide();
    OWTS_hide();
    ui->customPlot->xAxis->setAutoTicks(true);
    ui->customPlot->yAxis->setAutoTicks(true);
    ui->customPlot->xAxis->setAutoTickStep(true);
    ui->customPlot->yAxis->setAutoTickStep(true);

    ui->customPlot->show();
NO_OF_PLOTTING_VALS =NO_OF_RAW_VALS;
boundry_lower= 0;
boundry_upper= NO_OF_PLOTTING_VALS;
#ifdef DEBUG
qDebug("Screening mode selected\n");
#endif
LOC_MON_running=0;
ui->range->setText("SCREENING- TIME DOMAIN");
}

if(mode==SPECTRUM) {
    ui->customPlot_avg->hide();
    ui->customPlot_max->hide();
    OWTS_hide();
    ui->customPlot->xAxis->setAutoTicks(true);
    ui->customPlot->yAxis->setAutoTicks(true);
    ui->customPlot->xAxis->setAutoTickStep(true);
    ui->customPlot->yAxis->setAutoTickStep(true);
    ui->customPlot->show();
NO_OF_PLOTTING_VALS =NO_OF_FFT_VALS;
boundry_lower =0;
boundry_upper =NO_OF_PLOTTING_VALS;
#ifdef DEBUG
qDebug("Location monitoring mode selected\n");
#endif
LOC_MON_running=0;
ui->range->setText("SCREENING- FFT MODE");
}




if(mode==LOC_MON) {
//clear owts

    OWTS_hide();
    ui->customPlot_avg->show();
    ui->customPlot_max->show();
    ui->date_time->setText("0 seconds elapsed");


   // ui->customPlot->hide();
    LOC_MON_running=0;
    qDebug("loc.location=%d\n",loc.location);
    if(loc.location==0)//get time and threshold only for first location
    {
        dialog.Set_Threshold=0;
        dialog.Set_Time=0;
        dialog.show();
        dialog.grabKeyboard();
        //ui->customPlot_avg->show();
        //ui->customPlot_max->show();
        return;
    }

    if(loc.location>MAX_LOC_MON_LOCATIONS-1)
    {
        dataTimer.stop();
        loc.max_temp=0; //let 0 be max for very first time
        loc.avg_temp=0;
        loc.minutes_temp=0;
        loc.iteration=0;
        loc.location=0;
        #ifdef DEBUG
        qDebug("MAX LOCATIONS REACHED\n");
        #endif
        return;
    }
    LOC_MON_running=1;
    dataTimer.start(5000);
}




if(mode==OWTS)
{
    ui->customPlot_avg->hide();
    ui->customPlot_max->hide();
    ui->customPlot->show();
    dataTimer.stop();
    OWTS_display();
}



//Command routine for spectrum and screening mode
Present_Zoom_Level=0;
Total_Zoom_Levels=0;
PZL_lower=boundry_lower;
PZL_upper=boundry_upper;
RANGE_INDEX=0;
PAUSE_FLAG=0;
CLICKZOOM_FLAG=0;
set_range();//Initilize range

//Lut initilize
for(int i=0;((int)NO_OF_PLOTTING_VALS/(pow(2,i)))>=pixel_points;i++)
{
   lut.zoom_level[i]=i;
   lut.L[i]=(int)NO_OF_PLOTTING_VALS/(pow(2,i));
   lut.skip_value[i]=lut.L[i]/pixel_points;
   cout<<lut.zoom_level[i]<<" "<<lut.L[i]<<" "<<lut.skip_value[i]<<endl;
   Total_Zoom_Levels++;
}

#ifdef DEBUG
cout<<"LUT_initialization_completed"<<endl;
#endif


}



void MainWindow::OWTS_hide()
{
    if (!owts.label[0][0]->isHidden()){
    for(int i=0;i<OWTS_steps;i++)
    {
        for(int j=0;j<MAX_LOC_MON_LOCATIONS;j++)
        {
            owts.label[i][j]->hide();
        }
    }
    }


   // memset(owts.owts_array,0,sizeof(owts.owts_array));

}

