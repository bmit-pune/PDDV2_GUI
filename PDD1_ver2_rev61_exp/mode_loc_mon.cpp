#include "all_headers.h"
#include "global_indexes.h"


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
        //qDebug("Timer started:25\n");
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
                if (arr_float_mV[i] >= 0 && arr_float_mV[i] < (dialog.Set_Threshold * 1*0.05)) //for owts
                    owts.owts_array[0][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 1*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 2*0.05))
                    owts.owts_array[1][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 2*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 3*0.05))
                    owts.owts_array[2][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 3*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 4*0.05))
                    owts.owts_array[3][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 4*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 5*0.05))
                    owts.owts_array[4][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 5*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 6*0.05))
                    owts.owts_array[5][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 6*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 7*0.05))
                    owts.owts_array[6][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 7*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 8*0.05))
                    owts.owts_array[7][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 8*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 9*0.05))
                    owts.owts_array[8][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 9*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 10*0.05))
                    owts.owts_array[9][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 10*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 11*0.05))
                    owts.owts_array[10][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 11*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 12*0.05))
                    owts.owts_array[11][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 12*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 13*0.05))
                    owts.owts_array[12][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 13*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 14*0.05))
                    owts.owts_array[13][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 14*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 15*0.05))
                    owts.owts_array[14][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 15*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 16*0.05))
                    owts.owts_array[15][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 16*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 17*0.05))
                    owts.owts_array[16][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 17*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 18*0.05))
                    owts.owts_array[17][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 18*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 19*0.05))
                    owts.owts_array[18][dialog.Set_Location]++;
                else if (arr_float_mV[i] > (dialog.Set_Threshold * 19*0.05) && arr_float_mV[i] < (dialog.Set_Threshold * 20*0.05))
                    owts.owts_array[19][dialog.Set_Location]++;
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
        beep(LONG);

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

        for (int i = 0; i <= MAX_LOC_MON_LOCATIONS - 1; i++) {
            if (loc.monitored_locations[i] == 1) {
                key1_locavg.push_back(i);
                //y0_locavg.push_back(set_precision(loc.loc_avg[i],3));
                y0_locavg.push_back(loc.loc_avg[i]);
                qDebug("loc->%d::avg->%f*", i, loc.loc_avg[i]);
                //ui->customPlot_avg->graph(0)->setData(key1_locavg,y0_locavg);//plot avg data

                key1_locmax.push_back(i);
                //y0_locmax.push_back(set_precision(loc.loc_max[i],3));
                y0_locmax.push_back(loc.loc_max[i]);
                qDebug("        max->%f*", loc.loc_max[i]);
                //ui->customPlot_max->graph(0)->setData(key1_locmax,y0_locmax);//plot max data
            }
        }


        ui->customPlot_avg->yAxis->setNumberFormat("g");
        ui->customPlot_max->yAxis->setNumberFormat("g");
        ui->customPlot_avg->yAxis->setNumberPrecision(4);
        ui->customPlot_max->yAxis->setNumberPrecision(4);

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

double MainWindow::set_precision(double number, int precision){
      // qDebug("$$$$$$$For Location=%d\n",loc.location);
      // qDebug("$$$$$$$$Before Precision=%f\n",number);
       double  intpart;
       double fractpart = modf (number, &intpart);
       fractpart  = roundf(fractpart * (float)pow(10.0,precision) ) / (float)pow(10.0,precision) ; // Round to precision decimal places
     //  qDebug("$$$$$$$$After Precision=%f\n",intpart + fractpart);
       return(intpart + fractpart) ;

}
