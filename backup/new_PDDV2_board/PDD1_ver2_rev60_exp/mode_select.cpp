#include <all_headers.h>

using namespace std;

void MainWindow::set_mode(MODE setmode)
{

    mode = setmode;


     if (mode == LOC_MON) {
        //clear owts
        OWTS_hide();
        ui->customPlot_avg->show();
        ui->customPlot_max->show();
        ui->date_time->setText("0 seconds elapsed");

        LOC_MON_running = 0;
        qDebug("loc.location=%d\n", loc.location);

        dialog.setup_select_mode(LOC_MON_first_transition);
        dialog.setParent(this);
        dialog.grabKeyboard();
        dialog.show();
        dialog.setGeometry(200,200,81,182);
        dialog.hasFocus();
        return;
    }


    else if (mode == SCREENING) {
         beep(SHORT);
        ui->customPlot_avg->hide();
        ui->customPlot_max->hide();
        OWTS_hide();
        LOC_MON_OWTS_data_clean();
        NOISE_FLAG=0;
        ui->customPlot->xAxis->setAutoTicks(true);
        ui->customPlot->yAxis->setAutoTicks(true);
        ui->customPlot->xAxis->setAutoTickStep(true);
        ui->customPlot->yAxis->setAutoTickStep(true);
        ui->customPlot->xAxis->setAutoTickLabels(true);
        ui->customPlot->show();
        NO_OF_PLOTTING_VALS = NO_OF_RAW_VALS;
        boundry_lower = 0;
        boundry_upper = NO_OF_PLOTTING_VALS;
#ifdef DEBUG
        qDebug("Screening mode selected\n");
#endif
        LOC_MON_running = 0;
        ui->range->setText("SCREENING- TIME DOMAIN");
    }

    else if (mode == SPECTRUM) {
         beep(SHORT);
        key1_noise.clear(); /*Clear previous noise data*/
        y0_noise.clear();
        memset(arr_float_noise,0,sizeof(arr_float_noise));

        ui->customPlot_avg->hide();
        ui->customPlot_max->hide();
        OWTS_hide();
        LOC_MON_OWTS_data_clean();
        ui->customPlot->xAxis->setAutoTicks(true);
        ui->customPlot->yAxis->setAutoTicks(true);
        ui->customPlot->xAxis->setAutoTickStep(true);
        ui->customPlot->yAxis->setAutoTickStep(true);
        ui->customPlot->xAxis->setAutoTickLabels(true);
        ui->customPlot->show();
        NO_OF_PLOTTING_VALS = NO_OF_FFT_VALS;
        boundry_lower = 0;
        boundry_upper = NO_OF_PLOTTING_VALS;
#ifdef DEBUG
        qDebug("Location monitoring mode selected\n");
#endif
        LOC_MON_running = 0;
        ui->range->setText("SCREENING- FFT MODE");
    }
    else if (mode == OWTS_MODE) {
        ui->customPlot_avg->hide();
        ui->customPlot_max->hide();
        ui->customPlot->show();
        dataTimer.stop();
        OWTS_display();
    }

    //Command routine for spectrum and screening mode
    Present_Zoom_Level = 0;
    Total_Zoom_Levels = 0;
    PZL_lower = boundry_lower;
    PZL_upper = boundry_upper;
    RANGE_INDEX = 0;
    S_P_FLAG = 0;
    CLICKZOOM_FLAG = 0;
    set_range(); //Initilize range

    //Lut initilize
    for (int i = 0; ((int)NO_OF_PLOTTING_VALS / (pow(2, i))) >= pixel_points; i++) {
        lut.zoom_level[i] = i;
        lut.L[i] = (int)NO_OF_PLOTTING_VALS / (pow(2, i));
        lut.skip_value[i] = lut.L[i] / pixel_points;
        cout << lut.zoom_level[i] << " " << lut.L[i] << " " << lut.skip_value[i] << endl;
        Total_Zoom_Levels++;
    }

#ifdef DEBUG
    cout << "LUT_initialization_completed" << endl;
#endif
}

void MainWindow::OWTS_hide()
{

    for (int i = 0; i < MAX_LOC_MON_LOCATIONS; i++) {
        owts.severity_label[i]->hide();
    }

    ui->index_label->setPalette(QPalette(QColor(255, 255, 255)));
}
