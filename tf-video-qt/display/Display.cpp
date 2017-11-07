#include "Display.h"
#include "Controller.h"
#include "Log.h"
#include <QImage>
#include <QPainter>

Display* Display::uniqueInstance = NULL;

Display::Display(QWidget *parent)
    : QGraphicsView(parent), pixmap(get_panel_width(), get_panel_height())
{
    //this->resize(get_panel_width(), get_panel_height());

 #ifndef Q_OS_WIN32
    this->setWindowFlags(Qt::FramelessWindowHint);
 #endif

    int width = get_panel_width();
    int height = get_panel_height();

    scene = new QGraphicsScene(this);
    scene->setSceneRect(-width/2, -height/2, width, height);
    setScene(scene);
    //setCacheMode(CacheBackground);

    pixmap.fill(Qt::black);
    viewport()->update();
}

Display* Display::getInstance()
{
    if (NULL == uniqueInstance) {
        uniqueInstance = new Display();
    }

    return uniqueInstance;
}

Display::~Display()
{

}


void Display::onAddQGraphicsItem(QGraphicsItem* item, int x, int y)
{
    scene->addItem(item);
    item->setPos(scene->sceneRect().left()+x, scene->sceneRect().top()+y);
}

void Display::onClearQGraphicsItem()
{
    QList<QGraphicsItem*> listItem = scene->items();

    for (int i=0; i<listItem.size(); i++) {
        scene->removeItem(listItem[i]);
    }
    /*while (!listItem.empty())
    {
        scene->removeItem(listItem.at(0));
        listItem.removeAt(0);
    }*/
}


void Display::displayImage(const char* buf, int w, int h)
{
    if ( (0==buf) || (w==0) || (h==0) ) {
        print_error("invalid parameter");
        return;
    }

    QImage img((u8*)buf, w, h, QImage::Format_RGB32);
    pixmap = QPixmap::fromImage(img);

    //update();
    this->viewport()->update();
}

void Display::clearDisplay()
{
    pixmap.fill(Qt::black);
    viewport()->update();
}

void Display::drawBackground(QPainter *painter, const QRectF &)
{
    //QPainter painter(this);

    if (pixmap.isNull()) {
        return;
    }

    painter->drawPixmap(int(sceneRect().left()),int(sceneRect().top()), pixmap);
}
