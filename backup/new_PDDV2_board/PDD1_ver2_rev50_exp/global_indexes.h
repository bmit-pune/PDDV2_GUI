#ifndef GLOBAL_INDEXES
#define GLOBAL_INDEXES

#define MEGA_HERTZ                  1000000
#define GIGA_HERTZ                  1000000000
#define KILO_SAMPLES                1024
#define MEGA_SAMPLES                1024*1024
#define MILLIVOLTS                  1000

#define NO_OF_RAW_VALS              8 * MEGA_SAMPLES//Screening
#define NO_OF_FFT_VALS              4 * KILO_SAMPLES //Spectrum
#define pixel_points                512

#define MAX_LOC_MON_LOCATIONS       10

//ADC
#define ADC_REF_VOLTAGE             1.26 * MILLIVOLTS //mV 2.5 in PDDv2
#define ADC_BIT                     8                 //16 in PPDv2

//#define FPGA_DATUM                  32754
#define ADC_RESOLUTION              0.040  /*mV*/

#define EIM_BASE_ADDR               0x08000000
#define VIRT_MEM_ALLOC_SIZE         0x5000
#define STATUS_REG_OFFSET           0x90
#define CONTROL_REG_OFFSET          0x94
#define DATUM_REG_OFFSET            0x0C

//Debug directives
//#define DEBUG                       1
#define PCGUI                       1
//#define BOARDGUI                    1

#define PATTERN1                     1
//#define PATTERN2                     1
//#define PATTERN3                     1
//#define PATTERN4                      1
//#define PATTERN5                        1


extern unsigned short int array[NO_OF_RAW_VALS];
extern float arr_float[NO_OF_FFT_VALS];
extern float arr_float_noise[NO_OF_FFT_VALS];
extern float arr_float_mV[NO_OF_RAW_VALS];

#endif // GLOBAL_INDEXES

