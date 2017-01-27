#include "gpio.h"


gpio::gpio()
{
    virt_addr = 0;
    gpio_base_reg = 0;
    gpio_dir_reg = 0;
    gpio_data_reg = 0;
    mem_pointer = 0;
    gpio_data_val = GPIO_HIGH_LEVEL;

}

void gpio::gpio_set_direction(u32 port, u32 pin, u32 dir)
{
    u32 oldVal = 0, newVal = 0;

    switch (port) {
    case 1:
        calculate_virtual_address(REGS_GPIO1_BASE);
        break;
    case 2:
        calculate_virtual_address(REGS_GPIO2_BASE);
        break;
    case 3:
        calculate_virtual_address(REGS_GPIO3_BASE);
        break;
    case 4:
        calculate_virtual_address(REGS_GPIO4_BASE);
        break;
    case 5:
        calculate_virtual_address(REGS_GPIO5_BASE);
        break;
    case 6:
        calculate_virtual_address(REGS_GPIO6_BASE);
        break;
    case 7:
        calculate_virtual_address(REGS_GPIO7_BASE);
        break;
    default:
        break;
    }

    gpio_base_reg = virt_addr;
    gpio_dir_reg = gpio_base_reg + 0x01;
    gpio_data_reg = gpio_base_reg;

    oldVal = gdir_read();  //read the gpio direction register for the particular port

    if (dir == GPIO_GDIR_INPUT)
        newVal = oldVal & (~(1 << pin));
    else
        newVal = oldVal | (1 << pin);


    gdir_write(newVal);

    //printf("The direction register value is %x\n",*(gpio_dir_reg));

}
void gpio::gpio_set_direction_standalone(u32 port, u32 pin, u32 dir)
{
    u32 oldVal = 0, newVal = 0;

    switch (port) {
    case 1:
        calculate_virtual_address(REGS_GPIO1_BASE);
        break;
    case 2:
        calculate_virtual_address(REGS_GPIO2_BASE);
        break;
    case 3:
        calculate_virtual_address(REGS_GPIO3_BASE);
        break;
    case 4:
        calculate_virtual_address(REGS_GPIO4_BASE);
        break;
    case 5:
        calculate_virtual_address(REGS_GPIO5_BASE);
        break;
    case 6:
        calculate_virtual_address(REGS_GPIO6_BASE);
        break;
    case 7:
        calculate_virtual_address(REGS_GPIO7_BASE);
        break;
    default:
        break;
    }

    gpio_base_reg = virt_addr;
    gpio_dir_reg = gpio_base_reg + 0x01;
    gpio_data_reg = gpio_base_reg;

    oldVal = gdir_read();  //read the gpio direction register for the particular port

    if (dir == GPIO_GDIR_INPUT)
        newVal = oldVal & (~(1 << pin));
    else
        newVal = oldVal | (1 << pin);


    gdir_write(newVal);
   if(munmap( mem_pointer,alloc_mem_size)==(-1))
       printf("unmap failure \n");

    //printf("The direction register value is %x\n",*(gpio_dir_reg));

}

void gpio::gpio_set_level(u32 port, u32 pin, u32 level)
{


    gpio_set_direction(port, pin, 1); /*We dont want to set gpio to output each time*/

    u32 mask=0;
    mask = 1 << pin;

    //printf("The masked for port:%d pin:%d level:%d value is %x\n",port,pin,level,mask);
    u32 value =0;

    u32 dir =0;
    u32 dir_val=0;
    dir_val = gdir_read();


    value = gdata_read();   // read current value


    if (level == GPIO_LOW_LEVEL)            // fix it up
        value &= ~mask;
    else if ( level == GPIO_HIGH_LEVEL)
        value |= mask;

    gdata_write(value);            // write new value
    if(munmap( mem_pointer,alloc_mem_size)==(-1))
        printf("unmap failure \n");

}

void gpio::gpio_get_level(u32 port, u32 pin)
{


    gpio_set_direction(port, pin, 0);

    u32 mask=0;
    mask = 1 << pin;

    //printf("The masked for port:%d pin:%d level:%d value is %x\n",port,pin,level,mask);
    u32 value =0;

    u32 dir =0;
    u32 dir_val=0;
    dir_val = gdir_read();

  // printf("The direction register value is %x\n",dir);
    dir = (dir_val & mask )? GPIO_GDIR_OUTPUT : GPIO_GDIR_INPUT;

    if (dir != GPIO_GDIR_INPUT)
    {
       printf("GPIO_PORT%d_%d is not configured to be input!\n", port, pin);

    }

    value = gdata_read();   // read current value

    gpio_data_val = (value & mask);
    if(munmap( mem_pointer,alloc_mem_size)==(-1))
        printf("unmap failure \n");

   // printf("the data old value is %x\n",value);

}


void gpio::calculate_virtual_address(u32 physical_add)
{
    unsigned int mem_address = physical_add;		//start physical address to access
    mem_size = 0x1000;
    int cnt = 0;
    int k=0;

    //mem  is  a character device file that is an image of the main memory of the computer.
    //Byte	addresses  in  mem	are interpreted as physical memory addresses.
    //int mem_dev=openmem();
    mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
    //open the mem device file
    if(mem_dev == -1)
    {
        printf("ERROR!!! NO MEMORY DEVICE\n");
    }

    //sysconf gives configuration info at run time
    //here we obtain the value of page size.
    page_size = sysconf(_SC_PAGESIZE);

    alloc_mem_size = (((mem_size / page_size) + 1) * page_size);

    page_mask = (page_size - 1);


    mem_pointer = (unsigned int *) mmap(NULL,
                                        alloc_mem_size,
                                        PROT_READ | PROT_WRITE,
                                        MAP_SHARED,
                                        mem_dev,
                                        (mem_address & ~page_mask)
                                        );

    if(mem_pointer == MAP_FAILED)	//MAP_FAILED is flag for mmap
    {
        printf("Error!!! No pointer obtained");
    }


    virt_addr = (mem_pointer + (mem_address & page_mask));

if(close(mem_dev)!=(0))
     printf( "gpio close failure \n");


}

u32 gpio::gdir_read()
{
    u32 reg_data;

        reg_data = *(gpio_dir_reg);

        return reg_data;

}

void gpio::gdir_write(u32 reg_value)
{
    *(gpio_dir_reg) = reg_value;
}

u32 gpio::gdata_read()
{
    u32 reg_data;

        reg_data = *(gpio_data_reg);

        return reg_data;
}

void gpio::gdata_write(u32 dat_value)
{
    *(gpio_data_reg) = dat_value;
}
