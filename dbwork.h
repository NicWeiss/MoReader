#ifndef DBWORK_H
#define DBWORK_H

#include <qstring.h>

class DBWork
{
public:
    static const int DB_CONNECT_ERROR = 1;
    static const int USER_NOT_FOUND = 2;
    static const int WRONG_DIRECTION = 3;
    static QString do_config();
    static int try_to_register_pass(QString mocard, QString &u_name, QString &surname, int direction, QString &error);
};

#endif // DBWORK_H
