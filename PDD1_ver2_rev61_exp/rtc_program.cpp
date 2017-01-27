#include"all_headers.h"
#include<iostream>
#include<QString>

using namespace std;


void MainWindow::rtc_program()
{

    printf("\n\nrtc_program() started\n\n ");
    string temp_date="";
    string temp_time="";
    temp_date="20"+gps_date.substr(4,2)+"-"+gps_date.substr(2,2)+"-"+gps_date.substr(0,2);//format date
    temp_time=gps_time.substr(0,2)+":"+gps_time.substr(2,2)+":"+gps_time.substr(4,2);//format time
    exec("sudo date -s '"+temp_date+" "+temp_time+"'");//update system date & time according to UTC
    exec("date -s '+8 hour' \"+%d.%m.%Y %H:%M:%S\"");//add 8 hours to get malay time
    exec("sudo hwclock --systohc");//sync hardware clock
    exec("sudo hwclock -w -f /dev/rtc0");//entry of hw clock
    rtc_flag++;
    user_hint("Clock set to  Malaysia Time from GPS...");

}

