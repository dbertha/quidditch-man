#include "fenetre.hpp"

fenetre::fenetre() : QWidget()//on dit qu'on appelle le constructeur de QWidget (on peut lui passer des paramettre si besoin)
{
    setFixedSize(500, 500);

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
	_fieldMatrice(field),_listeJoueur(vPlaying),_listeBall(vBall)
{
	//setFixedSize(500, 500);

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

	initFieldGui();
}

void fenetre::initFieldGui(){
	//vas pourcourir tout le HexagonalField _fieldMatrice pour crée des hexagone correspondant
	// on part du principe que le point (0,0) de hexagonalFiel _fieldMatrice est au centre de la matrice
	// on parcours donc de -tailleMax/2 a tailleMax/2 sur les lignes et colonnes
	// on se sert de AxialCoordinates() pour traduire c'est coord dans un system "classique informatique"
	qDebug()<< "affiche test for axialCoord";
	int idOccupant;
	int indexRow;
	int indexCol;
	for (int indexRowAxial = -MATRIX_SIZE/2; indexRowAxial < MATRIX_SIZE/2 +1; ++indexRowAxial){
		for(int indexColAxial = -MATRIX_SIZE/2; indexColAxial < MATRIX_SIZE/2 +1; ++indexColAxial){
//			qDebug()<<"--- row puis Col puis val a cette posi";
//			qDebug()<< indexRowAxial;
//			qDebug()<< indexColAxial;
//			qDebug()<< _fieldMatrice.getOccupant(AxialCoordinates(indexRowAxial,indexColAxial));
//			qDebug()<< AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
//			qDebug()<< AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
			idOccupant=_fieldMatrice.getOccupant(AxialCoordinates(indexRowAxial,indexColAxial));
			if(idOccupant != NOT_ON_HEX_GRID){
				indexRow=AxialCoordinates(indexRowAxial,indexColAxial).getLineOnMatrix();
				indexCol=AxialCoordinates(indexRowAxial,indexColAxial).getColOnMatrix();
				ListeHexa[indexRow][indexCol] =new hexagone(indexRowAxial,indexColAxial,idOccupant);
				scene->addItem(ListeHexa[indexRow][indexCol]);
				QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int))
						, this, SLOT(changerTexte(int,int)));

			}


			//QObject::connect(ListeHexa[indexRow][indexCol], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));

		}
	}


}
/*
int HexagonalField::getOccupant(AxialCoordinates coord) const{
	return matrix[coord.getLineOnMatrix()][coord.getColOnMatrix()];
}*/

void fenetre::changerTexte(int i,int j){
	int indexRow=AxialCoordinates(i,j).getLineOnMatrix();
	int indexCol=AxialCoordinates(i,j).getColOnMatrix();

	texte->setText("coord Axial: i= "+QString::number(i)+", j ="+QString::number(j) +"\n"
				   +"coord standard: i="+QString::number(indexRow)+"j="+QString::number(indexCol));
    update();


}

/*
void MaFenetre::changerLargeur(int largeur) //defeni un nouveau slot (methode qui vas reagir a un signal)
{
    setFixedSize(largeur,this->height());//setFixedSize est ratacher a MaFenetre
    //this->height() -> permet d'avoir la hauteur actuel de la fenetre (this)
    if (largeur == 600)
    {
        //emit agrandissementMax();//emet un signal si j'ai atteint tail max
        emit agrandissementMaxV2(333);
    }
}
*/
