#include<mydialog.h>
#include<QGridLayout>
#include<QMessageBox>
MyDialog::MyDialog(QWidget *parent) :
    QWidget(parent)
{

        QGridLayout *layout = new QGridLayout();
        //label->setLayout(layout);
        //label->setText("string");
        //frame->setLayout(layout);
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);
        //label->setAlignment(Qt::AlignCenter);
        //label->setStyleSheet("QLabel {color: #0000ff;font: 16pt;border-radius:5px}");
        //frame->setStyleSheet("QFrame{background-color:#ffffff; border-radius:6px}");
        setLayout(layout);
        layout->setSizeConstraint(QLayout::SetMinimumSize);//layoutgrows automatically
        this->hide();


}

void MyDialog::settext(QString text)
{


    /*layout()->addWidget(label);//increase size dynamically
    this->frame->resize(this->label->width()+10,this->label->height()+10);//Frame Little bigger than label
    this->label->setText(text);
    */
}
