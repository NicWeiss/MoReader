#ifndef NCURSESMODE_H
#define NCURSESMODE_H

#include <QString>
#include <QThread>

class NcursesMode
{
public:
    const int GREEN_COLOR_PAIR=2;
    const int RED_COLOR_PAIR=3;
    const int RED_TEXT_PAIR=4;
    int run(bool mousemode, QString mousedev);



    static int mouse_x;
    static int mouse_y;
    static QString mouse_dev;
    static QString static_debug;
    static bool mousedriver_run;

    static int mouse_thread_func();

    class MouseThread : QThread{
        void run() {
            mouse_thread_func();
        }

    public:
        void do_start(){
            start();
        }
    };


private:
    MouseThread *mouse_thread;

};



#endif // NCURSESMODE_H
