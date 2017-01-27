#ifndef READ_BATT_H
#define READ_BATT_H

#include <QMainWindow>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include </usr/include/linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QObject>
#include <QtCore>
#include<QProgressBar>


class read_batt: public QObject
{
    Q_OBJECT
public:
    read_batt();
    ~read_batt();

public:
    int  read_battery_soc();
    void read_batt_open();
    void read_batt_close();
    void batt_calibrate();
    std::string exec(std::string string_cmd);

public :
signals:
    void batt1Signal(int);

public:
    int fd_i2c;
        int slave_addr;
        int read_data;
        int reg_address;
        char device_name[20];
        char read_buf[20];
        char write_buf[8];


};

#endif // READ_BATT_H
