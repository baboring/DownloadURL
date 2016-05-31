#ifndef __URL_H__
#define __URL_H__

#include <iostream>

#include "strutil.h"

#define INVALID	1
#define TIMEOUT	2
#define FAIL	3
#define RETRY	4

class URL 
{
public:
	char * GetLinkText();
	int SetLinkText(char *LinkText);
	unsigned char *hostname;
	unsigned char *identifier;
	unsigned long ipaddress;
	char *LinkText;
	short int Port;
	int Cycle;

	URL();
	URL(unsigned char *s);
	URL(URL *u);
	URL(URL &u);
	~URL();
	void operator=(URL url);
	void Destroy();

	unsigned long GetGroup();
	unsigned long GetGroupId(unsigned long ipaddress);
	int GetFourthByteOfIP(unsigned long ip);
	int Compare(URL *ob);
	void Copy(URL *ob);
	void MakeUrl(unsigned char *host, unsigned long ip, char *id, short int port = 80, int Cycle = 0);
	unsigned char *GetFullUrl();
	int GetUrlInfo(unsigned char *Str);
	unsigned char *GetHostName();

	void SetIp(unsigned long ip);
	void SetHostName(unsigned char *host);
	int SetIdentifier(unsigned char *Id);

	unsigned long GetIP();
	short int GetPort();
	unsigned char *GetIdentifier();

	int GetDepth();
	void SetCycle(int Cycle);
	int GetCycle();

	int GetMime(char *Mime);
};

#endif
