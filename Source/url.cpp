#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "url.h"
#include "strutil.h"

URL::URL()
{
	hostname = NULL;
	identifier = NULL;
	ipaddress = 0;
	LinkText = NULL;
	Port = 0;
	Cycle = 0;
}

URL::URL(unsigned char *Str)
{
	GetUrlInfo(Str);
	LinkText = NULL;
}

URL::URL(URL *Url)
{
	hostname = NULL;
	identifier = NULL;

	hostname = new unsigned char[strlen((char *)Url->hostname) + 1];
	strcpy((char *)hostname, (char *)Url->hostname);
	ipaddress = Url->ipaddress;
	Port = Url->Port;

	identifier = new unsigned char[strlen((char *)Url->identifier) + 1];
	strcpy((char *)identifier, (char *)Url->identifier);

	Cycle = Url->Cycle;

	if(Url->LinkText)
	{
		LinkText = new char[strlen(Url->LinkText) + 1];
		strcpy(LinkText, Url->LinkText);
	}
	else
		LinkText = NULL;

}

URL::URL(URL &Url)
{
	hostname = NULL;
	identifier = NULL;

	hostname = new unsigned char[strlen((char *)Url.hostname) + 1];
	strcpy((char *)hostname, (char *)Url.hostname);
	ipaddress = Url.ipaddress;
	Port = Url.Port;

	identifier = new unsigned char[strlen((char *)Url.identifier) + 1];
	strcpy((char *)identifier, (char *)Url.identifier);

	Cycle = Url.Cycle;

	if(Url.LinkText)
	{
		LinkText = new char[strlen(Url.LinkText) + 1];
		strcpy(LinkText, Url.LinkText);
	}
	else
		LinkText = NULL;
}

URL::~URL()
{
	Destroy();
}

void URL::operator=(URL url)
{
	if(hostname)
		delete[] hostname;
	if(identifier)
		delete[] identifier;

	hostname = new unsigned char[strlen((char *)url.hostname) + 1];
	strcpy((char *)hostname, (char *)url.hostname);
	ipaddress = url.GetIP();
	Port = url.GetPort();

	identifier = new unsigned char[strlen((char *)url.identifier) + 1];
	strcpy((char *)identifier, (char *)url.GetIdentifier());

	Cycle = url.Cycle;

	if(url.LinkText)
	{
		LinkText = new char[strlen(url.LinkText) + 1];
		strcpy(LinkText, url.LinkText);
	}
	else
		LinkText = NULL;
}

int URL::GetUrlInfo(unsigned char *Str)
{
	char tmp[4096];
    char *p;

    strcpy(tmp, (char *)Str);

    // domain과 path를 구분
    p = strchr(tmp, '/');
    if(!p)
	{
		identifier = new unsigned char[2];
		strcpy((char *)identifier, "/");
	}
    else
    {
		identifier = new unsigned char[strlen(p) + 1];
        strcpy((char *)identifier, p);
		*p = '\0';
    }

	hostname = new unsigned char[strlen(tmp) + 1];
    strcpy((char *)hostname, tmp);

    // host와 port를 분리
    p = strchr((char *)hostname, ':');
    if(!p)
        Port = 80;
    else
    {
        *p = '\0';
        Port = atoi(p + 1);
    }

	ipaddress = 0;
	Cycle = 0;

    return 1;
}

unsigned char *URL::GetHostName()
{
	return hostname;
}

unsigned long URL::GetIP()
{
	return ipaddress;
}

short int URL::GetPort()
{
	return Port;
}

unsigned char *URL::GetIdentifier()
{
	return identifier;
}

unsigned long URL::GetGroup()
{
	return GetGroupId(ipaddress);
}

int URL::GetFourthByteOfIP(unsigned long ip)
{
	IPMask Mask;

	Mask.Ip = ip;
	return Mask.Class[3];
}

unsigned long URL::GetGroupId(unsigned long ipaddress)
{
	IPMask Mask;

	Mask.Ip = ipaddress;
	Mask.Class[3] = 0;
	return Mask.Ip;
}

int URL::Compare(URL *ob)
{
	if(ob->ipaddress < ipaddress) 
		return -1;
	else if(ob->ipaddress > ipaddress) 
		return 1;

	return strcmp((char *)ob->identifier, (char *)identifier);
}

void URL::Copy(URL *ob)
{
	if(hostname)
		delete[] hostname;
	if(identifier)
		delete[] identifier;

	strcpy((char *)hostname, (char *)ob->hostname);
	ipaddress = ob->ipaddress;
	strcpy((char *)identifier, (char *)ob->identifier);
	Port = ob->Port;

	Cycle = ob->Cycle;

	if(ob->LinkText)
	{
		LinkText = new char[strlen(ob->LinkText) + 1];
		strcpy(LinkText, ob->LinkText);
	}
	else
		LinkText = NULL;
}

void URL::MakeUrl(unsigned char *host, unsigned long ip, char *id, short int port, int Cycle)
{
	if(hostname) 
		delete[] hostname;
	hostname = new unsigned char[strlen((char *)host) + 1];
	strcpy((char *)hostname, (char *)host);
	ipaddress = ip;

	if(identifier) 
		delete[] identifier;
	identifier = new unsigned char[strlen(id) + 1];
	strcpy((char *)identifier, (char *)id);
	Port = port;
	this->Cycle = Cycle;
}

void URL::Destroy()
{
	if(hostname) 
		delete[] hostname;
	if(identifier) 
		delete[] identifier;

	hostname = NULL;
	identifier = NULL;

	Port = 80;
	Cycle = 0;

	if(LinkText)
		delete[] LinkText;
}

unsigned char *URL::GetFullUrl()
{
	unsigned char *p;

	p = new unsigned char[4096];
	sprintf((char *)p, "%s:%d%s", (char *)hostname, Port, (char *)identifier);

	if(strlen((char *)p) > 4095)
	{
		printf("URL::GetFullUrl : Buffer overflow!!\n");
		exit(1);
	}

	return p;
}

void URL::SetIp(unsigned long ip)
{
	ipaddress = ip;
}

void URL::SetHostName(unsigned char *host)
{
	if(hostname)
		delete[] hostname;

	hostname = new unsigned char[strlen((char *)host) + 1];
	strcpy((char *)hostname, (char *)host);
}

int URL::SetIdentifier(unsigned char *Id)
{
	char *newid;
	char *tmp;
	char *IdTmp;
	char *p;
	char *pos[128];	// Id의 각 경로를 저장할 스택
	int i;
	int len;
	int j;

	tmp = new char[strlen((char *)identifier) + 1];
	strcpy(tmp, (char *)identifier);
	sm_trim(tmp);

	// make stack list
	p = strrchr(tmp, '/');
	if(p)
		*p = 0;
	else
		tmp[0] = 0;

	p = tmp;
	i = -1;
	while(*p)
	{
		if(*p == '/')
		{
			i ++;
			if(i > 127)
			{
				printf("\t!! Invalid identifier : %s\n",identifier);
				delete[] tmp;

				return -1;
			}

			*p = 0;
			pos[i] = p + 1;
		}

		p ++;
	}

	IdTmp = new char[strlen((char *)Id) + 1];
	strcpy(IdTmp, (char *)Id);
	sm_trim(IdTmp);

	p = (char *)IdTmp;
	if(Id[0] == '/')
		i = -1;

	while(*p)
	{
		if(*p != '.')
		{
			i ++;

			if(i > 127)
			{
				printf("\t!! Invalid identifier : %s\n",identifier);
				delete[] tmp;
				delete[] IdTmp;
				return -1;
			}

			if(i < 0) // path error
			{
				delete[] tmp;
				delete[] IdTmp;
				return i;
			}

			if(*p == '/')
				p ++;

			pos[i] = p;
			while(*p != '/' && *p != 0)
				p ++;

			if(*p == 0)
				break;
	
			*p = 0;
			p ++;
		}
		else
		{
			p ++;
			if(*p == '/')
				p ++;
			else if(*p == '.')
			{
				p ++;
				if(*p == '/')
				{
					i --;
					if(i < -1) // path error
					{
						delete[] tmp;
						delete[] IdTmp;
						return i;
					}

					p ++;
				}
				else
				{
					i ++;

					if(i > 127)
					{
						printf("\t!! Invalid identifier : %s\n",identifier);
						delete[] tmp;
						delete[] IdTmp;
						return -1;
					}
					if(i < 0) // path error
					{
						delete[] tmp;
						delete[] IdTmp;
						return i;
					}

					pos[i] = p - 2;
					while(*p != '/' && *p != 0)
						p ++;

					if(*p == 0)
						break;

					*p = 0;
					p ++;
				}
			}
			else
			{
				i ++;

				if(i > 127)
				{
					printf("\t!! Invalid identifier : %s\n",identifier);
					delete[] tmp;
					delete[] IdTmp;
					return -1;
				}
				if(i < 0) // path error
				{
					delete[] tmp;
					delete[] IdTmp;
					return i;
				}

				pos[i] = p - 1;
				while(*p != '/' && *p != 0)
					p ++;

				if(*p == 0)
					break;

				*p = 0;
				p ++;
			}
		}
	}

	if(i < -1)
	{
		delete[] tmp;
		delete[] IdTmp;
		return i;
	}

	if(i == -1)
	{
		strcpy((char *)identifier, "/");
		delete[] IdTmp;
		delete[] tmp;
		return 1;	
	}
	if(i > 127)
	{
		printf("\t!! Invalid identifier : %s\n",identifier);
		delete[] tmp;
		delete[] IdTmp;
		return -1;
	}

	len = 0;
	for(j = 0; j <= i; j ++)
		len += strlen(pos[j]);

	newid = new char[len + i + 2];
	newid[0] = 0;
	for(j = 0; j <= i; j ++)
	{
		strcat(newid, "/");
		strcat(newid, pos[j]);
	}

	delete[] tmp;
	delete[] IdTmp;
	delete[] identifier;

	identifier = (unsigned char *)newid;

	return 1;
}

int URL::GetDepth()
{
	char *p;
	int i = 0;

	p = (char *)identifier;

	while((p = strchr(p, '/')) != NULL)
	{
		i ++;
		p ++;
	}

	return i;
}

void URL::SetCycle(int Cycle)
{
	this->Cycle = Cycle;
}

int URL::GetCycle()
{
	return Cycle;
}

int URL::GetMime(char *Mime)
{
	char *p;
	char *s;

	p = strrchr((char *)identifier, '/');
	if(!p)
		return FALSE;

	s = strchr(p, '.');
	while(s)
	{
		p = s;
		s = strchr(p + 1, '.');
	}

	if(!p)
		return FALSE;

	if(*p != '.')
		return FALSE;

	p ++;
	s = strchr(p, '?');
	if(s)
	{
		*s = 0;
		if(strlen(p) > 32)
		{
			*s = '?';
			return FALSE;
		}
		
		strcpy(Mime, p);
		*s = '?';
	}
	else
	{
		if(strlen(p) > 32)
		{
			return FALSE;
		}
		
		strcpy(Mime, p);
	}

	return TRUE;
}

int URL::SetLinkText(char *LinkText)
{
	if(this->LinkText)
		delete[] this->LinkText;

	this->LinkText = new char[strlen(LinkText) + 1];
	strcpy(this->LinkText, LinkText);

	return 1;
}

char * URL::GetLinkText()
{
	return LinkText;
}
