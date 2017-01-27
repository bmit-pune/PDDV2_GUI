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


        *control_reg_addr |= 1<<2;//reset on
        QThread::msleep(50);
        *control_reg_addr &= ~(1<<2);//reset off


        *control_reg_addr = 0x01 ; //write control register bit to start pipeline
        gettimeofday(&t0,0);

        while((*status_reg_addr) & 0x0001 != 1); //Wait till bit 0 of status reg is 0

        QThread::msleep(10);

        *control_reg_addr &= 0x0002; //To indicate uP started reading



        while((*status_reg_addr & 0x4) != 0x4 && (*status_reg_addr & 0x2) != 0x2 );
        gettimeofday(&t1,0);
        printf("Time for data full: %lf\n",(double)((t1.tv_sec - t0.tv_sec) + (double)(t1.tv_usec - t0.tv_usec)/1000000));
        int i=0;

        //Data transfers starts here
        while((*status_reg_addr & 0x4) == 0x4){
            copy(virt_addr + 0x20 ,array+ i*16);
            i++;
            if(i==256)
                break;
        }
        //Data transfer ends here

        printf("Total samples received %d\n",i*16);

        arr_float=(float *)array;

        #ifdef DEBUG
        for( i=0;i<NO_OF_PLOTTING_VALS;i++)
        {
            if(i>(NO_OF_PLOTTING_VALS-16))
            printf("<->%f<->",arr_float[i]);
        }
        cout<<"update_data_end"<<endl;
        #endif

#endif




/*--------------------------------------------------------------*/

#ifdef PCGUI
    for(int i=0;i<NO_OF_PLOTTING_VALS;i++)
         {
            array[i]=i;
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

inline void MainWindow::copy(uint32_t *src_addr,unsigned int *dest_addr){

    asm("pld [%[src_address]]\n\t"
        "vldm %[src_address],{d0-d7}\n\t"
        "vstm %[dest_address],{d0-d7}\n\t"
        ://outputs
            :[src_address]"r"(src_addr),[dest_address]"r"(dest_addr)//inputs
        :"memory"//clobbers
    );
//D0-D7 double word 64bit*7=64 bytes
}
