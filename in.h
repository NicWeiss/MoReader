#ifndef IN_H
#define IN_H

#include <QWidget>
#include <QString>
#include <QStringRef>
#include <QTextEdit>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSqlQuery>
#include <QLabel>
#include <QTextStream>
#include <QTextCodec>
#include <QThread>
#include <QTimer>
#include <QPixmap>
#include <march.h>
#include <QTime>
#include <QDate>

namespace Ui {
class in;
}

class in : public QWidget
{
    Q_OBJECT

public:    
    QTimer *timer;
    QTimer *timeranim_in;
    QTimer *timer_restore_text_input;
    QTimer *timeranim_out;
    QTimer *timer_clear;
    explicit in(QWidget *parent = 0);
    void truecard();
    ~in();
    void setin();
    void setout();

private slots:
    void on_textEdit_textChanged();

    void on_label_9_linkActivated();

    void on_label_7_linkActivated();

private:
    Ui::in *ui;
    QDate currDate;

public slots:
//Слот, который срабатывает при переполнении таймера
void timer_overflow();
void timer_anim_in();
void timer_anim_out();
void timer_rest_text_input();
void timer_clear_processor();

};



#endif // IN_H
