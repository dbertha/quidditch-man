// Module réseau en C (compilation C++), gère la communication basique entre serveur et clients


#include "NetworkBase.hpp"

/* recv(socket, *message, sizeToWrite, 0) return sizeWriten if ok
//send(socket, *message, sizeToRead, 0) return sizeRead if ok */


SerializedObject receiveOnSocket(int socketfd){
    SerializedObject received;
    
    int result;
    result = recv(socketfd, &received, sizeof(SerializedObject), 0);
    //Attention, on ne renvoie pas la valeur de retour
    received.typeOfInfos = ntohs(received.typeOfInfos);
    if(result != sizeof(SerializedObject)){
        printf("%s\n", "receiveOnSocket : Erreur dans la réception des données");
        received.typeOfInfos = SERVER_DOWN;
    }
    return received;
}

int sendOnSocket(int socketfd, SerializedObject toSend){
    int result;
    toSend.typeOfInfos = htons(toSend.typeOfInfos);
    result =  send(socketfd, &toSend, sizeof(SerializedObject), 0);
    if(result != sizeof(SerializedObject)){
        printf("%s\n", "SendOnSocket : Erreur dans l'envoi des données"); //TODO : sur le flux err
    }else{
#ifdef __DEBUG
    printf("%s\n", "SendOnSocket : Paquet envoyé");
#endif
    }
    return result;
}

int buildConnection (const int port_) {

	struct sockaddr_in sockAddress_;

	int sockfd_=socket(PF_INET,SOCK_STREAM,0);
	if (sockfd_ == ERROR) {
		std::cerr<<"Socket descriptor initialization error"<<std::endl;
		return ERROR;
	}
	int yes=1; //autorise deux binds successifs
	if (setsockopt(sockfd_, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==ERROR) {
		std::cerr<<"Socket descriptor options initialization error"<<std::endl;
		return ERROR;
	}
	int n;
	unsigned int m = sizeof(n);
	getsockopt(sockfd_,SOL_SOCKET,SO_RCVBUF,(void *)&n, &m);
	std::cout<<"TCP buffer size : "<<n<<std::endl;
	sockAddress_.sin_family=AF_INET;
	sockAddress_.sin_port=htons(port_);
	sockAddress_.sin_addr.s_addr=INADDR_ANY;
	memset(&sockAddress_.sin_zero,'\0',8);

	if (bind(sockfd_,(struct sockaddr*)&sockAddress_,sizeof(struct sockaddr))==ERROR) {
		std::cerr<<"Socket bind error"<<std::endl;
		return ERROR;
	}
	else return sockfd_;
}



