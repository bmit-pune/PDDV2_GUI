#include<QKeyEvent>
#include "ThresholdTime.h"
#include "ui_ThresholdTime.h"

#include<QThread>
#include<qdebug.h>


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


    List << "1mV" << "2mV" << "3mV" <<"4mV"<<"5mV"<< "6mV" << "7mV" << "8mV"<< "9mV"<<"10mV"<<"11mV" << "12mV"<< "13mV"<<"14mV" <<"15mV";
    List1 << "1 Minute" << "2 Minutes" << "3 Minutes" <<"4 Minutes"<<"5 Minutes";

    model->setStringList(List);

    // Glue model and view together
    ui1->listView->setModel(model);

    index = model->index(row);
    index1 = model1->index(row1);

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


    }

if(e->key()==Qt::Key_M)//enter
{

    if(pos==0)
    {
        qDebug()<<"this case executed";
        model1->setStringList(List1);
        ui1->listView->setModel(model1);
        pos=1;//Time selection mode

        switch(model->index(row).row()){
        case 0:Set_Threshold=1;
            break;
        case 1:Set_Threshold=2;
            break;
        case 2:Set_Threshold=3;
            break;
        case 3:Set_Threshold=4;
            break;
        case 4:Set_Threshold=5;
            break;
        case 5:Set_Threshold=6;
            break;
        case 6:Set_Threshold=7;
            break;
        case 7:Set_Threshold=8;
            break;
        case 8:Set_Threshold=9;
            break;
        case 9:Set_Threshold=10;
            break;
        case 10:Set_Threshold=11;
            break;
        case 11:Set_Threshold=12;
            break;
        case 12:Set_Threshold=13;
            break;
        case 13:Set_Threshold=14;
            break;
        case 14:Set_Threshold=15;
            break;
        }

        ui1->label->setText("Set\nTime");
        goto event_end;
    }



    if((pos==1))
    {
        qDebug()<<"this also case executed";
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

        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();

        on_exit_clicked();
    }


    event_end:;
}


}






void ThresholdTime::on_exit_clicked()
{
    qDebug("exit submenu with Threshold=%d & Time=%d",Set_Threshold,Set_Time);
    model->setStringList(List);
    ui1->listView->setModel(model);
    pos=0;
    row=-1;
    row1=-1;
    this->hide();
    emit on_exit_signal();
}



