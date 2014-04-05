#include "HexagonalCase.hpp"

//constructeur:
HexagonalCase::HexagonalCase(int i, int j,int type, QGraphicsItem *parent) : QGraphicsObject(parent),
	_typeCase(type),_indiceI(i),_indiceJ(j),_ifGoal(false),_ifSelect(false),_ifAccesible(false),
	_ifLine(false),_ifDiagonalBasDroite(false),_ifDiagonalhautDroite(false),_ifForCatch(false),
	_ifMarkForBludger(false), _ifMarkForQuaffle(false), _ifMarkForGoldenSnitch(false){
	_ifSelectForAction=false;
	_blocked=false;//note: un joueur bloquer ne peut etre debloquer que quand on redefini la case (setType)
	_contientBalleEnPlus=0;
	//this->setCursor(QCursor(Qt::OpenHandCursor));//test pour changer le cursor
	//setBoundingRegionGranularity(1);
	//QObject::connect(this, SIGNAL(clicked()), qApp, SLOT(quit()));
	if(_typeCase == NOT_ON_HEX_GRID){
		setActive(false);
		setEnabled(false);
		setVisible(false);
	}
}

//fonction appeler par QT pour dessiner l'objet (ou le redessiner/update)
void HexagonalCase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	dessinerHexagone(painter);
	dessinerType(painter);
}

//----------------------------------------------------------------------------------------------
//FONCTION POUR DESSINER LES DIFFERENTS ELEMENT D'UNE CASE
void HexagonalCase::dessinerHexagone(QPainter *painter){

	//1. definir les options de coloriage = definir contour et fond utiliser
	QBrush brush(_couleurFondNoSelect,Qt::SolidPattern);//vas servir a definir le fond

	if(_ifSelect){ //si la case est selectionner par le manager
		brush.setColor(_couleurFondSelect);
	}else{//une case selectionner ne peut pas etre dessiner accesible
		if(_ifAccesible){//si la case a été marquer accesible
			brush.setColor(Qt::green);
			brush.setStyle(Qt::DiagCrossPattern);
		}
		if(_ifLine){
			brush.setStyle(Qt::HorPattern);
		}
		if(_ifDiagonalBasDroite){
			brush.setStyle(Qt::FDiagPattern);
		}
		if(_ifDiagonalhautDroite){
			brush.setStyle(Qt::BDiagPattern);
		}
		if(_ifForCatch){
			brush.setStyle(Qt::SolidPattern);
		}

		if(_ifMarkForBludger){
			brush.setColor(_couleurBludger);
		}
		if(_ifMarkForQuaffle){
			brush.setColor(_couleurQuaffle);
		}
		if(_ifMarkForGoldenSnitch){
			brush.setColor(_couleurGoldenSnitch);
		}
		if(_ifGoal){//si la case est un but
			brush.setColor(_couleurFondGoal);
		}
	}


	if(_blocked){
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(Qt::gray);
	}

	QColor couleurBord=Qt::black;
	if(_ifSelectForAction){
		couleurBord = Qt::green;
	}


	painter->setBrush(brush );
	painter->setPen(QPen(couleurBord,2,Qt::SolidLine)); //defini le pinceaux qui dessine les contours

	if(_contientBalleEnPlus==1){//bludger en plus sur case
		dessinerBludger(painter);
	}
	if(_contientBalleEnPlus==2){//bludger en plus sur case
		dessinerQuaffle(painter);
	}


	//2. dessiner l'HexagonalCase
	painter->drawPolygon(caseBuilt());

}

void HexagonalCase::dessinerType(QPainter *painter){
	//pas mal de joueur son dessiner de la meme maniere pour l'instant, certain case son donc inutile
	//mais on garde pour pouvoir differencier joueur de meme type au sein d'une equipe
	// (2 chaser n'ont pas forcement meme stat donc interresant de pouvoir les differencier)
	switch(_typeCase){
		case FREE_SPACE -1:
			dessinerCaseVide(painter);
			break;
		//TEAM 1
		case TEAM1_KEEPER :
			dessinerKeeper(painter,_couleurEquipe1);
			break;
		case TEAM1_SEEKER :
			dessinerSeeker(painter,_couleurEquipe1);
			break;
		case TEAM1_CHASER1 :
			dessinerChaser(painter,_couleurEquipe1);
			break;
		case TEAM1_CHASER2 :
			dessinerChaser(painter,_couleurEquipe1);
			break;
		case TEAM1_CHASER3 :
			dessinerChaser(painter,_couleurEquipe1);
			break;
		case TEAM1_BEATER1 :
			dessinerBeater(painter,_couleurEquipe1);
			break;
		case TEAM1_BEATER2 :
			dessinerBeater(painter,_couleurEquipe1);
			break;
		//TEAM 2
		case TEAM2_KEEPER :
			dessinerKeeper(painter,_couleurEquipe2);
			break;
		case TEAM2_SEEKER :
			dessinerSeeker(painter,_couleurEquipe2);
			break;
		case TEAM2_CHASER1 :
			dessinerChaser(painter,_couleurEquipe2);
			 break;
		case TEAM2_CHASER2 :
			dessinerChaser(painter,_couleurEquipe2);
			break;
		case TEAM2_CHASER3 :
			dessinerChaser(painter,_couleurEquipe2);
			break;
		case TEAM2_BEATER1 :
			dessinerBeater(painter,_couleurEquipe2);
			break;
		case TEAM2_BEATER2 :
			dessinerBeater(painter,_couleurEquipe2);
			break;
		case GOLDENSNITCH :
			dessinerGoldenSnitch(painter);
			break;
		case BLUDGER1 :
			dessinerBludger(painter);
			break;
		case BLUDGER2 :
			dessinerBludger(painter);
			break;
		case QUAFFLE :
			dessinerQuaffle(painter);
			break;
	}
}

void HexagonalCase::dessinerCaseVide(QPainter *painter){
	setToolTip("");
}
//les 4 fonctions font la meme chose, laissez pour pouvoir changer affichage d'un type joueur en particulier (pour remplir avec petit icone.png ou autre)
void HexagonalCase::dessinerKeeper(QPainter *painter,QColor colorEquipe ){
	setToolTip("Keeper");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("K"));
}
void HexagonalCase::dessinerSeeker(QPainter *painter,QColor colorEquipe ){
	setToolTip("Seeker");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("S"));
}
void HexagonalCase::dessinerChaser(QPainter *painter,QColor colorEquipe ){
	setToolTip("Chaser");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("C"));
}
void HexagonalCase::dessinerBeater(QPainter *painter,QColor colorEquipe ){
	setToolTip("Beater");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("B"));
}

void HexagonalCase::dessinerQuaffle(QPainter *painter){
	setToolTip("Quaffle");
	painter->setBrush( _couleurQuaffle);
	painter->setPen(QPen(Qt::black,1));
	painter->drawEllipse ( centreHexagon(), largeur/3,largeur/3);
}

void HexagonalCase::dessinerGoldenSnitch(QPainter *painter){
	setToolTip("GoldenSnitch");
	painter->setBrush( _couleurGoldenSnitch);
	painter->setPen(QPen(Qt::black,1));
	painter->drawEllipse ( centreHexagon(), largeur/3,largeur/3);
}

void HexagonalCase::dessinerBludger(QPainter *painter){
	setToolTip("Bludger");
	painter->setBrush( _couleurBludger);
	painter->setPen(QPen(Qt::black,1));
	painter->drawEllipse ( centreHexagon(), largeur/3,largeur/3);
}

//----------------------------------------------------------------------------------------------
//FONCTION DE PRECONSTRUCTION D'ENSEMBLE DE POINT
//pre-construit l'HexagonalCase
QPolygonF HexagonalCase::caseBuilt() const{
	//a chaque point de l'hexagon de base, on ajoute un decallage pour rapport a son indice ligne(i), colonne(j)
	QVector<QPointF> points;
	points << pHaut +(_indiceI*pasIndiceI)+(_indiceJ*pasIndiceJ)
			<< pHautDroite +(_indiceI*pasIndiceI)+(_indiceJ*pasIndiceJ)
			<< pBasDroite +(_indiceI*pasIndiceI)+(_indiceJ*pasIndiceJ)
			<< pBas +(_indiceI*pasIndiceI)+(_indiceJ*pasIndiceJ)
			<< pBasGauche +(_indiceI*pasIndiceI)+(_indiceJ*pasIndiceJ)
			<< pHautGauche +(_indiceI*pasIndiceI)+(_indiceJ*pasIndiceJ);
	return QPolygonF(points);

}

//pre-construit le rectangle inscrit dans l'HexagonalCase
QRectF HexagonalCase::rectangleInterne() const{
	return QRectF(
				pHautGauche.x() + (_indiceI*pasIndiceI.x())+(_indiceJ*pasIndiceJ.x())
			   ,pHautGauche.y() +(_indiceI*pasIndiceI.y())+(_indiceJ*pasIndiceJ.y())
			   , largeur
			   , hauteurCentral);
}

QPointF HexagonalCase::centreHexagon() const{
	return QPointF(
				pHaut.x() +(_indiceI*pasIndiceI.x())+(_indiceJ*pasIndiceJ.x())
				,pHaut.y() +(_indiceI*pasIndiceI.y())+(_indiceJ*pasIndiceJ.y()) + hauteur/2);

}
//----------------------------------------------------------------------------------------------
void HexagonalCase::setType(int typeCase){
	_typeCase = typeCase;
	_blocked = false;
	_contientBalleEnPlus=0;
	update();
}
int HexagonalCase::getType(){
	return _typeCase ;
}
//----------------------------------------------------------------------------------------------
int HexagonalCase::getIAxial(){
	return _indiceI;
}
int HexagonalCase::getJAxial(){
	return _indiceJ;
}

int HexagonalCase::getHauteur(){
	return hauteur-4;
}

int HexagonalCase::getLargeur(){
	return largeur+1;
}

int HexagonalCase::getTypeMarkBall(){
	return _markTypeBalle;
}
void HexagonalCase::rajouterBalle(int nbr){
	_contientBalleEnPlus=nbr;
}

//----------------------------------------------------------------------------------------------
//Changement d'etat d'une case (selection ou pas, si c'est un goal, si case accessible,...)

void HexagonalCase::select(){
	_ifSelect = true;
	update();
}

void HexagonalCase::selectForAction(){
	_ifSelectForAction = true;
	update();
}


void HexagonalCase::unselect(){
	_ifSelect = false;
	update();
}

void HexagonalCase::bloquerLeJoueur(){
	_blocked = true;
	update();
}

//TODO : changer les noms, cela ressemble à des getters plutot qu'à des setter
void HexagonalCase::isAGoal(){
	_ifGoal = true;
	update();
}

void HexagonalCase::isAccessible(){
	_ifAccesible = true;
	update();
}

void HexagonalCase::isLine(){
	_ifLine = true;
	update();
}

void HexagonalCase::isDiagonalGoBasDroite(){
	_ifDiagonalBasDroite = true;
	update();
}

void HexagonalCase::isDiagonalGohautDroite(){
	_ifDiagonalhautDroite = true;
	update();
}

void HexagonalCase::isForCatch(){
	_ifForCatch = true;
	update();
}

void HexagonalCase::isMarkForBludger(int idBludger){
	_ifMarkForBludger=true;
	_markTypeBalle=idBludger;
	update();
}

void HexagonalCase::isMarkForQuaffle(){
	_ifMarkForQuaffle=true;
	_markTypeBalle=QUAFFLE;
	update();
}

void HexagonalCase::isMarkForGoldenSnitch(){
	_ifMarkForGoldenSnitch=true;
	update();
}

void HexagonalCase::isNonAccessible(){
	_ifAccesible = false;
	_ifSelectForAction = false;

	_ifLine = false;
	_ifDiagonalBasDroite = false;
	_ifDiagonalhautDroite = false;

	_ifForCatch=false;

	_ifMarkForBludger=false;
	_ifMarkForQuaffle=false;
	_ifMarkForGoldenSnitch=false;

	_markTypeBalle=-7;

	setZValue(0);
	update();
}

bool HexagonalCase::ifMark(){
	qDebug()<<"check si marquer, i,j";
	qDebug()<< _indiceI;
	qDebug()<< _indiceJ;
	qDebug()<< _ifAccesible;
	qDebug()<< _ifLine ;
	qDebug()<< _ifDiagonalBasDroite ;
	qDebug()<< _ifDiagonalhautDroite ;

	qDebug()<< _ifForCatch;

	qDebug()<< _ifMarkForBludger;
	qDebug()<< _ifMarkForQuaffle;
	qDebug()<< _ifMarkForGoldenSnitch;

	return ( _ifAccesible||_ifLine || _ifDiagonalBasDroite || _ifDiagonalhautDroite ||
			_ifForCatch || _ifMarkForBludger || _ifMarkForQuaffle ||_ifMarkForGoldenSnitch );
}

bool HexagonalCase::ifBlocked(){
	return _blocked;
}

//----------------------------------------------------------------------------------------------
//fonction test pour voir la reaction a un signal
void HexagonalCase::changerCouleur(){//slots de reaction a un signal

	if(_ifSelect){
		_ifSelect=false;
	}else{
		_ifSelect=true;
	}

	update();
}


void HexagonalCase::mousePressEvent(QGraphicsSceneMouseEvent *)
 {//emet un signal si on clicke sur une case
	 emit caseSelect(_indiceI,_indiceJ);
 }

//----------------------------------------------------------------------------------------------
//methode pour definir la zone d'interaction/definition des objets
//(utiliser pour les clicke mais aussi par Qt pour reddessiner les objet)
QRectF HexagonalCase::boundingRect() const
{   //attention: ne peut renvoyer qu'un rectangle

	qreal penWidth = 0;//pas sure que soit utile
	return QRectF( -largeur/2+(_indiceI*pasIndiceI.x())+(_indiceJ*pasIndiceJ.x()) - penWidth / 2
				  ,-hauteur/2+(_indiceI*pasIndiceI.y())+(_indiceJ*pasIndiceJ.y()) - penWidth / 2
				  , largeur + penWidth
				  , hauteur+ penWidth);//renvoie le rectangle qui contient l'HexagonalCase
}

//permet de devenir exactement la zone de selection des HexagonalCases
QPainterPath HexagonalCase::shape() const
{
	QPainterPath path;
	path.addPolygon(caseBuilt());
	return path;
}
//----------------------------------------------------------------------------------------------
