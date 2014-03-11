#ifndef HEXAGONE_HPP
#define HEXAGONE_HPP

#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPainter>
#include <QColor>
#include <QCursor>
#include <QtCore/qmath.h> //pour qFabs

#include <QDebug> //permet de dispose d'un affichage dans console debug

#include "Defines.hpp"

#define pHaut baseCoord[0]
#define pHautDroite baseCoord[1]
#define pBasDroite baseCoord[2]
#define pBas baseCoord[3]
#define pBasGauche baseCoord[4]
#define pHautGauche baseCoord[5]

//const static car commune a toutes les hexagone
//attention: l'axe y est inversé (augmente en allant vers la bas)

//construit un hexagone grace a 6 point, l'hexagone peut etre vu comme un triangle (partie superieur) + un rectangle + triangle retourné
static const QPointF baseCoord[6] =
    {QPointF(  0.0, -20.0), //0. pointe haute
     QPointF( 10.0, -10.0), //1. haut-droite
     QPointF( 10.0,  10.0), //2. bas-droite
     QPointF(  0.0,  20.0), //3. pointe bas
     QPointF(-10.0,  10.0), //4. bas-gauche
     QPointF(-10.0, -10.0)};//5. haut-gauche

static const qreal hauteur = pBas.y() - pHaut.y() ;
static const qreal largeur = pHautDroite.x() - pHautGauche.x() ;
//hauteurTiers: utilise pour le decalage des hexagones par rapport a l'axe vertical
//  on prend pas en conte la hauteur d'un des "triangle"
static const qreal hauteurTiers = pBas.y() - pHautDroite.y() ;
//hauteurCentral: hauteur du rectangle central inscrit dans l'hexagone
static const qreal hauteurCentral = pBasDroite.y() - pHautDroite.y() ;

//permet de quantifier le decalage subit par un haxagone le long de l'axe horizontal
static const QPointF pasIndiceI(largeur,0);
//permet de quantifier le decalage subit par un haxagone le long de l'axe diagonal/vertical
static const QPointF pasIndiceJ(largeur/2,hauteurTiers);

//defini les couleurs utilise pour le coloriage selon le type de case (vide,joueur,but,balle)
static const QColor _couleurFondSelect=Qt::cyan;
static const QColor _couleurFondNoSelect=Qt::white;
static const QColor _couleurEquipe1=Qt::blue;
static const QColor _couleurEquipe2=Qt::black;


class hexagone : public QGraphicsObject
{
    Q_OBJECT //vas permettre de def ces propres slot et signaux

    public:
        hexagone(int i = 0, int j = 0,int type=FREE_SPACE, QGraphicsItem *parent = 0);

        //paint => comment dessiner l'objet
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

        void select();
        void unselect();

        //boudingRect => defnie la region frontiere de l'objet
        QRectF boundingRect() const;
        QPainterPath shape() const;
//        QRegion boundingRegion( const QTransform & itemToDeviceTransform ) const;

    public slots://reaction a un signal
        void changerCouleur();

    signals:
        void hexagoneSelect(int,int);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);

    private:
        int _typeCase;//indique le type de case (vide, contient un joueur,balle,...)
        int _indiceI;
        int _indiceJ;
        bool _ifSelect;


        void dessinerHexagone(QPainter *painter);//dessine le coutour de la case
        void dessinerType(QPainter *painter);//dessine le type de la case (balle,joueur,...)
        QPolygonF hexagoneBuilt() const;
        QRectF rectangleInterne() const;
        void dessinerKeeper(QPainter *painter,QColor colorEquipe );
        void dessinerSeeker(QPainter *painter,QColor colorEquipe );
        void dessinerChaser(QPainter *painter,QColor colorEquipe );
        void dessinerBeater(QPainter *painter,QColor colorEquipe );
};


#endif // HEXAGONE_HPP
