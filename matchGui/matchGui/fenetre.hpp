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
#include <QGroupBox>
#include <QRadioButton>

#include <QDebug> //permet de dispose d'un affichage dans console debug

#include "hexagone.hpp"

#include "Coordinates.hpp" //permet d'avoir MATRIX_SIZE et systeme de coord
#include "HexagonalField.hpp"
#include "PlayingPlayer.hpp"
#include "Ball.hpp"


class fenetre : public QWidget // On hérite de QWidget (IMPORTANT)
{
	Q_OBJECT //vas permettre de def ces propres slot (public slots:) et signaux (signals:)

    public:
		fenetre();//constructeur pour test
		fenetre(HexagonalField,std::vector <PlayingPlayer>,std::vector <Ball>);
		fenetre(int idMaTeam,std::vector <PlayingPlayer>,std::vector <Ball>);//constructeur de la fenetre

		fenetre(int idMaTeam);

    public slots:
		void changerTexte(int,int);
		void handlerMove(int,int);
		void handlerChoixAction(bool);

    signals:
//    void agrandissementMax();
//    void agrandissementMaxV2(int);//test perso pour renvoyé des valeur

    private:
		int numMaTeam;
		bool iHaveASelection;

		int scoreTeam1 = 0;
		int scoreTeam2 = 0;
		int winner = 0;
		std::vector<AxialCoordinates> allPositions;

		typedef struct { //pas besoin de la classe complète
			int attributes[5];
			AxialCoordinates position;
			int hasQuaffle;
		} playerAttr;
		playerAttr attributs;

		int playerRole;

		QLabel *infoJoueur;
		QLabel *texte;
        QGridLayout *layout;
        QGraphicsScene *scene;
		QGraphicsView *view;

		QGroupBox *groupbox ;
		QRadioButton *deplacer ;
		QRadioButton *lancer ;
		QRadioButton *taper ;
		QRadioButton *recupSouaffle;
		QRadioButton *recupVifDOr;

		hexagone *caseJoueurSelect;
        hexagone *ListeHexa[MATRIX_SIZE][MATRIX_SIZE];

		HexagonalField __field;
		std::vector <PlayingPlayer> _listeJoueur;
		std::vector <Ball> _listeBall;

		std::vector <PlayingPlayer> __players;
		std::vector <Ball> __balls;


		void initFieldGuiWithHexagonalField();
		void initHexagonalFieldWithDefine();

		void initListeHexa();
		void updateListeHexa();

		void marquerToutesCaseNonAccessible();

		void marquerCaseAccessibleDepuis(int iAxial,int jAxial,int maxDistance);

		void marquerFrappe(int iAxial,int jAxial,int maxDistance);
		void marquerLancer(int iAxial,int jAxial,int maxDistance);

		void marquerAttraperSouaffle(int iAxial,int jAxial);
		void marquerAttraperVifDOr(int iAxial,int jAxial);

		bool ifNotOut(int iAxial, int jAxial);
//        AxialCoordinates coord;
//    QPushButton *m_bouton;
//    QLCDNumber *m_lcd;
//    QLCDNumber *m_lcdTest;
//    QSlider *m_slider;
//    void paintEvent(QPaintEvent *);
};

#endif // FENETRE_HPP
