#ifndef GLOBAL_INDEXES
//#include <fftw3.h>
#define GLOBAL_INDEXES

#define MEGA_HERTZ                  1000000
#define GIGA_HERTZ                  1000000000
#define KILO_SAMPLES                1024
#define MEGA_SAMPLES                1024*1024


#define NO_OF_PLOTTING_VALS         4 * KILO_SAMPLES
#define FFT_VALS                    4 * KILO_SAMPLES //N
#define Fs                          3  * GIGA_HERTZ

#define FFT_VALS_DIVIDE_BY_FS       0.000001365
#define FS_DIVIDE_BY_FFT_VALS       732421.87

#define pixel_points                512
#define REAL                        0
#define IMAG                        1


#define EIM_BASE_ADDR               0x08000000
#define VIRT_MEM_ALLOC_SIZE         0x5000
#define STATUS_REG_OFFSET           0x90
#define CONTROL_REG_OFFSET          0x94
#define DATUM_REG_OFFSET            0x0C





extern unsigned int array[NO_OF_PLOTTING_VALS];
extern float *arr_float;
//extern unsigned int * arr_int;
/*extern fftwf_complex in[FFT_VALS];
extern fftwf_complex out[FFT_VALS];*/

#endif // GLOBAL_INDEXES

