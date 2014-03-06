#include "commAPI.hpp"
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

//~ int receiveMsg(const int sockfd, char * buf, const int len) {
	//~ int length=recv(sockfd,buf,len,0);
	//~ if(length==0 || length==ERROR) std::cerr<<"message receive error"<<std::endl;
	//~ else {
		//~ buf[length]='\0';
//~ #ifdef __DEBUG
		//~ std::cout<<"**got "<<length<<" char. : "<<buf<<"**"<<std::endl;
//~ #endif
	//~ }
	//~ return length;
//~ }
//~ 
//~ int sendMsg(const int sockfd, char * buf, const int length) {
	//~ if(send(sockfd,buf,length,0)==ERROR) {
		//~ std::cerr<<" message send error on socket "<<sockfd<<std::endl;
		//~ return EXIT_FAILURE;
	//~ }
	//~ else {
//~ #ifdef __DEBUG
		//~ std::cout<<"**sent on "<<sockfd<<" : "<<buf<<"**"<<std::endl;
//~ #endif
		//~ return EXIT_SUCCESS;
	//~ }
//~ }
