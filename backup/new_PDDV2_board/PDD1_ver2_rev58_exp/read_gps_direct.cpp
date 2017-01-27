#include<all_headers.h>
#include <stdexcept>
using namespace std;
int MainWindow::read_gps_direct()
{
    dataTimer.stop();

    using namespace std;

    try{
        string line;
        ifstream myfile ("/opt/gps_log.txt");
        int i=0;
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )//read till all lines end
            {

                if(i==0)
                    gps_time=line;//first line time
                if(i==1)
                    __latitude=QString::fromStdString( line);//second line latitude
                if(i==2)
                    __longitude=QString::fromStdString(line);//third line longitude
                if(i==3)
                    gps_date=line;//fourth line is date
                i++;

            }
            myfile.close();
            if(i==1)
            {
                /*ui->latitude->setText(QString::fromStdString(gps_time));//searching gps
                ui->longitude->setText("");*/
            }
            else
            {
                /*ui->latitude->setText(__latitude);//update lat & long on GUI
                ui->longitude->setText(__longitude);*/
            }


        }

        else cout << "Unable to open file";


        if(i!=1 && rtc_flag==0)
            rtc_program();//program rtc first time gps fix is found
        gps_counter=0;
        dataTimer.start(5000);
    }

    //error handling if gps_log.txt is not found or empty
    catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
        dataTimer.start(5000);
        return 0;
    }

    return 0;
}

