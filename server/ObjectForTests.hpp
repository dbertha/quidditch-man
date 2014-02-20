#ifndef OBJECTFORTESTS_HPP
#define OBJECTFORTESTS_HPP

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


#endif
