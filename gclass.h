#ifndef GCLASS_H
#define GCLASS_H
#include <QPainter>
#include <QImage>

    static  QString  dbhost;
    static  QString dbname;
    static  QString dbuser;
    static  QString dbpass;
    static  bool exit_timer;
    static int sleep_time,frame_in, frame_out;
    static bool in_out;
    static QString pattern[200];
    static QString red_out_arrow, green_in_arrow;
    static int pattern_limit;


    static float march_i;
    static QPainter p;
    static QImage gear;

    class global{
    public:
        static QString gender;
        static  QString name;
        static  bool anim_state;
        static  bool cursor;

    };

#endif // GCLASS_H
