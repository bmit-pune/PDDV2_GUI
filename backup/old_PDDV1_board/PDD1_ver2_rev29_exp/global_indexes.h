#ifndef GLOBAL_INDEXES
#define GLOBAL_INDEXES

#define MEGA_HERTZ                  1000000
#define GIGA_HERTZ                  1000000000
#define KILO_SAMPLES                1024
#define MEGA_SAMPLES                1024*1024

#define NO_OF_PLOTTING_VALS         4 * KILO_SAMPLES
#define pixel_points                512

#define EIM_BASE_ADDR               0x08000000
#define VIRT_MEM_ALLOC_SIZE         0x5000
#define STATUS_REG_OFFSET           0x90
#define CONTROL_REG_OFFSET          0x94
#define DATUM_REG_OFFSET            0x0C

//Debug directives
#define DEBUG                       1
//#define PCGUI                       1
#define BOARDGUI                    1


extern unsigned int array[NO_OF_PLOTTING_VALS];
extern float *arr_float;

#endif // GLOBAL_INDEXES

