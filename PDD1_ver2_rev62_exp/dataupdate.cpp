#include "dataupdate.h"
#include "all_headers.h"
#include "global_indexes.h"

using namespace std;

dataUpdate::dataUpdate(){

}

dataUpdate::~dataUpdate(){

}

void dataUpdate::run(){
    update_data();

}

inline void dataUpdate::update_data()
{

    //#ifdef DEBUG
    cout<<"update_data_started"<<endl;
    //#endif

    #ifdef BOARDGUI

    uint32_t mem_address;
        unsigned int mem_size;
        volatile unsigned int alloc_mem_size, page_mask, page_size;
        volatile uint32_t *virt_addr,*control_reg_addr,*status_reg_addr,*datum_reg_addr;
        volatile uint32_t *mem_pointer;
        struct timeval t0,t1;

        mem_address = EIM_BASE_ADDR;
        mem_size    = VIRT_MEM_ALLOC_SIZE;

        int mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
        if(mem_dev == -1){
                printf("ERROR!!! NO MEMORY DEVICE\n");
            }

        page_size = sysconf(_SC_PAGESIZE);
            alloc_mem_size = (((mem_size / page_size) + 1) * page_size);
            page_mask = (page_size - 1);

        mem_pointer = (uint32_t *) mmap(NULL,
                                            alloc_mem_size,
                                            PROT_READ | PROT_WRITE,
                                            MAP_SHARED,
                                            mem_dev,
                                            (mem_address & ~page_mask)
                                            );

        if(mem_pointer == MAP_FAILED){
                printf("Error!!! No pointer obtained");
            }

        virt_addr = (mem_pointer + (mem_address & page_mask)); //virtual address of EIM base address
        #ifdef DEBUG
        printf("Obtained Virtual address: 0x%x\n",virt_addr);
        #endif

        status_reg_addr = virt_addr + STATUS_REG_OFFSET;       //virtual status register address
        control_reg_addr = virt_addr + CONTROL_REG_OFFSET;     //virtual control register address
        //datum_reg_addr =virt_addr + DATUM_REG_OFFSET;

       // DATUM=*(datum_reg_addr);

        #ifdef DEBUG
        printf("Status reg address: 0x%x\n",status_reg_addr);
        printf("Control reg address: 0x%x\n",control_reg_addr);
        #endif

        printf("Powering up ADC\n");
        *control_reg_addr = 0x00000040; //ADC Power up
        printf("ADC Powered up...\n");

        QThread::msleep(60);//delay-110


        *control_reg_addr = 0x00000004;//reset on
        QThread::msleep(25);//50
        *control_reg_addr = 0X00000000;//reset off

        QThread::msleep(25);//50

        *control_reg_addr = 0x00000001 ; //write control register bit to start pipeline
        gettimeofday(&t0,0);
        QThread::msleep(25);//50

        while((*status_reg_addr) & 0x00000001 != 1); //Wait till bit 0 of status reg is 0
        printf("New Status reg:%x\n",*status_reg_addr);

        QThread::msleep(20);

        *control_reg_addr = 0x00000002; //To indicate uP started reading



        while((*status_reg_addr & 0x00000004) != 0x00000004);
        #ifdef DEBUG
        printf("FFT buffer full\n");
        #endif
        while((*status_reg_addr & 0x00000002) != 0x00000002);
        #ifdef DEBUG
        printf("RAW buffer full\n");
        #endif
        gettimeofday(&t1,0);
        printf("Time for data full: %lf\n",(double)((t1.tv_sec - t0.tv_sec) + (double)(t1.tv_usec - t0.tv_usec)/1000000));
        int i=0;

        printf("Turning Off ADC\n");
        *control_reg_addr = 0x00000080; //ADC Power Down
        printf("ADC Turned Off...\n");

        //Data transfers starts here
        #ifdef DEBUG
        printf("**********entering screening....\n" );
        #endif
        if(mode==SCREENING || mode==LOC_MON) {

            while((*status_reg_addr & 0x00000002) == 0x00000002){ /*8Mdata*/
                    copy_raw(virt_addr ,array+ i*32);
                i++;
                if(i==262144)
                break;
            }
        }

        if(mode==SPECTRUM) {
            while((*status_reg_addr & 0x00000004) == 0x00000004){
                    copy_fft(virt_addr + 0x20 ,(unsigned int *)arr_float+ i*16);
                i++;
                if(i==256)
                break;
            }
        }
        //Data transfer ends here



 #endif

#if 0

        for(int i=0;i<NO_OF_FFT_VALS;i++)
        {
            if(i <= 100)
            printf("%x->%f \n",arr_float[i],arr_float[i]);
        }

#endif






/*--------------------------------------------------------------*/

#ifdef PCGUI

         if(mode==SCREENING || mode==LOC_MON){
             for(int i=0;i<NO_OF_RAW_VALS;i++)
             array[i]= rand() % ( 32970 + 1 - 32740 ) + 32740 ;

         }

         else if(mode==SPECTRUM) {
             for(int i=0;i<=NO_OF_FFT_VALS;i++)
                 arr_float[i]=134+rand()%561;

         }

#endif



    if(mode==LOC_MON )
    {
        for(int i=0;i<NO_OF_RAW_VALS;i++){

            arr_float_mV[i]=sample_to_millivolt((unsigned int)array[i]);
            #ifdef DEBUG
            if(i<10)
            printf("arr_float_mV[i]=%f\n",arr_float_mV[i]);
            #endif
        }


    }

#if 1 /*find datum*/

    if( mode==LOC_MON || mode==SCREENING )
    {
        unsigned long long int total=0;
        for(int i=0 ; i< (8*MEGA_SAMPLES) ; i++){
            //qDebug("array[i]=%d\n",array[i]);
            /*qDebug("total=%lld\n",*/total+=array[i]/*)*/;
        }
        qDebug("#######Found DATUM of screening =%d\n",FPGA_DATUM=(total/8388608));
    }

#endif


/*-------------------------------------------------------------*/

#if 0
    ifstream infile;

    //memset(array,174,sizeof(array));
        int z=0;

        infile.open ("00_14_36.csv", ifstream::in);
                        if (infile.is_open())
                        {
                                while (infile.good())
                                {
                                        infile.getline(cNum, 256, ' ,');
                                        array[z] = atof(cNum);
                                        //printf("%f ",array[z]);
                                        z++ ;



                                        if(z==(NO_OF_PLOTTING_VALS))
                                            break;


                                }


                                infile.close();
                                //cout<<"finished & z is"<<z;
                        }
                        else
                        {
                                cout << "Error opening file";
                        }
                        cout<<"finished & z is"<<z;


#endif


}

inline void dataUpdate::copy_fft(uint32_t *src_addr,unsigned int *dest_addr){

    asm("pld [%[src_address]]\n\t"
        "vldm %[src_address],{d0-d7}\n\t"
        "vstm %[dest_address],{d0-d7}\n\t"
        ://outputs
            :[src_address]"r"(src_addr),[dest_address]"r"(dest_addr)//inputs
        :"memory"//clobbers
    );
//D0-D7 double word 64bit*7=64 bytes
}

inline void dataUpdate::copy_raw(uint32_t *src_addr,unsigned short int *dest_addr){

    asm("pld [%[src_address]]\n\t"
        "vldm %[src_address],{d0-d7}\n\t"
        "vstm %[dest_address],{d0-d7}\n\t"
        ://outputs
            :[src_address]"r"(src_addr),[dest_address]"r"(dest_addr)//inputs
        :"memory"//clobbers
    );
//D0-D7 double word 64bit*7=64 bytes
}


inline float dataUpdate::sample_to_millivolt(unsigned int sample) {
    //printf("Got sample=%d\n,FPGA_DATUM = %d",sample,FPGA_DATUM);
    signed int temp=sample-32747;
    if(temp<0)
        temp=temp*-1;

return (float) (ADC_RESOLUTION * temp);

/*ADC resolution= Vp-p / pow(2,ENOB) */
/* here, Vp-p=2.5 ENOB=12 , ADC Resolution=0.610mV*/

}
