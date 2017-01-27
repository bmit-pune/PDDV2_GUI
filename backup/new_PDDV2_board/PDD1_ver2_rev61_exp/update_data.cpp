#include<global_indexes.h>
#include<all_headers.h>

using namespace std;
void MainWindow::update_data()
{
DATAUpdate->start(QThread::LowPriority);
QThread::msleep(200);//wait 200mSecs to get all data
}

inline void MainWindow::copy_fft(uint32_t *src_addr,unsigned int *dest_addr){

    asm("pld [%[src_address]]\n\t"
        "vldm %[src_address],{d0-d7}\n\t"
        "vstm %[dest_address],{d0-d7}\n\t"
        ://outputs
            :[src_address]"r"(src_addr),[dest_address]"r"(dest_addr)//inputs
        :"memory"//clobbers
    );
//D0-D7 double word 64bit*7=64 bytes
}

inline void MainWindow::copy_raw(uint32_t *src_addr,unsigned short int *dest_addr){

    asm("pld [%[src_address]]\n\t"
        "vldm %[src_address],{d0-d7}\n\t"
        "vstm %[dest_address],{d0-d7}\n\t"
        ://outputs
            :[src_address]"r"(src_addr),[dest_address]"r"(dest_addr)//inputs
        :"memory"//clobbers
    );
//D0-D7 double word 64bit*7=64 bytes
}


inline float MainWindow::sample_to_millivolt(unsigned int sample) {
    signed int temp=sample-32747;
    if(temp<0)
        temp=temp*-1;   

return (float) (ADC_RESOLUTION * temp);
/*ADC resolution= Vp-p / pow(2,ENOB) */
/* here, Vp-p=2.5 ENOB=12 , ADC Resolution=0.610mV*/

}

