#include "exi.h"
#include "ui_exi.h"
#include "in.h"
#include "gclass.h"
#include <QTime>

exi::exi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::exi)
{
    //скрываем курсор
    QCursor oCursor(Qt::BlankCursor);
    oCursor.setPos(-100, -100);
    setCursor(oCursor);
    //--------------------------------------------------------------------------------------------
    ui->setupUi(this);
    showFullScreen();
    exit_timer = false;
    timer = new QTimer;
    //Связываем сигнал переполнения таймера со слотом
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_overflow()));
    //Задаем время срабатывания таймера (в мс)
    timer->start(100);
    ui->label->setText("Подтвердите");
    ui->label_9->setText("<FONT COLOR=#ff0000>ВЫХОД</FONT>");
    ui->label_2->setText("Проведите <FONT COLOR=#ff0000>Mo</FONT>.Картой");
    ui->label_5->setText("");
    ui->label_4->setText("");
    QMessageBox msgBox;
    QFile file("moConfig.ini");
       if(file.open(QIODevice::ReadOnly |QIODevice::Text))
       {
           int tmp;
           QString tms="";
           while(!file.atEnd())
           {
               //читаем строку
               QString str = file.readLine();
               //Делим строку на слова разделенные пробелом
               QStringList lst = str.split(" ");
                 // выводим первых три слова
               if(lst.at(0).at(0) != QString("#")){
                   if(lst.at(0)=="dbhost"){
                       dbhost=lst.at(2);
                       tmp = lst.at(2).length();
                       dbhost=dbhost.left(tmp-1);
                   }
                   if(lst.at(0)=="dbname"){
                       dbname=lst.at(2);
                       tmp = lst.at(2).length();
                       dbname=dbname.left(tmp-1);
                   }
                   if(lst.at(0)=="dbuser"){
                       dbuser=lst.at(2);
                       tmp = lst.at(2).length();
                       dbuser=dbuser.left(tmp-1);
                   }
                   if(lst.at(0)=="dbpass"){
                       dbpass=lst.at(2);
                       tmp = lst.at(2).length();
                       dbpass=dbpass.left(tmp-1);
                   }
               }
           }

       }
       else
       {
           this->hide();
           msgBox.setText("Файл moConfig.ini не найден\r\nФайл будет создан заново.\r\nСконфигурируйте его и запустите приложение.");
           msgBox.exec();
           QFile fileOut("moConfig.ini");
           if(fileOut.open(QIODevice::WriteOnly)){
                   QTextStream writeStream(&fileOut);
                   writeStream.setCodec("UTF8");
                   writeStream << QString("#Паттерн записи 'ключ = значение'\r\n");
                   writeStream << QString("#Хост сервера базы данных\r\n");
                   writeStream << QString("dbhost = localhost\r\n");
                   writeStream << QString("#Имя базы данных\r\n");
                   writeStream << QString("dbname = test\r\n");
                   writeStream << QString("#Польователь\r\n");
                   writeStream << QString("dbuser = user\r\n");
                   writeStream << QString("#Пароль\r\n");
                   writeStream << QString("dbpass = 12345678\r\n");
                   fileOut.close();
                   exit(0);
           }
       }

}

exi::~exi()
{
    delete ui;
}



void exi::timer_overflow()
{
    //Обновляем значение времени на форме
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->lcdNumber->display(text);
    ui->textEdit->setEnabled(true);
    ui->textEdit->setFocus();
    if (exit_timer == true){
        sleep_time = sleep_time-1;
      if (sleep_time <= 0){
        //обновление информации в текстовых полях
        ui->label->setText("Подтвердите");
        ui->label_7->setText("<FONT COLOR=#ff0000>ВЫХОД</FONT>");
        ui->label_2->setText("Проведите <FONT COLOR=#ff0000>Mo</FONT>.Картой");
        ui->label_5->setText("");
        ui->label_4->setText("");
        exit_timer = false; //сброс флага выхода
      }
    }

}


void exi::on_textEdit_textChanged()
{
    //--------------------------------------------------------------------------------------------
    //инициация переменных
    int l;
    //--------------------------------------------------------------------------------------------
    QString text = ui->textEdit->toPlainText(); //получение введённого текста
    l = text.length();//получение длинны текста
    //--------------------------------------------------------------------------------------------
    if (l>0){
        QString firstsymbol=text.at(0);
        QString nowsymbol=text.at(l-1);
        //если символ = Z просто сбрасывае форму ввода
            if (text == "x" || text =="ч"){
                ui->textEdit->clear();
            }
            //если символ = X уходим в другую форму
            if (text == "z" || text =="я"){
                ui->textEdit->clear();
                this->hide();
                in *w = new in;
                timer->stop();
                w->show();
            }
            if ( firstsymbol != ";") ui->textEdit->clear();

    }
    //--------------------------------------------------------------------------------------------нж
    if (l >= 33) {
    bool ok = false;
    bool user_exists;
    int u_id =0;
    int direction = 0;
    QMessageBox msgBox;
    QString subString = text.right(11);
    QString mo = subString.left(9);
    QString str_mo="";
    QString username="";
    QString surname="";
    ui->textEdit->clear();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbhost);
    db.setDatabaseName(dbname);
    db.setUserName(dbuser);
    db.setPassword(dbpass);
    ok = db.open();
    if (ok != true){
        ui->label->setText("<FONT COLOR=#ff0000>can't connect to db</FONT>");
        ui->label_2->setText("<FONT COLOR=#ff0000>критическая ошибка</FONT>");
        ui->label_4->setText("<FONT COLOR=#ff0000>обратитесь за помощью к системному администратору</FONT>");
        //OOps бд сломалась... ну или нет коннекта)
          }
    if (ok == true) {
        QSqlDatabase firstDB = QSqlDatabase::addDatabase("QMYSQL", dbname);
        QSqlQuery query;
        str_mo = QString("SELECT * FROM `i_users` WHERE mo_card="+mo);
        query.exec(str_mo);
        user_exists = false;
        while (query.next()) {
            u_id = query.value(0).toInt();
            if (u_id != 0){user_exists = true;}
            username = query.value(1).toString();
            surname= query.value(2).toString();
            ui->label_2->setText("<FONT COLOR=#ff0000>Мо</FONT>.Карта");
            ui->label_4->setText(mo);
            ui->label_5->setText(username+"  "+surname);
          }
        //проверка наличия входа
        str_mo = QString("SELECT * FROM `i_time_lord` WHERE mo_card="+mo);
        query.exec(str_mo);
        while (query.next()) {
            direction = query.value(3).toInt();
        }
        //выполнение записи или вывод об ошибке
        if(user_exists==true){
            if (direction == 1){
              str_mo = QString("INSERT INTO `i_time_lord` (`u_id`, `mo_card`, `direction`, `time`) VALUES ("+QString::number(u_id)+","+mo+",0,NOW())");
              query.exec(str_mo);
              ui->label->setText("<FONT COLOR=#008000>Выход подтверждён</FONT>");
              sleep_time = 40;
           }else{
                ui->label->setText("<FONT COLOR=#f05050>Вы уже вышли</FONT>");
                sleep_time = 40;
            }
        }
        //дополнительная запись для неизвестных пользователей
        if(user_exists==false){
            ui->label->setText("<FONT COLOR=#ff0000>Нет доступа!</FONT>");
            ui->label_2->setText("Вы не являетесь сотрудником!");
            ui->label_4->setText("Обратитесь к менеджеру по персоналу, для занесения вас в список сотрудников.");
            ui->label_5->setText("");
            if (direction == 1){
                  str_mo = QString("INSERT INTO `i_time_lord` (`mo_card`, `direction`, `time`) VALUES ("+mo+",0,NOW())");
                  query.exec(str_mo);
                  ui->label_5->setText("Данные о попытке выхода были сохранены.");
            }
           sleep_time = 80;
        }
          db.close();
        exit_timer = true;

    }

    }

}
