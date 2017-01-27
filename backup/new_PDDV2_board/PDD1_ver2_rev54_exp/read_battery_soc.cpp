
#include<QFile>
#include"read_batt.h"
#include"all_headers.h"
read_batt::read_batt()
{

}

read_batt::~read_batt()
{

}


void read_batt::read_batt_open()
{
#ifdef BOARDGUI
    //open i2c
    if((fd_i2c = open(device_name, O_RDWR)) < 0) {
            //ERROR HANDLING: you can check errno to see what went wrong
            perror("Failed to open the i2c bus");
            exit(1);
        }
#endif


}

void read_batt::read_batt_close()
{
#ifdef BOARDGUI
    //close i2c
    if(::close(fd_i2c)!=0)//close file descriptor
    {
        printf("Problem closing i2c File \n");

    }
#endif

}

int read_batt::read_battery_soc(){

    #ifdef BOARDGUI


    write_buf[0]=0x1c;
    write_buf[1]=0x1d;
    reg_address=0x1d;
    slave_addr=0x55;


        sprintf(device_name, "/dev/i2c-%d",0);

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

if((read_data < 0) || (read_data >100))
{batt_calibrate();return CALIBRATION_IN_PROCESS;}

//pass data to battery progress bar
   emit batt1Signal(read_data);
    return EXIT_SUCCESS;
#endif

}

void read_batt::batt_calibrate()
{

#ifdef BOARDGUI
/*--------------For i2c0---------------------------------------*/
/*(UNSEAL)*/
exec("i2cset -y 0 0x55 0x00 0x14 b ");
exec("i2cset -y 0 0x55 0x01 0x04 b ");
exec("i2cset -y 0 0x55 0x00 0x72 b ");
exec("i2cset -y 0 0x55 0x01 0x36 b ");
/*(GET FLASH ACCESS)*/
exec("i2cset -y 0 0x55 0x61 0x00 b ");
exec("i2cset -y 0 0x55 0x3e 0x30 b ");
exec("i2cset -y 0 0x55 0x3f 0x00 b ");
/*(WRITE DESIGN CAPACITY & DESIGN ENERGY REF:sula596)*/
exec("i2cset -y 0 0x55 0x53 0x1d b ");
exec("i2cset -y 0 0x55 0x54 0x4c b ");
exec("i2cset -y 0 0x55 0x55 0x60 b ");
exec("i2cset -y 0 0x55 0x56 0x66 b ");
/*(WRITE CHECKSUM)*/
exec("i2cset -y 0 0x55 0x60 0x1e b ");
/*DUMP TO SEE IF ITS UPDATED*/
/*IF NO:*/
/*(AGAIN GET FLASH ACCESS)*/
exec("i2cset -y 0 0x55 0x61 0x00 b ");
exec("i2cset -y 0 0x55 0x3e 0x30 b ");
exec("i2cset -y 0 0x55 0x3f 0x00 b ");
/*(WRITE CHECKSUM)*/
exec("i2cset -y 0 0x55 0x60 0x1e b ");
/*(RESET)*/
exec("i2cset -y 0 0x55 0x00 0x41 b ");
exec("i2cset -y 0 0x55 0x01 0x00 b ");
/*IT SHOULD SHOW ALL 0X00 IN 0X50 ADDRESS LINE*/
/*IF NOT::GET FLASH ACCESS,AGAIN CHECK FOR ALL ZEROS*/
/*IF STILL NOT::AGAIN RESET*/
/*IF OK GET FLASH ACCESS TO SEE VALS INSTEAD OF ZEROS*/
/*(GET FLASH ACCESS)*/
exec("i2cset -y 0 0x55 0x61 0x00 b ");
exec("i2cset -y 0 0x55 0x3e 0x30 b ");
exec("i2cset -y 0 0x55 0x3f 0x00 b ");
/*(BATTERY INSERT TO START BATTERY GAUGING)*/
exec("i2cset -y 0 0x55 0x00 0x0c b ");
exec("i2cset -y 0 0x55 0x01 0x00 b ");
/*CHECK 0X1C VAL IF SOC IS APPEARING,*/
/*IF YES DONE NOW:*/
/*(SEAL CONFIGURATION)*/
exec("i2cset -y 0 0x55 0x00 0x20 b ");
exec("i2cset -y 0 0x55 0x01 0x00 b ");
/*--------------For i2c1---------------------------------------*/
/*(UNSEAL)*/
exec("i2cset -y 1 0x55 0x00 0x14 b ");
exec("i2cset -y 1 0x55 0x01 0x04 b ");
exec("i2cset -y 1 0x55 0x00 0x72 b ");
exec("i2cset -y 1 0x55 0x01 0x36 b ");
/*(GET FLASH ACCESS)*/
exec("i2cset -y 1 0x55 0x61 0x00 b ");
exec("i2cset -y 1 0x55 0x3e 0x30 b ");
exec("i2cset -y 1 0x55 0x3f 0x00 b ");
/*(WRITE DESIGN CAPACITY & DESIGN ENERGY REF:sula596)*/
exec("i2cset -y 1 0x55 0x53 0x1d b ");
exec("i2cset -y 1 0x55 0x54 0x4c b ");
exec("i2cset -y 1 0x55 0x55 0x60 b ");
exec("i2cset -y 1 0x55 0x56 0x66 b ");
/*(WRITE CHECKSUM)*/
exec("i2cset -y 1 0x55 0x60 0x1e b ");
/*DUMP TO SEE IF ITS UPDATED*/
/*IF NO:*/
/*(AGAIN GET FLASH ACCESS)*/
exec("i2cset -y 1 0x55 0x61 0x00 b ");
exec("i2cset -y 1 0x55 0x3e 0x30 b ");
exec("i2cset -y 1 0x55 0x3f 0x00 b ");
/*(WRITE CHECKSUM)*/
exec("i2cset -y 1 0x55 0x60 0x1e b ");
/*(RESET)*/
exec("i2cset -y 1 0x55 0x00 0x41 b ");
exec("i2cset -y 1 0x55 0x01 0x00 b ");
/*IT SHOULD SHOW ALL 0X00 IN 0X50 ADDRESS LINE*/
/*IF NOT::GET FLASH ACCESS,AGAIN CHECK FOR ALL ZEROS*/
/*IF STILL NOT::AGAIN RESET*/
/*IF OK GET FLASH ACCESS TO SEE VALS INSTEAD OF ZEROS*/
/*(GET FLASH ACCESS)*/
exec("i2cset -y 1 0x55 0x61 0x00 b ");
exec("i2cset -y 1 0x55 0x3e 0x30 b ");
exec("i2cset -y 1 0x55 0x3f 0x00 b ");
/*(BATTERY INSERT TO START BATTERY GAUGING)*/
exec("i2cset -y 1 0x55 0x00 0x0c b ");
exec("i2cset -y 1 0x55 0x01 0x00 b ");
/*CHECK 0X1C VAL IF SOC IS APPEARING,*/
/*IF YES DONE NOW:*/
/*(SEAL CONFIGURATION)*/
exec("i2cset -y 1 0x55 0x00 0x20 b ");
exec("i2cset -y 1 0x55 0x01 0x00 b ");

printf("******calibrating battery done***\n");
#endif
}

