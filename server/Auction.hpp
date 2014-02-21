#include <stdio.h>
#include <time.h>
#include <string>

#include "User.hpp"
#include "Manager.hpp"
#include "ManagedPlayer.hpp"
#include <vector>

class Auction {

	User* auctionCreator_;
	vector<User*> usersInAuction_;
	vector<User*> bidders_;
	ManagedPlayer player_;
	struct tm instant_;
	int startingPrice_;
	int currentPrice_;
	int currentTurn_;
	int auctionID_;

 public:

 	Auction(User* creator, ManagedPlayer player, int startingPrice, int ID);

 	User* getAuctionCreator();
 	Manager* getManager();
 	User* getLastBidder();
 	void resetBidders();
 	ManagedPlayer getPlayer();
 	vector<int> getPlayerInfos();
 	int getTimeBeforeFirstTurn();
 	int getStartingPrice();
 	int getCurrentPrice();
 	int getCurrentTurn();
 	int getAuctionID();
 	int isAuctionFinished();
 	string getPlayerName();
 	void bid(User* bidder);

};