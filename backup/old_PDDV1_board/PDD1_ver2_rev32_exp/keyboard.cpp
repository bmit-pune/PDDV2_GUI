#include<all_headers.h>

using namespace std;
void MainWindow::keyPressEvent(QKeyEvent *e)

{

    if((e->key()==Qt::Key_M) && (e->isAutoRepeat()==false))         //NOISE CAPTURE
    {
        /*key1_noise.clear();
        y0_noise.clear();
        ui->customPlot->replot();
        realtimeDataSlot();*/
        dataTimer.stop();
        if(mode==SCREENING)
            set_mode(SPECTRUM);
        else if(mode==SPECTRUM)
            set_mode(SCREENING);
        dataTimer.start(5000);
    }
    if((e->key()==Qt::Key_I) && (e->isAutoRepeat()==false))         //RANGE INCREASE
    {
        RANGE_INDEX++;
        set_range();
    }
    if((e->key()==Qt::Key_D) && (e->isAutoRepeat()==false))         //RANGE DECREASE
    {
        RANGE_INDEX--;
        set_range();
    }

    if((e->key()==Qt::Key_G) && (e->isAutoRepeat()==false))         //STORE FILE
    {
    QTime time;
    QString filename = time.currentTime().toString().replace(":","_")+".csv";
    FILE* pFile;
    pFile = fopen(filename.toStdString().c_str(), "w");


        for(int i=0;i<NO_OF_PLOTTING_VALS;i++)
        {
            if(mode==SCREENING)
        fprintf(pFile,"%d ,",array[i]);
            if(mode==SPECTRUM)
        fprintf(pFile,"%f ,",arr_float[i]);
        }
    fclose(pFile);
    msgBox.setText(" file stored");
    qDebug()<<filename;
    msgBox.exec();

    }


    if((e->key()==Qt::Key_S) && (e->isAutoRepeat()==false))         //SCREENSHOT
    {
        std::ostringstream oss;
        oss << Present_Zoom_Level;
        std::string temp= "zoomlevel"+oss.str()+"time"+QTime::currentTime().toString().replace(":","_",Qt::CaseInsensitive).toStdString();
        take_screenshot("/opt/pdd/", temp );
    }
    if((e->key()==Qt::Key_P) && (e->isAutoRepeat()==false))         //PAUSE
    {
    #ifdef DEBUG
    printf("Pause key pressed\n");
    #endif

        if(PAUSE_FLAG==0)
        {dataTimer.stop();PAUSE_FLAG=1;}
        else if(PAUSE_FLAG==1)
        {dataTimer.start(5000);PAUSE_FLAG=0;}
    }

}


//connect to Qcustomplot wheel event
void MainWindow::wheelEvent(QWheelEvent *event)
{
on_mouseWheel(event);
}



void MainWindow::on_mouseWheel(QWheelEvent *e)
{
    SCROLL scroll;

    point=e->pos();
    int x=point.x();
    int y=point.y();
    int pixel_abs=x-69;
    int clicked_pixel_point;
        if(mode==SCREENING)
            clicked_pixel_point=pixel_abs* (NO_OF_RAW_VALS/373.72);
        else if(mode==SPECTRUM)
            clicked_pixel_point=pixel_abs* (NO_OF_FFT_VALS/372.89);
    #ifdef DEBUG
        printf("clicked_pixel_point is:%d\n",clicked_pixel_point);
    #endif

    //qDebug("clicked_pixel_point=%d",clicked_pixel_point);


if(e->angleDelta().y()<0)
update_display_array(clicked_pixel_point,scroll=OUT);
else
    update_display_array(clicked_pixel_point,scroll=IN);
}


void MainWindow::take_screenshot(std::string location, std::string filename)
{   //store screenshot to received location
    std::string temp=location+filename+".png";
    QString path=QString::fromUtf8(temp.c_str());
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(QApplication::desktop()->winId(),0,0,
                       QApplication::desktop()->screenGeometry().width(),
                       QApplication::desktop()->screenGeometry().height()).save(path,0,-1);

    msgBox.setText("Screenshot Captured!!!");
    msgBox.exec();
}



