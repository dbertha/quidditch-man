#include "Tournament.hpp"




Tournament::Tournament(int nbOfParticipants, int startingPrice) : 
    __startingPrice(startingPrice), __startingNbOfPlayers(nbOfParticipants), __currentNbOfPlayers(0), \
    __nbOfLevels(log2(nbOfParticipants)), __started(false){
        
    assert(not((nbOfParticipants != 1) && (nbOfParticipants & (nbOfParticipants - 1)))); //must be a power of two    
    for(int i = 0; i < __nbOfLevels; ++i){
        __tournamentTree.push_back(std::vector<User *>()); //TODO : last level useless (only the winner)
    }

}

int Tournament::subscribeManager(User * participant){
    std::vector<User *> & firstList = __tournamentTree[__nbOfLevels - 1];
    if(__started){
        return -1;
    }
    int participantIndex = std::find(firstList.begin(), firstList.end(), participant) - firstList.begin();
    if(participantIndex < int(firstList.size())){
        return -1; //already in list
    }
    if(__currentNbOfPlayers == __startingNbOfPlayers){
        return -1; //already full : redondant with started test
    }
    firstList.push_back(participant);
    ++__currentNbOfPlayers;
    if(__currentNbOfPlayers == __startingNbOfPlayers){
        __started = true;
        return 1; //should start
    }
    return 0;
}

int Tournament::recordResult(User * winner){
    if(__nbOfLevels > 0){ //nbOfLevels already adapted
        __tournamentTree[__nbOfLevels - 1].push_back(winner);
        ++__currentNbOfPlayers;
        return log2(__currentNbOfPlayers) == __nbOfLevels ? 1 : 0; //test if next level should start
    }
    __started = false;
    return -1; //tournament over
}

std::vector<User *> Tournament::getNextMatches(){
    assert(__started); //tournament should be started
    --__nbOfLevels; //nbOfLevels adapted for the next round
    __currentNbOfPlayers = 0;
    return __tournamentTree[__nbOfLevels];
}

int Tournament::getReward(int level){
    return __startingPrice * pow(REWARDGROWINGRATE, log2(__startingNbOfPlayers) - level);
}
int Tournament::getReward(){
    return getReward(__nbOfLevels + 1); //__nbOfLevels already adapted
}

bool Tournament::isStarted(){
    return __started;
}
    
    
int Tournament::serialize(char * buffer){
    //order : __startingNbOfPlayers, __currentNbOfPlayers, __startingPrice
    memcpy(buffer, &__startingNbOfPlayers, sizeof(__startingNbOfPlayers));
    buffer += sizeof(__startingNbOfPlayers);
    memcpy(buffer, &__currentNbOfPlayers, sizeof(__currentNbOfPlayers));
    buffer += sizeof(__currentNbOfPlayers);
    memcpy(buffer, &__startingPrice, sizeof(__startingPrice));
    //buffer += sizeof(__startingPrice);
    return 1; //TODO : tester retour memcpy
}




int log2(int x){
    //for integral only : get highest bit set
    int i = x;
    int result = 0;
    while (i >>= 1) { ++result; }
    return result;
}
