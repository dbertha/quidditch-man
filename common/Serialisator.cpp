#include "Serialisator.hpp"
#include "NetworkBase.h"
#include <iostream>

//TODO : utilisation de vector ?
//TODO : utiliser à des endroits propres malloc/free

SerializedObjectList* Serialisator::serialize(ObjectToTransmit &toTransmit){
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
    memcpy(position, &toTransmit.attribut1, sizeof(toTransmit.attribut1));
    position += sizeof(toTransmit.attribut1);
    memcpy(position, &toTransmit.attribut2, sizeof(toTransmit.attribut2));
    position += sizeof(toTransmit.attribut2);
    memcpy(position, toTransmit.attribut3, sizeof(toTransmit.attribut3));
    position += sizeof(toTransmit.attribut3);
    //TODO : plus de découplage, méthodes s'appliquant à SerializedObjectList, serialisation et déserialisation hors de la classe
    return result;
}

ObjectToTransmit Serialisator::deserialize(SerializedObjectList *serialized){
    if(serialized->paquet->typeOfInfos == OBJECTTOTRANSMIT){ //TODO : decouplage
        ObjectToTransmit object;
        char * position = serialized->paquet->stringData;
        memcpy(&object.attribut1, position, sizeof(object.attribut1));
        position += sizeof(object.attribut1);
        memcpy(&object.attribut2, position, sizeof(object.attribut2));
        position += sizeof(object.attribut2);
        memcpy(&object.attribut3, position, sizeof(object.attribut3));
        position += sizeof(object.attribut3);
        return object;
    }else{
        throw "Pas le bon type d'infos";
    }
}



int main(int argc, char** argv) {
    ObjectToTransmit toPass;
    toPass.attribut1 = 123;
    SerializedObjectList *serialized;
    std::cout << "Attributs l'objet à transmettre : " << std::endl;
    std::cout << "Entier : " << toPass.attribut1 << std::endl;
    std::cout << "Long : " << toPass.attribut2 << std::endl;
    std::cout << "Char[10] : " << toPass.attribut3 << std::endl;
    std::cout << "Sérialisation... " << std::endl;
    serialized = Serialisator::serialize(toPass);
    std::cout << "Dé-sérialisation... " << std::endl;
    ObjectToTransmit received = Serialisator::deserialize(serialized);
    std::cout << "Attributs l'objet dé-sérialisé: " << std::endl;
    std::cout << "Entier : " << received.attribut1 << std::endl;
    std::cout << "Long : " << received.attribut2 << std::endl;
    std::cout << "Char[10] : " << received.attribut3 << std::endl;
    return EXIT_SUCCESS;
}
