#include "ncursesmode.h"
#include <locale>
#include "ncurses.h"

#include <QTime>
#include <dbwork.h>
#include <QDebug>

int NcursesMode::mouse_x=-1;
int NcursesMode::mouse_y=-1;
QString NcursesMode::mouse_dev="";
QString NcursesMode::static_debug="";
bool NcursesMode::mousedriver_run=true;



int NcursesMode::run(bool mousemode, QString mousedev)
{

    if(mousemode){
        mouse_dev = mousedev;
        mouse_thread = new MouseThread();
        mousedriver_run=true;
        mouse_thread->do_start();
    }

    QString error = DBWork::do_config();
    if(error.length()>0){
        qWarning()<<error;
        return 0;
    }

    setlocale(LC_ALL, "ru_RU.UTF8");
    initscr();                   // Переход в curses-режим
    noecho();
    //raw();                          /* Line buffering disabled      */
    nonl();
    keypad(stdscr, TRUE);
    start_color();


    timeout(300);
    int key=0;
    bool full_refresh=false;
    int w,h;

    bool exit=false;

    init_pair(GREEN_COLOR_PAIR,COLOR_WHITE,COLOR_GREEN);
    init_pair(RED_COLOR_PAIR,COLOR_WHITE,COLOR_RED);
    init_pair(RED_TEXT_PAIR,COLOR_RED,COLOR_BLACK);

    QString mo_buffer;
    QString time;
    QString user_name;
    QString debug;

    //in m;
    int clear_counter=0;

    while(true){

        if(mousemode){
            exit=(mouse_x>2100);
        }

        getmaxyx(stdscr, h, w);
        if(full_refresh){
            full_refresh=false;
            wclear(stdscr);
        }


        wmove(stdscr,2,1);
        clrtoeol();
        wprintw(stdscr,debug.toStdString().c_str());

        mvwprintw(stdscr,1,(w/2)-mo_buffer.length()/2,mo_buffer.toStdString().c_str());

        time = QTime::currentTime().toString("hh:mm");

        mvwprintw(stdscr,3,(w/2)-time.length()/2,time.toStdString().c_str());


        if(!exit) wattron(stdscr,COLOR_PAIR(GREEN_COLOR_PAIR));
        mvwprintw(stdscr,h/2-1,(w/2)/2-3,"      ");
        mvwprintw(stdscr,h/2,(w/2)/2-3,  " Вход ");
        mvwprintw(stdscr,h/2+1,(w/2)/2-3,"      ");
        if(!exit) wattroff(stdscr,COLOR_PAIR(GREEN_COLOR_PAIR));

        if(exit) wattron(stdscr,COLOR_PAIR(RED_COLOR_PAIR));
        mvwprintw(stdscr,h/2-1,(w/2)+(w/2)/2-5,"       ");
        mvwprintw(stdscr,h/2,(w/2)+(w/2)/2-5,  " Выход ");
        mvwprintw(stdscr,h/2+1,(w/2)+(w/2)/2-5,"       ");
        if(exit) wattroff(stdscr,COLOR_PAIR(RED_COLOR_PAIR));

        if(!error.isEmpty()){
            wmove(stdscr,h-5,0);
            wattron(stdscr,COLOR_PAIR(RED_COLOR_PAIR));
            clrtoeol();
            wmove(stdscr,h-5,(w/2)-error.length()/2);
            wprintw(stdscr,error.toStdString().c_str());
            wattroff(stdscr,COLOR_PAIR(RED_COLOR_PAIR));
        }else{
            wmove(stdscr,h-5,0);
            clrtoeol();
        }

        if(!user_name.isEmpty()){
            wmove(stdscr,h-4,0);
            if(error.isEmpty())wattron(stdscr,COLOR_PAIR(GREEN_COLOR_PAIR));
            clrtoeol();
            wmove(stdscr,h-4,(w/2)-user_name.length()/2);
            wprintw(stdscr,user_name.toStdString().c_str());
            if(error.isEmpty())wattroff(stdscr,COLOR_PAIR(GREEN_COLOR_PAIR));
        }else{
            wmove(stdscr,h-4,0);
            clrtoeol();
        }

        mvwprintw(stdscr,h-3,(w/2)-10,"Проведите ");
        wattron(stdscr,COLOR_PAIR(RED_TEXT_PAIR));
        wprintw(stdscr,"МО");
        wattroff(stdscr,COLOR_PAIR(RED_TEXT_PAIR));
        wprintw(stdscr," картой");

        box(stdscr,0,0);
        refresh();

        key=getch();
        debug=QString::number(key);
        if(key==-1){
            //full_refresh=true;
            if(clear_counter>10){
                error="";
                user_name="";
                mo_buffer="";
                clear_counter=0;
            }else{
                clear_counter++;
            }
            debug+=" x:"+QString::number(mouse_x)+" y:"+QString::number(mouse_y)+" "+static_debug;
        }else
        if(KEY_RESIZE==key){
            full_refresh=true;
        }else
        if('x'==key){
            exit=true;
            mo_buffer="";
        }else
        if('z'==key){
            exit=false;
            mo_buffer="";
        }else
        if(key==13){}
        else
        if(key=='?'){
            QString mo = mo_buffer.right(9);

            user_name=mo;
            QString fname;
            QString sname;
            int io;
            if (exit){io = 0;}else{ io = 1;} //значение входа для записи в бд

            int result = DBWork::try_to_register_pass(mo,fname,sname,io,error);
            //--------------------------------------------------------------------------------------------
            if(result>0){
                switch (result) {
                case DBWork::DB_CONNECT_ERROR:
                    error="Ошибка подключения к BD, "+error;
                    break;
                case DBWork::USER_NOT_FOUND:
                    error="Вы не являетесь сотрудником. Данные о попытке входа/выхода сохранены";
                    break;
                case DBWork::WRONG_DIRECTION:
                    if(exit)
                        error="Вы уже вышли!";
                    else
                        error="Вы уже вошли!";
                    user_name = sname+" "+fname+", Провал!";
                break;
                }
            }else{
                user_name = sname+" "+fname+", Успех!";
            }

            clear_counter=0;
            mo_buffer="";
            full_refresh=true;

        }else
        {
            mo_buffer.append(QChar((char)key));
        }


    }

    endwin();                    // Выход из curses-режима. Обязательная команда.
    mousedriver_run=false;
    return 0;
}

int NcursesMode::mouse_thread_func()
{
    mouse_x=99;
    mouse_y=98;

    FILE *f;
    f=fopen(mouse_dev.toStdString().c_str(),"rb");
    if(f==NULL){
        static_debug="error opening file: "+mouse_dev;
        return 1;
    }

    unsigned char w;
    short x;
    short y;
    while(mousedriver_run){
         fread(&w,1,1,f);
         if(w==0x02){
             fread(&x,2,1,f);
             fread(&y,2,1,f);
             mouse_x = x;
             mouse_y = x;
         }


    }

    fclose(f);

    return 0;
}
