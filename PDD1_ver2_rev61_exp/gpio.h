#ifndef GPIO_H
#define GPIO_H

#include <QWidget>
//#include <types.h>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <QLabel>
#include <QString>
#include "stdbool.h"
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

                    /*GPIO definitions*/
/****************************************************************************/
#define GPIO_GDIR_INPUT             0   //!< GPIO pin is input
#define GPIO_GDIR_OUTPUT            1   //!< GPIO pin is output

#define GPIO_LOW_LEVEL              0   //!< GPIO pin is low
#define GPIO_HIGH_LEVEL             1   //!< GPIO pin is high

#define GPIO_ICR_LOW_LEVEL          0   //!< Interrupt is low-level
#define GPIO_ICR_HIGH_LEVEL         1   //!< Interrupt is high-level
#define GPIO_ICR_RISE_EDGE          2   //!< Interrupt is rising edge
#define GPIO_ICR_FALL_EDGE          3   //!< Interrupt is falling edge

#define GPIO_IMR_MASKED             0   //!< Interrupt is masked
#define GPIO_IMR_UNMASKED           1   //!< Interrupt is unmasked

#define GPIO_ISR_NOT_ASSERTED       0   //!< Interrupt is not asserted
#define GPIO_ISR_ASSERTED           1   //!< Interrupt is asserted

#define GPIO_EDGE_SEL_DISABLE       0   //!< Edge select is disabled
#define GPIO_EDGE_SEL_ENABLE        1   //!< Edge select is enabled

#define REGS_GPIO1_BASE (0x0209c000) //!< Base address for GPIO instance number 1.
#define REGS_GPIO2_BASE (0x020a0000) //!< Base address for GPIO instance number 2.
#define REGS_GPIO3_BASE (0x020a4000) //!< Base address for GPIO instance number 3.
#define REGS_GPIO4_BASE (0x020a8000) //!< Base address for GPIO instance number 4.
#define REGS_GPIO5_BASE (0x020ac000) //!< Base address for GPIO instance number 5.
#define REGS_GPIO6_BASE (0x020b0000) //!< Base address for GPIO instance number 6.
#define REGS_GPIO7_BASE (0x020b4000) //!< Base address for GPIO instance number 7.

#define u32 unsigned int

/************************************************************************************/

class gpio
{
public:
    explicit gpio();
   void gpio_set_direction(u32 port, u32 pin, u32 dir);
   void gpio_set_direction_standalone(u32 port, u32 pin, u32 dir);
   void gpio_set_level(u32 port, u32 pin, u32 level);
    void gpio_get_level(u32 port, u32 pin);
    u32  gpio_data_val;
    int port_copy,pin_copy;


private:
   volatile unsigned int alloc_mem_size, page_mask, page_size;
   int mem_dev;
   unsigned int mem_size;
   u32	 *virt_addr;							//virtual address pointer
   u32  *gpio_base_reg;
   u32  *gpio_dir_reg;
   u32  *gpio_data_reg;
   u32	 *mem_pointer;							 //virtual space mem_pointer



   void calculate_virtual_address(u32 physical_add);
   u32 gdir_read();
   void gdir_write(u32 val);
   u32 gdata_read();
   void gdata_write(u32 dat_value);

};
#endif // GPIO_H
