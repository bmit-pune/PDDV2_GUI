#ifndef THRESHOLD_TIME
#define THRESHOLD_TIME


#include <QDialog>
#include <QStringListModel>
#include<QKeyEvent>
#include<QKeySequence>
#include"global_indexes.h"
//#include"gpio.h"


namespace Ui {
class ThresholdTime;
}

class ThresholdTime : public QDialog
{
    Q_OBJECT

public:
    explicit ThresholdTime(QWidget *parent = 0);
    ~ThresholdTime();

    Ui::ThresholdTime *ui1;
    int Set_Threshold,Set_Time,Set_Location;



private:

    QStringListModel *model,*model1,*model2,*model3,*model4;
    QModelIndex index,index1,index2,index3,index4;
    int row,row1,row2,row3,row4;
    QStringList List,List1,List2,List3,List4;
    int pos,pos1,clean_ok_flag;




public:
    void setup_select_mode(int mode);
signals:
    void on_exit_signal();


protected:
    void keyPressEvent(QKeyEvent *e);

private:
      void on_exit_clicked();


};




#endif // THRESHOLD_TIME

