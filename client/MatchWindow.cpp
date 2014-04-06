#include "MatchWindow.hpp"

//TODO:note: code pas du tout optimise/"modualiser"/"umliser", grosse phase de refactoring necessaire

MatchWindow::MatchWindow(Client * client, int numTeam, QWidget * parent) : QDialog(parent),
	numMaTeam(numTeam), iHaveASelection(false),  scoreTeam1(0), scoreTeam2(0), winner(0), currentMove(0), 
	__field(), __client(client), __forfeitAndDrawNotifier(new QSocketNotifier(client->getSockfd(),  QSocketNotifier::Read, this))
{

	//initialisation liste de mouvement:

	for(int i = 0; i < 7; ++i){
	  moves[i][0] = i;
	  moves[i][1] = NO_SPECIAL_ACTION;
	  moves[i][2] = 10000; //sentinelle : mouvement vide
	  moves[i][3] = 10000;
	}
	connect(__forfeitAndDrawNotifier,SIGNAL(activated(int)),this,SLOT(pushesHandler()));

	setFixedSize(800, 800);//defini la taille de toute la fenetre (contient zone de jeux, description vie joueur, action possible,...)

	//*****************************************************************************************
	infoJoueur = new QLabel("Info sur le joueur selection \n nom, prenom, vitesse, force,....");
	infoJoueur->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	if(numMaTeam == 1){//on vas adapter le texter d'indication de score selon qu'on soit l'equipe 1 ou 2
		texteEquipe1 = new QLabel(texteMaTeam);
		texteEquipe2 = new QLabel(texteAutreTeam);
	}else{
		texteEquipe1 = new QLabel(texteAutreTeam);
		texteEquipe2 = new QLabel(texteMaTeam);
	}
	//on colorie le texte selon les couleurs utiliser dans les hexagonalCase
	QPalette* palette = new QPalette();
	palette->setColor(QPalette::WindowText,_couleurEquipe1);
	texteEquipe1->setPalette(*palette);
	palette->setColor(QPalette::WindowText,_couleurEquipe2);
	texteEquipe2->setPalette(*palette);

	scoreEquipe1 = new QLabel(QString::number(0));
	scoreEquipe2 = new QLabel(QString::number(0));
	//*****************************************************************************************
	//création de la scene, zone principal où seront afficher les cases haxagonal
	scene = new QGraphicsScene;
	//note: la taille de la scene sera defini a sont initialisation
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
	deplacer->setEnabled(false);//setEnabled => descative le bouton, le rend "grisé"
	lancer->setEnabled(false);
	taper->setEnabled(false);
	recupSouaffle->setEnabled(false);
	recupVifDOr->setEnabled(false);
//	taper->setVisible(false); //TODO: choisir ou non si afficher action (setVisible vas masquer le bouton, on pourrai alors
	// seulement afficher les action possible pour un joueur au lieu de "grisé" celle inacessible)

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
	BoutonFinirTour = new QPushButton("terminer le tour");
	BoutonFinirTour->setMaximumSize ( 110, 30);

	textConfirm = new QLabel("Confirmer action:");
	layoutConformi->addWidget(textConfirm,0,0,Qt::AlignBottom);
	layoutConformi->addWidget(BoutonConfirm,1,0,Qt::AlignTop);
	layoutConformi->addWidget(BoutonFinirTour,2,0);
	QObject::connect(BoutonConfirm,SIGNAL(clicked()),this,SLOT(handlerAction()));
	QObject::connect(BoutonFinirTour,SIGNAL(clicked()),this,SLOT(handlerTourEnd()));
	//*****************************************************************************************

	//initialisation d'un layout pour organniser le Qlabel,la view, radio bouton,....
	layout = new QGridLayout;
//	layout->addWidget(texte,3,0);
	layout->addWidget(texteEquipe1,0,0,Qt::AlignRight); //(nom_Widget_a_afficher,indexLigne, indexColonne)
	layout->addWidget(scoreEquipe1,0,1,Qt::AlignLeft);
	layout->addWidget(texteEquipe2,0,2,Qt::AlignRight);
	layout->addWidget(scoreEquipe2,0,3,Qt::AlignLeft);
	layout->addWidget(view, 1, 0,1,4);//peut occuper 1 ligne et 3 colonne
	layout->addWidget(infoJoueur,2,0,1,2);
	layout->addWidget(groupbox,2,2);
	layout->addLayout(layoutConformi,2,3);
	this->setLayout(layout);

	__forfeitAndDrawNotifier->setEnabled(false);
	allPositions = __client->receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);
	

	initListeHexa();
	updateListeHexa();
	__forfeitAndDrawNotifier->setEnabled(true);
}

void MatchWindow::initListeHexa(){
	//initialise un terrain vide avec juste des goal
	int idOccupant;
	int indexRow;
	int indexCol;

	//initialise les HexagonalCase et les connectes a la MatchWindow (this)
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){ //index de la diag
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){ //index de la ligne
			idOccupant=__field.getOccupant(AxialCoordinates(indexRowAxial,indexColAxial));
			indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
			indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
			ListeHexa[indexRow][indexCol] =new HexagonalCase(indexRowAxial,indexColAxial,idOccupant);
			scene->addItem(ListeHexa[indexRow][indexCol]);
			QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(caseSelect(int,int))
					, this, SLOT(handlerMove(int,int)));
		}
	}
	//defini la taille de la scene:
	//  calculer a partir de nombre d'hexagone et de leur taille
	int x_depart = (-MATRIX_SIZE/2) *ListeHexa[0][0]->getLargeur();
	int y_depart = (-MATRIX_SIZE/2) *ListeHexa[0][0]->getHauteur();
	int largeur = MATRIX_SIZE*ListeHexa[0][0]->getLargeur();
	int hauteur = MATRIX_SIZE*ListeHexa[0][0]->getHauteur();
	scene->setSceneRect(x_depart,y_depart,largeur,hauteur);


	//defini les case qui sont des goal
	indexRow=AxialCoordinates(GOAL1_SIDE1_DIAG,GOAL1_SIDE1_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL1_SIDE1_DIAG,GOAL1_SIDE1_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
	indexRow=AxialCoordinates(GOAL2_SIDE1_DIAG,GOAL2_SIDE1_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL2_SIDE1_DIAG,GOAL2_SIDE1_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
	indexRow=AxialCoordinates(GOAL3_SIDE1_DIAG,GOAL3_SIDE1_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL3_SIDE1_DIAG,GOAL3_SIDE1_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();

	indexRow=AxialCoordinates(GOAL1_SIDE2_DIAG,GOAL1_SIDE2_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL1_SIDE2_DIAG,GOAL1_SIDE2_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
	indexRow=AxialCoordinates(GOAL2_SIDE2_DIAG,GOAL2_SIDE2_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL2_SIDE2_DIAG,GOAL2_SIDE2_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
	indexRow=AxialCoordinates(GOAL3_SIDE2_DIAG,GOAL3_SIDE2_LINE).getLineOnMatrix();
	indexCol=AxialCoordinates(GOAL3_SIDE2_DIAG,GOAL3_SIDE2_LINE).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isAGoal();
}

void MatchWindow::resetListeHexa(){
	int indexRow;
	int indexCol;
	//reset le terrain (toutes les case redeviennent vide)
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){ //index de la diag
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){ //index de la ligne
			indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
			indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
			ListeHexa[indexRow][indexCol]->setType(FREE_SPACE);
		}
	}
}

void MatchWindow::updateListeHexa(){
	//mets a jout les case HexagonalCases selon un vecteur allPositions
	// allPosition[0]  contient les coord axial du joueur 0 de l'equipe 1 (TEAM1_KEEPER)
	// les autre allPosition[x] vont dans le meme ordre qu'il sont decrit dans le fichier definie.hpp
	int indexRow;
	int indexCol;

	//i indique aussi la type de joueur ou balle (voir definie.hpp->TEAM1_KEEPER,...)
	for(unsigned int i = 0; i < allPositions.size(); ++i){
		indexRow=allPositions[i].getLineOnMatrix();
		indexCol=allPositions[i].getColOnMatrix();
		if(indexRow == 10000){
			int playerRole = i;
            int playerTeam = 1;
			if(i >= TEAM2_KEEPER){//TODO: verif si c'est pas un >= qu'il faut (avant, c'etait juste i> )
				playerTeam = 2;
                playerRole = i - TEAM2_KEEPER;
			}
			QStringList orderedRoles;
			QString boxTxt("Le joueur ");
			orderedRoles << "KEEPER" << "SEEKER" << "CHASER1" << "CHASER2" << "CHASER3" << "BEATER1" << "BEATER2";
			boxTxt += orderedRoles[i] + "de l'équipe " + QString::number(playerTeam) + " a été abattu par les cognards.";
			QMessageBox msgBox;
			msgBox.setText(boxTxt);
			msgBox.exec();
		}else if(ListeHexa[indexRow][indexCol]->getType()==FREE_SPACE){
			//1 balle et un joueur peuvent se supperposer, si la case est vide, la case devient une case joueur ou balle
			ListeHexa[indexRow][indexCol]->setType(i);
		}else{//si la case n'est pas vide, on rajoute une balle sur la casse du joueur
			if(i==QUAFFLE){
				ListeHexa[indexRow][indexCol]->rajouterBalle(QUAFFLE);
			}else{
				ListeHexa[indexRow][indexCol]->rajouterBalle(BLUDGER1);
			}
		}


	}
	update();
}


//TODO: supprimier
void MatchWindow::changerTexte(int i,int j){
/*	int indexRow=AxialCoordinates(i,j).getLineOnMatrix();
	int indexCol=AxialCoordinates(i,j).getColOnMatrix();

	texte->setText("coord Axial: i= "+QString::number(i)+", j ="+QString::number(j) +"\n"
				   +"coord standard: i="+QString::number(indexRow)+"j="+QString::number(indexCol));
//	marquerCaseAccessibleDepuis(i,j,2);
//	if(i==0 && j==0){
//		qDebug()<<"rentre dans if acces";
//		marquerToutesCaseNonAccessible();
//	}
	update();*/

}


//TODO:optimiser, back Tracking?
void MatchWindow::marquerCaseAccessibleDepuis(int rowAxial, int colAxial, int maxDistance){
	//crée le point de depart, celui par rapport au quel ont vas estimer la distance
	AxialCoordinates pointDepart(rowAxial,colAxial);

	int indexRow;
	int indexCol;
	qDebug()<<"******";
	//on vas comparer la distance entre le pointDepart et toutes les cases et marquer celle qui sont accesible
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
				ListeHexaAccessible<<ListeHexa[indexRow][indexCol];
			}
		}
	}
	update();
}

void MatchWindow::demarquerToutesCase(){
	//vas marquer toutes les casses non Accesible
	//NOTE: pas optimal, faut s'arranger pour sauvergarder une liste de case qui ont
	//  été marquer accesible pour les dermarquer facilement

//	int indexRow;
//	int indexCol;
	qDebug()<<"rentre dans non accessible1";

	while(!ListeHexaAccessible.isEmpty()){
		temp = ListeHexaAccessible.takeFirst();//supprime le 1er element et le renvoye
		qDebug()<<"#####";
		qDebug()<<temp->getIAxial();
		qDebug()<<temp->getJAxial();
		temp->isNonAccessible();

	}
/*

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
	}*/
}

//TODO *!!!!!!!!!!!!!!!!!!! A refactorer, marquerFrappe + marquerLancer
void MatchWindow::marquerFrappe(int iAxialDepart,int jAxialDepart,int maxDistance,int idBudlger){
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

void MatchWindow::marquerLancer(int iAxialDepart,int jAxialDepart,int maxDistance){
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

void MatchWindow::marquerAttraperSouaffle(int iAxial,int jAxial){
	int indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
	int indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isMarkForQuaffle();
	ListeHexa[indexRow][indexCol]->isForCatch();

}
void MatchWindow::marquerAttraperVifDOr(int iAxial,int jAxial){
	int indexRow=AxialCoordinates(iAxial,jAxial).getLineOnMatrix();
	int indexCol=AxialCoordinates(iAxial,jAxial).getColOnMatrix();
	ListeHexa[indexRow][indexCol]->isMarkForGoldenSnitch();
	ListeHexa[indexRow][indexCol]->isForCatch();

}



bool MatchWindow::ifNotOut(int iAxial,int jAxial){
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
void MatchWindow::handlerMove(int iAxial,int jAxial){
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
				caseSelect->setZValue(15);

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
		if( ListeHexa[indexRow][indexCol]->getType() != FREE_SPACE){//si pas une case vide (j'ai un joueur)
			qDebug() << "j'ai une nouvelle selection de qlq chose";
			if( (((ListeHexa[indexRow][indexCol]->getType() / 7) +1) == numMaTeam)
					&& !(ListeHexa[indexRow][indexCol]->ifBlocked()) ){//je verif qu'il est a mon equipe
				qDebug() << "joueur de ma team trouver";
				iHaveASelection = true;
				caseJoueurSelect = ListeHexa[indexRow][indexCol];
				caseJoueurSelect->select();
				
				//recup info sur le joueur et afficher
				//communication réseaux pour avoir info
				__forfeitAndDrawNotifier->setEnabled(false);
				attributs = __client->receiveSelectedPlayerInfos(caseJoueurSelect->getType());
				__forfeitAndDrawNotifier->setEnabled(true);
				//attributs = { {3,5,3,4,5}, AxialCoordinates(iAxial,jAxial) ,1 };
//*!!!!!!!!!!!! playerRole qui vas de 0 à 6
//*!!!!!!!!!!!! et selectedPlayerID = playerRole + 7 si numMaTeam = 2 (de 0 à 6 ou de 7 à 13)

				playerRole = caseJoueurSelect->getType() %7; //playerRole est un attribut de la MatchWindow pour pas se perdre
				qDebug() << "id du joueur";
				qDebug() << caseJoueurSelect->getType();
				qDebug() << "role du joueur";
				qDebug() << playerRole;
				infoJoueur->setText("Atribut du joueur:\n Vitesse  : "
									+ QString::number( attributs.attributes[SPEED]) +
				"  Force:"+ QString::number( attributs.attributes[STRENGTH]) +
				"\n Precision:"+ QString::number( attributs.attributes[PRECISION]) +
				"  Reflexe:"+QString::number( attributs.attributes[REFLEX]) +
				"\n Resistance:"+QString::number( attributs.attributes[RESISTANCE]) + 
				"  Vie restante :"+QString::number( attributs.life) );

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

void MatchWindow::handlerChoixAction(bool){
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

void MatchWindow::handlerAction(){
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
		caseJoueurSelect->bloquerLeJoueur();
	}
	//--------------------------------------------------------------------------
	if(lancer->isChecked()){
		qDebug() << "    lancer choisi";
		moves[currentMove][0] = caseSelect->getTypeMarkBall();
		moves[currentMove][1] = NO_SPECIAL_ACTION;
		moves[currentMove][2] = caseSelect->getIAxial();
		moves[currentMove][3] = caseSelect->getJAxial();
		caseJoueurSelect->bloquerLeJoueur();
	}
	//--------------------------------------------------------------------------
	if(taper->isChecked()){
		qDebug() << "    taper choisi: bu iAxial,jAxial,";
		moves[currentMove][0] = caseSelect->getTypeMarkBall();
		moves[currentMove][1] = NO_SPECIAL_ACTION;
		moves[currentMove][2] = caseSelect->getIAxial();
		moves[currentMove][3] = caseSelect->getJAxial();
		caseJoueurSelect->bloquerLeJoueur();
	}
	//--------------------------------------------------------------------------
	if( recupSouaffle->isChecked() ){
		qDebug() << "    recupSouaffle choisi";
		moves[currentMove][0] = caseJoueurSelect->getType();
		moves[currentMove][1] = INTERCEPT_QUAFFLE;
		caseJoueurSelect->bloquerLeJoueur();
	}
	//--------------------------------------------------------------------------
	if( recupVifDOr->isChecked() ){
		qDebug() << "    recupVifDOr choisi";
		moves[currentMove][0] = caseJoueurSelect->getType();
		moves[currentMove][1] = CATCH_GOLDENSNITCH;
		caseJoueurSelect->bloquerLeJoueur();
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


void MatchWindow::handlerTour(){
	//check si toutes action fait -> si oui, envoyer donner serveur, recup allPosition et mettre a jour
	if(currentMove==7 || BoutonFinirTour->isChecked()){//le joueur a fait tout c'est deplacement
		qDebug()<<"toutes les actions sont entrées";
		nextTurn();
		endHandler();
	}
}

void MatchWindow::handlerTourEnd(){//correction derniere minute pour gerer bouton fin tour
	//check si toutes action fait -> si oui, envoyer donner serveur, recup allPosition et mettre a jour
	qDebug()<<"FIN du tour provoquer par le joueur";
	nextTurn();
	endHandler();
}

void MatchWindow::nextTurn(){
	__forfeitAndDrawNotifier->setEnabled(false);
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
    allPositions = __client->receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);
	//TODO: mieux gerer affichage
	scoreEquipe1->setText(QString::number(scoreTeam1));
	scoreEquipe2->setText(QString::number(scoreTeam2));
	//update affichage
	resetListeHexa();
	updateListeHexa();
	__forfeitAndDrawNotifier->setEnabled(true);
}

void MatchWindow::pushesHandler(){
	bool over = false;
	__forfeitAndDrawNotifier->setEnabled(false);
	SerializedObject received = receiveOnSocket(__client->getSockfd());
    switch(received.typeOfInfos){
		case OPPONENTFORFEIT : {
			QMessageBox::information(this,QMessageBox::tr("Match over !"),QString("Opponent forfeited"),QMessageBox::Ok);
			over = true;
			break;
		}
		case OPPONENTASKFORDRAW : {
			int ret, code;
			QMessageBox msgBox;
			QString texte("The opponent ask for a draw, do you accept ? ?");
			msgBox.setWindowTitle("Draw proposal !");
			msgBox.setText(texte);
			
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			ret = msgBox.exec();
			if(ret == QMessageBox::Yes){
				code = DRAWACCEPTED;
				over = true;
			}else{
				code = DRAWDENIED;
			}
			__client->sendAnswerToDrawProposition(code);
			break;
        }
    }
    if(over){
		close();
	}
	else __forfeitAndDrawNotifier->setEnabled(true);
}

void MatchWindow::endHandler(){
	bool over = false;
	if(winner != 0){
		QString str = QString("Winner is team %1").arg(QString::number(winner));
		QMessageBox::information(this,QMessageBox::tr("Match over !"),str,QMessageBox::Ok);
		over = true;
	}else{
		int ret;
		QMessageBox msgBox;
		QString texte("Do you want to continue the match ?");
        msgBox.setWindowTitle("Next turn starts !");
        msgBox.setText(texte);
        msgBox.setInformativeText("abort to forfeit, cancel to ask for a draw ");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Abort | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        //QPushButton *connectButton = msgBox.addButton(tr("Forfeit"), QMessageBox::ActionRole);
        //QPushButton *connectButton = msgBox.addButton(tr("AskForDraw"), QMessageBox::ActionRole);
        ret = msgBox.exec();
        if(ret == QMessageBox::Abort){
			//forfeit
			__forfeitAndDrawNotifier->setEnabled(false);
			__client->sendForfeit();
			QMessageBox::information(this,QMessageBox::tr("Match over !"),QString("You forfeited"),QMessageBox::Ok);
			over = true;
		}else if(ret == QMessageBox::Cancel){
			//ask for a draw
			__forfeitAndDrawNotifier->setEnabled(false);
			QMessageBox::information(this,QMessageBox::tr("About draw"),QString("You are considered as the looser if you ask for a draw during a tournament and it's accepted"),QMessageBox::Ok); 
			//TODO : chance de revenir en arrière
			
			__client->sendDrawRequest();
			int result = __client->getConfirmation();
			if(result == DRAWACCEPTED){
				QMessageBox::information(this,QMessageBox::tr("Match over !"),QString("Draw accepted"),QMessageBox::Ok);
				over = true;
            }else{
				QMessageBox::information(this,QMessageBox::tr("Draw"),QString("Draw refused"),QMessageBox::Ok);
            }
            __forfeitAndDrawNotifier->setEnabled(true);
		}
	}
	if(over){
		close();
	}
}
