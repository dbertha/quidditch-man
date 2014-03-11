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

    ListeHexa[0][0] =new hexagone(0,0);
    scene->addItem(ListeHexa[0][0]);
    QObject::connect(ListeHexa[0][0], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[0][1] = new hexagone(0,1);
    scene->addItem(ListeHexa[0][1]);
    QObject::connect(ListeHexa[0][1], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[1][0] = new hexagone(1,0);
    scene->addItem(ListeHexa[1][0]);
    QObject::connect(ListeHexa[1][0], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[1][1] = new hexagone(1,1);
    scene->addItem(ListeHexa[1][1]);
    QObject::connect(ListeHexa[1][1], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[0][2] = new hexagone(0,2);
    scene->addItem(ListeHexa[0][2]);
    QObject::connect(ListeHexa[0][2], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[1][2] = new hexagone(1,2);
    scene->addItem(ListeHexa[1][2]);
    QObject::connect(ListeHexa[1][2], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[2][2] = new hexagone(2,2);
    scene->addItem(ListeHexa[2][2]);
    QObject::connect(ListeHexa[2][2], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[2][0] = new hexagone(2,0);
    scene->addItem(ListeHexa[2][0]);
    QObject::connect(ListeHexa[2][0], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));
    ListeHexa[2][1] = new hexagone(2,1);
    scene->addItem(ListeHexa[2][1]);

    scene->addRect(40,10,20,40);
    QObject::connect(ListeHexa[2][1], SIGNAL(hexagoneSelect(int,int)), this, SLOT(changerTexte(int,int)));



}

void fenetre::changerTexte(int i,int j){
    texte->setText("j'ai recu nouvelle coord: i= "+QString::number(i)+"; j ="+QString::number(j));
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
