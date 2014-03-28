#include "Auction.hpp"

#define TIMEOFTURNZERO 70 //en secondes.
#define SECONDSINMINUTE 60
#define SECONDSINHOUR 3600
#define SECONDSINDAY 86400

using namespace std;

Auction::Auction(User* creator, ManagedPlayer player, int startingPrice, int ID): auctionCreator_(creator),lastBidder_(NULL), player_(player), \
startingPrice_(startingPrice), currentPrice_(startingPrice), currentTurn_(0), auctionID_(ID), nbOfEndOfTurn_(0) {
	time_t secondes;
    time(&secondes);
    instant_=*localtime(&secondes);
}

void Auction::endAuction() {
	try {
		getManager()->unlockPlayer(getPlayerName());
	} catch (const char err[]) {}
}

User* Auction::getAuctionCreator() {return auctionCreator_;}
Manager* Auction::getManager() {return auctionCreator_->getManager();}
User* Auction::getLastBidder() {return lastBidder_;}
void Auction::resetBidders() {
	nbOfEndOfTurn_=0;
	++currentTurn_;
	usersInAuction_ = bidders_;
	bidders_.clear();}
ManagedPlayer Auction::getPlayer() {return player_;}
vector<int> Auction::getPlayerInfos() {return player_.getInformations();}
int Auction::getTimeBeforeFirstTurn() {
	time_t secondes;
	struct tm instant;
	time(&secondes);
	instant=*localtime(&secondes);
	int secondsNow = instant.tm_mday*SECONDSINDAY + instant.tm_hour*SECONDSINHOUR + instant.tm_min*SECONDSINMINUTE + instant.tm_sec;
	int secondsAtStart = instant_.tm_mday*SECONDSINDAY + instant_.tm_hour*SECONDSINHOUR + instant_.tm_min*SECONDSINMINUTE + instant_.tm_sec;
	int diff = secondsNow - secondsAtStart;

	return TIMEOFTURNZERO-diff;
	//printf("%d/%d ; %d:%d:%d\n", instant.tm_mday+1, instant.tm_mon+1, instant.tm_hour, instant.tm_min, instant.tm_sec);
}

int Auction::getStartingPrice() {return startingPrice_;}
int Auction::getCurrentPrice() {return currentPrice_;}
int Auction::getCurrentTurn() {return currentTurn_;}
int Auction::getAuctionID() {return auctionID_;}
int Auction::getNumberOfBidders() {return usersInAuction_.size();}
int Auction::isAuctionFinished() {
	if (bidders_.size()==1) return 1;
	else if (bidders_.size()>1) return 0;
	else return -1;
}

void Auction::bid(User* bidder) {
	bidders_.push_back(bidder);
	if (currentTurn_==0) usersInAuction_.push_back(bidder);
	currentPrice_+=startingPrice_/10;
	lastBidder_=bidder;
}

string Auction::getPlayerName() {
	string name = player_.getFirstName() + " " + player_.getLastName();
	return name;
}

void Auction::endOfTurnAsked() {++nbOfEndOfTurn_;}
int Auction::getNbOfEndOfTurn() {return nbOfEndOfTurn_;}