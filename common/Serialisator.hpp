#ifndef SERIALISATOR_HPP
#define SERIALISATOR_HPP

#include "ObjectForTests.hpp"
#include "NetworkBase.h"

#define OBJECTTOTRANSMIT 11

class Serialisator {
public :
    static SerializedObjectList* serialize(ObjectToTransmit &toTransmit); //à surcharger pour chaque objet
    static ObjectToTransmit deserialize(SerializedObjectList *serialized);
};



#endif
