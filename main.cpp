#include "in.h"
#include <QApplication>
#include <ncursesmode.h>
#include "gclass.h"

#include <iostream>

int main(int argc, char *argv[])
{
    bool ncurses = false;
    bool mousedriver = false;
    bool noanim=false;
    global::anim_state=false;
    global::cursor=false;
    QString tty_device;

    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-h")==0){
            std::cout<<"Usage:\n";
            std::cout<<"    -h          : show this help\n";
            std::cout<<"    -ncurses    : run in ncurses mode\n";
            std::cout<<"    -noanim    : run in base static mode\n";
            std::cout<<"    -mosedriver </dev/ttySX> : use com mouse (ncurses)\n";
            return 0;
        }
        if(strcmp(argv[i],"-mousedriver")==0){
            mousedriver=true;
            tty_device = QString(argv[i+1]);
            i++;
            continue;
        }
        if(strcmp(argv[i],"-ncurses")==0){
            ncurses=true;
            continue;
        }
        if(strcmp(argv[i],"-noanim")==0){
            noanim=true;
            continue;
        }
        if(strcmp(argv[i],"-cursor")==0){
            global::cursor=true;
            continue;
        }
    }

    if(noanim){
        global::anim_state=true;
    }

    if(!ncurses){
        QApplication a(argc, argv);
        in w;
        w.show();
        return a.exec();
    }



    NcursesMode app;
    return app.run(mousedriver,tty_device);
}
