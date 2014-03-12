#include "hexagone.hpp"

//constructeur:
hexagone::hexagone(int i, int j,int type, QGraphicsItem *parent) : QGraphicsObject(parent),
	_typeCase(type),_indiceI(i),_indiceJ(j),_ifGoal(false),_ifSelect(false){

	//this->setCursor(QCursor(Qt::OpenHandCursor));//test pour changer le cursor
	//setBoundingRegionGranularity(1);
	//QObject::connect(this, SIGNAL(clicked()), qApp, SLOT(quit()));
}

//fonction appeler par QT pour dessiner l'objet (ou le redessiner/update)
void hexagone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	dessinerHexagone(painter);
	dessinerType(painter);
}

//----------------------------------------------------------------------------------------------
//FONCTION POUR DESSINER LES DIFFERENTS ELEMENT D'UNE CASE
void hexagone::dessinerHexagone(QPainter *painter){
	//1. definir les options de coloriage
	painter->setPen(QPen(Qt::black,2,Qt::SolidLine)); //defini le pinceaux qui dessine les contours

	if(_ifSelect){ //defini la couleur de remplissage
		painter->setBrush( _couleurFondSelect );
	}else{
		if(_ifGoal){
			painter->setBrush( _couleurFondGoal );
		}else{
		painter->setBrush( _couleurFondNoSelect);
		}
	}

	//2. dessiner l'hexagone

	painter->drawPolygon(hexagoneBuilt());

}




void hexagone::dessinerType(QPainter *painter){
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

void hexagone::dessinerCaseVide(QPainter *painter){
	setToolTip("");
}
//les 4 fonctions font la meme chose, laissez pour pouvoir changer affichage d'un type joueur en particulier (pour remplir avec petit icone.png ou autre)
void hexagone::dessinerKeeper(QPainter *painter,QColor colorEquipe ){
	setToolTip("Keeper");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("K"));
}
void hexagone::dessinerSeeker(QPainter *painter,QColor colorEquipe ){
	setToolTip("Seeker");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("S"));
}
void hexagone::dessinerChaser(QPainter *painter,QColor colorEquipe ){
	setToolTip("Chaser");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("C"));
}
void hexagone::dessinerBeater(QPainter *painter,QColor colorEquipe ){
	setToolTip("Beater");
	painter->setPen(QPen(colorEquipe));
	painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("B"));
}

void hexagone::dessinerQuaffle(QPainter *painter){
	setToolTip("Quaffle");
	painter->setBrush( _couleurQuaffle);
	painter->setPen(QPen(Qt::black,1));
	painter->drawEllipse ( centreHexagon(), largeur/3,largeur/3);
}

void hexagone::dessinerGoldenSnitch(QPainter *painter){
	setToolTip("GoldenSnitch");
	painter->setBrush( _couleurGoldenSnitch);
	painter->setPen(QPen(Qt::black,1));
	painter->drawEllipse ( centreHexagon(), largeur/3,largeur/3);
}

void hexagone::dessinerBludger(QPainter *painter){
	setToolTip("Bludger");
	painter->setBrush( _couleurBludger);
	painter->setPen(QPen(Qt::black,1));
	painter->drawEllipse ( centreHexagon(), largeur/3,largeur/3);
}

//----------------------------------------------------------------------------------------------
//FONCTION DE PRECONSTRUCTION D'ENSEMBLE DE POINT
//pre-construit l'hexagone
QPolygonF hexagone::hexagoneBuilt() const{
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

//pre-construit le rectangle inscrit dans l'hexagone
QRectF hexagone::rectangleInterne() const{
	return QRectF(
				pHautGauche.x() + (_indiceI*pasIndiceI.x())+(_indiceJ*pasIndiceJ.x())
			   ,pHautGauche.y() +(_indiceI*pasIndiceI.y())+(_indiceJ*pasIndiceJ.y())
			   , largeur
			   , hauteurCentral);
}

QPointF hexagone::centreHexagon() const{
	return QPointF(
				pHaut.x() +(_indiceI*pasIndiceI.x())+(_indiceJ*pasIndiceJ.x())
				,pHaut.y() +(_indiceI*pasIndiceI.y())+(_indiceJ*pasIndiceJ.y()) + hauteur/2);

}
//----------------------------------------------------------------------------------------------


void hexagone::select(){
	_ifSelect = true;

}

void hexagone::unselect(){
	_ifSelect = false;

}

void hexagone::changerCouleur(){//slots de reaction a un signal

	if(_ifSelect){
		_ifSelect=false;
	}else{
		_ifSelect=true;
	}

	update();
}




void hexagone::mousePressEvent(QGraphicsSceneMouseEvent *)
 {//test pour voir les reaction possible a un clickage
	 //setCursor(Qt::ClosedHandCursor);
	 changerCouleur();
	 emit hexagoneSelect(_indiceI,_indiceJ);

 }

//----------------------------------------------------------------------------------------------
//methode pour definir la zone d'interaction/definition des objets
//(utiliser pour les clicke mais aussi par Qt pour reddessiner les objet)
QRectF hexagone::boundingRect() const
{   //attention: ne peut ranvoyer qu'un rectangle

	qreal penWidth = 1;//pas sure que soit utile
	return QRectF( -largeur/2+(_indiceI*pasIndiceI.x())+(_indiceJ*pasIndiceJ.x()) - penWidth / 2
				  ,-hauteur/2+(_indiceI*pasIndiceI.y())+(_indiceJ*pasIndiceJ.y()) - penWidth / 2
				  , largeur + penWidth
				  , hauteur+ penWidth);//renvoie le rectangle qui contient l'hexagone
}

//permet de devenir exactement la zone de selection des hexagones
QPainterPath hexagone::shape() const
{
	QPainterPath path;
	path.addPolygon(hexagoneBuilt());
	return path;
}
//----------------------------------------------------------------------------------------------
