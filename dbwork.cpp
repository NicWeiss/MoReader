#include "dbwork.h"
#include "gclass.h"
#include <QFile>
#include <QTextStream>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

QString DBWork::do_config()
{
    QString error;
    //проверяем наличие текстового, конфигурационного файла
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
               //заносим значения в переменные
               if(lst.at(0).at(0) != QString("#")){
                   if(lst.at(0)=="dbhost"){
                       dbhost=lst.at(2);
                       tmp = lst.at(2).length();
                       dbhost=dbhost.left(tmp-1);
                   }
                   if(lst.at(0)=="dbname"){
                       dbname=lst.at(2);
                       tmp = lst.at(2).length();
                       dbname=dbname.left(tmp-1);    for (int i = 0; i<200; i++) {
                           pattern[i]="-";
                       }
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
           error="Файл moConfig.ini не найден\r\nФайл будет создан заново.\r\nСконфигурируйте его и запустите приложение.";
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
           }
       }
       return error;
}


int DBWork::try_to_register_pass(QString mocard, QString &u_name, QString &surname, int direction, QString &error)
{
    bool ok;
    int u_id =0;
    //инициалинация подключения к бд
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbhost);
    db.setDatabaseName(dbname);
    db.setUserName(dbuser);
    db.setPassword(dbpass);
    ok = db.open(); //попытка открытия бд
    if(!ok){
        error=db.lastError().text();
        return DB_CONNECT_ERROR;
    }

    //формирование запроса
   // ui->label->setText("<FONT COLOR=#ffffff>Подтвердите</FONT>");
    QSqlDatabase firstDB = QSqlDatabase::addDatabase("QMYSQL", dbname);
    QSqlQuery query;
    QString str_mo = QString("SELECT * FROM `i_users` WHERE mo_card="+mocard+" and u_is_disabled=0");
    //запрос в бд
    query.exec(str_mo);
    bool user_exists = false;
    //чтение записей из бд
    while (query.next()) {
        u_id = query.value(0).toInt();
        if (u_id != 0){user_exists = true;}
        u_name = query.value(1).toString();
        global::name = u_name;
        surname= query.value(2).toString();
        global::gender =  query.value(14).toString();
    }
    QMessageBox msgBox;
            msgBox.setText(global::gender);
            //msgBox.exec();



    //проверка наличия входа
    str_mo = QString("SELECT * FROM `i_time_lord` WHERE mo_card="+mocard);
    query.exec(str_mo);
    int last_direction;
    while (query.next()) {
        last_direction = query.value(3).toInt();
    }
    //выполнение записи или вывод об ошибке
    if(!user_exists){
        if (direction != last_direction){
              str_mo = QString("INSERT INTO `i_time_lord` (`mo_card`, `direction`, `time`) VALUES ("+mocard+","+QString::number(direction)+",NOW())");
              query.exec(str_mo);
        }
        return USER_NOT_FOUND;
    }

    if (direction == last_direction){
        return WRONG_DIRECTION;
    }

    str_mo = QString("INSERT INTO `i_time_lord` (`u_id`, `mo_card`, `direction`, `time`) VALUES ("+QString::number(u_id)+","+mocard+","+QString::number(direction)+",NOW())");
    query.exec(str_mo);

    //закрываю соединения
    db.close();

    return 0;
}
