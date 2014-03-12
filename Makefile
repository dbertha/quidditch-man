# calls client and server Makefile
all: client.out server.out clientGUI.out

client.out: client/Client.cpp \
			client/Client.hpp \
			client/ClientMain.cpp \
			common/commAPI.hpp \
			common/Coordinates.hpp \
			common/Defines.hpp \
			common/HexagonalField.hpp \
			common/NetworkBase.hpp \
			common/NetworkInterface.hpp \
			common/commAPI.cpp \
			common/HexagonalField.cpp \
			common/NetworkBase.cpp
	make -f Makefile.client
	
server.out: server/Auction.hpp \
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
			common/commAPI.hpp \
			common/Coordinates.hpp \
			common/Defines.hpp \
			common/HexagonalField.hpp \
			common/NetworkBase.hpp \
			common/NetworkInterface.hpp \
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
			common/commAPI.cpp \
			common/HexagonalField.cpp \
			common/NetworkBase.cpp
	make -f Makefile.server
	
clientGUI.out: client/clientMatchHandler.hpp \
           client/loginDialog.hpp \
           client/mainGui.hpp \
           client/selectionDialog.hpp \
           client/ticker.hpp \
           client/buildingsDialog.hpp \
           client/buildingsModel.hpp \
           client/playerMgr.hpp \
           common/NetworkInterface.hpp \
           client/clientMatchHandler.cpp \
           client/loginDialog.cpp \
           client/GUImain.cpp \
           client/mainGui.cpp \
           client/selectionDialog.cpp \
           client/ticker.cpp \
           client/buildingsDialog.cpp \
           client/buildingsModel.cpp \
           client/playerMgr.cpp \
           common/NetworkInterface.cpp 
	make -f Makefile.GUI
	
clean:
	make clean -f Makefile.server
	make clean -f Makefile.client
	make clean -f Makefile.GUI

