#ifndef GLOBAL_INDEXES
#define GLOBAL_INDEXES
//http://format.krzaq.cc/
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


#define SCR_MNUMBER             2000   /*Magic number*/
#define SCR_PORT                2      /*   Port     */
#define SCR_PIN                 1      /*   Pin      */

#define S_P_MNUMBER             2001
#define S_P_PORT                1
#define S_P_PIN                 2

#define SCR_CAP_MNUMBER         2002
#define SCR_CAP_PORT            1
#define SCR_CAP_PIN             28

#define LOC_MNUMBER             2003
#define LOC_PORT                2
#define LOC_PIN                 2

#define NOISE_MNUMBER           2004
#define NOISE_PORT              1
#define NOISE_PIN               4

#define DATA_STR_MNUMBER        2005
#define DATA_STR_PORT           1
#define DATA_STR_PIN            22

#define OWTS_MNUMBER            2006
#define OWTS_PORT               2
#define OWTS_PIN                3

#define GPS_MNUMBER             2007
#define GPS_PORT                1
#define GPS_PIN                 31

#define DATA_RETV_MNUMBER       2008
#define DATA_RETV_PORT          1
#define DATA_RETV_PIN           23

#define SLEEP_MNUMBER           2009
#define SLEEP_PORT              1
#define SLEEP_PIN               6

#define GPS_GPIO_MNUMBER             2010
#define GPS_GPIO_PORT                1
#define GPS_GPIO_PIN                 14


#define LEDON                   1
#define LEDOFF                  0
#define NONE                    4000

#define CALIBRATION_IN_PROCESS      1001





//Debug directives
//#define DEBUG                       1
#define PCGUI                       1
//#define BOARDGUI                    1






extern unsigned short int array[NO_OF_RAW_VALS];
extern float arr_float[NO_OF_FFT_VALS];
extern float arr_float_noise[NO_OF_FFT_VALS];
extern float arr_float_mV[NO_OF_RAW_VALS];
//extern int LOC_MON_first_transition;

#endif // GLOBAL_INDEXES

