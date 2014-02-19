#include "User.hpp"
#include "Server.hpp"
#include "CommonMgr.hpp"
User::User(Server * server, CommonMgr * commonMgr, int sockfd): server_(server), commonMgr_(commonMgr), sockfd_(sockfd), userId_(""), state_(INIT) {}
void* createMatch(void* tData) {
	struct threadData* tData_=(struct threadData*)tData;
// début du match entre this (qui a invité) et opponent (l'invité).
// durant le match, les sockets de ces deux comparses ne sont plus surveillés directement par le serveur.
//								pthread_create(&server_->usersList_[i]->thread,NULL,createMatch,NULL);
//fin du match
//	tData_->host->state_=User::FREE;
//	tData_->opponent->state_=User::FREE;
	pthread_exit(NULL);
}
int User::cmdHandler(char * msg, const int length) {
// msg[0] : master command (dans les deux sens)
// msg[1] : sub-command (vers le serveur), return-code (vers le client : blanc=OK ou no operation, 0 = NOK
	answer_="";
	if(state_==INIT && msg[0]!='L') {
		answer_="0Internal error : non logged user tried to play";
		return sendAnswer(this,msg[0],answer_);
	}
	else {
		switch(msg[0]) {
			case 'F' : {//permet de fermer une conversation qui n'a pas abouti
				state_=FREE;
				return EXIT_SUCCESS;
			}
			case 'L' : {//login
				userId_.assign(&msg[2],length-2);
				answer_=" Bonjour "+userId_;
				state_=FREE;
				return sendAnswer(this,msg[0],answer_);
			}
			case 'M' : //prémices d'un match amical
				switch(msg[1]) {
					case 'S' : {//selection d'un manager
						dataRequest_.assign(&msg[2],length-2);
						std::cout<<userId_<<" veut inviter "<<dataRequest_<<" à un match amical"<<std::endl;
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->state_==FREE && server_->usersList_[i]->getUserId()==dataRequest_) {
								opponent_=server_->usersList_[i];
								opponent_->state_=MATCH_INVITED;
								state_=MATCH_INVITING;
								answer_=" "+userId_+" vous invite à jouer un match amical.";
								return sendAnswer(server_->usersList_[i],msg[0],answer_);
							}
						answer_="0Vous n'avez pas sélectionné un joueur disponible.";
						//le manager reste dans l'état MATCH_LIST tant qu'il n'envoie pas une commande F
						return sendAnswer(this,msg[0],answer_);
					}
					case '+': {//reponse positive à une invitation reçue sur le socket de l'invité
						const char space[]=" ";
						dataRequest_.assign(&msg[2],strcspn(&msg[2],space));
						std::cout<<userId_<<" est d'accord pour rencontrer "<<dataRequest_<<" en match amical"<<std::endl;
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->state_==MATCH_INVITING && server_->usersList_[i]->getUserId()==dataRequest_) {
								server_->usersList_[i]->state_=MATCH_INGAME;
								state_=MATCH_INGAME;
								answer_=" Ce joueur est d'accord !";
								tData.host=this;
								tData.opponent=opponent_;
								pthread_create(&thread,NULL,createMatch,(void*)&tData);
								return sendAnswer(server_->usersList_[i],msg[0],answer_);
							}
						state_=FREE;
						answer_="0Le joueur qui vous invitait n'est plus disponible !";
						return sendAnswer(this,msg[0],answer_);
					}
					case '-': {//reponse négative à une invitation
						const char space[]=" ";
						dataRequest_.assign(&msg[2],strcspn(&msg[2],space));
						answer_="0Ce joueur n'est pas d'accord.";
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->getUserId()==dataRequest_) {
								server_->usersList_[i]->state_=FREE;
								state_=FREE;
								return sendAnswer(server_->usersList_[i],msg[0],answer_);
							}
						break;
					}
					default : {//demande la liste des joueurs disponibles
						if(state_!=FREE) {
							answer_="0Internal error : user already busy in a conversation with the server.";
							return sendAnswer(this,msg[0],answer_);
						}
						state_=MATCH_LIST;
						answer_=' ';
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->state_==FREE) {
								answer_=answer_+server_->usersList_[i]->userId_+"\n";
								state_=MATCH_LIST;
							}
						if(answer_.length()==1) answer_="0Il n'y a pas d'autres managers disponibles.";
						else 	if (answer_.length()>=INPUTSIZE) {
									std::ostringstream convert;
									convert<<answer_.length()+1;
									std::string preAnswer_="1"+convert.str();
									return sendAnswer(this,msg[0],preAnswer_);
								}
						return sendAnswer(this,msg[0],answer_);
					}
				}
		}
	}
}

int User::sendAnswer(User * user, const char cmd, std::string answer) {
	char *buffer_ = new char[answer.length()+1];
	if (buffer_==NULL) return EXIT_FAILURE;
	buffer_[0]=cmd;
	strcpy(&buffer_[1],answer.c_str());
//	buffer_[answer_.length()+1]='\0';
	server_->sendToClient(user,buffer_,answer.length()+1);
	delete [] buffer_;
	return EXIT_SUCCESS;
}

void User::setDisconnection() {
	//ajouter ici les actions à prendre en cas de déconnection
	state_=DISCONNECTING;
}

bool User::isDisconnecting() {return (state_==DISCONNECTING);}

int User::getSockfd() {return sockfd_;}

std::string User::getUserId() {return userId_;}

//Status User::getState() {return state_;}
