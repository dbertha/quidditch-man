#include <stdio.h>
#include <time.h>
#include "User.hpp"
#include "Manager.hpp"
#include "ManagedPlayer.hpp"
#include <vector>
#include <string>
#include "Auction.hpp"

#define TIMEOFTURNZERO 70 //en secondes.
#define SECONDSINMINUTE 60
#define SECONDSINHOUR 3600
#define SECONDSINDAY 86400

using namespace std;

Auction::Auction(User* creator, ManagedPlayer player, int startingPrice, int ID): auctionCreator_(creator), player_(player), \
startingPrice_(startingPrice), currentPrice_(startingPrice), currentTurn_(0), auctionID_(ID) {
	time_t secondes;
    time(&secondes);
    instant_=*localtime(&secondes);
}

User* Auction::getAuctionCreator() {return auctionCreator_;}
Manager* Auction::getManager() {return auctionCreator_->getManager();}
User* Auction::getLastBidder() {return bidders_.back();}
void Auction::resetBidders() {bidders_.clear();}
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
int Auction::isAuctionFinished() {
	if (bidders_.size()==1) return 1;
	else if (bidders_.size()>1) return 0;
	else return -1;
}

void Auction::bid(User* bidder) {
	bidders_.push_back(bidder);
	currentPrice_+=startingPrice_/10;
}

string Auction::getPlayerName() {
	string name = player_.getFirstName() + " " + player_.getLastName();
	return name;
}
