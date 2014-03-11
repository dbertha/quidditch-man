#ifndef FENETRE_HPP
#define FENETRE_HPP

#include <QApplication>
#include <QWidget>
//#include <QPushButton>
//#include <QLCDNumber>
//#include <QSlider>
//#include <QPainter>

#include <QLabel>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>

#include <QDebug> //permet de dispose d'un affichage dans console debug

#include "hexagone.hpp"
#include "Coordinates.hpp" //permet d'avoir MATRIX_SIZE et systeme de coord

class fenetre : public QWidget // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT //vas permettre de def ces propres slot et signaux

    public:
        fenetre();//constructeur de la fenetre

    public slots:
        void changerTexte(int,int);//peut etre fait parceque j'ai mis Q_OBJECT

    signals:
//    void agrandissementMax();
//    void agrandissementMaxV2(int);//test perso pour renvoyé des valeur

    private:
        QLabel *texte;
        QGridLayout *layout;
        QGraphicsScene *scene;
        QGraphicsView *view;
        hexagone *hexa;
        hexagone *ListeHexa[MATRIX_SIZE][MATRIX_SIZE];
//        AxialCoordinates coord;
//    QPushButton *m_bouton;
//    QLCDNumber *m_lcd;
//    QLCDNumber *m_lcdTest;
//    QSlider *m_slider;
//    void paintEvent(QPaintEvent *);
};

#endif // FENETRE_HPP
