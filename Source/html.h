/******************************************************************************
Html Parser
******************************************************************************/

#ifndef __HTML_PARSER_H__
#define __HTML_PARSER_H__

#include <stdio.h>
#include <string.h>
#include <iostream>

#define OPEN_TAG '<'
#define CLOSE_TAG '>'
#define STRING_TAG1 '\'' 
#define STRING_TAG2 '\"' 


// whatIsTag 에서 리턴할 값
#define SCRIPT_TAG 1
#define STYLE_TAG 2
#define COMMENT_TAG 3
#define VALID_TAG 9
#define INVALID_TAG 0


#define MAX_URL_LEN 2048
#define NAME_SIZE 64

void skipUntilCloseTag(char **buf);
void skipBlank(char **buf);
int whatIsTag(char *buf);
int normalizationString(char *buf);
int transCodeSet(char *buf);
int rmSpecialChar(char *buf);
char *getTerm(char **buf, char *sep);
char *ConcatenateUrl(char *s1, char *s2, char *result);


typedef struct 
{
	char code[8];
	int	dec;
} codeSet;


class TagVar;
class HtmlTag;
class HtmlParser;

class TagVar
{
public:
	TagVar *nextTagVar;

	TagVar();
	~TagVar();

	int setVar(char *var);
	int setVal(char *val);

	char *getVar() const;
	char *getVal() const;

	char *szVar, *szVal;	// 변수이름과 값을 저장
};


class HtmlTag
{
public:
	HtmlTag *nextHtmlTag;	// 다음번 HtmlTag를 가리킴
	TagVar *firstTagVar;	// HtmlTag안의 첫번째 Variale이 저장될 TagVar를 가리킴
	TagVar *lastTagVar;	// HtmlTag안의 첫번째 Variale이 저장될 TagVar를 가리킴
	char szTagName[NAME_SIZE];	// Tag Name 저장

	HtmlTag();
	~HtmlTag();

	TagVar *getTagVar(char *szName) const;
	char *getTagName() const;	

	int setTagName(char *name);

	int addTagVar(TagVar *tv);
	void print() const;
};


class HtmlParser
{
public:
	HtmlTag *m_firstHtmlTag;	// 문서의 첫번째 HtmlTag를 가리킴.
	HtmlTag *m_lastHtmlTag;	// 문서의 첫번째 HtmlTag를 가리킴.

	char *m_szTitle, *meta_abs, *meta_key, *meta_type, *meta_char, *date;
	int size;
	char *m_szUrl;
	unsigned long ip;


	HtmlParser();
	~HtmlParser();

	void Destroy();
	bool Initial();

	HtmlTag *getHtmlTag(char *tagName, HtmlTag *startNode) const;
	HtmlTag *getHtmlTagVal(char *tagName, char *memberName, HtmlTag *startNode, char **Val) const;

	HtmlTag *getUrlAnchor(char **url, char **anchor, HtmlTag *ht);
	HtmlTag *getFrameLink(char **url, HtmlTag *ht);

	int	LoadHtml(char* szFIleName);

	char *getUrl() const;
	unsigned long getIp() const;

	// ---------------
	void setTitle(char* szVal);
	void setUrl(char *url);
	void setUrl(char *hostname, char *identifier, int port);
	void setDate(char *Date);
	void setIp(unsigned long ip);
	void setAbsPath(char *pPath);
	// ---------------

	int Ananysis();
	int exeParser(char *szHtmlTxt);
	int writeToFile(FILE *fp, int *Offset, int *Length);
	int writeToFile(char *file, int *Offset, int *Length);
	void print() const;
	int addHtmlTag(HtmlTag *ht);

	HtmlTag *parseHtmlTag(char **buf);
	HtmlTag *parseTextData(char **buf);
	HtmlTag *parseScript(char **buf);
	HtmlTag *parseStyle(char **buf);
	HtmlTag *parseComment(char **buf);
	int parseVarVal(HtmlTag *ht, char **buf);

	char *MakeFullUrl(char *src);
};

#endif

