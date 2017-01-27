/*
#include "threshold.h"
#include "ui_threshold.h"
#include<QKeyEvent>
#include<iostream>
#include<global_indexes.h>



int THRESHOLD=0;

using namespace std;


Threshold::Threshold(QWidget *parent) :
    QDialog(parent),
    ui1(new Ui::Threshold)
{
    ui1->setupUi(this);
    model = new QStringListModel(this);




    List << "1mv" << "2mv" << "3mv" << "4mv" << "5mv"<<"6mv"<<"7mv"<<"8mv";
    model->setStringList(List);    
    ui1->listView->setModel(model);
    index = model->index(row);
}

Threshold::~Threshold()
{
    delete ui1;
}

void Threshold::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Plus)
    {

        row=ui1->listView->currentIndex().row();
        cout<<"Row is:"<<row;
        row=row-1;
        if(row==(-1)||row==(-2))row=7;
        index=model->index(row);
        ui1->listView->setCurrentIndex(index);
        ui1->listView->hasFocus();

    }

    if(e->key()==Qt::Key_Minus)
    {

            row=ui1->listView->currentIndex().row();
            row=row+1;
            index=model->index(row);
            ui1->listView->setCurrentIndex(index);
            ui1->listView->hasFocus();



    }

if(e->key()==Qt::Key_M)
{

   switch(ui1->listView->currentIndex().row())
   {
   case 0:THRESHOLD=1000;//In uVolts
       break;
   case 1:THRESHOLD=2000;
       break;
   case 2:THRESHOLD=3000;
       break;
   case 3:THRESHOLD=4000;
       break;
   case 4:THRESHOLD=5000;
       break;
   case 5:THRESHOLD=6000;
       break;
   case 6:THRESHOLD=7000;
       break;
   case 7:THRESHOLD=8000;
       break;
   }
cout<<"threshold set to"<<THRESHOLD<<endl;
threshold_status=SET;
this->hide();
if(mode==BNC_mode)
emit calibration_signal();

if(mode==BCH_mode)
    emit start_BCHmode_display();


}
}






*/
