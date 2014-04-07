# calls client and server Makefile
all: client.out server.out clientGUI.out

client.out: Makefile.client \
			client/Client.cpp \
			client/Client.hpp \
			client/ClientMain.cpp \
			client/TerminalGame.hpp \
			client/TerminalGame.cpp \
			common/Coordinates.hpp \
			common/Defines.hpp \
			common/HexagonalField.hpp \
			common/NetworkBase.hpp \
			common/NetworkInterface.hpp \
			common/HexagonalField.cpp \
			common/NetworkBase.cpp
	make -f Makefile.client
	
server.out: Makefile.server \
			server/Auction.hpp \
			server/Ball.hpp \
			server/Broomstick.hpp \
			server/Building.hpp \
			server/Calendar.hpp \
			server/FanShop.hpp \
			server/Hospital.hpp \
			server/ImprovementBuilding.hpp \
			server/ManagedPlayer.hpp \
			server/Manager.hpp \
			server/Match.hpp \
			server/MatchesHandler.hpp \
			server/Player.hpp \
			server/PlayingPlayer.hpp \
			server/PromotionCenter.hpp \
			server/DataBase.hpp \
			server/Server.hpp \
			server/Stadium.hpp \
			server/TrainingCenter.hpp \
			server/User.hpp \
			server/Tournament.hpp \
			common/Coordinates.hpp \
			common/Defines.hpp \
			common/HexagonalField.hpp \
			common/NetworkBase.hpp \
			server/Auction.cpp \
			server/Broomstick.cpp \
			server/Building.cpp \
			server/Calendar.cpp \
			server/FanShop.cpp \
			server/Hospital.cpp \
			server/ImprovementBuilding.cpp \
			server/ManagedPlayer.cpp \
			server/Manager.cpp \
			server/Match.cpp \
			server/MatchesHandler.cpp \
			server/Player.cpp \
			server/PlayingPlayer.cpp \
			server/PromotionCenter.cpp \
			server/DataBase.cpp \
			server/Server.cpp \
			server/ServerMain.cpp \
			server/Stadium.cpp \
			server/TrainingCenter.cpp \
			server/User.cpp \
			server/Tournament.cpp \
			common/HexagonalField.cpp \
			common/NetworkBase.cpp
	make -f Makefile.server
	
clientGUI.out: Makefile.GUI \
           client/Client.hpp \
           client/MatchWindow.hpp \
           client/HexagonalCase.hpp \
	   client/MainWindow.hpp \
	   client/ConnexionPage.hpp \
	   client/LoginPage.hpp \
	   client/MainPage.hpp \
	   client/InfosWidget.hpp \
	   client/StadiumPage.hpp \
	   client/OfficePage.hpp \
	   client/DomainPage.hpp \
	   client/BuildingWidget.hpp \
           client/BuyAPWidget.hpp \
	   client/GainAPWidget.hpp \
	   client/ListPlayersWidget.hpp \
	   client/PlayerWidget.hpp \
	   client/ManagePlayerWidget.hpp \
	   client/ListAuctionsWidget.hpp \
	   client/AuctionWidget.hpp \
	   client/AdminPage.hpp \
	   client/TournamentsWidget.hpp \
	   client/NotificationWidget.hpp \
	   client/SelectPlayersWidget.hpp \
           client/GUImain.cpp \
           client/Client.cpp \
           client/MatchWindow.cpp \
           client/HexagonalCase.cpp \
	   client/MainWindow.cpp \
	   client/ConnexionPage.cpp \
	   client/LoginPage.cpp \
	   client/MainPage.cpp \
	   client/InfosWidget.cpp \
	   client/StadiumPage.cpp \
	   client/OfficePage.cpp \
	   client/DomainPage.cpp \
	   client/BuildingWidget.cpp \
	   client/BuyAPWidget.cpp \
	   client/GainAPWidget.cpp \
	   client/ListPlayersWidget.cpp \
	   client/PlayerWidget.cpp \
	   client/ManagePlayerWidget.cpp \
	   client/ListAuctionsWidget.cpp \
	   client/AuctionWidget.cpp \
	   client/AdminPage.cpp \
	   client/TournamentsWidget.cpp \
	   client/NotificationWidget.cpp \
	   client/SelectPlayersWidget.cpp
	make -f Makefile.GUI
	
Makefile.client :
	qmake client.pro -o Makefile.client
	
Makefile.server :
	qmake server.pro -o Makefile.server
	
Makefile.GUI :
	qmake clientGui.pro -o Makefile.GUI
	
clean:
	make clean -f Makefile.server
	make clean -f Makefile.client
	make clean -f Makefile.GUI
	rm Makefile.server
	rm Makefile.client
	rm Makefile.GUI

