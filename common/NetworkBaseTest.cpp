#include "NetworkBase.h"
#include "Serialisator.hpp"
#include <iostream>

#define OBJECTTOTRANSMIT 11

//TODO : tester retour malloc

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
