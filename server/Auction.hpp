

#include "User.hpp"
#include "Manager.hpp"
#include "ManagedPlayer.hpp"
#include <vector>

class Auction {

	User* auctionCreator_;
	User* lastBidder_;
	vector<User*> userInAuctions_;
	vector<User*> bidders_;
	ManagedPlayer* player_;
	int startingPrice_;
	int currentPrice_;

 public:

 	Auction(User* creator, ManagedPlayer* player, int startingPrice);

 	User* getAuctionCreator();
 	User* getLastBidder();
 	ManagedPlayer* getPlayer();
 	int getStartingPrice();
 	int getCurrentPrice();
 	void bid(User* bidder, bool realBid);

 	void startAuction(); //tour 0

 	void startAuctionTurn(); //tour 1+
}