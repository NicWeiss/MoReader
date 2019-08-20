#ifndef MARCH_H
#define MARCH_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPixmap>

namespace Ui {
class March;
}

class March : public QWidget
{
    Q_OBJECT

public:
    explicit March(QWidget *parent = 0);
    ~March();
    QTimer *timer;
    QTimer *time_to_go;


private:
    Ui::March *ui;
    QImage canvas,scale;
    QImage may_bg;
    QPixmap img;

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent ( QKeyEvent * event );
    void mousePressEvent( QMouseEvent * event );

public slots:
//Слот, который срабатывает при переполнении таймера
void timer_overflow();
void go_back();
};

#endif // MARCH_H
