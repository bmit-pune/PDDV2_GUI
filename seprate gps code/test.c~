	/*
	Command for compiling application on host:
	/opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-gcc -o app.o baseband_app.c
	*/

	#include <stdio.h>
	#include <sys/mman.h>
	#include <sys/stat.h>
	#include <sys/ioctl.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <errno.h>
	#include <termios.h>
	#include <stdint.h>
	#include <sys/types.h>


	#define UART "/dev/ttymxc1"
	#define SUCCESS 1
	#define ERR -1
	#define TRUE 1
	#define FALSE 0

	/*Function declarations*/
	enum gprmc {nmea_field, UTC, Valid, latitude, lat_ho, longitude, lon_ho, SOG, Track, day};
	//////////////////////////////////
	int open_1(const char *port);
	void close_1();
	int rd(char *buff, int count);
	int wr(char *buff, int count);
	int gps_read(char *buff);
	int chk_lck(void);
	int get_val(void);
	//////////////////////////////////


	char buff_gps[200];
	char* search = "Searching gps...";
	char* gpsoff = "GPS OFF";
	char* hello = "hello";
	char UTC_str[9];
	char lat_str[10];
	char lat_o_str[1];
	char longi_str[11];
	char longi_o_str[1];
	char day_str[6];


			
	/*global variable*/
	int fd_uart;
	FILE *gps;

	void main()
	{    
		int i,lock=0;
		unsigned char rd_sg;	
		
		while(1)
		{
			open_1(UART);
			gps_read(buff_gps);
			
			
	//		for(i=0;i<80;i++)
	//		{
	//			putchar(buff_gps[i]);
	//		}
	//		printf("%s",buff_gps);
	//		putchar('\n');
			printf("I am here also");
			if((chk_lck()))
				{
					get_val();
					gps = fopen("/opt/gps_log.txt", "w");
					if (gps == NULL)
					{
						printf("error opening file!\n");
					//exit(1);
					}
			
					fprintf(gps, "%s\n%s%s\n%s%s\n%s",UTC_str,lat_str,lat_o_str,longi_str,longi_o_str,day_str);
					fclose(gps);
					
					//printf("Locked GPS");
				}
			else {
				gps = fopen("/opt/gps_log.txt", "w");
				if (gps == NULL)
				{
					printf("error opening file!\n");
					//exit(1);
				}
			
				fprintf(gps, "%s\n",search);
				fclose(gps);
			
				 }
			close_1();
			sleep(60);	
			
		}
		
	}


	int open_1(const char *port)
	{
		fd_uart = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd_uart == -1)
		{
		 /* Could not open the port. */
		  perror("open_port: Unable to open UART port");
		  return ERR;
		}
		else
		{
		  fcntl(fd_uart, F_SETFL, 0);
		  return SUCCESS;
		}
		int flags = fcntl(fd_uart, F_GETFL, 0);
		fcntl(fd_uart, F_SETFL, flags | O_NONBLOCK);
		printf("uart open");
	}

	void close_1()
	{
		close(fd_uart);
	}


	int wr(char *buff, int count)
	{
		int no_write_bytes = 0;

		while(count > 0)
		{
			no_write_bytes = write(fd_uart, buff, count);
			if (no_write_bytes < 1)
			{
				fputs("write() bytes failed!\n", stderr);
				return ERR;
			}
			count -= no_write_bytes;
			buff += no_write_bytes;
		}
		
		return SUCCESS;
	}

	int rd(char *buff, int count)
	{
		int no_read_bytes = 0;

		while(count>0)
		{
			no_read_bytes = read(fd_uart, buff, count);
			//printf("%d is the no of bytes read\n",no_read_bytes);
			if (no_read_bytes < 1)
			{
				fputs("read() bytes failed!\n", stderr);
				return ERR;
			}
				count-=no_read_bytes;
				buff+=no_read_bytes;

		}
		
		return SUCCESS;
	}

	/* int gps_read(char *buff)
	{
		int i;
		while(1)
		{
			//printf("inside while");
			read(fd_uart, buff, 1);
			if (buff[0] == '$')
				{
				read(fd_uart, buff+1, 1);
				if(buff[1] == 'G')
					{
					read(fd_uart, buff+2, 1);
					if(buff[2] == 'P')
						{
						read(fd_uart, buff+3, 1);
						if(buff[3] == 'G')
							{
							read(fd_uart, buff+4, 1);
							if(buff[4] == 'G')
								{
								//read(fd_uart, buff, 1);
								rd(buff+5,150);
								break;
								}	

							}	
						}
					}
				}

		}
	}
	 */

	int gps_read(char *buff)
	{
		int i,val=0,fail=0;
		char* gpgga = "$GPRMC";
		while(1)
		{
			for(i=0;i<5;i++)
			{
				val = read(fd_uart, buff+i,1);
				if(val<1)
				{
					fail++;
					if(fail>=2) 
					{
						return 0;
					}
				}
				if (buff[i]!=gpgga[i])
				{
					break;
				}
			}
			if(i==5)
			{
				rd(buff+i,70);
				return 1;
			}
		}
	}

	int chk_lck(void)
	{
		
		int i,cnt=0;
		for(i=0; buff_gps[i]!='\n';i++)
		{
			if(buff_gps[i]==',')
			{
				cnt++;
				if(cnt==2)
			{
				if(buff_gps[i+1] == 'A'){return 1;}
				else {return 0;}
			}
			}
			
		}
	}

	int get_val(void)
	{
		
		int i,j=0,cnt=0;
		for(i=0;buff_gps[i]!='\n';i++)
		{
			if(buff_gps[i]==',')
			{
				cnt++;
				switch(cnt) {
						case UTC:
							for(j=1;buff_gps[i+j]!=',';j++)
							{
								UTC_str[j-1] = buff_gps[i+j];
							}
				break;	
						case latitude:
							for(j=1;buff_gps[i+j]!=',';j++)
							{
								lat_str[j-1] = buff_gps[i+j];
							}
						break;
						case lat_ho:
							for(j=1;buff_gps[i+j]!=',';j++)
							{
								lat_o_str[j-1] = buff_gps[i+j];
							}
				break;	
						case longitude:
							for(j=1;buff_gps[i+j]!=',';j++)
							{
								longi_str[j-1] = buff_gps[i+j];
							} 
				break;   
						case lon_ho:
							for(j=1;buff_gps[i+j]!=',';j++)
							{
								longi_o_str[j-1] = buff_gps[i+j];
							} 
				break; 
				case day:
							for(j=1;buff_gps[i+j]!=',';j++)
							{
								day_str[j-1] = buff_gps[i+j];
							}
				break;      

					}
			}
			
		}
	}
