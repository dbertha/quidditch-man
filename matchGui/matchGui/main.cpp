#include <QApplication>
#include <QPushButton>

#include <QHBoxLayout>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QVector>
#include <QLabel>

#include "fenetre.hpp"
#include "Defines.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenetre test;
    test.show();

    return app.exec();
}
