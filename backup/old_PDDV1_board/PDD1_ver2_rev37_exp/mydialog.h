#ifndef MYDIALOG_H
#define MYDIALOG_H
#include<QWidget>
#include<QFrame>
#include<QLabel>
#include<QObject>
class MyDialog :public QWidget
{

     Q_OBJECT
 public:

     //QFrame *frame = new QFrame(this);
     //QLabel *label = new QLabel(this);
     explicit MyDialog(QWidget *parent = 0);
     void settext(QString text);




};
#endif // MYDIALOG_H

