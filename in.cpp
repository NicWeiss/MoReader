#include "in.h"
#include "gclass.h"
#include "ui_in.h"
#include "march.h"

#include <dbwork.h>

in::in(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::in)
{
    //скрываем курсор
    if(!global::cursor){
        QCursor oCursor(Qt::BlankCursor);
        oCursor.setPos(-100, -100);
        setCursor(oCursor);
    }
    //--------------------------------------------------------------------------------------------
    //инициация переменных
    global::gender="tst";
    //if (global::anim_state){ui->label_3->hide();}
    frame_in=0;
    frame_out=50;
    pattern_limit=50;
    green_in_arrow="<b><FONT SIZE=6 COLOR=#00ff00>></FONT></b>";
    red_out_arrow="<b><FONT SIZE=6 COLOR=#ff0000>&lt;</FONT></b>";
    exit_timer = false; //сброс флага выхода
    QMessageBox msgBox; //объявляем мессаджбокс
    currDate = QDate::currentDate();
    msgBox.setText(currDate.toString("dd.MM"));
    //msgBox.exec();

    //--------------------------------------------------------------------------------------------Timer's
    timer = new QTimer;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_overflow())); //Связываем сигнал переполнения таймера со слотом
    timer->start(100);  //Задаем время срабатывания таймера (в мс)

    timeranim_in = new QTimer;
    QObject::connect(timeranim_in, SIGNAL(timeout()), this, SLOT(timer_anim_in())); //Связываем сигнал переполнения таймера со слотом

    timer_restore_text_input = new QTimer;
    QObject::connect(timer_restore_text_input, SIGNAL(timeout()), this, SLOT(timer_rest_text_input())); //Связываем сигнал переполнения таймера со слотом


    timer_clear = new QTimer;
    QObject::connect(timer_clear, SIGNAL(timeout()), this, SLOT(timer_clear_processor())); //Связываем сигнал переполнения таймера со слотом

    timeranim_out = new QTimer;
    QObject::connect(timeranim_out, SIGNAL(timeout()), this, SLOT(timer_anim_out())); //Связываем сигнал переполнения таймера со слотом

    ui->setupUi(this);  //задаём юи для отрисовки
    showFullScreen();   //открываем во весь экран
    setin();

    //--------------------------------------------------------------------------------------------
    QString error = DBWork::do_config();
    if(error.length()>0){
        this->hide();
        msgBox.setText(error);
        msgBox.exec();
        exit(0);
    }

}
in::~in()
{
    delete ui; //разрушение формы
}


void in::setin(){
    //задаём текстовые поля для входа
    ui->label->setText("<FONT COLOR=#ffffff>Подтвердите</FONT>");
    ui->label_7->setText("<a href='#'><FONT COLOR=#008000 SIZE=6>ВХОД</FONT></a>");
    ui->label_2->setText("<FONT COLOR=#ffffff>Проведите </FONT><FONT COLOR=#ff0000>Mo</FONT><FONT COLOR=#ffffff>.Картой</FONT>");
    ui->label_3->setText("-----------------------------------------------------------------------------------------------------------------------------------------");
    ui->label_4->setText(" ");
    ui->label_5->setText(" ");
    ui->label_9->setText("<a href='#'><FONT COLOR=#ffffff SIZE=1 style=\"text-decoration:none;\">ВЫХОД</FONT></a>");
    in_out = true; //  флаг входа

}

void in::setout(){
    //задаём текстовые поля для выхода
    ui->label->setText("<FONT COLOR=#ffffff>Подтвердите</FONT>");
    ui->label_9->setText("<a href='#'><FONT COLOR=#ff0000 SIZE=6>ВЫХОД</FONT></a>");
    ui->label_2->setText("<FONT COLOR=#ffffff>Проведите </FONT><FONT COLOR=#ff0000>Mo</FONT><FONT COLOR=#ffffff>.Картой</FONT>");
    ui->label_3->setText("-----------------------------------------------------------------------------------------------------------------------------------------");
    ui->label_5->setText(" ");
    ui->label_4->setText(" ");
    ui->label_7->setText("<a href='#'><FONT COLOR=#ffffff SIZE=1 style=\"text-decoration:none;\">ВХОД</FONT></a>");
    in_out = false; //  флаг выхода

}

void in::timer_overflow()
{
    //--------------------------------------------------------------------------------------------
    //Обновляем значение времени на форме
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0) text[2] = ' ';
    //--------------------------------------------------------------------------------------------
    ui->lcdNumber->display(text); //вывод часов
    ui->textEdit->setEnabled(true); //поле ввода активно
    ui->textEdit->setFocus(); //фокус на поле ввода
    //--------------------------------------------------------------------------------------------
    if (exit_timer == true){
        sleep_time = sleep_time-1;
      if (sleep_time <= 0){
        //обновление информации в текстовых полях
        if (in_out){setin();}else{ setout();}
        exit_timer = false; //сброс флага выхода
      }
    }

}

void in::timer_rest_text_input()
{
//..
    ui->textEdit->setReadOnly(false);
}

void in::timer_clear_processor()
{
//..
    ui->textEdit->clear();
    timer_clear->stop();
}

void in::timer_anim_in()
{

    QString to_out;
    to_out="";
    for (int i = 0; i<pattern_limit; i++) {
        pattern[i]="<FONT COLOR=#383C4A>></FONT>";
    }

    //-----------blue
    if(frame_in>5 and frame_in<pattern_limit){pattern[frame_in]=green_in_arrow;}
    if(frame_in-1>5 and frame_in-1<pattern_limit){ pattern[frame_in-1]=green_in_arrow;}

    //---------yellow
    if(frame_in-2>5 and frame_in-2<pattern_limit){pattern[frame_in-2]=green_in_arrow;}
    if(frame_in-3>5 and frame_in-3<pattern_limit){pattern[frame_in-3]=green_in_arrow;}

    //----------red
    if(frame_in-4>5 and frame_in-4<pattern_limit){pattern[frame_in-4]=green_in_arrow;}
    if(frame_in-5>5 and frame_in-5<pattern_limit){pattern[frame_in-5]=green_in_arrow;}

   // to_out.at(0).

    for (int i = 0; i < pattern_limit; i++) {
        to_out=to_out+pattern[i];
    }
    ui->label_3->setText(to_out);
    //----------------
    frame_in++;
    if (frame_in>42){
        ui->label_3->setText("-----------------------------------------------------------------------------------------------------------------------------------------");
        frame_in=0; timeranim_in->stop();
    }

}

void in::timer_anim_out()
{
    QString to_out;
    to_out="";
    for (int i = 0; i<pattern_limit; i++) {
        pattern[i]="<FONT COLOR=#383C4A>&lt;</FONT>";
    }

    //-----------blue
    if(frame_out>5 and frame_out<pattern_limit){pattern[frame_out]=red_out_arrow;}
    if(frame_out+1>5 and frame_out+1<pattern_limit){ pattern[frame_out+1]=red_out_arrow;}

    //---------yellow
    if(frame_out+2>5 and frame_out+2<pattern_limit){pattern[frame_out+2]=red_out_arrow;}
    if(frame_out+3>5 and frame_out+3<pattern_limit){pattern[frame_out+3]=red_out_arrow;}

    //----------red
    if(frame_out+4>5 and frame_out+4<pattern_limit){pattern[frame_out+4]=red_out_arrow;}
    if(frame_out+5>5 and frame_out+5<pattern_limit){pattern[frame_out+5]=red_out_arrow;}


    for (int i = 0; i < pattern_limit; i++) {
        to_out=to_out+pattern[i];
    }
    ui->label_3->setText(to_out);
    //-----------------------------
    frame_out--;
    if (frame_out<-10){
        frame_out=36;
        ui->label_3->setText("-----------------------------------------------------------------------------------------------------------------------------------------");
        timeranim_out->stop();
    }

}



void in::on_textEdit_textChanged()
{
    //--------------------------------------------------------------------------------------------
    //инициация переменных
    int io;
    int l;
    bool moCard=false;
    QString nowsymbol, tio;
    //--------------------------------------------------------------------------------------------
    if (in_out){io = 1; tio="1";}else{ io = 0; tio="0";} //значение входа для записи в бд
    QString text = ui->textEdit->toPlainText(); //получение введённого текста
    l = text.length();//получение длинны текста
    timer_clear->start(800);
    //--------------------------------------------------------------------------------------------
    if (l>0){
        QString firstsymbol=text.at(0);
        nowsymbol=text.at(l-1);
        //если символ = Z просто сбрасывае форму ввода
            if (text == "z" || text =="я"){
                ui->textEdit->clear();
                setin();
            }
            //если символ = X уходим в другую форму
            if (text == "x" || text =="ч"){
                ui->textEdit->clear();
                setout();
            }
            if (text == "c" || text =="с"){
                //this->hide();
                March *mar = new March;
                mar->show();

            }
            //if ( firstsymbol != ";") ui->textEdit->clear();

    }
    if (nowsymbol == "?"){
     if(!global::anim_state)if (in_out){timeranim_in->start(15);}else{timeranim_out->start(15);}
        //QMessageBox msgBox;
        //msgBox.setText(tio);
        //msgBox.exec();
    }

    if (text.left(4)=="2220"){
        if(text.length()==13){
            text=text+"?";
            if(!global::anim_state)if (in_out){timeranim_in->start(15);}else{timeranim_out->start(15);}
            QMessageBox msgBox;
            msgBox.setText(tio);
            moCard=true;
        }else{
            moCard=false;}
    }

    //--------------------------------------------------------------------------------------------
    if (nowsymbol == "?" or moCard==true) {
        ui->textEdit->setReadOnly(true);
        timer_restore_text_input->start(1000); //лок поля ввода
        //инициализация переменных
        //--------------------------------------------------------------------------------------------
        //получение мо номера
        QString subString = text.left(text.length()-1);
        QString mo = subString.right(9);
        QString username="";
        QString surname="";
        QString terror;
        //--------------------------------------------------------------------------------------------
        ui->textEdit->clear();


        if(mo.length()!=9){
            ui->label->setText("<FONT COLOR=#ff0000>ОШИБКА ЧТЕНИЯ КАРТЫ</FONT>");
            ui->label_2->setText("<FONT COLOR=#ff0000>попробуйте снова</FONT>");
            ui->label_4->setText("<FONT COLOR=#ff0000></FONT>");
            sleep_time = 80; //устанавливаем время вывода сообщения на экране
            exit_timer = true;
            return;
        }

        //--------------------------------------------------------------------------------------------
        int result = DBWork::try_to_register_pass(mo,username,surname,io,terror);
        //--------------------------------------------------------------------------------------------
        //QMessageBox msgBox;
        //msgBox.setText(QString::number(result));
       // msgBox.exec();

        if (io == 1 and result == 0 and global::gender == "1"){                         // Поздравление с восьмым марта (если 7-е или 8-е число марта)
            currDate = QDate::currentDate();
            if(currDate.toString("dd.MM")=="07.03" || currDate.toString("dd.MM")=="08.03"){
                March *mar = new March;
                mar->showFullScreen();
            }

        }

        //QMessageBox msgBox;
       // if(global::anim_state){msgBox.setText("true");}else{msgBox.setText("false");} //QString::number(result));
        //msgBox.exec();


        if(result>0){
            switch (result) {
            case DBWork::DB_CONNECT_ERROR:
                ui->label->setText("<FONT COLOR=#ff0000>НЕТ ПОДКЛЮЧЕНИЯ</FONT>");
                ui->label_2->setText("<FONT COLOR=#ff0000>критическая ошибка</FONT>");
                ui->label_4->setText("<FONT COLOR=#ff0000>обратитесь за помощью в IT отдел</FONT>");
                //OOps бд сломалась... ну или нет коннекта)
                return;
            case DBWork::USER_NOT_FOUND:
                ui->label->setText("<FONT COLOR=#ff0000>Нет доступа!</FONT>");
                ui->label_2->setText("<FONT COLOR=#ffffff>Вас нет в списке сотрудников!</FONT>");
                ui->label_4->setText("<FONT COLOR=#ffffff>Если вы являетесь сотрудником.</FONT>");
                ui->label_5->setText("<FONT COLOR=#ffffff>Обратитесь в отдел кадров</FONT>");
                sleep_time = 80; //устанавливаем время вывода сообщения на экране
                exit_timer = true;
                return;
            case DBWork::WRONG_DIRECTION:
                ui->label->setText(in_out ? "<FONT COLOR=#f05050>Вы уже вошли</FONT>" :"<FONT COLOR=#f05050>Вы уже вышли</FONT>");
                sleep_time = 40; //устанавливаем время вывода сообщения на экране
                exit_timer = true;
            break;
            default:
                break;
            }
            exit_timer = true; //запуск процедуры вывода информации
        }

        //--------------------------------------------------------------------------------------------
        if(result==0){ui->label->setText("<FONT COLOR=#00ff00>УСПЕХ!</FONT>");}
        ui->label_2->setText("<FONT COLOR=#ff0000>Мо</FONT><FONT color=#ffffff>.Карта</FONT>");
        ui->label_4->setText("<FONT color=#ffffff>"+mo+"</FONT>");
        ui->label_5->setText("<FONT color=#ffffff>"+username+"  "+surname+"</FONT>");
        sleep_time = 40; //устанавливаем время вывода сообщения на экране

        exit_timer = true; //запуск процедуры вывода информации



    }


}


void in::on_label_9_linkActivated()
{
setout();
}

void in::on_label_7_linkActivated()
{
setin();
}


