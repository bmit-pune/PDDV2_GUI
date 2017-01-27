#include<QKeyEvent>
#include "ThresholdTime.h"
#include "ui_ThresholdTime.h"

#include<QThread>
#include<qdebug.h>
#include"global_indexes.h"

#include"mainwindow.h"


ThresholdTime::ThresholdTime(QWidget *parent) :
    QDialog(parent),
    ui1(new Ui::ThresholdTime)
{
    ui1->setupUi(this);
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setFocusPolicy(Qt::StrongFocus);
    this->hasFocus();


    model = new QStringListModel(this);
    model1 = new QStringListModel(this);
    model2 = new QStringListModel(this);


    List << "3mV" << "6mV" << "9mV" <<"12mV"<<"15mV"<< "18mV" << "21mV" << "24mV"<< "27mV"<<"30mV"<<"33mV" << "36mV"<< "39mV"<<"42mV" <<"45mV";
    List1 << "1 Minute" << "2 Minutes" << "3 Minutes" <<"4 Minutes"<<"5 Minutes";
    List2 << "Loc1" << "Loc2" << "Loc3" <<"Loc4"<<"Loc5"<<"Loc6"<<"Loc7"<<"Loc8"<<"Loc9"<<"Loc10";

    model->setStringList(List);

    // Glue model and view together
    ui1->listView->setModel(model);

    index = model->index(row);
    index1 = model1->index(row1);
    index2=model2->index(row2);

    ui1->label->setText("Set\nThreshold");


    //connect(this,SIGNAL(on_exit_signal()),this,SLOT(on_exit_clicked()));
    pos=0;



 }

ThresholdTime::~ThresholdTime()
{
    delete ui1;
}

void ThresholdTime::keyPressEvent(QKeyEvent *e)
{


    if(e->key()==Qt::Key_I)
    {
        if(pos==0)
        {
        row=ui1->listView->currentIndex().row();
        if(row<0)
            row=15;
        row=row-1;
        index=model->index(row);
        ui1->listView->setCurrentIndex(index);
        ui1->listView->hasFocus();
        }

        if(pos==1)
        {
        row1=ui1->listView->currentIndex().row();
        if(row1<0)
            row1=5;
        row1=row1-1;
        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();
        }

        if(pos==2)
        {
        row2=ui1->listView->currentIndex().row();
        if(row2<0)
            row2=10;
        row2=row2-1;
        index2=model2->index(row2);
        ui1->listView->setCurrentIndex(index2);
        ui1->listView->hasFocus();
        }



    }

    if(e->key()==Qt::Key_D)
    {
        if(pos==0)
        {
            row=ui1->listView->currentIndex().row();
            row=row+1;
            index=model->index(row);
            ui1->listView->setCurrentIndex(index);
            ui1->listView->hasFocus();
        }

        if(pos==1)
        {
            row1=ui1->listView->currentIndex().row();
            row1=row1+1;
            index1=model1->index(row1);
            ui1->listView->setCurrentIndex(index1);
            ui1->listView->hasFocus();
        }

        if(pos==2)
        {
            row2=ui1->listView->currentIndex().row();
            row2=row2+1;
            index2=model2->index(row2);
            ui1->listView->setCurrentIndex(index2);
            ui1->listView->hasFocus();
        }


    }

if(e->key()==Qt::Key_M)//enter
{

    if(pos==0)
    {

        model1->setStringList(List1);
        ui1->listView->setModel(model1);
        pos=1;//Time selection mode

        switch(model->index(row).row()){
        case 0:Set_Threshold=3;
            break;
        case 1:Set_Threshold=6;
            break;
        case 2:Set_Threshold=9;
            break;
        case 3:Set_Threshold=12;
            break;
        case 4:Set_Threshold=15;
            break;
        case 5:Set_Threshold=18;
            break;
        case 6:Set_Threshold=21;
            break;
        case 7:Set_Threshold=24;
            break;
        case 8:Set_Threshold=27;
            break;
        case 9:Set_Threshold=30;
            break;
        case 10:Set_Threshold=33;
            break;
        case 11:Set_Threshold=36;
            break;
        case 12:Set_Threshold=39;
            break;
        case 13:Set_Threshold=42;
            break;
        case 14:Set_Threshold=45;
            break;
        }

        ui1->label->setText("Set\nTime");
        goto event_end;
    }



    if((pos==1))
    {
        model2->setStringList(List2);
        ui1->listView->setModel(model2);
        pos=2;//Loc selection mode
        ui1->label->setText("Set\nLocation");

        switch(model1->index(row1).row()){
        case 0:row1=0;Set_Time=1;
            break;
        case 1:row1=1;Set_Time=2;
            break;
        case 2:row1=2;Set_Time=3;
            break;
        case 3:row1=3;Set_Time=4;
            break;
        case 4:row1=4;Set_Time=5;
            break;
        }
        goto event_end;


        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();

        on_exit_clicked();
    }



    if((pos==2))
    {


        switch(model2->index(row2).row()){
        case 0:Set_Location=0;
            break;
        case 1:Set_Location=1;
            break;
        case 2:Set_Location=2;
            break;
        case 3:Set_Location=3;
            break;
        case 4:Set_Location=4;
            break;
        case 5:Set_Location=5;
            break;
        case 6:Set_Location=6;
            break;
        case 7:Set_Location=7;
            break;
        case 8:Set_Location=8;
            break;
        case 9:Set_Location=9;
            break;
        }
        row2=0;
        on_exit_clicked();
    }


    event_end:;
}



}






void ThresholdTime::on_exit_clicked()
{
//#ifdef DEBUG
    qDebug("exit submenu with Threshold=%d & Time=%d & LOC=%d ",Set_Threshold,Set_Time,Set_Location);
//#endif
    model->setStringList(List);
    ui1->listView->setModel(model);
    pos=0;
    row=-1;
    row1=-1;
    this->hide();
    emit on_exit_signal();
}

void ThresholdTime::setup_select_mode(int mode){
if(mode==1) {  //first time here, gice option to select threshold time and location
    model->setStringList(List);
    ui1->listView->setModel(model);
    pos=0;
    row=-1;
    row1=-1;
}

    else{ //only option to select location
    ui1->label->setText("Set\nLocation");
    model2->setStringList(List2);
    ui1->listView->setModel(model2);
    pos=2;
    row=-1;
    row1=-1;
    row2=-1;
}

}



