#include<all_headers.h>

using namespace std;

void MainWindow::set_mode(MODE setmode)
{

mode=setmode;

if(mode==SCREENING) {
NO_OF_PLOTTING_VALS =NO_OF_RAW_VALS;
boundry_lower= 0;
boundry_upper= NO_OF_PLOTTING_VALS;
#ifdef DEBUG
qDebug("Screening mode selected\n");
#endif
ui->range->setText("SCREENING...");
}

if(mode==SPECTRUM) {
NO_OF_PLOTTING_VALS =NO_OF_FFT_VALS;
boundry_lower =0;
boundry_upper =NO_OF_PLOTTING_VALS;
#ifdef DEBUG
qDebug("Location monitoring mode selected\n");
#endif
ui->range->setText("SPECTRUM...");
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
