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
  string에서 SPACE로 구분되어지는 한 단어를 추출한다.
input : char *src
output: char *dst
return: int		src에서 한단어를 자른 위치
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
  구분자를 지정하여 한단어를 추출한다.
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
  SPACE를 기본으로 단어를 구분하고 구분자를 지정하여 한단어를 추출한다.
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
 String의 길이만큼 버퍼를 할당하여 복사한다.
 복사될 변수에 메모리가 할당되어 있으면 다시 할당한다.
input : char *src
output: char **dst	반드시 포인터 변수이어야 한다.
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
 Url string을 파싱하여 해당하는 정보들을 돌려준다

input : char *Url	Url string
output: char **host	host name
		char **id	identifier
		int port	port number
return: TRUE/FALSE 
etc   : **host와 **id는 호출하는 곳에서는 반드시 *변수로 선언만하고 메모리할당을
		하지 말아야 한다. 그리고 호출후에는 더이상 사용하지 않을때 메모리를 
		해제해 주여야 한다.
******************************************************************************/
int ParseUrlString(char *Url, char **host, char **id, int *port)
{
	char *p, *tmp;
	char *h, *i;
	int po;

	tmp = new char[strlen(Url) + 1];
	strcpy(tmp, Url);

	// domain과 path를 구분
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

    // host와 port를 분리
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
  디렉토리 두개의 디렉토리 경로를 하나로 합친다.
  예) str1 = /aa/b/c/d.dat
      str2 = ./../../b.dat
	  결과 : /aa/b.dat
	  만일 str2의 패스가 '/'로 시작하면 str2의 내용을 돌려준다.
input : char *s1		이전 디렉토리 경로
		char *s2		추가시킬 디렉토리 경로
output:	char *result	합친 결과	
return: char *			result에 대한 포인터
******************************************************************************/
char *ConcatenatePath(char *s1, char *s2, char *result)
{
	char tmp[4096], *p1;

	// s2의 처음 글자가 '/'이면 루트부터이므로 s2만 결과에 복사
	if(s2[0] == '/')
	{
		strcpy(result, s2);
		return result;
	}

	// cgi가 있을 경우는 cgi를 parm을 삭제
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
  Full path를 정정한다.
  예) aa/bb/./cc/// => aa/bb/cc/

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
  문자열중에 특정 문자를 삭제한후 뒤의 문자열은 앞으로 붙여 넣는다
input : char *str		입력될 문자열
		char DelChar	지울 문자
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
  문자열중에 특정 문자들을 삭제한후 뒤의 문자열은 앞으로 붙여 넣는다
input : char *str		입력될 문자열
		char *DelChar	지울 문자 리스트
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
