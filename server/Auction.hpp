#ifndef AUCTION_HPP
#define AUCTION_HPP

#include "User.hpp"
#include "Manager.hpp"
#include "ManagedPlayer.hpp"
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>

class User;

class Auction {

	User* _auctionCreator;
	vector<User*> _usersInAuction;
	vector<User*> _bidders;
	User* _lastBidder;
	ManagedPlayer _player;
	struct tm _instant;
	int _startingPrice;
	int _currentPrice;
	int _currentTurn;
	int _auctionID;
	int _nbOfEndOfTurn;

 public:

 	Auction(User* creator, ManagedPlayer player, int startingPrice, int ID);
 	void endAuction();

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
 	int getNumberOfBidders();
 	int isAuctionFinished();
 	string getPlayerName();
 	void bid(User* bidder);
 	void endOfTurnAsked();
 	int getNbOfEndOfTurn();
};

#endif
