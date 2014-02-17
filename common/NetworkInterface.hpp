/* Interface réseau en C++ qui fera appel au module C */

//TODO : répartir les sendToClient et sendToServer dans les dossiers server/ et client/ ?

//Ces fonctions construisent le struct à passer sur le réseau et le passent à la routine C qui s'en chargerax²

int sendToClient(const Client &client, const Building &building);

int sendToClient(const Client &client, const Player &player);

int sendToClient(const Client &client, const Field &field);

int sendToServer(const Message &msg);
