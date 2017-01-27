/***************************************************************
EIM SYNCHRONOUS READ

	EIM DEFAULT CLOCK ---------------> 133MHz
        BURST CLOCK DIVISOR(BCD)-----> 3
 	EIM CLOCK ------------------> (133/3)MHz

	EIM DATA BUS WIDTH ---------> 32bit
	EIM BURST LENGTH ------------> 16
	ONE BURST -----> (32bit*16) -----> 64 bytes 

	EIM Register Settings
	CS1GCR1(0x021b8018) ----> 0x31132235
	CS1GCR2(0x021b801c) ----> 0x00000000
	CS1RCR1(0x021b8020) ----> 0x01000000
	CS1RCR2(0x021b8024) ----> 0x00000000
	CS1WCR1(0x021b8028) ----> 0x0804A240
		
****************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define NUM_SAMPLES 13631488
#define EIM_BASE_ADDR 0x08000000
#define VIRT_MEM_ALLOC_SIZE 0x5000
#define STATUS_REG_OFFSET 0x90 
#define CONTROL_REG_OFFSET 0x94

static unsigned char read_data[NUM_SAMPLES];
static inline void copy(uint32_t *src_addr,unsigned char *dest_addr) __attribute__((always_inline,unused));

int main(){

	uint32_t mem_address;
	unsigned int mem_size;
	volatile unsigned int alloc_mem_size, page_mask, page_size;
	volatile uint32_t *virt_addr,*control_reg_addr,*status_reg_addr;
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
	printf("Obtained Virtual address: 0x%x\n",virt_addr);

	status_reg_addr = virt_addr + STATUS_REG_OFFSET;       //virtual status register address
	control_reg_addr = virt_addr + CONTROL_REG_OFFSET;     //virtual control register address
	
	printf("Status reg address: 0x%x\n",status_reg_addr);
	printf("Control reg address: 0x%x\n",control_reg_addr);

	*control_reg_addr = 0x01 ; //write control register bit to start pipeline
	printf("Control value is 0x%x\n",*control_reg_addr);
	
	while(!(*status_reg_addr & 0x01)); //wait for ack from status reg
		
	*control_reg_addr = 0x02 ; //write control register bit to reset pipeline
	
	printf("Control value is 0x%x\n",*control_reg_addr);

	while((*status_reg_addr & 0x2) != 0x2); //wait for data ack

	int i=0;

	gettimeofday(&t0,0);

	//Data transfers starts here
	while((*status_reg_addr & 0x2) == 0x2){
	//while(i < (NUM_SAMPLES/64)){
		//if(!(*status_reg_addr & 0x2)){
		//	break;
		//}
		copy(virt_addr,read_data + i*64);
		i++;
	}
	//Data transfer ends here

	printf("Total samples received %d\n",i*64);
	gettimeofday(&t1,0);
	printf("Time for data transfer: %lf\n",(double)((t1.tv_sec - t0.tv_sec) + (double)(t1.tv_usec - t0.tv_usec)/1000000));
	
	for(i=0;i<NUM_SAMPLES;i++){
		if(read_data[i] != i){
			printf("Test failed :( at %d value obtained is %d\n",i,read_data[i]);
			break;
		}
	}

	if(i==NUM_SAMPLES)
		printf("Test passed :)\n");

#if 1
	//for debug
	for(i=0;i<100;i++){
		printf("read data %hu\n",read_data[i]);
	}
#endif

	return 0;

}


static inline void copy(uint32_t *src_addr,unsigned char *dest_addr){

	asm("pld [%[src_address]]\n\t"
	    "vldm %[src_address],{d0-d7}\n\t"
	    "vstm %[dest_address],{d0-d7}\n\t"
	    ://outputs
            :[src_address]"r"(src_addr),[dest_address]"r"(dest_addr)//inputs
	    :"memory"//clobbers
	);


}
