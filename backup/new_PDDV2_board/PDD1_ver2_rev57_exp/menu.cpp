#include "menu.h"
#include "ui_menu.h"
#include<QKeyEvent>
#include<QThread>


menu::menu(QWidget *parent) :
    QDialog(parent),
    ui2(new Ui::menu)
{
    ui2->setupUi(this);
    this->setWindowTitle("PD DetectorV2 Menu");

    gps_off=false;
    model = new QStringListModel(this);
    model1 = new QStringListModel(this);
    model2 = new QStringListModel(this);
    model4 = new QStringListModel(this);

    List << "Backlight Manager" << "Data Manager"<<"Update Application"<<"HELP" << "Exit";
    List1 << "10" << "40" << "60" <<"80"<<"100"<<"Exit";
    List2 << "Clean Data" << "Exit";
    List4 << "Exit"<<"Press SLEEP to turn display off"<<"longpress SLEEP to shutdown"<<"PAUSE for continuous data logging"<<"Repress to stop data-logging"<<"GAIN BOOST to boost Signal";
    // Populate our model
    model->setStringList(List);

    // Glue model and view together
    ui2->listView->setModel(model);

    index = model->index(row);
    index1 = model1->index(row1);
    index2 = model2->index(row2);
    //index3 = model3->index(row3);
    index4 = model4->index(row4);



    connect(this,SIGNAL(on_menu_exit_signal()),this,SLOT(on_exit_clicked()));
    pos=0;
    clean_ok_flag=0;
//ui2->label->setText("Press Menu to Select \nRemaning Memory(%):"+QString::fromUtf8(exec("df -kh | grep -v \"Filesystem\" | awk '{print $4}' | awk '{print $1}' | head -n 2 | tail -1 | awk '{print 100-$1}' ").c_str()));



 }

menu::~menu()
{
    delete ui2;
}



void menu::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_I)
    {

        if(pos==0)
        {
        row=ui2->listView->currentIndex().row();
        row=row-1;
        index=model->index(row);
        ui2->listView->setCurrentIndex(index);
        ui2->listView->hasFocus();
        }

        if(pos==1)
        {
        row1=ui2->listView->currentIndex().row();
        row1=row1-1;
        index1=model1->index(row1);
        ui2->listView->setCurrentIndex(index1);
        ui2->listView->hasFocus();
        }

        if(pos==2)
        {
        row2=ui2->listView->currentIndex().row();
        row2=row2-1;
        index2=model2->index(row2);
        ui2->listView->setCurrentIndex(index2);
        ui2->listView->hasFocus();
        }


        if(pos==4)
        {
        row4=ui2->listView->currentIndex().row();
        row4=row4-1;
        index4=model4->index(row4);
        ui2->listView->setCurrentIndex(index4);
        ui2->listView->hasFocus();
        }


    }

    if(e->key()==Qt::Key_D)
    {
        if(pos==0)
        {
            row=ui2->listView->currentIndex().row();
            row=row+1;
            index=model->index(row);
            ui2->listView->setCurrentIndex(index);
            ui2->listView->hasFocus();
        }

        if(pos==1)
        {
            row1=ui2->listView->currentIndex().row();
            row1=row1+1;
            index1=model1->index(row1);
            ui2->listView->setCurrentIndex(index1);
            ui2->listView->hasFocus();
        }

        if(pos==2)
        {
            row2=ui2->listView->currentIndex().row();
            row2=row2+1;
            index2=model2->index(row2);
            ui2->listView->setCurrentIndex(index2);
            ui2->listView->hasFocus();
        }



        if(pos==4)
        {
            row4=ui2->listView->currentIndex().row();
            row4=row4+1;
            index4=model4->index(row4);
            ui2->listView->setCurrentIndex(index4);
            ui2->listView->hasFocus();
        }

    }

if(e->key()==Qt::Key_M)//enter
{

    if((pos==0) && model->index(row).row()==0)
    {
        model1->setStringList(List1);
        ui2->listView->setModel(model1);

        pos=1;//BackLight Manager Mode
        goto event_end;
    }

    if((pos==0) &&model->index(row).row()==1)
    {


        model2->setStringList(List2);
        ui2->listView->setModel(model2);

        pos=2;//Data Manager Mode
        goto event_end;
    }

    if((pos==0) &&model->index(row).row()==3)
    {

        model4->setStringList(List4);
        ui2->listView->setModel(model4);

        pos=4;//Help Mode
        goto event_end;
    }


    if((pos==1) &&model1->index(row1).row()==0)
    {

        exec("echo 1 > /sys/class/backlight/backlight.34/brightness");
        row1=0;
        index1=model1->index(row1);
        ui2->listView->setCurrentIndex(index1);
        ui2->listView->hasFocus();
    }
    if((pos==1) &&model1->index(row1).row()==1)
    {

        exec("echo 3 > /sys/class/backlight/backlight.34/brightness");
        row1=1;
        index1=model1->index(row1);
        ui2->listView->setCurrentIndex(index1);
        ui2->listView->hasFocus();
    }
    if((pos==1) &&model1->index(row1).row()==2)
    {

        exec("echo 5 > /sys/class/backlight/backlight.34/brightness");
        row1=2;
        index1=model1->index(row1);
        ui2->listView->setCurrentIndex(index1);
        ui2->listView->hasFocus();
    }
    if((pos==1) && model1->index(row1).row()==3)
    {

        exec("echo 6 > /sys/class/backlight/backlight.34/brightness");
        row1=3;
        index1=model1->index(row1);
        ui2->listView->setCurrentIndex(index1);
        ui2->listView->hasFocus();
    }
    if((pos==1) && model1->index(row1).row()==4)
    {

        exec("echo 7 > /sys/class/backlight/backlight.34/brightness");
        row1=4;
        index1=model1->index(row1);
        ui2->listView->setCurrentIndex(index1);
        ui2->listView->hasFocus();
    }

    if((pos==1) && model1->index(row1).row()==5)
    {

        //qDebug("exiting backlight\n");
        emit on_menu_exit_signal();
        row1=0;
    }

    if((pos==2) && model2->index(row2).row()==0)
    {

        if(clean_ok_flag==0)
   {
        ui2->label->setText("Press menu to clean data\nWarning! This will delete data\npermanantly");
        clean_ok_flag=1;
        row2=0;
        index2=model2->index(row2);
        ui2->listView->setCurrentIndex(index2);
        ui2->listView->hasFocus();
        goto event_end;
    }
#ifdef BOARDGUI
        exec("sudo rm -r /home/root/data");
#endif
        ui2->label->setText("Data Cleaned\nSuccessfully");
        clean_ok_flag=0;
        row2=0;
        index2=model2->index(row2);
        ui2->listView->setCurrentIndex(index2);
        ui2->listView->hasFocus();
    }
    if((pos==2) && model2->index(row2).row()==1)
    {

        //qDebug("exiting data manager\n");
        row2=0;
        clean_ok_flag=0;
        emit on_menu_exit_signal();

    }



    if((pos==4) && model4->index(row4).row()==0)
    {

        //qDebug("exiting help\n");
        emit on_menu_exit_signal();
        row4=1;
    }



    if((pos==0) && model->index(row).row()==2)//Update Application
    {

        int Upgrade_Result=usb_upgrade();
        if(Upgrade_Result == FILE_UPGRADE_SUCCESS)
        {
            ui2->label->setText("Update successful\nSystem will shutdown now\n");
            ui2->label->repaint();
            QThread::msleep(5000);
            exec("poweroff");
        }
        else
        {
            switch(Upgrade_Result) {
                case FILE_NOT_PRESENT_ON_PENDRIVE:
                printf("FILE_NOT_PRESENT_ON_PENDRIVE ERROR \n");
                ui2->label->setText("Application missing on pendrive\nor pendrive not attached properly");
                break;

                case FILE_CANNOT_COPY_TO_TEMP :
                printf("FILE_CANNOT_COPY_TO_TEMP ERROR \n");
                ui2->label->setText("Cannot copy application\nfrom pendrive\nretry after reboot");
                break;

                default:break;

            }


        }

        row=3;
        pos=0;
        //on_exit_clicked();
    }
    if((pos==0) && model->index(row).row()==4)
    {

        //qDebug("exiting dialog\n");
        row=0;
        this->releaseKeyboard();
        emit on_menu_exit_signal();
        this->close();
        //emit on_exit_signal();
    }
    event_end:;
}


}



std::string menu::exec(std::string string_cmd)
{
    char* cmd = new char[string_cmd.size() + 1];
          memcpy(cmd, string_cmd.c_str(), string_cmd.size() + 1);
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

int menu::usb_upgrade()
{
#ifdef BOARDGUI

exec("mkdir -p /mnt/pendrive");
exec("sudo mount /dev/sda1 /mnt/pendrive/");
if ( access( "/mnt/pendrive/PDD1_ver2_test", F_OK ) != -1 )
{
    ui2->label->setText("File exists on pendrive\nUpdate started...");
    ui2->label->repaint();
    exec("mkdir -p /opt/temp");
    exec("cp /mnt/pendrive/PDD1_ver2_test /opt/temp/");
    if ( access( "/opt/temp/PDD1_ver2_test", F_OK ) != -1 )
        unlink("/home/root/deploy/PDD1_ver2_test");
    else
    {   exec("umount /mnt/pendrive"); \
        exec("umount /dev/sda1");

        return FILE_CANNOT_COPY_TO_TEMP;
    }
    exec("cp /opt/temp/PDD1_ver2_test /home/root/deploy/PDD1_ver2_test ");
    exec("chmod a+x /home/root/deploy/PDD1_ver2_test");
    exec("rm -r /opt/temp");

    exec("umount /mnt/pendrive"); \
    exec("umount /dev/sda1");
    return FILE_UPGRADE_SUCCESS;
}

else
{   exec("umount /mnt/pendrive"); \
    exec("umount /dev/sda1");

    return FILE_NOT_PRESENT_ON_PENDRIVE;
}
#endif
#ifdef PCGUI
return FILE_NOT_PRESENT_ON_PENDRIVE;
#endif
}


void menu::on_exit_clicked()
{
    //qDebug("exit submenu");
    model->setStringList(List);
    ui2->listView->setModel(model);
    pos=0;
    clean_ok_flag=0;
    ui2->label->setText("Press Menu to Select \nRemaning Memory(%):"+QString::fromUtf8(exec("df -kh | grep -v \"Filesystem\" | awk '{print $4}' | awk '{print $1}' | head -n 2 | tail -1 | awk '{print 100-$1}' ").c_str()));
}


