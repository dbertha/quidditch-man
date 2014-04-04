#include "Auction.hpp"

#define TIMEOFTURNZERO 70 //en secondes.
#define SECONDSINMINUTE 60
#define SECONDSINHOUR 3600
#define SECONDSINDAY 86400

using namespace std;

Auction::Auction(User* creator, ManagedPlayer player, int startingPrice, int ID): _auctionCreator(creator),_lastBidder(NULL), _player(player), \
_startingPrice(startingPrice),_currentPrice(startingPrice),_currentTurn(0),_auctionID(ID),_nbOfEndOfTurn(0) {
	time_t secondes;
    time(&secondes);
    _instant=*localtime(&secondes);
}

void Auction::endAuction() {
	try {
		getManager()->unlockPlayer(getPlayerName());
	} catch (const char err[]) {}
}

User* Auction::getAuctionCreator() {return _auctionCreator;}
Manager* Auction::getManager() {return _auctionCreator->getManager();}
User* Auction::getLastBidder() {return _lastBidder;}
void Auction::resetBidders() {
	_nbOfEndOfTurn=0;
	++_currentTurn;
	_usersInAuction = _bidders;
	_bidders.clear();}
ManagedPlayer Auction::getPlayer() {return _player;}
vector<int> Auction::getPlayerInfos() {return _player.getInformations();}
int Auction::getTimeBeforeFirstTurn() {
	time_t secondes;
	struct tm instant;
	time(&secondes);
	instant=*localtime(&secondes);
	int secondsNow = instant.tm_mday*SECONDSINDAY + instant.tm_hour*SECONDSINHOUR + instant.tm_min*SECONDSINMINUTE + instant.tm_sec;
	int secondsAtStart = _instant.tm_mday*SECONDSINDAY + _instant.tm_hour*SECONDSINHOUR + _instant.tm_min*SECONDSINMINUTE + _instant.tm_sec;
	int diff = secondsNow - secondsAtStart;

	return TIMEOFTURNZERO-diff;
	
}

int Auction::getStartingPrice() {return _startingPrice;}
int Auction::getCurrentPrice() {return _currentPrice;}
int Auction::getCurrentTurn() {return _currentTurn;}
int Auction::getAuctionID() {return _auctionID;}
int Auction::getNumberOfBidders() {return _usersInAuction.size();}
int Auction::isAuctionFinished() {
	if (_bidders.size()==1) return 1;
	else if (_bidders.size()>1) return 0;
	else return -1;
}

void Auction::bid(User* bidder) {
	_bidders.push_back(bidder);
	if (_currentTurn==0) _usersInAuction.push_back(bidder);
	_currentPrice+=_startingPrice/10;
	_lastBidder=bidder;
}

string Auction::getPlayerName() {
	string name = _player.getFirstName() + " " + _player.getLastName();
	return name;
}

void Auction::endOfTurnAsked() {++_nbOfEndOfTurn;}
int Auction::getNbOfEndOfTurn() {return _nbOfEndOfTurn;}