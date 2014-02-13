#ifndef SERIALISATOR_HPP
#define SERIALISATOR_HPP

#include "NetworkBase.h"

#define OBJECTTOTRANSMIT 11

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

};

class Serialisator {
public :
    static SerializedObjectList* serialize(ObjectToTransmit &toTransmit); //à surcharger pour chaque objet
    static ObjectToTransmit deserialize(SerializedObjectList *serialized);
};



#endif
