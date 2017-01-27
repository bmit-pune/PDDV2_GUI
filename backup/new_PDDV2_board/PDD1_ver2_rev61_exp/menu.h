#ifndef MENU
#define MENU


#include <QDialog>
#include <QStringListModel>
#include<QKeyEvent>
#include"global_indexes.h"
#include"gpio.h"


namespace Ui {
class menu;
}

class menu : public QDialog
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    ~menu();
    std::string exec(std::string);
    gpio gps_gpio;
    bool gps_off;
    Ui::menu *ui2;
    int usb_upgrade();

public :
    signals:
    void on_menu_exit_signal();


private:

    QStringListModel *model,*model1,*model2,*model3,*model4;
    QModelIndex index,index1,index2,index3,index4;
    int row,row1,row2,row3,row4;
    QStringList List,List1,List2,List3,List4;
    int pos,pos1,clean_ok_flag;




protected:
    void keyPressEvent(QKeyEvent *e);
   // void keyReleaseEvent(QKeyEvent *e);
private slots:
      void on_exit_clicked(void);


};



#endif // MENU

