#ifndef HEXAGONE_HPP
#define HEXAGONE_HPP

#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPainter>
#include <QColor>
#include <QCursor>
#include <QtCore/qmath.h> //pour qFabs
#include <QBrush>

#include <QDebug> //permet de dispose d'un affichage dans console debug

#include "Defines.hpp"

#define pHaut baseCoord[0]
#define pHautDroite baseCoord[1]
#define pBasDroite baseCoord[2]
#define pBas baseCoord[3]
#define pBasGauche baseCoord[4]
#define pHautGauche baseCoord[5]

//const static car commune a toutes les HexagonalCase
//attention: l'axe y est inversé (augmente en allant vers la bas)

//construit un HexagonalCase grace a 6 point, l'HexagonalCase peut etre vu comme un triangle (partie superieur) + un rectangle + triangle retourné
static const QPointF baseCoord[6] =
	{QPointF(  0.0, -12.0), //0. pointe haute
	 QPointF( 10.0, -6.0), //1. haut-droite
	 QPointF( 10.0,  6.0), //2. bas-droite
	 QPointF(  0.0,  12.0), //3. pointe bas
	 QPointF(-10.0,  6.0), //4. bas-gauche
	 QPointF(-10.0, -6.0)};//5. haut-gauche

static const qreal hauteur = pBas.y() - pHaut.y() ;
static const qreal largeur = pHautDroite.x() - pHautGauche.x() ;
//hauteurTiers: utilise pour le decalage des HexagonalCases par rapport a l'axe vertical
//  on prend pas en conte la hauteur d'un des "triangle"
static const qreal hauteurTiers = pBas.y() - pHautDroite.y() ;
//hauteurCentral: hauteur du rectangle central inscrit dans l'HexagonalCase
static const qreal hauteurCentral = pBasDroite.y() - pHautDroite.y() ;

//permet de quantifier le decalage subit par un haxagone le long de l'axe horizontal
static const QPointF pasIndiceI(largeur,0);
//permet de quantifier le decalage subit par un haxagone le long de l'axe diagonal/vertical
static const QPointF pasIndiceJ(largeur/2,hauteurTiers);

//defini les couleurs utilise pour le coloriage selon le type de case (vide,joueur,but,balle)
static const QColor _couleurFondSelect=Qt::cyan;
static const QColor _couleurFondNoSelect=Qt::white;//Qt::NoBrush si on met image de fond
static const QColor _couleurFondGoal=Qt::darkCyan;

static const QColor _couleurEquipe1=Qt::blue;
static const QColor _couleurEquipe2=Qt::black;

static const QColor _couleurQuaffle=QColor (222,184,135);//burlywood
static const QColor _couleurGoldenSnitch=Qt::yellow;
static const QColor _couleurBludger=Qt::darkRed;


class HexagonalCase : public QGraphicsObject
{
    Q_OBJECT //vas permettre de def ces propres slot et signaux

    public:
        HexagonalCase(int i = 0, int j = 0,int type=FREE_SPACE, QGraphicsItem *parent = 0);

        //paint => comment dessiner l'objet
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

		void setType(int);
		int getType();

		int getIAxial();
		int getJAxial();

		int getHauteur();
		int getLargeur();

		int getTypeMarkBall();

		void rajouterBalle(int);

        void select();
		void selectForAction();
        void unselect();
		void bloquerLeJoueur();

		void isAGoal();
		void isAccessible();

		void isLine();
		void isDiagonalGoBasDroite();
		void isDiagonalGohautDroite();
		void isForCatch();

		void isMarkForBludger(int);
		void isMarkForQuaffle();
		void isMarkForGoldenSnitch();

		void isNonAccessible();

		bool ifMark();
		bool ifBlocked();


		//boudingRect + shape => defnie la region frontiere de l'objet
		QRectF boundingRect() const;//def un rectangle qui entoure l'hexagone
		QPainterPath shape() const;//restreint la region exactement a l'hexagone
		//note fonctionnement QGraphicsItem (QGraphicsObject en herite):
		//  on est obliger de d'abord definir un domaine rectangulaire qui englobe complemetenent l'Item
		//  et apres on, on peut limiter ce domaine aux dimentione de l'objet grace a shape. Seulement les
		//  clicke dans le domaine "shape" seront prit en compte


    public slots://reaction a un signal
        void changerCouleur();

    signals:
		void caseSelect(int,int);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);

    private:
        int _typeCase;//indique le type de case (vide, contient un joueur,balle,...)
        int _indiceI;
        int _indiceJ;

		int _markTypeBalle;// BLUDGER1 15, BLUDGER2 16, QUAFFLE 17

		bool _ifGoal;

		bool _blocked;
        bool _ifSelect;
		bool _ifSelectForAction;

		bool _ifAccesible;
		bool _ifLine;
		bool _ifDiagonalBasDroite; //diagonal qui vas d'en haut a gauche vers le bas a droite = suis la vertical "Axial"
		bool _ifDiagonalhautDroite; //diagonal qui vas d'en bas a gauche vers le haut a droite = vas du 3ieme cadran trigonometrique vers 1ers cadran
		bool _ifForCatch;

		bool _ifMarkForBludger;
		bool _ifMarkForQuaffle;
		bool _ifMarkForGoldenSnitch;

		int _contientBalleEnPlus;

        void dessinerHexagone(QPainter *painter);//dessine le coutour de la case
        void dessinerType(QPainter *painter);//dessine le type de la case (balle,joueur,...)

		void dessinerCaseVide(QPainter *painter);
		void dessinerKeeper(QPainter *painter,QColor colorEquipe );
        void dessinerSeeker(QPainter *painter,QColor colorEquipe );
        void dessinerChaser(QPainter *painter,QColor colorEquipe );
		void dessinerBeater(QPainter *painter,QColor colorEquipe );
		void dessinerQuaffle(QPainter *painter);
		void dessinerGoldenSnitch(QPainter *painter);
		void dessinerBludger(QPainter *painter);

        QPolygonF caseBuilt() const;//HexagonalCase pre-fabriquer
        QRectF rectangleInterne() const;
        QPointF centreHexagon() const;


};


#endif // HEXAGONE_HPP
