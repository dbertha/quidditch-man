#ifndef COMMONMGR_H
#define COMMONMGR_H

class Server;
class CommonMgr {
public:
	CommonMgr(Server *);

private:
	Server* server_;
};

#endif
