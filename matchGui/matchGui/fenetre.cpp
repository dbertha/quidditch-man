#include "fenetre.hpp"

fenetre::fenetre() : QWidget()//on dit qu'on appelle le constructeur de QWidget (on peut lui passer des paramettre si besoin)
{
	//setFixedSize(500, 500);

	texte = new QLabel("Zone txte \n blabla",this); //vas servir a afficher resultat

    //création de la scene, zone principal où seront afficher les cases haxagonal
    scene = new QGraphicsScene;
    //def la taille de la scene
    scene->setSceneRect(-200,-200,400,400); //(point x depart, point y depart, largeur,hauteur)
    //ajout d'un point reperer au centre
    //scene->addRect(QRect(-2, -2, 2, 2));

    //création de la view qui vas contenir la scene
    view = new QGraphicsView(scene);


/*    //test ajout hexagone personnalisé unique
    hexa = new hexagone(0,0,Qt::red);
    scene->addItem(hexa);*/

	ListeHexa[0][0] =new hexagone(0,0,-1);
    scene->addItem(ListeHexa[0][0]);
    QObject::connect(ListeHexa[0][0], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[0][1] = new hexagone(0,1,0);
    scene->addItem(ListeHexa[0][1]);
    QObject::connect(ListeHexa[0][1], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[1][0] = new hexagone(1,0,1);
    scene->addItem(ListeHexa[1][0]);
    QObject::connect(ListeHexa[1][0], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[1][1] = new hexagone(1,1,2);
    scene->addItem(ListeHexa[1][1]);
    QObject::connect(ListeHexa[1][1], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[0][2] = new hexagone(0,2,3);
    scene->addItem(ListeHexa[0][2]);
    QObject::connect(ListeHexa[0][2], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[1][2] = new hexagone(1,2,4);
    scene->addItem(ListeHexa[1][2]);
    QObject::connect(ListeHexa[1][2], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[2][2] = new hexagone(2,2,5);
    scene->addItem(ListeHexa[2][2]);
    QObject::connect(ListeHexa[2][2], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[2][0] = new hexagone(2,0,6);
    scene->addItem(ListeHexa[2][0]);
    QObject::connect(ListeHexa[2][0], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

	ListeHexa[2][1] = new hexagone(2,1,7);
    scene->addItem(ListeHexa[2][1]);
	scene->addRect(40,10,20,40);//test pour voir zone defini de boundingRect
    QObject::connect(ListeHexa[2][1], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));


}

fenetre::fenetre(HexagonalField field,std::vector <PlayingPlayer> vPlaying,std::vector <Ball> vBall) : QWidget(),
	__field(field),_listeJoueur(vPlaying),_listeBall(vBall)
{
	//init a rajouter dans le constructeur avec juste int idMaTeam
	iHaveASelection=false;
	numMaTeam=1;

	setFixedSize(700, 700);

	//*****************************************************************************************
	texte = new QLabel("Zone texte",this); //vas servir a afficher resultat

	infoJoueur = new QLabel("Info sur le joueur selection \n nom, prenom, vitesse, force,....",this);
	infoJoueur->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	//*****************************************************************************************
	//création de la scene, zone principal où seront afficher les cases haxagonal
	scene = new QGraphicsScene;
	//def la taille de la scene
	scene->setSceneRect(-200,-200,400,400); //(point x depart, point y depart, largeur,hauteur)
	//ajout d'un point reperer au centre
	//scene->addRect(QRect(-2, -2, 2, 2));

	//création de la view qui vas contenir la scene
	view = new QGraphicsView(scene);

	//*****************************************************************************************
	//création box de Radio bouton pour les choix action d'un joueur
	groupbox = new QGroupBox("Action possible:");

	deplacer = new QRadioButton("Deplacement");
	lancer = new QRadioButton("Lancer souaffle");
	taper = new QRadioButton("Frapper le cognard");

	deplacer->setChecked(true);
	deplacer->setEnabled(false);
	lancer->setEnabled(false);
	taper->setEnabled(false);
//	taper->setVisible(false);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(deplacer);
	vbox->addWidget(lancer);
	vbox->addWidget(taper);

	groupbox->setLayout(vbox);

	//*****************************************************************************************

	//initialisation d'un layout pour organniser le Qlabel,la view, radio bouton,....
	layout = new QGridLayout;
	layout->addWidget(texte);
	layout->addWidget(view, 1, 0,1,2);//peut occuper 1 ligne et 2 colonne
	layout->addWidget(infoJoueur,2,0);
	layout->addWidget(groupbox,2,1);

	this->setLayout(layout);

	initFieldGuiWithHexagonalField();
}

fenetre::fenetre(int idMaTean,std::vector <PlayingPlayer> vPlaying,std::vector <Ball> vBall) : QWidget(),
	__field(),_listeJoueur(vPlaying),_listeBall(vBall)
{
	setFixedSize(700, 700);

	texte = new QLabel("Zone texte",this); //vas servir a afficher resultat

	//création de la scene, zone principal où seront afficher les cases haxagonal
	scene = new QGraphicsScene;
	//def la taille de la scene
	scene->setSceneRect(-200,-200,400,400); //(point x depart, point y depart, largeur,hauteur)
	//ajout d'un point reperer au centre
	//scene->addRect(QRect(-2, -2, 2, 2));

	//création de la view qui vas contenir la scene
	view = new QGraphicsView(scene);

	//initialisation d'un layout pour organniser le Qlabel et la view
	layout = new QGridLayout;
	layout->addWidget(texte);
	layout->addWidget(view, 1, 0);
	this->setLayout(layout);

	initFieldGuiWithHexagonalField();
}

fenetre::fenetre(int numTeam) : QWidget(),
	__field(),_listeJoueur(),_listeBall()
{
	setFixedSize(700, 700);

	texte = new QLabel("Zone texte",this); //vas servir a afficher resultat

	//création de la scene, zone principal où seront afficher les cases haxagonal
	scene = new QGraphicsScene;
	//def la taille de la scene
	scene->setSceneRect(-200,-200,400,400); //(point x depart, point y depart, largeur,hauteur)
	//ajout d'un point reperer au centre
	//scene->addRect(QRect(-2, -2, 2, 2));

	//création de la view qui vas contenir la scene
	view = new QGraphicsView(scene);

	//initialisation d'un layout pour organniser le Qlabel et la view
	layout = new QGridLayout;
	layout->addWidget(texte);
	layout->addWidget(view, 1, 0);
	this->setLayout(layout);

	initListeHexa();
}

void fenetre::initListeHexa(){
	//initialise un terrain vide avec juste des goal
	int idOccupant;
	int indexRow;
	int indexCol;
	//crée les hexagone selon ce qu'il doivent contenir et les connectes a la fenetre (this)
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){
			idOccupant=__field.getOccupant(AxialCoordinates(indexRowAxial,indexColAxial));
			indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
			indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
			ListeHexa[indexRow][indexCol] =new hexagone(indexRowAxial,indexColAxial,idOccupant);
			scene->addItem(ListeHexa[indexRow][indexCol]);
			QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int))
					, this, SLOT(changerTexte(int,int)));
		}
	}

	//defini les case qui sont des goal
	//bizarre de devoir inverser DIAG et LINE
	indexRow=AxialCoordinates(GOAL_SIDE1_DIAG,GOAL_SIDE1_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL_SIDE1_DIAG,GOAL_SIDE1_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();

	indexRow=AxialCoordinates(GOAL_SIDE2_DIAG,GOAL_SIDE2_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL_SIDE2_DIAG,GOAL_SIDE2_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
}

void fenetre::updateListeHexa(std::vector<AxialCoordinates> allPositions){
	//mets a jout les case hexagones selon un vecteur allPositions
	// allPosition[0]  contient les coord axial du joueur 0 de l'equipe 1 (TEAM1_KEEPER)
	// les autre allPosition[x] vont dans le meme ordre qu'il sont decrit dans le fichier definie.hpp
//	AxialCoordinates coord;
	int indexRow;
	int indexCol;

	//i indique aussi la type de joueur ou balle (voir definie.hpp->TEAM1_KEEPER,...)
	for(unsigned int i = 0; i < allPositions.size(); ++i){
//		coord = allPositions[i];
		indexRow=allPositions[i].getLineOnMatrix();
		indexCol=allPositions[i].getColOnMatrix();
		ListeHexa[indexRow][indexCol]->setType(i);
	}
	update();
}

void fenetre::initFieldGuiWithHexagonalField(){
	//vas pourcourir tout le HexagonalField _fieldMatrice pour crée des hexagone correspondant
	// on part du principe que le point (0,0) de hexagonalFiel _fieldMatrice est au centre de la matrice
	// on parcours donc de -tailleMax/2 a tailleMax/2 +1 sur les lignes et colonnes
	// on se sert de AxialCoordinates() pour traduire ces coord dans un system "classique"
//	qDebug()<< "affiche test for axialCoord";
	int idOccupant;
	int indexRow;
	int indexCol;
	//crée les hexagone selon ce qu'il doivent contenir et les connectes a la fenetre (this)
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){
//			qDebug()<<"--- row puis Col puis val a cette posi";
//			qDebug()<< indexRowAxial;
//			qDebug()<< indexColAxial;
//			qDebug()<< _fieldMatrice.getOccupant(AxialCoordinates(indexRowAxial,indexColAxial));
//			qDebug()<< AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
//			qDebug()<< AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
			idOccupant=__field.getOccupant(AxialCoordinates(indexRowAxial,indexColAxial));
//			if(idOccupant != NOT_ON_HEX_GRID){
				indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
				indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
				ListeHexa[indexRow][indexCol] =new hexagone(indexRowAxial,indexColAxial,idOccupant);
				scene->addItem(ListeHexa[indexRow][indexCol]);
				QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int))
						, this, SLOT(changerTexte(int,int)));
				QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int))
						, this, SLOT(handlerMove(int,int)));

//			}
		//QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
		}
	}

	//defini les case qui sont des goal
	//bizarre de devoir inverser DIAG et LINE
	indexRow=AxialCoordinates(GOAL_SIDE1_DIAG,GOAL_SIDE1_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL_SIDE1_DIAG,GOAL_SIDE1_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();

	indexRow=AxialCoordinates(GOAL_SIDE2_DIAG,GOAL_SIDE2_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL_SIDE2_DIAG,GOAL_SIDE2_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
}

void fenetre::initHexagonalFieldWithDefine(){
	//vas crée un hexagonalField qui pourra etre utiliser par initFieldGuiWithHexagonalField()


	//COPIER-COLLER de match.cpp -> constructeur match(...)
	//les positions des joueurs sont defini a partir des position stocker dans DEFINE.hpp

	ManagedPlayer temp;
	std::vector<ManagedPlayer> team1, team2;
	temp.setCapacity(0,5);
	temp.setCapacity(1,5);
	temp.setCapacity(2,5);
	temp.setCapacity(3,5);
	temp.setCapacity(4,5);
	temp.setFirstName("Power");
	temp.setLastName("Prout");
	team1.push_back(temp); //TEAM1_KEEPER
	team1.push_back(temp); //TEAM1_SEEKER
	team1.push_back(temp); //TEAM1_CHASER1
	team1.push_back(temp);//TEAM1_CHASER2
	team1.push_back(temp); //TEAM1_CHASER3
	team1.push_back(temp); //TEAM1_BEATER1
	team1.push_back(temp); //TEAM1_BEATER2

	team2.push_back(temp); //TEAM2_KEEPER
	team2.push_back(temp); //TEAM2_SEEKER
	team2.push_back(temp); //TEAM2_CHASER1
	team2.push_back(temp);//TEAM2_CHASER2
	team2.push_back(temp); //TEAM2_CHASER3
	team2.push_back(temp); //TEAM2_BEATER1
	team2.push_back(temp); //TEAM2_BEATER2	*/

	for(int i = 0; i < 7; ++i){
		int role = i % TEAM2_KEEPER; //les 2 premiers roles correspondent déjà
		if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
		else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
		AxialCoordinates position;
		switch(i){
			case TEAM1_KEEPER :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_KEEPER, STARTINGLINE_TEAM1_KEEPER);
				break;
			case TEAM1_SEEKER :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_SEEKER, STARTINGLINE_TEAM1_SEEKER);
				break;
			case TEAM1_CHASER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER1, STARTINGLINE_TEAM1_CHASER1);
				break;
			case TEAM1_CHASER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER2, STARTINGLINE_TEAM1_CHASER2);
				break;
			case TEAM1_CHASER3 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER3, STARTINGLINE_TEAM1_CHASER3);
				break;
			case TEAM1_BEATER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER1, STARTINGLINE_TEAM1_BEATER1);
				break;
			case TEAM1_BEATER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER2, STARTINGLINE_TEAM1_BEATER2);
				break;
		}
		__players.push_back(PlayingPlayer(team1[i], role, position));
		__field.setOccupant(position, i);
	}

	for(int i = 0; i < 7; ++i){ //création et positionnement de l'équipe 2
		int role = i % TEAM2_KEEPER; //les 2 premiers roles correspondent déjà
		if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
		else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
		AxialCoordinates position;
		switch(i+TEAM2_KEEPER){
			case TEAM2_KEEPER :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_KEEPER, STARTINGLINE_TEAM2_KEEPER);
				break;
			case TEAM2_SEEKER :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_SEEKER, STARTINGLINE_TEAM2_SEEKER);
				break;
			case TEAM2_CHASER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER1, STARTINGLINE_TEAM2_CHASER1);
				break;
			case TEAM2_CHASER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER2, STARTINGLINE_TEAM2_CHASER2);
				break;
			case TEAM2_CHASER3 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER3, STARTINGLINE_TEAM2_CHASER3);
				break;
			case TEAM2_BEATER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER1, STARTINGLINE_TEAM2_BEATER1);
				break;
			case TEAM2_BEATER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER2, STARTINGLINE_TEAM2_BEATER2);
				break;
		}
		__players.push_back(PlayingPlayer(team2[i], role, position));
		__field.setOccupant(position, i+TEAM2_KEEPER);
	}
	//les 4 balles :
	__balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH), GOLDENSNITCH);
	__balls.push_back(Ball(BLUDGER1, AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1), BLUDGER1);
	__balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2), BLUDGER2);
	__balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE), QUAFFLE);

	initFieldGuiWithHexagonalField();
}

void fenetre::changerTexte(int i,int j){
	int indexRow=AxialCoordinates(i,j).getLineOnMatrix();
	int indexCol=AxialCoordinates(i,j).getColOnMatrix();

	texte->setText("coord Axial: i= "+QString::number(i)+", j ="+QString::number(j) +"\n"
				   +"coord standard: i="+QString::number(indexRow)+"j="+QString::number(indexCol));
//	marquerCaseAccessibleDepuis(i,j,2);
//	if(i==0 && j==0){
//		qDebug()<<"rentre dans if acces";
//		marquerToutesCaseNonAccessible();
//	}
	update();

}

void fenetre::marquerCaseAccessibleDepuis(int rowAxial, int colAxial, int maxDistance){
	//crée le point de depart, celui par rapport au quel ont vas estimer la distance
	AxialCoordinates pointDepart(rowAxial,colAxial);

	int indexRow;
	int indexCol;
	qDebug()<<"******";
	//on vas comparer la distance entre le pointDepart et toutes les occases et marquer celle qui sont accesible
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){
			if( pointDepart.getDistanceTo(AxialCoordinates(indexRowAxial,indexColAxial)) <= maxDistance){
				//marquage
				indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
				indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
				ListeHexa[indexRow][indexCol]->isAccessible();

				qDebug()<<"---";
				qDebug()<<indexRowAxial;
				qDebug()<<indexColAxial;
			}
		}
	}
}

void fenetre::marquerToutesCaseNonAccessible(){
	//vas marquer toutes les casses non Accesible
	//NOTE: pas optimal, faut s'arranger pour sauvergarder une liste de case qui ont
	//  été marquer accesible pour les dermarquer facilement

//	int indexRow;
//	int indexCol;
//	qDebug()<<"rentre dans non accessible1";

	for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
		for(int indexCol = 0; indexCol < MATRIX_SIZE; ++indexCol){
//			qDebug()<<"---";
//			qDebug()<<indexRowAxial;
//			qDebug()<<indexColAxial;

//			indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
//			indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
//			qDebug()<<indexRow;
//			qDebug()<<indexCol;

			ListeHexa[indexRow][indexCol]->isNonAccessible();

		}
	}
}


void fenetre::handlerMove(int iAxial,int jAxial){
	int indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
	int indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();

	if(iHaveASelection){//object deja selectionné
		//verif si peut deplacer, faire action, ...
	}else{//aucune selection
		//verifier si je peux selectionner l'objet
		if( ListeHexa[indexRow][indexCol]->getType() != -1){//si pas une case vide (j'ai un joueur)

			if( ((ListeHexa[indexRow][indexCol]->getType() / 7) +1) == numMaTeam){//je verif qu'il est a mon equipe
				iHaveASelection = true;
				joueurSelect = ListeHexa[indexRow][indexCol];
				joueurSelect->select();
				qDebug() << joueurSelect->getType();
				//recup info sur le joueur et afficher
//#############
/*				//communication réseaux pour avoir info
				selectPlayer(selectedPlayerID);
				attributs = receiveSelectedPlayerInfos(); */
//#############

				infoJoueur->setText("Atribut du joueur:\n Vitesse  : "
									+ QString::number( _listeJoueur[joueurSelect->getType()].getCapacity(SPEED)) +
				"  Force:"+ QString::number( _listeJoueur[joueurSelect->getType()].getCapacity(STRENGTH)) +
				"\n Précision:"+ QString::number( _listeJoueur[joueurSelect->getType()].getCapacity(PRECISION)) +
				"  Reflexe:"+QString::number( _listeJoueur[joueurSelect->getType()].getCapacity(REFLEX)) +
				"\n Resistance:"+QString::number( _listeJoueur[joueurSelect->getType()].getCapacity(RESISTANCE)) );

				//debloquer action selon type de joueur
				deplacer->setChecked(true);
				deplacer->setEnabled(true);
				marquerCaseAccessibleDepuis(iAxial,jAxial,_listeJoueur[joueurSelect->getType()].getCapacity(SPEED));


			}
		}

	}
}
/*
void fenetre::choixAction(){
	QWidget fenetre;
	QGroupBox *groupbox = new QGroupBox("Action possible:", &fenetre);

	QRadioButton *deplacer = new QRadioButton("Deplacement");
	QRadioButton *lancer = new QRadioButton("Lancer balle");
	QRadioButton *taper = new QRadioButton("Taper balle");

	deplacer->setChecked(true);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(deplacer);
	vbox->addWidget(lancer);
	vbox->addWidget(taper);

	groupbox->setLayout(vbox);
	groupbox->move(5, 5);

	fenetre.show();
}*/
