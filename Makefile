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
	client/ClientMatchHandler.hpp \
           client/LoginDialog.hpp \
           client/MainGui.hpp \
           client/SelectionDialog.hpp \
           client/Ticker.hpp \
           client/BuildingsDialog.hpp \
           client/BuildingsModel.hpp \
           client/PlayerMgr.hpp \
           client/Client.hpp \
           client/MainMenu.hpp \
           client/PlayersDialog.hpp \
           client/TournamentDialog.hpp \
           client/Tournaments.hpp \
           client/BuyAPDialog.hpp \
           client/FreeAPDialog.hpp \
	   client/AuctionMgr.hpp \
           client/AuctionsDialog.hpp \
           client/AuctionWindow.hpp \
           client/MatchWindow.hpp \
           client/HexagonalCase.hpp \
           client/ClientMatchHandler.cpp \
           client/LoginDialog.cpp \
           client/GUImain.cpp \
           client/MainGui.cpp \
           client/SelectionDialog.cpp \
           client/Ticker.cpp \
           client/BuildingsDialog.cpp \
           client/BuildingsModel.cpp \
           client/PlayerMgr.cpp \
           client/Client.cpp \
           client/MainMenu.cpp \
           client/PlayersDialog.cpp \
           client/TournamentDialog.cpp \
           client/Tournaments.cpp \
	   client/BuyAPDialog.cpp \
           client/FreeAPDialog.cpp \
	   client/AuctionMgr.cpp \
           client/AuctionsDialog.cpp \
           client/AuctionWindow.cpp \
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

