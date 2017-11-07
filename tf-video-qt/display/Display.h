#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "IDisplay.h"


class Display : public QGraphicsView, public IDisplay
{
    Q_OBJECT
private:
    QPixmap pixmap;
    //QGraphicsView* view;
    QGraphicsScene* scene;

private:
    static Display* uniqueInstance;
    Display(QWidget *parent = 0);
    Display(const Display& obj);
    Display& operator = (const Display& obj);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

public:
    static Display* getInstance();
    ~Display();

    virtual void displayImage(const char* buf, int w, int h);
    virtual void clearDisplay();

public slots:
    void onAddQGraphicsItem(QGraphicsItem* item, int x, int y);
    void onClearQGraphicsItem();

};

#endif // DISPLAY_H
