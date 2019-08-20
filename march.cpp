#include "march.h"
#include "ui_march.h"
#include "gclass.h"
#include "in.h"

March::March(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::March)
{
    ui->setupUi(this);
    showFullScreen();   //открываем во весь экран
    march_i=10;
    timer = new QTimer;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_overflow())); //Связываем сигнал переполнения таймера со слотом
    timer->start(13);  //Задаем время срабатывания таймера (в мс)

    time_to_go = new QTimer;
    QObject::connect(time_to_go, SIGNAL(timeout()), this, SLOT(go_back())); //Связываем сигнал переполнения таймера со слотом
    time_to_go->start(30000);  //Задаем время срабатывания таймера (в мс)


    canvas = QImage(1920,1080,QImage::Format_RGB32);
    //QPainter p;
    p.begin(&canvas);

    may_bg = QImage(":may_bg.png");
    img = QPixmap(":gear.png");

    //p.drawImage(0,0,gear);

    p.end();
    //canvas = QImage(":gear.png");
}

void March::go_back(){
   this->close();
    delete this;
}

void March::mousePressEvent ( QMouseEvent * event ){
        this->close();
        delete this;
}

void March::keyPressEvent ( QKeyEvent * event ){
    this->close();
    delete this;
}

void March::timer_overflow()
{
    QMatrix rm;
    //rm.rotate(march_i);
    //QPixmap img_rot = img.transformed(rm);

    p.begin(&canvas);
    p.drawImage(0,0,may_bg);

    p.translate(2120,200);
    p.scale(2,2);
    p.rotate(march_i);
    p.drawPixmap(-447,-447,img);
    p.resetMatrix();

    //p.drawPixmap(0,600,img);

    if(!global::anim_state)march_i=march_i+0.3;
    if (march_i>360)march_i=0;



    p.save();
    QFont font( "Adlery Pro" );
    font.setPointSize( 64 );
    p.setPen(Qt::yellow);
    p.setFont( font );
    p.drawText( 900, 400, "Дорогая "+global::name);

    QFont font2( "C059 italic" );
    font2.setPointSize( 50 );
    font2.setItalic(true);
    p.setFont( font2 );
    p.drawText( 770, 530, "Поздравляем тебя с 8 марта");
    p.drawText( 770, 600, "и желаем, чтобы, не взирая ");
    p.drawText( 670, 670, "ни на какие ненастья и жизненные ");
    p.drawText( 730, 740, "невзгоды, ты всегда цвела, как ");
    p.drawText( 670, 810, "самый прекрасный в мире цветок!");

    QFont font3( "Droid" );
    font3.setPointSize( 20 );
    font3.setItalic(true);
    p.setFont( font3);
    p.setPen(Qt::gray);
    p.drawText( 400, 1050, "Для выхода коснитесь экрана или нажмите кнопку | Поздравление будет скрыто через 30 секунд");

    p.end();

    update();
}

void March::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    int width = canvas.width();
    int height = canvas.height();

    if(height==0)
        return;
    width = this->width();
    height = width*canvas.height()/canvas.width();
    if(height>this->height()){
        height = this->height();
        width = height*canvas.width()/canvas.height();
    }

    scale = canvas.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    p.setBrush(QBrush(QColor(0x3b,0x3b,0x3f,0xff),Qt::SolidPattern));
    p.drawRect(0,0,this->width(),this->height());
    p.drawImage(this->width()/2-scale.width()/2,this->height()/2-scale.height()/2,scale);
}

March::~March()
{
    delete ui;
}
