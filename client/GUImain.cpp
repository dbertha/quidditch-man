/* How to compile :
 * qmake -project
 * qmake
 * make        */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mainGui.hpp"
#include "loginDialog.hpp"
#include "common/Defines.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{//   Q_INIT_RESOURCE(mdi);
    int sockfd_;
    struct sockaddr_in serverAddr;
    struct hostent *he; // Pointeur vers le descripteur IP du serveur

// Initialisation des paramètres de connection au serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); // Le port du serveur.
    memset(&(serverAddr.sin_zero), '\0', 8);
    switch (argc) {
       case 2 : if ((he=gethostbyname(argv[1])) == NULL) { // Pointeur vers le descripteur IP du serveur a partir du nom passe en parametre.
                    std::cout<<"La machine "<<argv[1]<<" n'a pas ete trouvee"<<std::endl;
                    return EXIT_FAILURE;
                }
            serverAddr.sin_addr = *((struct in_addr*)he->h_addr);
            std::cout<<"Adresse du serveur "<<argv[1]<<" : "<<inet_ntoa(serverAddr.sin_addr)<<std::endl;
            break;
       case 3 :if (inet_aton(argv[2],&serverAddr.sin_addr) != 0) break;
       case 1 :if (inet_aton("127.0.1.1",&serverAddr.sin_addr) != 0) break; //machine locale
    default : std::cout<<"Donner le nom de la machine distante en argument."<<std::endl	\
                <<"ou son adresse IP sous la forme \"IP <adresse>\". Exemples :"<<std::endl		\
                <<"[exécutable] nom-machine-serveur"<<std::endl<<"[exécutable] IP 192.168.1.6"<<std::endl;
            return EXIT_FAILURE;
    }
    sockfd_ = socket(PF_INET,SOCK_STREAM, 0);
    if (sockfd_ == ERROR) return EXIT_FAILURE;
    if (connect(sockfd_, (struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == ERROR) {
            std::cout<<"Le serveur est absent."<<std::endl;
            return EXIT_FAILURE;
   }
    QApplication application(argc, argv);
    MainGui * window=new MainGui(sockfd_);
    window->show();
    std::cout<<"Client GUI started"<<std::endl;
    return application.exec();
}
