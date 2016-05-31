#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *sm_rtrim(char *str)
{
    char *ptr;

    ptr = str + strlen(str) - 1;
    while(ptr >= str)
    {
        if(*ptr == '\r' || *ptr == '\n' || *ptr == '\t' || *ptr == ' ')
            ptr --;
        else
			break;
    }
	
	*(ptr + 1) = '\0';

	return str;
}

char *sm_ltrim(char *str)
{
    char *ptr;

    ptr = str;
    while(*ptr)
    {
        if(*ptr == '\r' || *ptr == '\n' || *ptr == '\t' || *ptr == ' ')
            ptr ++;
        else
            break;
    }

    strcpy(str, ptr);

	return str;
}

char *sm_ltrim_pos(char *str)
{
	char *p= str;
	while(*p)
	{
        if(*p == '\r' || *p == '\n' || *p == '\t' || *p == ' ')
            p++;
        else
            break;
	}

	return p;
}

char *sm_rtrim_pos(char *str)
{
    char *p= str + strlen(str) - 1;

    while(p>= str)
    {
        if(*p == '\r' || *p == '\n' || *p == '\t' || *p == ' ')
            p--;
        else
            break;
    }

    return p;
}


char *sm_trim(char *str)
{
    sm_rtrim(str);
    sm_ltrim(str);

	return str;
}

char *sm_strtoupper(char *src, char *dest)
{
    while(*src)
        *dest ++ = toupper(*src ++);

    *dest = '\0';

	return dest;
}

char *sm_strtolower(char *src, char *dest)
{
    while(*src)
        *dest ++ = tolower(*src ++);

    *dest = '\0';

	return dest;
}

/******************************************************************************
getword
  string���� SPACE�� ���еǾ����� �� �ܾ �����Ѵ�.
input : char *src
output: char *dst
return: int		src���� �Ѵܾ �ڸ� ��ġ
******************************************************************************/
int getword(char *src, char *dst)
{
	char *p;

	p = src;
	while(*p)
	{
		if(*p == ' ')
			break;
		*dst++ = *p++;
	}

	*dst = '\0';

	return p - src;
}

int getwordex(char *src, char *dst)
{
	int n;
	char *p;

	p = strpbrk(src, " \r\n\t\
");
	n = p - src;
	strncpy(dst, src, n);
	dst[n] = '\0';

	return n;
}

/******************************************************************************
getword1
  �����ڸ� �����Ͽ� �Ѵܾ �����Ѵ�.
input : char *src
		char sep
output: char *dst
return: char *
******************************************************************************/
int getword1(char *src, char *dst, char sep)
{
	char *p;

	p = src;
	while(*p)
	{
		if(*p == sep)
			break;
		*dst++ = *p++;
	}

	*dst = '\0';

	return p - src;
}

/******************************************************************************
getword2
  SPACE�� �⺻���� �ܾ �����ϰ� �����ڸ� �����Ͽ� �Ѵܾ �����Ѵ�.
input : char *src
		char sep
output: char *dst
return: char *
******************************************************************************/
int getword2(char *src, char *dst, char sep)
{
	char *p;

	p = src;
	while(*p)
	{
		if(*p == sep || *p == ' ')
			break;
		*dst++ = *p++;
	}

	*dst = '\0';

	return p - src;
}

int getline(char *src, char *dst)
{
	char *p;

	p = src;
	while(*p)
	{
		if(*p == '\n' || *p == '\r')
			break;
		*dst++ = *p++;
	}

	*dst = '\0';

	if(*p == '\r' || *p == '\n')
		p ++;

	return p - src;
}

/******************************************************************************
StringCopy
 String�� ���̸�ŭ ���۸� �Ҵ��Ͽ� �����Ѵ�.
 ����� ������ �޸𸮰� �Ҵ�Ǿ� ������ �ٽ� �Ҵ��Ѵ�.
input : char *src
output: char **dst	�ݵ�� ������ �����̾�� �Ѵ�.
return: char *
******************************************************************************/
char *StringCopy(char **dst, char *src)
{
	if(!src) return NULL;

	if(*dst)
		delete[] dst;

	*dst = new char[strlen(src) + 1];
	if(*dst)
		strcpy(*dst, src);
	else
	{
		cout << "Memory allocation error\n";
		return NULL;
	}
		
	return *dst;
}

char *StringCat(char **dst, char *src)
{
	char *p;
	int len;

	if(!src) return *dst;

	p = *dst;

	if(*dst == NULL)
		return StringCopy(dst, src);

	len = strlen(*dst) + strlen(src) + 1;
	*dst = new char[len];

	if(*dst)
	{
		strcpy(*dst, p);
		strcat(*dst, src);
		delete[] p;
	}
	else
	{
		*dst = p;
		cout << "Memory allocation error\n";
	}
		
	return *dst;
}

/******************************************************************************
ParseUrlString
 Url string�� �Ľ��Ͽ� �ش��ϴ� �������� �����ش�

input : char *Url	Url string
output: char **host	host name
		char **id	identifier
		int port	port number
return: TRUE/FALSE 
etc   : **host�� **id�� ȣ���ϴ� �������� �ݵ�� *������ �����ϰ� �޸��Ҵ���
		���� ���ƾ� �Ѵ�. �׸��� ȣ���Ŀ��� ���̻� ������� ������ �޸𸮸� 
		������ �ֿ��� �Ѵ�.
******************************************************************************/
int ParseUrlString(char *Url, char **host, char **id, int *port)
{
	char *p, *tmp;
	char *h, *i;
	int po;

	tmp = new char[strlen(Url) + 1];
	strcpy(tmp, Url);

	// domain�� path�� ����
    p = strchr(tmp, '/');
    if(!p)
    {
        i = new char[2];
        strcpy(i, "/");
    }
    else
    {
        i = new char[strlen(p) + 1];
        strcpy(i, p);
        *p = '\0';
    }

    h = new char[strlen(tmp) + 1];
    strcpy(h, tmp);

    // host�� port�� �и�
    p = strchr(h, ':');
    if(!p)
        po = 80;
    else
    {
        *p = '\0';
        po = atoi(p + 1);
    }

	*host = h;
	*id = i;
	*port = po;

	delete[] tmp;

	return 1;
}

/******************************************************************************
ConcatenatePath
  ���丮 �ΰ��� ���丮 ��θ� �ϳ��� ��ģ��.
  ��) str1 = /aa/b/c/d.dat
      str2 = ./../../b.dat
	  ��� : /aa/b.dat
	  ���� str2�� �н��� '/'�� �����ϸ� str2�� ������ �����ش�.
input : char *s1		���� ���丮 ���
		char *s2		�߰���ų ���丮 ���
output:	char *result	��ģ ���	
return: char *			result�� ���� ������
******************************************************************************/
char *ConcatenatePath(char *s1, char *s2, char *result)
{
	char tmp[4096], *p1;

	// s2�� ó�� ���ڰ� '/'�̸� ��Ʈ�����̹Ƿ� s2�� ����� ����
	if(s2[0] == '/')
	{
		strcpy(result, s2);
		return result;
	}

	// cgi�� ���� ���� cgi�� parm�� ����
	strcpy(tmp, s1);
	p1 = strchr(tmp, '?');
	if(p1)
		*p1 = '\0';

	p1 = strrchr(tmp, '/');
	if(p1)
		*p1 = '\0';
	else
		tmp[0] = '\0';

	while(1)
	{
		if(s2[0] == '.')
		{
			if(s2[1] == '.')
			{
				if(s2[2] == '/')
				{
					s2 = s2 + 3;
					p1 = strrchr(tmp, '/');
					if(p1)
						*p1 = '\0';
					else
						s1[0] = '\0';

					continue;
				}
			}
			else if(s2[1] == '/')
			{
				s2 = s2 + 2;
				continue;
			}

			break;
		}
		else if(s2[0] == '/')
			s2 ++;
		else
			break;
	}

	sprintf(result, "%s/%s", tmp, s2);

	return result;
}

/******************************************************************************
CorrectPath
  Full path�� �����Ѵ�.
  ��) aa/bb/./cc/// => aa/bb/cc/

input : char *src
output: char *dst
return: 
******************************************************************************/
void CorrectPath(char *src, char *dst)
{
	char *p = src;
	char *r = dst;
	
	while(*p)
	{
		if(*p == '/')
		{
 			if(*(p + 1) == '/')
			{
				p ++;
				continue;
			}
			else if(*(p + 1) == '.')
			{
				if(*(p + 2) == '/')
				{
					p += 2;
					continue;
				}
				else if(*(p + 2) == '.' && *(p + 3) == '/')
				{
					p += 3;
					
					while(r > dst && *r != '/')
						r --;
					continue;
				}
			}
		}
		*r++ = *p ++;
	}
	*r = '\0';
}

/******************************************************************************
sm_DeleteChar
  ���ڿ��߿� Ư�� ���ڸ� �������� ���� ���ڿ��� ������ �ٿ� �ִ´�
input : char *str		�Էµ� ���ڿ�
		char DelChar	���� ����
******************************************************************************/
char *sm_DeleteChar(char *str, char DelChar)
{
	char *p;
	char *w;

	p = str;
	w = str;
	while(*p)
	{
		if(*p == DelChar)
			continue;

		*w ++ = *p ++;
	}
	*w = '\0';

	return str;
}

/******************************************************************************
sm_DeleteCharStr
  ���ڿ��߿� Ư�� ���ڵ��� �������� ���� ���ڿ��� ������ �ٿ� �ִ´�
input : char *str		�Էµ� ���ڿ�
		char *DelChar	���� ���� ����Ʈ
******************************************************************************/
char *sm_DeleteCharStr(char *str, char *DelChar)
{
	char *p;
	char *w;
	char *s;
	int flag;

	p = str;
	w = str;
	while(*p)
	{
		s = DelChar;
		flag = 0;
		while(*s)
		{
			if(*p == *s ++)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 1)
			continue;

		*w ++ = *p ++;
	}
	*w = '\0';

	return str;
}

int strncasecmp(char *src, char *dst, int size)
{
	int i;
	char s;
	char d;

	for(i = 0; i < size; i ++)
	{
		if(*src == 0)
		{
			if(*dst == 0)
				return 0;
			else
				return -1;
		}
		if(*dst == 0)
			return 1;

		s = *src++;
		d = *dst++;
		if(s >= 'a' && s <= 'z')
			s = s - ('a' - 'A');
		if(d >= 'a' && d <= 'z')
			d = d - ('a' - 'A');

		if(s < d)
			return -1;
		else if(s > d)
			return 1;

	}

	return 0;
}

int strcasecmp(char *src, char *dst)
{
	char s;
	char d;

	while(*src)
	{
		if(*dst == 0)
			return 1;

		s = *src++;
		d = *dst++;
		if(s >= 'a' && s <= 'z')
			s = s - ('a' - 'A');
		if(d >= 'a' && d <= 'z')
			d = d - ('a' - 'A');

		if(s < d)
			return -1;
		else if(s > d)
			return 1;

	}

	if(*dst)
		return -1;

	return 0;
}
