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


    List << "1mV" << "3mV" << "5mV" <<"10mV"<<"15mV"<< "20mV";
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
            row=6;
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

    if((pos==0) && model->index(row).row()==0)
    {
        qDebug()<<"this case executed";



        model1->setStringList(List1);
        ui1->listView->setModel(model1);

        pos=1;//Time selection mode
        Set_Threshold=1;
        ui1->label->setText("Set\nTime");

        goto event_end;
    }

    if((pos==0) &&model->index(row).row()==1)
    {


        model1->setStringList(List1);
        ui1->listView->setModel(model1);

        pos=1;//Time selection mode
        Set_Threshold=3;
        ui1->label->setText("Set\nTime");
        goto event_end;
    }

    if((pos==0) &&model->index(row).row()==2)
    {

        model1->setStringList(List1);
        ui1->listView->setModel(model1);

        pos=1;//Time selection mode
        Set_Threshold=5;
        ui1->label->setText("Set\nTime");
        goto event_end;
    }

    if((pos==0) &&model->index(row).row()==3)
    {

        model1->setStringList(List1);
        ui1->listView->setModel(model1);

        pos=1;//Time selection mode
        Set_Threshold=10;
        ui1->label->setText("Set\nTime");
        goto event_end;
    }

    if((pos==0) &&model->index(row).row()==4)
    {

        model1->setStringList(List1);
        ui1->listView->setModel(model1);

        pos=1;//Time selection mode
        Set_Threshold=15;
        ui1->label->setText("Set\nTime");
        goto event_end;
    }

    if((pos==0) &&model->index(row).row()==5)
    {

        model1->setStringList(List1);
        ui1->listView->setModel(model1);

        pos=1;//Time selection mode
        Set_Threshold=20;
        ui1->label->setText("Set\nTime");
        goto event_end;
    }


    if((pos==1) &&model1->index(row1).row()==0)
    {
        qDebug()<<"this also case executed";
        row1=0;
        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();
        Set_Time=1;
        on_exit_clicked();
    }
    if((pos==1) &&model1->index(row1).row()==1)
    {

        row1=1;
        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();
        Set_Time=2;
        on_exit_clicked();
    }
    if((pos==1) &&model1->index(row1).row()==2)
    {

        row1=2;
        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();
        Set_Time=3;
        on_exit_clicked();
    }
    if((pos==1) && model1->index(row1).row()==3)
    {

        row1=3;
        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();
        Set_Time=4;
        on_exit_clicked();
    }
    if((pos==1) && model1->index(row1).row()==4)
    {

        row1=4;
        index1=model1->index(row1);
        ui1->listView->setCurrentIndex(index1);
        ui1->listView->hasFocus();
        Set_Time=5;
        on_exit_clicked();
    }

    if((pos==1) && model1->index(row1).row()==5)
    {
        row1=0;
    }

    event_end:;
}


}






void ThresholdTime::on_exit_clicked()
{
    //qDebug("exit submenu");
    model->setStringList(List);
    ui1->listView->setModel(model);
    pos=0;
    row=-1;
    row1=-1;
    this->hide();
    emit on_exit_signal();
}



