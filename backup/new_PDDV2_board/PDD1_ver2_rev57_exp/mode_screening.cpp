#include "all_headers.h"
#include "global_indexes.h"


void MainWindow::update_display_array(int clicked_pixel_point, SCROLL scroll)
{
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

        if (mode == SPECTRUM ) {
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

