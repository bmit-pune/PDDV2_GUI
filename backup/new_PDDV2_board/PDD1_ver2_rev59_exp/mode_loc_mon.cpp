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
        qDebug("Timer started:25\n");
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

        for (int i = 0; i <= MAX_LOC_MON_LOCATIONS - 1; i++) {
            if (loc.monitored_locations[i] == 1) {
                key1_locavg.push_back(i);
                y0_locavg.push_back(set_precision(loc.loc_avg[i],3));
                //y0_locavg.push_back(loc.loc_avg[i]);
                qDebug("loc->%d::avg->%f*", i, loc.loc_avg[i]);
                //ui->customPlot_avg->graph(0)->setData(key1_locavg,y0_locavg);//plot avg data

                key1_locmax.push_back(i);
                y0_locmax.push_back(set_precision(loc.loc_max[i],3));
                //y0_locmax.push_back(loc.loc_max[i]);
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
       qDebug("$$$$$$$For Location=%d\n",loc.location);
       qDebug("$$$$$$$$Before Precision=%f\n",number);
       double  intpart;
       double fractpart = modf (number, &intpart);
       fractpart  = roundf(fractpart * (float)pow(10.0,precision) ) / (float)pow(10.0,precision) ; // Round to precision decimal places
       qDebug("$$$$$$$$After Precision=%f\n",intpart + fractpart);
       return(intpart + fractpart) ;

}
