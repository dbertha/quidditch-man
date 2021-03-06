#ifndef FENETRE_HPP
#define FENETRE_HPP

#include <QApplication>
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QGroupBox>
#include <QRadioButton>
#include <QSocketNotifier>
#include <QMessageBox>
#include <QList>

#include <QDebug> //permet de dispose d'un affichage dans console debug

#include "HexagonalCase.hpp"

#include "../common/Coordinates.hpp" //permet d'avoir MATRIX_SIZE et systeme de coord
#include "../common/HexagonalField.hpp"

#include "Client.hpp" //network

#include "MainWindow.hpp"
class MainWindow;
#define texteMaTeam "score de mon equipe : "
#define texteAutreTeam "score de l'equipe adverse : "


class MatchWindow : public QDialog
{
	Q_OBJECT //vas permettre de def ces propres slot (public slots:) et signaux (signals:)

    public:
		MatchWindow(Client * client, int idTeam, MainWindow * parent = 0);

    public slots:
		void handlerMove(int,int);
		void handlerChoixAction(bool);
		void handlerAction();
		void handlerTour();
		void pushesHandler();
		void nextTurn();


    signals:

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
		QLabel *attenteJoueur;
		QLabel *scoreEquipe1;
		QLabel *scoreEquipe2;
		QLabel *texteEquipe1;
		QLabel *texteEquipe2;

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

		QMessageBox* _msgBox;

		HexagonalCase *caseJoueurSelect;
		HexagonalCase *caseSelect;
		HexagonalCase *temp;
        HexagonalCase *ListeHexa[MATRIX_SIZE][MATRIX_SIZE];

		QList<HexagonalCase*> ListeHexaMarquer;

		HexagonalField __field;

		void initListeHexa();
		void resetListeHexa();
		void updateListeHexa();

		void demarquerCase();

		void marquerCaseAccessibleDepuis(int iAxial,int jAxial,int maxDistance);

		void marquerUneDirection(int iAxialDepart,int jAxialDepart,int maxDistance,int idBudlger,int direction);
		void marquerBalle(int iAxial,int jAxial,int maxDistance, int idBalle);
		void marquerAttraperSouaffle(int iAxial,int jAxial);
		void marquerAttraperVifDOr(int iAxial,int jAxial);

		bool ifNotOut(int iAxial, int jAxial);

		void reset();

		void endHandler();

		Client * __client;
		MainWindow* _parent;
		QSocketNotifier * __forfeitAndDrawNotifier;
};

#endif // FENETRE_HPP
