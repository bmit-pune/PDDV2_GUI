#include "all_headers.h"
#include "global_indexes.h"

void MainWindow::OWTS_display()
{

#define LABELWIDTH 35
#define LABELHEIGHT 25

#define SEV_LABELWIDTH 55
#define SEV_LABELHEIGHT 335
    owts.adjx = 48;
    owts.adjy = -68;

    owts.sev_label_adjx = 48;
    owts.sev_label_adjy = -25;

    key1.clear();
    y0.clear();
    for (int i = 0; i < 10; i++) {
        key1.push_back(i);
        y0.push_back(10);
    }

    ui->customPlot->xAxis->setTickStep(1);
    ui->customPlot->yAxis->setTickStep(1);
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->graph(0)->setData(key1, y0);

    ui->customPlot->xAxis->setRangeLower(0);
    ui->customPlot->xAxis->setRangeUpper(11);
    ui->customPlot->xAxis->setLabel("Location");

    ui->customPlot->yAxis->setRangeLower(0);
    ui->customPlot->yAxis->setRangeUpper(5);
    ui->customPlot->yAxis->setLabel("mV");
    ui->customPlot->replot();

    ui->range->setText("OWTS- HISTOGRAM");



    QFont my_font("Helvetica [Cronyx]", 7, QFont::Cursive);
    my_font.setWeight(75);

    int count_0, count_1, count_2, count_3, count_4, count_5, count_6, count_7, count_8, count_9;
    count_0 = count_1 = count_2 = count_3 = count_4 = count_5 = count_6 = count_7 = count_8 = count_9 = 0;

    for (int i = 0; i < OWTS_steps; i++) {
        for (int j = 0; j < MAX_LOC_MON_LOCATIONS; j++) {
            //owts.owts_array[i][j]=(unsigned long)1 + ( std::rand() % ( 240 - 1 + 1 ) );

            qDebug("%ld", owts.owts_array[i][j]);

            if (i == 4) {
                owts.severity_label[j]->setGeometry(ui->customPlot->xAxis->coordToPixel(j) + owts.sev_label_adjx, ui->customPlot->yAxis->coordToPixel(i) + owts.sev_label_adjy, SEV_LABELWIDTH, SEV_LABELHEIGHT);
            }

            switch (j) {
            case 0:
                owts.severity_arr_loc0[count_0] = owts.owts_array[i][j];
                count_0++;
                break;
            case 1:
                owts.severity_arr_loc1[count_1] = owts.owts_array[i][j];
                count_1++;
                break;
            case 2:
                owts.severity_arr_loc2[count_2] = owts.owts_array[i][j];
                count_2++;
                break;
            case 3:
                owts.severity_arr_loc3[count_3] = owts.owts_array[i][j];
                count_3++;
                break;
            case 4:
                owts.severity_arr_loc4[count_4] = owts.owts_array[i][j];
                count_4++;
                break;
            case 5:
                owts.severity_arr_loc5[count_5] = owts.owts_array[i][j];
                count_5++;
                break;
            case 6:
                owts.severity_arr_loc6[count_6] = owts.owts_array[i][j];
                count_6++;
                break;
            case 7:
                owts.severity_arr_loc7[count_7] = owts.owts_array[i][j];
                count_7++;
                break;
            case 8:
                owts.severity_arr_loc8[count_8] = owts.owts_array[i][j];
                count_8++;
                break;
            case 9:
                owts.severity_arr_loc9[count_9] = owts.owts_array[i][j];
                count_9++;
                break;
            }


            //owts.label[i][j]->setText(QString::number(owts.owts_array[i][j], 'g', 4));
        }
    }

    static int* recv_index = (int*)malloc(5 * sizeof(int));

    recv_index = index_sort_owts(owts.severity_arr_loc0);
    set_OWTS_label(0, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc1);
    set_OWTS_label(1, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc2);
    set_OWTS_label(2, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc3);
    set_OWTS_label(3, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc4);
    set_OWTS_label(4, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc5);
    set_OWTS_label(5, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc6);
    set_OWTS_label(6, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc7);
    set_OWTS_label(7, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc8);
    set_OWTS_label(8, recv_index);

    recv_index = index_sort_owts(owts.severity_arr_loc9);
    set_OWTS_label(9, recv_index);

    QPalette index_palette;
    QLinearGradient index_gradient(ui->index_label->rect().bottomLeft(), ui->index_label->rect().topLeft());

    index_gradient.setColorAt(0.2, QColor(0, 204, 204));
    index_gradient.setColorAt(0.4, QColor(153, 204, 0));
    index_gradient.setColorAt(0.6, Qt::yellow);
    index_gradient.setColorAt(0.8, QColor(255, 111, 0));
    index_gradient.setColorAt(1.0, Qt::red);
    index_palette.setBrush(QPalette::Background, QBrush(index_gradient));

    ui->index_label->setParent(this);
    ui->index_label->setAutoFillBackground(true);
    ui->index_label->setPalette(index_palette);
    ui->index_label->setText("    High\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n    Low");
    ui->index_label->show();

    ui->index_label->setGeometry(720, 60, SEV_LABELWIDTH, SEV_LABELHEIGHT);
}

void MainWindow::set_OWTS_label(int location, int* recv_index)
{

    QPalette sample_palette;
    QLinearGradient gradient(owts.severity_label[location]->rect().bottomLeft(), owts.severity_label[location]->rect().topLeft());


    qDebug("OWTS For loc=%d\n",loc.location);

    float gradient_position;
    for (int i = 0; i < 5; i++) {
         switch ((*(recv_index + i))) {
        case 0:
            gradient_position = 0.2;
            break;
        case 1:
            gradient_position = 0.4;
            break;
        case 2:
            gradient_position = 0.6;
            break;
        case 3:
            gradient_position = 0.8;
            break;
        case 4:
            gradient_position = 1.0;
            break;
        }

        switch (i) {
        case 0:
            qDebug("gradient_position-->%f\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                 gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            break;
        case 1:
            qDebug("gradient_position-->%f\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                gradient.setColorAt((gradient_position), QColor(153, 204, 0));
            break;
        case 2:
            qDebug("gradient_position-->%f\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                gradient.setColorAt((gradient_position), Qt::yellow);
            break;
        case 3:
            qDebug("gradient_position-->%f\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                gradient.setColorAt((gradient_position), QColor(255, 111, 0));
            break;
        case 4:
            qDebug("gradient_position-->%f\n", gradient_position);
            if (owts.owts_array[*(recv_index + i)][location] == 0)
                gradient.setColorAt((gradient_position), QColor(0, 204, 204));
            else
                gradient.setColorAt((gradient_position), Qt::red);
            break;
        }
    }

    sample_palette.setBrush(QPalette::Background, QBrush(gradient));

    owts.severity_label[location]->setParent(this);
    owts.severity_label[location]->setAutoFillBackground(true);
    owts.severity_label[location]->setPalette(sample_palette);

    if (loc.monitored_locations[location] == 1) //Show bar only if location is aldready monitored
        owts.severity_label[location]->show();
}

int* MainWindow::index_sort_owts(unsigned long temp[])
{
    std::pair<unsigned long, int> pair[5];
    static int array_sort_index[5];

    for (int i = 0; i < 5; i++) {
        pair[i].first = temp[i];
        pair[i].second = i;
    }

#ifdef DEBUG
    qDebug("~~~~~~~~~~~~~~~~~~Before sorting~~~~~~~~~~~~~~~~~~");
    for (int i = 0; i < 5; i++) {
        qDebug("array[%d]=%ld\n", pair[i].second, pair[i].first);
    }
#endif

    std::sort(pair, pair + 5);

#ifdef DEBUG
    qDebug("~~~~~~~~~~~~~~~~~~After sorting~~~~~~~~~~~~~~~~~~");
    for (int i = 0; i < 5; i++) {
        qDebug("array[%d]=%ld\n", pair[i].second, pair[i].first);
    }
    qDebug("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

    for (int i = 0; i < 5; i++)
        array_sort_index[i] = pair[i].second;

    return array_sort_index;
}

void MainWindow::LOC_MON_OWTS_data_clean()
{
#ifdef DEBUG
    qDebug("####only first time condition executed...\n");
#endif
    loc.max_temp = 0; //let 0 be max for very first time
    loc.avg_temp = 0;
    loc.minutes_temp = 0;
    loc.iteration = 0;

    memset(loc.loc_avg, 0, sizeof(loc.loc_avg));
    memset(loc.loc_max, 0, sizeof(loc.loc_max));
    memset(loc.avg_one_loc_minutes, 0, sizeof(loc.avg_one_loc_minutes));
    memset(loc.monitored_locations, 0, sizeof(loc.monitored_locations));

    memset(owts.owts_array, 0, sizeof(owts.owts_array));
    memset(owts.severity_arr_loc0, 0, sizeof(owts.severity_arr_loc0));
    memset(owts.severity_arr_loc1, 0, sizeof(owts.severity_arr_loc1));
    memset(owts.severity_arr_loc2, 0, sizeof(owts.severity_arr_loc2));
    memset(owts.severity_arr_loc3, 0, sizeof(owts.severity_arr_loc3));
    memset(owts.severity_arr_loc4, 0, sizeof(owts.severity_arr_loc4));
    memset(owts.severity_arr_loc5, 0, sizeof(owts.severity_arr_loc5));
    memset(owts.severity_arr_loc6, 0, sizeof(owts.severity_arr_loc6));
    memset(owts.severity_arr_loc7, 0, sizeof(owts.severity_arr_loc7));
    memset(owts.severity_arr_loc8, 0, sizeof(owts.severity_arr_loc8));
    memset(owts.severity_arr_loc9, 0, sizeof(owts.severity_arr_loc9));

    if (mode == LOC_MON)
        ui->range->setText("LOCATION MONITORING");

    ui->customPlot_avg->xAxis->setRangeLower(-1);
    ui->customPlot_avg->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
    ui->customPlot_avg->xAxis->setAutoTickStep(false);
    ui->customPlot_avg->xAxis->setTickStep(1);
    ui->customPlot_avg->yAxis->setAutoSubTicks(false);

    ui->customPlot_max->xAxis->setRangeLower(-1);
    ui->customPlot_max->xAxis->setRangeUpper(MAX_LOC_MON_LOCATIONS);
    ui->customPlot_max->xAxis->setAutoTickStep(false);
    ui->customPlot_max->xAxis->setTickStep(1);

    ui->customPlot_max->xAxis->setAutoTickLabels(false);
    ui->customPlot_avg->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    QVector<QString> labels;
    labels << ""
           << "Loc1"
           << "Loc2"
           << "Loc3"
           << "Loc4"
           << "Loc5"
           << "Loc6"
           << "Loc7"
           << "Loc8"
           << "Loc9"
           << "Loc10";
    ui->customPlot_max->xAxis->setTickVectorLabels(labels);
    ui->customPlot_avg->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickVectorLabels(labels);

    ui->customPlot_avg->xAxis->setTickLabelRotation(-30);
    ui->customPlot_max->xAxis->setTickLabelRotation(-30);
    ui->customPlot->xAxis->setTickLabelRotation(-30);
}

void MainWindow::LOC_MON_OWTS_overwrite_loc_data_clean(int Set_Location)
{
    owts.owts_array[0][Set_Location] = owts.owts_array[1][Set_Location] = owts.owts_array[2][Set_Location] = owts.owts_array[3][Set_Location] = owts.owts_array[4][Set_Location] = 0;
    loc.loc_avg[Set_Location] = 0;
    loc.loc_max[Set_Location] = 0;
    loc.monitored_locations[dialog.Set_Location] = 1; //Log the location which we are monitoring
    qDebug("Location set is %d\n", dialog.Set_Location + 1);
    ui->index_label->setText("Monitor\nLocation:\n" + QString::number(dialog.Set_Location + 1) + "\n\nSet_Max:\n" + QString::number(dialog.Set_Threshold)+" mV"+"\n\nSet Time:\n" + QString::number(dialog.Set_Time)+" Mins" + "\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

