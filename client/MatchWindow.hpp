#ifndef FENETRE_HPP
#define FENETRE_HPP

//note: code pas du tout optimise/"modualiser"/"umliser", grosse phase de refactoring necessaire

#include <QApplication>
#include <QWidget>
#include <QPushButton>
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
#include <QSocketNotifier>
#include <QMessageBox>

#include <QDebug> //permet de dispose d'un affichage dans console debug

#include "HexagonalCase.hpp"

#include "../common/Coordinates.hpp" //permet d'avoir MATRIX_SIZE et systeme de coord
#include "../common/HexagonalField.hpp"

#include "Client.hpp" //network


class MatchWindow : public QWidget // On hérite de QWidget //TODO : changer pour QDialog
{
	Q_OBJECT //vas permettre de def ces propres slot (public slots:) et signaux (signals:)

    public:
		
		//MatchWindow(int idMaTeam);
		//final :
		MatchWindow(Client * client, int idTeam, QWidget * parent = 0);

    public slots:
		void changerTexte(int,int);
		void handlerMove(int,int);
		void handlerChoixAction(bool);
		void handlerAction();
		void handlerTour();
		void pushesHandler();

    signals:
		void checkTour();
//    void agrandissementMaxV2(int);//test perso pour renvoyé des valeur

    private:
		int numMaTeam;
		bool iHaveASelection;

		int scoreTeam1;
		int scoreTeam2;
		int winner;
		int moves[7][4];
		int currentMove;
		std::vector<AxialCoordinates> allPositions;
		playerAttr attributs;

		int playerRole;

		QLabel *infoJoueur;
		QLabel *scoreEquipe;
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

		QGridLayout *layoutConformi;
		QLabel *textConfirm;
		QPushButton *BoutonConfirm;
		QPushButton *BoutonFinirTour;

		QGridLayout *layoutChoixMatch;
		QPushButton *BoutonAbandon;
		QPushButton *BoutonMatchNul;


		HexagonalCase *caseJoueurSelect;
		HexagonalCase *caseSelect;
        HexagonalCase *ListeHexa[MATRIX_SIZE][MATRIX_SIZE];

		HexagonalField __field;



		void initFieldGuiWithHexagonalField();
		void initHexagonalFieldWithDefine();

		void initListeHexa();
		void resetListeHexa();
		void updateListeHexa();

		void demarquerToutesCase();

		void marquerCaseAccessibleDepuis(int iAxial,int jAxial,int maxDistance);

		void marquerFrappe(int iAxial,int jAxial,int maxDistance, int idBludger);
		void marquerLancer(int iAxial,int jAxial,int maxDistance);

		void marquerAttraperSouaffle(int iAxial,int jAxial);
		void marquerAttraperVifDOr(int iAxial,int jAxial);

		bool ifNotOut(int iAxial, int jAxial);

		void endHandler();
		void nextTurn();
		Client * __client;
		QSocketNotifier * __forfeitAndDrawNotifier;
		

//        AxialCoordinates coord;
//    QPushButton *m_bouton;
//    QLCDNumber *m_lcd;
//    QLCDNumber *m_lcdTest;
//    QSlider *m_slider;
//    void paintEvent(QPaintEvent *);
};

#endif // FENETRE_HPP
