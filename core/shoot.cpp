#include "shoot.h"

Shoot::Shoot(Tank *t, Network *n, QPoint *aim, QObject *parent) : QObject(parent)
{
    t_cool = new QTimer();
    t_main = new QTimer();
    isCool = false;
    click = false;
    mpos = aim;
    this->t = t;
    this->network = n;
    w = new Weapon(150,1000);
    connect(t_cool,SIGNAL(timeout()),this,SLOT(on_tcool()));
    connect(t_main,SIGNAL(timeout()),this,SLOT(on_tmain()));
    t_main->start(50);
}

void Shoot::on_tcool()
{
    isCool = false;
    t_cool->stop();
}

void Shoot::on_tmain()
{
    if(click&!isCool) {
        if(t->getID()==4) return;
        if(!t_cool->isActive()) {
            isCool = true;
            t_cool->start(t->getReload());
        }
        int v;
        if(t->getMoved()) {
            v = t->getBvel()+t->getSpeed();
        } else {
            v = t->getBvel();
        }
        int dmg = t->getDamage();
        network->send("|1#"+t->getName()+"#"+QString::number(mpos->x(),'f',0)+
                      "#"+QString::number(mpos->y(),'f',0)+"#1#~");
        Bullet *b = new Bullet(t->getShootPoint().x(),t->getShootPoint().y(),
                               (double)mpos->x(),(double)mpos->y(),v,t->getName());
        b->setDmg(dmg);
        bool enabled = true;
        if(!t->getRect().intersects(b->get())) {
            enabled = false;
        }
        b->setEnabled(enabled);
        emit newBullet(b);
    }
    if(!t->isSpawned()) {
        click = false;
    }
}

void Shoot::reset()
{
    click = false;
    t_cool->stop();
}

void Shoot::MousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton) {
        click = true;
    }
}

void Shoot::MouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton) {
        click = false;
    }
}
