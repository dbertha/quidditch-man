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
	client/clientMatchHandler.hpp \
           client/loginDialog.hpp \
           client/mainGui.hpp \
           client/selectionDialog.hpp \
           client/ticker.hpp \
           client/buildingsDialog.hpp \
           client/buildingsModel.hpp \
           client/playerMgr.hpp \
           client/Client.hpp \
           client/mainMenu.hpp \
           client/playersDialog.hpp \
           client/tournamentDialog.hpp \
           client/tournaments.hpp \
           client/buyAPDialog.hpp \
           client/freeAPDialog.hpp \
	   client/auctionMgr.hpp \
           client/AuctionsDialog.hpp \
           client/auctionWindow.hpp \
           client/MatchWindow.hpp \
           client/HexagonalCase.hpp \
           client/clientMatchHandler.cpp \
           client/loginDialog.cpp \
           client/GUImain.cpp \
           client/mainGui.cpp \
           client/selectionDialog.cpp \
           client/ticker.cpp \
           client/buildingsDialog.cpp \
           client/buildingsModel.cpp \
           client/playerMgr.cpp \
           client/Client.cpp \
           client/mainMenu.cpp \
           client/playersDialog.cpp \
           client/tournamentDialog.cpp \
           client/tournaments.cpp \
	   client/buyAPDialog.cpp \
           client/freeAPDialog.cpp \
	   client/auctionMgr.cpp \
           client/AuctionsDialog.cpp \
           client/auctionWindow.cpp \
           client/MatchWindow.cpp \
           client/HexagonalCase.cpp
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

