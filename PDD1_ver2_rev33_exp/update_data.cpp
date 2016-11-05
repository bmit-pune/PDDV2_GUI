#include<global_indexes.h>
#include<all_headers.h>

using namespace std;
void MainWindow::update_data()
{
    #ifdef DEBUG
    cout<<"update_data_started"<<endl;
    #endif

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
        datum_reg_addr =virt_addr + DATUM_REG_OFFSET;

        DATUM=*(datum_reg_addr);

        printf("Status reg address: 0x%x\n",status_reg_addr);
        printf("Control reg address: 0x%x\n",control_reg_addr);


        *control_reg_addr = 0x00000004;//reset on
        QThread::msleep(50);
        *control_reg_addr = 0X00000000;//reset off

        QThread::msleep(50);

        *control_reg_addr = 0x00000001 ; //write control register bit to start pipeline
        gettimeofday(&t0,0);
        QThread::msleep(50);

        while((*status_reg_addr) & 0x00000001 != 1); //Wait till bit 0 of status reg is 0
        printf("New Status reg:%x\n",*status_reg_addr);

        QThread::msleep(20);

        *control_reg_addr = 0x00000002; //To indicate uP started reading



        while((*status_reg_addr & 0x00000004) != 0x00000004);
        printf("FFT buffer full\n");
        while((*status_reg_addr & 0x00000002) != 0x00000002);
        printf("RAW buffer full\n");
        gettimeofday(&t1,0);
        printf("Time for data full: %lf\n",(double)((t1.tv_sec - t0.tv_sec) + (double)(t1.tv_usec - t0.tv_usec)/1000000));
        int i=0;

        //Data transfers starts here
        printf("**********entering screening....\n" );
        if(mode==SCREENING) {

            while((*status_reg_addr & 0x00000002) == 0x00000002){
                    copy_raw(virt_addr ,array+ i*64);
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

        printf("Total samples received %d\n",i*16);



        if(mode==SPECTRUM) {
        #ifdef DEBUG
        for( i=0;i<NO_OF_PLOTTING_VALS/2;i++)
        {
            printf("<->%d::%f<->",i,arr_float[i]);
        }
        cout<<"update_data_end"<<endl;
        #endif
        }
 #endif






/*--------------------------------------------------------------*/

#ifdef PCGUI

    for(int i=0;i<NO_OF_PLOTTING_VALS;i++)
         {
            if(mode==SCREENING)
                array[i]=rand()%1500;
            if(mode==SPECTRUM)
                arr_float[i]=134+rand()%561;
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

inline void MainWindow::copy_raw(uint32_t *src_addr,char *dest_addr){

    asm("pld [%[src_address]]\n\t"
        "vldm %[src_address],{d0-d7}\n\t"
        "vstm %[dest_address],{d0-d7}\n\t"
        ://outputs
            :[src_address]"r"(src_addr),[dest_address]"r"(dest_addr)//inputs
        :"memory"//clobbers
    );
//D0-D7 double word 64bit*7=64 bytes
}

