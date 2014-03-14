#include "fenetre.hpp"

//note: code pas du tout optimise/"modualiser"/"umliser", grosse phase de refactoring necessaire




fenetre::fenetre(Client * client, int numTeam) : QWidget(),
	numMaTeam(numTeam), iHaveASelection(false),  scoreTeam1(0), scoreTeam2(0), winner(0), currentMove(0), 
	__field(),_listeJoueur(),_listeBall(), __client(client), __forfeitAndDrawNotifier(new QSocketNotifier(client->getSockfd()))
{
	//initialisation : 
	for(int i = 0; i < 7; ++i){
	  moves[i][0] = i;
	  moves[i][1] = NO_SPECIAL_ACTION;
	  moves[i][2] = 10000; //sentinelle : mouvement vide
	  moves[i][3] = 10000;
	}
	__forfeitAndDrawNotifier->setEnabled(false);
	connect(__forfeitAndDrawNotifier,SIGNAL(activated(int)),this,SLOT(pushesHandler()));

	setFixedSize(800, 800);

	//*****************************************************************************************
	texte = new QLabel("Zone texte \n Zone texte",this); //vas servir a afficher resultat

	infoJoueur = new QLabel("Info sur le joueur selection \n nom, prenom, vitesse, force,....");
	infoJoueur->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	scoreEquipe = new QLabel("\t score equipe 1 = 0 \n\t score equipe 2 = 0");
	//*****************************************************************************************
	//création de la scene, zone principal où seront afficher les cases haxagonal
	scene = new QGraphicsScene;
	//def la taille de la scene
	scene->setSceneRect(-200,-200,400,400); //(point x depart, point y depart, largeur,hauteur)


	//création de la view qui vas contenir la scene
	view = new QGraphicsView(scene);

	//*****************************************************************************************
	//création box de Radio bouton pour les choix action d'un joueur
	groupbox = new QGroupBox("Action possible:");

	deplacer = new QRadioButton("Deplacement");
	lancer = new QRadioButton("Lancer souaffle");
	taper = new QRadioButton("Frapper le cognard");
	recupSouaffle =  new QRadioButton("Tenter de récupérer le souaffle");
	recupVifDOr = new QRadioButton("Tenter d'attraper le vif d'or");


	deplacer->setChecked(true);
	deplacer->setEnabled(false);
	lancer->setEnabled(false);
	taper->setEnabled(false);
	recupSouaffle->setEnabled(false);
	recupVifDOr->setEnabled(false);
//	taper->setVisible(false);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(deplacer);
	vbox->addWidget(lancer);
	vbox->addWidget(taper);
	vbox->addWidget(recupSouaffle);
	vbox->addWidget(recupVifDOr);
	groupbox->setLayout(vbox);
//	QObject::connect(groupbox,SIGNAL(clicked(bool)),this,SLOT(handlerChoixAction(bool)));
	QObject::connect(deplacer,SIGNAL(clicked(bool)),this,SLOT(handlerChoixAction(bool)));
	QObject::connect(lancer,SIGNAL(clicked(bool)),this,SLOT(handlerChoixAction(bool)));
	QObject::connect(taper,SIGNAL(clicked(bool)),this,SLOT(handlerChoixAction(bool)));
	QObject::connect(recupSouaffle,SIGNAL(clicked(bool)),this,SLOT(handlerChoixAction(bool)));
	QObject::connect(recupVifDOr,SIGNAL(clicked(bool)),this,SLOT(handlerChoixAction(bool)));

	//*****************************************************************************************
	layoutConformi = new QGridLayout;
//	layoutConformi->setGeometry();
	layoutConformi->setSizeConstraint(QLayout::SetMinimumSize);
	BoutonConfirm = new QPushButton("OK");
	BoutonConfirm->setMaximumSize ( 80, 30);
	BoutonConfirm->setEnabled(false);
	textConfirm = new QLabel("Confirmer action:");
	layoutConformi->addWidget(textConfirm,0,0);
	layoutConformi->addWidget(BoutonConfirm,1,0);
	QObject::connect(BoutonConfirm,SIGNAL(clicked()),this,SLOT(handlerAction()));
	//*****************************************************************************************

	//initialisation d'un layout pour organniser le Qlabel,la view, radio bouton,....
	layout = new QGridLayout;
	layout->addWidget(texte,3,0);
	layout->addWidget(scoreEquipe,0,0);
	layout->addWidget(view, 1, 0,1,3);//peut occuper 1 ligne et 3 colonne
	layout->addWidget(infoJoueur,2,0);
	layout->addWidget(groupbox,2,1);
	layout->addLayout(layoutConformi,2,2);
	this->setLayout(layout);


	__client->getAllPositions();
	allPositions = __client->receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);*/

	initListeHexa();
	updateListeHexa();
}

void fenetre::initListeHexa(){
	//initialise un terrain vide avec juste des goal
	int idOccupant;
	int indexRow;
	int indexCol;
	//crée les hexagone selon ce qu'il doivent contenir et les connectes a la fenetre (this)
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){ //index de la diag
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){ //index de la ligne
			idOccupant=__field.getOccupant(AxialCoordinates(indexRowAxial,indexColAxial));
			indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
			indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
			ListeHexa[indexRow][indexCol] =new hexagone(indexRowAxial,indexColAxial,idOccupant);
			scene->addItem(ListeHexa[indexRow][indexCol]);
			QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int))
					, this, SLOT(changerTexte(int,int)));
			QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int))
					, this, SLOT(handlerMove(int,int)));
		}
	}

	//defini les case qui sont des goal
	indexRow=AxialCoordinates(GOAL_SIDE1_DIAG,GOAL_SIDE1_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL_SIDE1_DIAG,GOAL_SIDE1_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();

	indexRow=AxialCoordinates(GOAL_SIDE2_DIAG,GOAL_SIDE2_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL_SIDE2_DIAG,GOAL_SIDE2_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
}

void fenetre::resetListeHexa(){
	int indexRow;
	int indexCol;
	//crée les hexagone selon ce qu'il doivent contenir et les connectes a la fenetre (this)
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){ //index de la diag
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){ //index de la ligne
			indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
			indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
			ListeHexa[indexRow][indexCol]->setType(-1);
		}
	}
}

void fenetre::updateListeHexa(){
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
			if( pointDepart.getDistanceTo(AxialCoordinates(indexRowAxial,indexColAxial)) <= maxDistance){ //TODO : optimiser
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
	update();
}

void fenetre::demarquerToutesCase(){
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

//TODO *!!!!!!!!!!!!!!!!!!! A refactorer, marquerFrappe + marquerLancer
void fenetre::marquerFrappe(int iAxialDepart,int jAxialDepart,int maxDistance,int idBudlger){
	qDebug()<<"Debut du marquage pour frappe, i,j,max";
	qDebug()<<iAxialDepart;
	qDebug()<<jAxialDepart;
	qDebug()<<maxDistance;
	int i;
	int iAxial=iAxialDepart+1;
	int jAxial=jAxialDepart+0;
	int indexRow;
	int indexCol;

	//6 direction :
	//(1,0) droite
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test frappe droite";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForBludger(idBudlger);
		ListeHexa[indexRow][indexCol]->isLine();
		i+=1;
		iAxial = iAxial + 1;
		jAxial = jAxial + 0;
	}

	//(0,1) bas-droite
	iAxial=iAxialDepart+0;
	jAxial=jAxialDepart+1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test frappe bas-droite";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForBludger(idBudlger);
		ListeHexa[indexRow][indexCol]->isDiagonalGoBasDroite();
		i+=1;
		iAxial = iAxial + 0;
		jAxial = jAxial + 1;
	}
	//(-1,1) bas-gauche
	iAxial=iAxialDepart-1;
	jAxial=jAxialDepart+1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForBludger(idBudlger);
		ListeHexa[indexRow][indexCol]->isDiagonalGohautDroite();
		i+=1;
		iAxial = iAxial - 1;
		jAxial = jAxial + 1;
	}

	//(-1,0) gauche
	iAxial=iAxialDepart-1;
	jAxial=jAxialDepart+0;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForBludger(idBudlger);
		ListeHexa[indexRow][indexCol]->isLine();
		i+=1;
		iAxial = iAxial - 1;
		jAxial = jAxial + 0;
	}
	//(0,-1) haut-gauche
	iAxial=iAxialDepart+0;
	jAxial=jAxialDepart-1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForBludger(idBudlger);
		ListeHexa[indexRow][indexCol]->isDiagonalGoBasDroite();
		i+=1;
		iAxial = iAxial + 0;
		jAxial = jAxial - 1;
	}
	//(1,-1) haut-droite
	iAxial=iAxialDepart+1;
	jAxial=jAxialDepart-1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForBludger(idBudlger);
		ListeHexa[indexRow][indexCol]->isDiagonalGohautDroite();
		i+=1;
		iAxial = iAxial + 1;
		jAxial = jAxial - 1;
	}

}

void fenetre::marquerLancer(int iAxialDepart,int jAxialDepart,int maxDistance){
	qDebug()<<"Debut du marquage pour Lancer, i,j,max";
	qDebug()<<iAxialDepart;
	qDebug()<<jAxialDepart;
	qDebug()<<maxDistance;
	int i;
	int iAxial=iAxialDepart+1;
	int jAxial=jAxialDepart+0;
	int indexRow;
	int indexCol;

	//6 direction :
	//(1,0) droite
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test frappe droite";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
		ListeHexa[indexRow][indexCol]->isLine();
		i+=1;
		iAxial = iAxial + 1;
		jAxial = jAxial + 0;
	}

	//(0,1) bas-droite
	iAxial=iAxialDepart+0;
	jAxial=jAxialDepart+1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test frappe bas-droite";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
		ListeHexa[indexRow][indexCol]->isDiagonalGoBasDroite();
		i+=1;
		iAxial = iAxial + 0;
		jAxial = jAxial + 1;
	}
	//(-1,1) bas-gauche
	iAxial=iAxialDepart-1;
	jAxial=jAxialDepart+1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
		ListeHexa[indexRow][indexCol]->isDiagonalGohautDroite();
		i+=1;
		iAxial = iAxial - 1;
		jAxial = jAxial + 1;
	}

	//(-1,0) gauche
	iAxial=iAxialDepart-1;
	jAxial=jAxialDepart+0;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
		ListeHexa[indexRow][indexCol]->isLine();
		i+=1;
		iAxial = iAxial - 1;
		jAxial = jAxial + 0;
	}
	//(0,-1) haut-gauche
	iAxial=iAxialDepart+0;
	jAxial=jAxialDepart-1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
		ListeHexa[indexRow][indexCol]->isDiagonalGoBasDroite();
		i+=1;
		iAxial = iAxial + 0;
		jAxial = jAxial - 1;
	}
	//(1,-1) haut-droite
	iAxial=iAxialDepart+1;
	jAxial=jAxialDepart-1;
	i=1;
	while(ifNotOut(iAxial,jAxial) && i<=maxDistance ){
		qDebug() << "test diagonal";
		qDebug() << iAxial;
		qDebug() << jAxial;
		indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
		indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
		ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
		ListeHexa[indexRow][indexCol]->isDiagonalGohautDroite();
		i+=1;
		iAxial = iAxial + 1;
		jAxial = jAxial - 1;
	}

}

void fenetre::marquerAttraperSouaffle(int iAxial,int jAxial){
	int indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
	int indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
	ListeHexa[indexRow][indexCol]->isForCatch();

}
void fenetre::marquerAttraperVifDOr(int iAxial,int jAxial){
	int indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
	int indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isMarkForGoldenSnitch();
	ListeHexa[indexRow][indexCol]->isForCatch();

}



bool fenetre::ifNotOut(int iAxial,int jAxial){
	int indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
	int indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();

	return (    ( (-MATRIX_SIZE/2) <=iAxial )
			 && (iAxial < (MATRIX_SIZE/2 +1))
			 && ( (-MATRIX_SIZE/2) <=jAxial )
			 && (jAxial < (MATRIX_SIZE/2 +1))
			 && (ListeHexa[indexRow][indexCol]->getType() != NOT_ON_HEX_GRID) );

/*	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){
*/
}

//------------------------------------------------------------------------------------------------------------
//gestion des events
void fenetre::handlerMove(int iAxial,int jAxial){
	int indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
	int indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
	caseSelect = ListeHexa[indexRow][indexCol];
//	if(iHaveASelection){//object deja selectionné
	if(iHaveASelection){
		//TODO : construire moves[4][7], compter le nombre d'actions déjà réalisées
		//TODO bouton pour finir le tour
		//nextTurn()
		qDebug() << "j'ai deja selection qlq chose";
		if(iAxial==caseJoueurSelect->getIAxial() and jAxial==caseJoueurSelect->getJAxial()){
			qDebug() << "je selection meme joueur";
		}else{
			if(ListeHexa[indexRow][indexCol]->ifMark()){
				qDebug() << "case marquer trouver";
				//marquer la case pour une action (l'encadrer ou autre)
				caseSelect->selectForAction();
				qDebug() << caseSelect->zValue();
				caseSelect->setZValue(10);

//				caseJoueurSelect->isSelected();
				BoutonConfirm->setEnabled(true);
			}else{
				qDebug() << "case NON-marquer trouver";
				//deselection le joueur, tout remettre a zero
				//TODO: faire une belle fonction pour ça
				caseJoueurSelect->unselect();
				iHaveASelection=false;
				demarquerToutesCase();
				deplacer->setChecked(true);
				deplacer->setEnabled(false);
				lancer->setEnabled(false);
				taper->setEnabled(false);
				recupSouaffle->setEnabled(false);
				recupVifDOr->setEnabled(false);
				infoJoueur->setText("Info sur le joueur selection \n nom, prenom, vitesse, force,....");
				BoutonConfirm->setEnabled(false);
			}
		}
	}else{//aucune selection

		//verifier si je peux selectionner l'objet
		if( ListeHexa[indexRow][indexCol]->getType() != -1){//si pas une case vide (j'ai un joueur)
			qDebug() << "j'ai une nouvelle selection de qlq chose";
			if( ((ListeHexa[indexRow][indexCol]->getType() / 7) +1) == numMaTeam){//je verif qu'il est a mon equipe
				qDebug() << "joueur de ma team trouver";
				iHaveASelection = true;
				caseJoueurSelect = ListeHexa[indexRow][indexCol];
				caseJoueurSelect->select();

				//recup info sur le joueur et afficher
				//communication réseaux pour avoir info
				__client->selectPlayer(selectedPlayerID);
				attributs = __client->receiveSelectedPlayerInfos();
				//attributs = { {3,5,3,4,5}, AxialCoordinates(iAxial,jAxial) ,1 };
//*!!!!!!!!!!!! playerRole qui vas de 0 à 6
//*!!!!!!!!!!!! et selectedPlayerID = playerRole + 7 si numMaTeam = 2 (de 0 à 6 ou de 7 à 13)

				playerRole = caseJoueurSelect->getType() %7; //playerRole est un attribut de la fenetre pour pas se perdre
				qDebug() << "id du joueur";
				qDebug() << caseJoueurSelect->getType();
				qDebug() << "role du joueur";
				qDebug() << playerRole;
				infoJoueur->setText("Atribut du joueur:\n Vitesse  : "
									+ QString::number( attributs.attributes[SPEED]) +
				"  Force:"+ QString::number( attributs.attributes[STRENGTH]) +
				"\n Précision:"+ QString::number( attributs.attributes[PRECISION]) +
				"  Reflexe:"+QString::number( attributs.attributes[REFLEX]) +
				"\n Resistance:"+QString::number( attributs.attributes[RESISTANCE]) );

				//debloque action de deplacement et affiche casse accesible
				deplacer->setChecked(true);
				deplacer->setEnabled(true);
				marquerCaseAccessibleDepuis(iAxial,jAxial,attributs.attributes[SPEED]);

				//si le jouer a le souaffle
				if(attributs.hasQuaffle){
					lancer->setEnabled(true); //debloquer action lancer souaffle
				}
				//si le joueur est un batteur
				if( (playerRole== TEAM1_BEATER1) ||
					 (playerRole== TEAM1_BEATER2) ) {
					qDebug()<<"joueur de type beater detecter";
					//verif s'il y a un bludger adjacent
					qDebug()<<allPositions[BLUDGER1].getDiagAxis();
					qDebug()<<allPositions[BLUDGER1].getLineAxis();
					//si le un des 2 bludger est sur une case adjacente
					if( allPositions[BLUDGER1].getDistanceTo(AxialCoordinates(iAxial,jAxial)) < 2 ||
						allPositions[BLUDGER2].getDistanceTo(AxialCoordinates(iAxial,jAxial)) < 2 ){
						taper->setEnabled(true); //debloque action taper souaffle
					}
				}
				//si le joueur est un poursuiveur ou gardien ET que le souaffle est a porter
				if((((playerRole <= TEAM1_CHASER3) and (playerRole>= TEAM1_CHASER1)) or (playerRole == TEAM1_KEEPER))
					  and (attributs.position.getDistanceTo(allPositions[QUAFFLE]) <= attributs.attributes[PRECISION])){
					recupSouaffle->setEnabled(true);

				}

				//si le joueur est un attrapeur
				if(caseJoueurSelect->getType() == TEAM1_SEEKER || caseJoueurSelect->getType() == TEAM2_SEEKER ){
					qDebug()<<"joueur de type attrapeur detecter";
					if(attributs.position.getDistanceTo(allPositions[GOLDENSNITCH]) <= attributs.attributes[PRECISION])
						recupVifDOr->setEnabled(true);
				}
			}

		}
	}
}

void fenetre::handlerChoixAction(bool){
	qDebug() << "--Bouton ratio clicker";
	demarquerToutesCase();
	//--------------------------------------------------------------------------
	if(deplacer->isChecked()){
		qDebug() << "    deplacer choisi";
		qDebug() << attributs.attributes[SPEED];
		marquerCaseAccessibleDepuis(caseJoueurSelect->getIAxial(),caseJoueurSelect->getJAxial(),attributs.attributes[SPEED]);
	}
	//--------------------------------------------------------------------------
	if(lancer->isChecked()){
		qDebug() << "    lancer choisi";
		marquerLancer(caseJoueurSelect->getIAxial(),caseJoueurSelect->getJAxial(),attributs.attributes[STRENGTH]);
	}
	//--------------------------------------------------------------------------
	if(taper->isChecked()){
		qDebug() << "    taper choisi: bu iAxial,jAxial,";
		//distanceAccepted = attributs.attributes[STRENGTH];
		qDebug() <<allPositions[BLUDGER1].getDiagAxis();
		qDebug() <<allPositions[BLUDGER1].getLineAxis();
		qDebug() <<caseJoueurSelect->getIAxial();
		qDebug() <<attributs.attributes[STRENGTH];
		if(allPositions[BLUDGER1].getDistanceTo(AxialCoordinates(caseJoueurSelect->getIAxial(),caseJoueurSelect->getJAxial())) < 2)
			marquerFrappe(allPositions[BLUDGER1].getDiagAxis(),allPositions[BLUDGER1].getLineAxis(),attributs.attributes[STRENGTH],BLUDGER1);
		if(allPositions[BLUDGER2].getDistanceTo(AxialCoordinates(caseJoueurSelect->getIAxial(),caseJoueurSelect->getJAxial())) < 2)
			marquerFrappe(allPositions[BLUDGER2].getDiagAxis(),allPositions[BLUDGER2].getLineAxis(),attributs.attributes[STRENGTH],BLUDGER2);
	}
	//--------------------------------------------------------------------------
	if( recupSouaffle->isChecked() ){
		qDebug() << "    recupSouaffle choisi";
		marquerAttraperSouaffle(allPositions[QUAFFLE].getDiagAxis(),allPositions[QUAFFLE].getLineAxis());
	}
	//--------------------------------------------------------------------------
	if( recupVifDOr->isChecked() ){
		qDebug() << "    recupVifDOr choisi";
		marquerAttraperVifDOr(allPositions[GOLDENSNITCH].getDiagAxis(),allPositions[GOLDENSNITCH].getLineAxis());
	}
}

void fenetre::handlerAction(){
	qDebug() <<"action confirmer => création du mouve et check si joueur tous bouger ou si veux arreter";
	//RAPPEL :
	//moves[][0] : indice de l'objet concerné
	//moves[][1] : action spéciale :
	//#define INTERCEPT_QUAFFLE 0
	//#define CATCH_GOLDENSNITCH 1
	//moves[][2] : diagonale destination
	//moves[][3] : ligne destination
	//--------------------------------------------------------------------------
	if(deplacer->isChecked()){
		qDebug() << "    deplacer choisi";
		moves[currentMove][0] = caseJoueurSelect->getType();
		moves[currentMove][1] = NO_SPECIAL_ACTION; //pas d'action spéciale
		moves[currentMove][2] = caseSelect->getIAxial();
		moves[currentMove][3] = caseSelect->getJAxial();
	}
	//--------------------------------------------------------------------------
	if(lancer->isChecked()){
		qDebug() << "    lancer choisi";
		moves[currentMove][0] = caseSelect->getTypeMarkBall();
		moves[currentMove][1] = NO_SPECIAL_ACTION;
		moves[currentMove][2] = caseSelect->getIAxial();
		moves[currentMove][3] = caseSelect->getJAxial();
	}
	//--------------------------------------------------------------------------
	if(taper->isChecked()){
		qDebug() << "    taper choisi: bu iAxial,jAxial,";
		moves[currentMove][0] = caseSelect->getTypeMarkBall();
		moves[currentMove][1] = NO_SPECIAL_ACTION;
		moves[currentMove][2] = caseSelect->getIAxial();
		moves[currentMove][3] = caseSelect->getJAxial();
	}
	//--------------------------------------------------------------------------
	if( recupSouaffle->isChecked() ){
		qDebug() << "    recupSouaffle choisi";
		moves[currentMove][0] = caseJoueurSelect->getType();
		moves[currentMove][1] = INTERCEPT_QUAFFLE;
	}
	//--------------------------------------------------------------------------
	if( recupVifDOr->isChecked() ){
		qDebug() << "    recupVifDOr choisi";
		moves[currentMove][0] = caseJoueurSelect->getType();
		moves[currentMove][1] = CATCH_GOLDENSNITCH;
	}
	qDebug() <<"Affichage Info dans Move: ";
	qDebug() <<"currentMove : " + QString::number(currentMove);
	qDebug() <<"indice de l'objet concerné : " + QString::number(moves[currentMove][0]);
	qDebug() <<"action spéciale : "+ QString::number( moves[currentMove][1]); //pas d'action spéciale
	qDebug() <<"diagonale destination : "+ QString::number( moves[currentMove][2]);
	qDebug() <<"ligne destination : " +QString::number( moves[currentMove][3]);
	++currentMove;

	//TODO: faire une belle fonction pour ça
	caseJoueurSelect->unselect();
	iHaveASelection=false;
	demarquerToutesCase();
	deplacer->setChecked(true);
	deplacer->setEnabled(false);
	lancer->setEnabled(false);
	taper->setEnabled(false);
	recupSouaffle->setEnabled(false);
	recupVifDOr->setEnabled(false);
	infoJoueur->setText("Info sur le joueur selection \n nom, prenom, vitesse, force,....");
	BoutonConfirm->setEnabled(false);

	handlerTour();

}


void fenetre::handlerTour(){
	//check si toutes action fait -> si oui, envoyer donner serveur, recup allPosition et mettre a jour
	if(currentMove==7){//le joueur a fait tout c'est deplacement
		qDebug()<<"toutes les actions sont entrées";
		nextTurn();
		endHandler();
	}

}

void fenetre::nextTurn(){
	//sendMoves
	__client->sendMoves(moves);
	//getConfirmation
	__client->getConfirmation(); //Attention, bloquant si adversaire n'a pas encore répondu

	//reset :
	currentMove = 0;
	for(int i = 0; i < 7; ++i){
		moves[i][0] = i;
		moves[i][1] = NO_SPECIAL_ACTION;
		moves[i][2] = 10000; //sentinelle : mouvement vide
		moves[i][3] = 10000;
	}
	//récupérer les positions
	__client->getAllPositions();
    allPositions = __client->receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);
	//update affichage
	resetListeHexa();
	updateListeHexa();
}

void fenetre::pushesHandler(){}

void fenetre::endHandler(){
	if(winner != 0){
		
	}
}
