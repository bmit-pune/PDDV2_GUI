#ifndef DATAUPDATE
#define DATAUPDATE

#include <QMainWindow>


#include "QThread"
#include "stdint.h"

//extern MODE mode;

class dataUpdate: public QThread
{
    Q_OBJECT
public:
    dataUpdate();
    ~dataUpdate();

public:
    void run();
    inline void update_data();
    inline void copy_fft(uint32_t *src_addr,unsigned int *dest_addr);
    inline void copy_raw(uint32_t *src_addr,unsigned short int *dest_addr);
    inline float sample_to_millivolt(unsigned int sample);



public :
signals:


public:
    unsigned int FPGA_DATUM;


};



#endif // DATAUPDATE

