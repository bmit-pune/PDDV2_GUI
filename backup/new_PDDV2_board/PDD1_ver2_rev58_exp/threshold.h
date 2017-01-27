#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <QDialog>
#include <QStringListModel>
#include<QKeyEvent>
#include <QMessageBox>



namespace Ui {
class Threshold;
}

class Threshold : public QDialog
{
    Q_OBJECT
    
public:
    explicit Threshold(QWidget *parent = 0);
    ~Threshold();




    
private:
    Ui::Threshold *ui1;
    QStringListModel *model;
    QModelIndex index;
    int row;
    QStringList List;



protected:
    void keyPressEvent(QKeyEvent *e);

public: signals:
    void calibration_signal();
    void start_BCHmode_display();

};

#endif // THRESHOLD_H
