#include"all_headers.h"
#include<QFile>
#include"read_batt.h"


read_batt::read_batt()
{

}

read_batt::~read_batt()
{

}


void read_batt::read_batt_open()
{
    //open i2c
    if((fd_i2c = open(device_name, O_RDWR)) < 0) {
            //ERROR HANDLING: you can check errno to see what went wrong
            perror("Failed to open the i2c bus");
            exit(1);
        }


}

void read_batt::read_batt_close()
{
    //close i2c
    if(::close(fd_i2c)!=0)//close file descriptor
    {
        printf("Problem closing i2c File \n");

    }


}

void read_batt::read_battery_soc(){



    write_buf[0]=0x1c;
    write_buf[1]=0x1d;
    reg_address=0x1d;
    slave_addr=0x55;


        sprintf(device_name, "/dev/i2c-%d",2);

        read_batt_open();
        //tie file descriptor to slave addr
        if(ioctl(fd_i2c, I2C_SLAVE_FORCE, slave_addr) < 0){
                printf("Oh dear, something went wrong with initialisation()! %s\n", strerror(errno));
                exit(2);
            }
//write 1c

        if(write(fd_i2c,write_buf,1) != 1){
            /* ERROR HANDLING: i2c transaction failed */
            printf("Oh dear, something went wrong with write()! %s\n", strerror(errno));
            printf("\n\n");
        }


//read soc
        if (read(fd_i2c,read_buf,1) != 1) {
                  /* ERROR HANDLING: i2c transaction failed */
                  printf("Failed to read from the i2c bus.\n");
                printf("\n\n");
              }

read_data=read_buf[0];

read_batt_close();
//pass data to battery progress bar

   emit batt1Signal(read_data);

qDebug("state of battery is:%d",read_data);

}

