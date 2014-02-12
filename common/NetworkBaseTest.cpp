#include "NetworkBase.h"
#include <iostream>

#define OBJECTTOTRANSMIT 11

//TODO : tester retour malloc
class ObjectToTransmit {
public :
    int attribut1;
    long attribut2;
    char attribut3[10];
    ObjectToTransmit() : attribut1(5), attribut2(100L) {
        for(int i = 0; i < 9 ; ++i){
            attribut3[i] = 'a';
        }
        attribut3[9] = '\0';
    }
    ObjectToTransmit(SerializedObjectList* serialized){ // /!\ désérialisation au sein de l'objet : besoin de connaître l'objet SerializedObjectList
        if(serialized->paquet->typeOfInfos == OBJECTTOTRANSMIT){
            char * position = serialized->paquet->stringData;
            memcpy(&attribut1, position, sizeof(attribut1));
            position += sizeof(attribut1);
            memcpy(&attribut2, position, sizeof(attribut2));
            position += sizeof(attribut2);
            memcpy(&attribut3, position, sizeof(attribut3));
            position += sizeof(attribut3);
        }else{
            std::cout << "Pas le bon type d'infos" << std::endl;
        }
            
    }
    SerializedObjectList* serialize(){
        SerializedObjectList *result = (SerializedObjectList *) malloc(sizeof(SerializedObjectList));
        result->paquet = (SerializedObject *) malloc(sizeof(SerializedObject));
        std::cout << "stringData initialisé ?" << std::endl;
        result->paquet->stringData[0] = 'a';
        result->paquet->stringData[1] = 'b';
        result->paquet->stringData[3] = '\0';
        std::cout << "accès à stringData" << std::endl;
        std::cout << result->paquet->stringData << std::endl;
        char * position = result->paquet->stringData;

        result->next = NULL;
        result->paquet->typeOfInfos = OBJECTTOTRANSMIT;
        memcpy(position, &attribut1, sizeof(attribut1));
        position += sizeof(attribut1);
        memcpy(position, &attribut2, sizeof(attribut2));
        position += sizeof(attribut2);
        memcpy(position, attribut3, sizeof(attribut3));
        position += sizeof(attribut3);
        //TODO : plus de découplage, méthodes s'appliquant à SerializedObjectList, serialisation et déserialisation hors de la classe
        return result;
    }
};

int main(int argc, char** argv) {
    ObjectToTransmit toPass;
    toPass.attribut1 = 123;
    SerializedObjectList *serialized;
    std::cout << "Attributs l'objet à transmettre : " << std::endl;
    std::cout << "Entier : " << toPass.attribut1 << std::endl;
    std::cout << "Long : " << toPass.attribut2 << std::endl;
    std::cout << "Char[10] : " << toPass.attribut3 << std::endl;
    std::cout << "Sérialisation... " << std::endl;
    serialized = toPass.serialize();
    std::cout << "Dé-sérialisation... " << std::endl;
    ObjectToTransmit received(serialized);
    std::cout << "Attributs l'objet dé-sérialisé: " << std::endl;
    std::cout << "Entier : " << received.attribut1 << std::endl;
    std::cout << "Long : " << received.attribut2 << std::endl;
    std::cout << "Char[10] : " << received.attribut3 << std::endl;
    return EXIT_SUCCESS;
}
