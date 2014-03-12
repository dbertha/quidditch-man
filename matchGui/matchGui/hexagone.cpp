#include "hexagone.hpp"

//constructeur:
hexagone::hexagone(int i, int j,int type, QGraphicsItem *parent) : QGraphicsObject(parent),
    _typeCase(type),_indiceI(i),_indiceJ(j),_ifSelect(false){
    //this->setCursor(QCursor(Qt::OpenHandCursor));//test pour changer le cursor
    //setBoundingRegionGranularity(1);
    //QObject::connect(this, SIGNAL(clicked()), qApp, SLOT(quit()));
}

//fonction appeler par QT pour dessiner l'objet (ou le redessiner/update)
void hexagone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    dessinerHexagone(painter);
    dessinerType(painter);
}

void hexagone::dessinerHexagone(QPainter *painter){
    //1. definir les options de coloriage
    painter->setPen(QPen(Qt::black,2,Qt::SolidLine)); //defini le pinceaux qui dessine les contours

    if(_ifSelect){ //defini la couleur de remplissage
        painter->setBrush( _couleurFondSelect );
    }else{
        painter->setBrush( _couleurFondNoSelect);
    }

    //2. dessiner l'hexagone

    painter->drawPolygon(hexagoneBuilt());

}

//pre-construit l'hexagone
QPolygonF hexagone::hexagoneBuilt() const{
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

//painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nNokia"));

void hexagone::dessinerType(QPainter *painter){
    if(_typeCase != FREE_SPACE){

    }
    dessinerKeeper(painter,_couleurEquipe2);
}

//les 4 fonctions vont la meme chose, laissez pour pouvoir changer affichage d'un joueur en particulier
void hexagone::dessinerKeeper(QPainter *painter,QColor colorEquipe ){
    setToolTip("Keeper");
    painter->setPen(QPen(colorEquipe));
    painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("K"));
}
void hexagone::dessinerSeeker(QPainter *painter,QColor colorEquipe ){
    painter->setPen(QPen(colorEquipe));
    painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("K"));
}
void hexagone::dessinerChaser(QPainter *painter,QColor colorEquipe ){
    painter->setPen(QPen(colorEquipe));
    painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("K"));
}

void hexagone::dessinerBeater(QPainter *painter,QColor colorEquipe ){
    setToolTip("Keeper");
    painter->setPen(QPen(colorEquipe));
    painter->drawText(rectangleInterne(), Qt::AlignCenter, tr("K"));
}


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
