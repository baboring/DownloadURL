/******************************************************************************
******************************************************************************/

#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <fstream >

#include "url.h"
#include "html.h"

/************************************************************************
tagList�� �ݵ�� ������������ ������ �Ǿ�� ��
�񱳽� strncasecmp ����ϱ� ������ �������� ������ �ƴϸ�
�߸��� ����� ���� �� ����
************************************************************************/
#define NO 300	// Number of Tag
char *tagList[NO]={
					"VAR", "UL", "U", "TT", "TR",
					"TITLE", "THEAD", "TH", "TFOOT", "TEXTAREA",
					"TD", "TBODY", "TABLE", "SUP", "SUB",
					"STYLE", "STRONG", "STRIKE", "SPAN", "SMALL",
					"SELECT", "SCRIPT", "SAMP", "S", "Q",
					"PRE", "PARAM", "P", "OPTION", "OPTGROUP",
					"OL", "OBJECT", "NOSCRIPT", "NOFRAMES", "META",
					"MENU", "MAP", "LINK", "LI", "LEGEND",
					"LABEL", "KBD", "ISINDEX", "INS", "INPUT",
					"IMG", "IFRAME", "I", "HTML", "HR",
					"HEAD", "H6", "H5", "H4", "H3",
					"H2", "H1", "FRAMESET", "FRAME", "FORM",
					"FONT", "FIELDSET", "EMBED", "EM", "DT",
					"DL", "DIV", "DIR", "DFN", "DEL",
					"DD", "COLGROUP", "COL", "CODE", "CITE",
					"CENTER", "CAPTION", "BUTTON", "BR", "BODY",
					"BLOCKQUOTE", "BIG", "BDO", "BASEFONT", "BASE",
					"B", "AREA", "APPLET", "ADDRESS", "ACRONYM",
					"ABBR", "A", "/VAR", "/UL", "/U",
					"/TT", "/TR", "/TITLE", "/THEAD", "/TH",
					"/TFOOT", "/TEXTAREA", "/TD", "/TBODY", "/TABLE",
					"/SUP", "/SUB", "/STYLE", "/STRONG", "/STRIKE",
					"/SPAN", "/SMALL", "/SELECT", "/SCRIPT", "/SAMP",
					"/S", "/Q", "/PRE", "/PARAM", "/P",
					"/OPTION", "/OPTGROUP", "/OL", "/OBJECT", "/NOSCRIPT",
					"/NOFRAMES", "/MENU", "/LI", "/LEGEND", "/LABEL",
					"/KBD", "/ISINDEX", "/INS", "/IFRAME", "/I",
					"/HTML", "/HEAD", "/H6", "/H5", "/H4",
					"/H3", "/H2", "/H1", "/FRAMESET", "/FRAME",
					"/FORM", "/FONT", "/FIELDSET", "/EMBED", "/EM",
					"/DT", "/DL", "/DIV", "/DIR", "/DFN",
					"/DEL", "/DD", "/COLGROUP", "/COL", "/CODE",
					"/CITE", "/CENTER", "/CAPTION", "/BUTTON", "/BODY",
					"/BLOCKQUOTE", "/BIG", "/BDO", "/B", "/APPLET",
					"/ADDRESS", "/ACRONYM", "/ABBR", "/A", "!DOCTYPE",
					"!--", "",
				};


#define CODE_NUM	400
codeSet code[CODE_NUM]={{"&#34", 34}, {"&quot", 34}, {"&#38", 38}, {"&aml", 38},
	{"&#60", 60}, {"&lt", 60}, {"&#62", 62}, {"&gt", 62},
	{"&#160", 32}, {"&nbsp", 32}, {"&#161", 161}, {"&iexcl", 161},
	{"&#162", 162}, {"&cent", 162}, {"&#163", 163}, {"&pound", 163},
	{"&#164", 164}, {"&curren", 164}, {"&#165", 165}, {"&yen", 165},
	{"&#166", 166}, {"&brkbar", 166}, {"&#167", 167}, {"&sect", 167},
	{"&#168", 168}, {"&uml", 168}, {"&#169", 169}, {"&copy", 169},
	{"&#170", 170}, {"&ordf", 170}, {"&#171", 171}, {"&laquo", 171},
	{"&#172", 172}, {"&not", 172}, {"&#173", 173}, {"&shy", 173},
	{"&#174", 174}, {"&reg", 174}, {"&#175", 175}, {"&hibar", 175},
	{"&#176", 176}, {"&deg", 176}, {"&#177", 177}, {"&plusmn", 177},
	{"&#178", 178}, {"&sup2", 178}, {"&#179", 179}, {"&sup3", 179},
	{"&#180", 180}, {"&acute", 180}, {"&#181", 181}, {"&micro", 181},
	{"&#182", 182}, {"&para", 182}, {"&#183", 183}, {"&middot", 183},
	{"&#184", 184}, {"&cedil", 184}, {"&#185", 185}, {"&sup1", 185},
	{"&#186", 186}, {"&ordm", 186}, {"&#187", 187}, {"&raquo", 187},
	{"&#188", 188}, {"&frac14", 188}, {"&#189", 189}, {"&frac12", 189},
	{"&#190", 190}, {"&frac34", 190}, {"&#191", 191}, {"&iquest", 191},
	{"&#192", 192}, {"&Agrave", 'A'}, {"&#193", 193}, {"&Aacute", 'A'},
	{"&#194", 194}, {"&Acirc", 194}, {"&#195", 195}, {"&Atilde", 'A'},
	{"&#196", 196}, {"&Auml", 196}, {"&#197", 197}, {"&Aring", 197},
	{"&#198", 198}, {"&Aelig", 198}, {"&#199", 199}, {"&Ccedil", 199},
	{"&#200", 200}, {"&Egrave", 'E'}, {"&#201", 201}, {"&Eacute", 'E'},
	{"&#202", 202}, {"&Ecirc", 202}, {"&#203", 203}, {"&Euml", 203},
	{"&#204", 204}, {"&Igrave", 'I'}, {"&#205", 205}, {"&Iacute", 'I'},
	{"&#206", 206}, {"&Icirc", 206}, {"&#207", 207}, {"&Iuml", 207},
	{"&#208", 208}, {"&ETH", 208}, {"&#209", 209}, {"&Ntilde", 'N'},
	{"&#210", 210}, {"&Ograve", 'O'}, {"&#211", 211}, {"&Oacute", 'O'},
	{"&#212", 212}, {"&Ocirc", 212}, {"&#213", 213}, {"&Otilde", 'O'},
	{"&#214", 214}, {"&Ouml", 214}, {"&#215", 215}, {"&times", 215},
	{"&#216", 216}, {"&Oslash", 216}, {"&#217", 217}, {"&Ugrave", 'U'},
	{"&#218", 218}, {"&Uacute", 'U'}, {"&#219", 219}, {"&Ucirc", 219},
	{"&#220", 220}, {"&Uuml", 220}, {"&#221", 221}, {"&Yacute", 'Y'},
	{"&#222", 222}, {"&THORN", 222}, {"&#223", 223}, {"&szlig", 223},
	{"&#224", 224}, {"&agrave", 'a'}, {"&#225", 225}, {"&aacute", 'a'},
	{"&#226", 226}, {"&acirc", 226}, {"&#227", 227}, {"&atilde", 'a'},
	{"&#228", 228}, {"&auml", 228}, {"&#229", 229}, {"&aring", 229},
	{"&#230", 230}, {"&aelig", 230}, {"&#231", 231}, {"&ccedil", 231},
	{"&#232", 232}, {"&egrave", 'e'}, {"&#233", 233}, {"&eacute", 'e'},
	{"&#234", 234}, {"&ecirc", 234}, {"&#235", 235}, {"&euml", 235},
	{"&#236", 236}, {"&igrave", 'i'}, {"&#237", 237}, {"&iacute", 'i'},
	{"&#238", 238}, {"&icirc", 238}, {"&#239", 239}, {"&iuml", 239},
	{"&#240", 240}, {"&eth", 240}, {"&#241", 241}, {"&ntilde", 'n'},
	{"&#242", 242}, {"&ograve", 'o'}, {"&#243", 243}, {"&oacute", 'o'},
	{"&#244", 244}, {"&ocirc", 244}, {"&#245", 245}, {"&otilde", 'o'},
	{"&#246", 246}, {"&ouml", 246}, {"&#247", 247}, {"&divide", 247},
	{"&#248", 248}, {"&oslash", 248}, {"&#249", 249}, {"&ugrave", 'u'},
	{"&#250", 250}, {"&uacute", 'u'}, {"&#251", 251}, {"&ucirc", 251},
	{"&#252", 252}, {"&uuml", 252}, {"&#253", 253}, {"&yacute", 'y'},
	{"&#254", 254}, {"&thorn", 254}, {"&#255", 255}, {"&yuml", 255}
	};


HtmlParser::HtmlParser()
{
	m_szUrl = NULL;
	m_szTitle  = NULL;

	m_firstHtmlTag = NULL;
    m_lastHtmlTag = NULL;

	meta_abs = NULL;
	meta_key  = NULL;
	meta_type  = NULL;
	meta_char = NULL;
	date = NULL;
	size = 0;
}

HtmlParser::~HtmlParser()
{
	Destroy();
}
void HtmlParser::Destroy()
{
	SAFE_DELETE_ARRAY(m_szUrl);
	SAFE_DELETE_ARRAY(m_szTitle);
	SAFE_DELETE_ARRAY(meta_abs);
	SAFE_DELETE_ARRAY(meta_key);
	SAFE_DELETE_ARRAY(meta_type);
	SAFE_DELETE_ARRAY(meta_char);
	SAFE_DELETE_ARRAY(date);

	HtmlTag *next;
	while(m_firstHtmlTag) 
	{
		next = m_firstHtmlTag->nextHtmlTag;
		delete m_firstHtmlTag;
		m_firstHtmlTag = next;
	}
}
bool HtmlParser::Initial()
{
	setUrl("");
	setTitle("");

	meta_abs = new char[10];
	strcpy(meta_abs,"");
	meta_key = new char[10];
	strcpy(meta_key,"");
	meta_type = new char[10];
	strcpy(meta_type,"");
	meta_char = new char[10];
	strcpy(meta_char,"");

	setDate("");

	return true;
}
int HtmlParser::writeToFile(char *file, int *Offset, int *Length)
{
	FILE *fp;

	if(!m_firstHtmlTag) 
		return 0;
	if(!(fp=fopen(file, "w"))) 
		return 0;

	writeToFile(fp, Offset, Length);

	fclose(fp);
	
	return 1;
}
void HtmlParser::setTitle(char* szVal)
{
	if(!szVal)
		return;
	if(m_szTitle)
		delete [] m_szTitle;

	m_szTitle = new char [strlen(szVal)+1];
	strcpy(m_szTitle, szVal);

}
int HtmlParser::writeToFile(FILE *fp, int *Offset, int *Length)
{
	char *szTagName, *szTxt=NULL, *szVal, *p;
	HtmlTag *ht = NULL;
	TagVar *tv;

	*Offset = ftell(fp);

	if(!m_firstHtmlTag) 
	{
		*Length = 0;
		return 0;
	}

	// META TAG �м�
	while((ht=getHtmlTag("META", ht)))
	{
		if(!(tv=ht->getTagVar("http-equiv"))) 
			continue;
		if(!(szVal=tv->getVal())) 
			continue;

		if(!strncasecmp(szVal, "description", 11)) 
		{
			if(!(tv=ht->getTagVar("content"))) 
				continue;
			if(!(szVal=tv->getVal())) 
				continue;
			rmSpecialChar(szVal);
			this->meta_abs = new char [strlen(szVal)+1];
			strcpy(this->meta_abs, szVal);
		}
		else if(!strncasecmp(szVal, "keywords", 8)) 
		{
			if(!(tv=ht->getTagVar("content"))) continue;
			if(!(szVal=tv->getVal())) continue;
			rmSpecialChar(szVal);
			this->meta_key = new char [strlen(szVal)+1];
			strcpy(this->meta_key, szVal);
		}
		else if(!strncasecmp(szVal, "Content-Type", 12)) 
		{
			if(!(tv=ht->getTagVar("content"))) 
				continue;
			if(!(szVal=tv->getVal())) 
				continue;

			szTxt = new char[strlen(szVal)+1];
			strcpy(szTxt, szVal);
			p = szVal = szTxt;

			while(1)
			{	
				szVal = getTerm(&szVal, "; ");
				if(!szVal) break;
				*(szVal++) = '\0';

				if(strchr(p, '/')) 
				{
					this->meta_type = new char [strlen(p)+1];
					strcpy(this->meta_type, p);
				}
				else if(!strncasecmp(p, "charset", 7))
				{
					p = getTerm(&p, "=");
					if(!p) break;	
					p++;
					skipBlank(&p);	

					this->meta_char = new char [strlen(p)+1];
					strcpy(this->meta_char, p);
				}

				skipBlank(&szVal);
				if(!szVal) break;	
				p = szVal;
			
			}
			if(szTxt) 
			{
				delete[] szTxt; 
				szTxt = NULL;
			}
		}

	} 


	// TITLE �м�
	ht = NULL;
	tv = NULL;

	if((ht=getHtmlTag("title", NULL)))
	{
		while((ht=ht->nextHtmlTag))
		{
			szTagName = ht->getTagName();
			if(!strcasecmp(szTagName, "/title"))
				break;
			if(!strcasecmp(szTagName, "TEXT_DATA")) 
			{
				szVal = ht->firstTagVar->getVal();
				if(!szVal) 
					break;
				setTitle(szVal);
			}
		}
	} 

	fwrite("<< START TEXT DATA >>\n", 22, 1, fp);

	fwrite("URL=", 4, 1, fp);
	if(m_szUrl) fwrite(m_szUrl, strlen(m_szUrl), 1, fp);

	fwrite("\nTITLE=", 7, 1, fp);
	if(m_szTitle) fwrite(m_szTitle, strlen(m_szTitle), 1, fp);

	fwrite("\nMETA_ABS=", 10, 1, fp);
	if(this->meta_abs) fwrite(this->meta_abs, strlen(this->meta_abs), 1, fp);

	fwrite("\nMETA_KEY=", 10, 1, fp);
	if(this->meta_key) fwrite(this->meta_key, strlen(this->meta_key), 1, fp);

	fwrite("\nMETA_TYPE=", 11, 1, fp);
	if(this->meta_type) fwrite(this->meta_type, strlen(this->meta_type), 1, fp);

	fwrite("\nMETA_CHAR=", 11, 1, fp);
	if(this->meta_char) fwrite(this->meta_char, strlen(this->meta_char), 1, fp);

	fwrite("\nDATE=", 6, 1, fp);
	if(this->date) fwrite(this->date, strlen(this->date), 1, fp);

	fwrite("\nSIZE=", 6, 1, fp);
	fprintf(fp, "%d", this->size);

	fwrite("\nBODY=", 6, 1, fp);

	ht = getHtmlTag("/title", NULL);
	ht = getHtmlTag("TEXT_DATA", ht);
	while(ht)
	{
//		szTagName = ht->getTagName();
//		if(!strcmp(szTagName, "TEXT_DATA")) {
			if(ht->firstTagVar == NULL)
				continue;
			szTxt = ht->firstTagVar->getVal();
			if(szTxt) 
				fwrite(szTxt, strlen(szTxt), 1, fp);
			fwrite(" ", 1, 1, fp);
//		}
		ht = getHtmlTag("TEXT_DATA", ht);
	}

	fwrite("\nLINK=", 6, 1, fp);

	char *url, *anchor;
	ht = NULL;
	while((ht = getUrlAnchor(&url, &anchor, ht)) != NULL)
	{
		if(!url)
		{
			if(anchor)
				delete[] anchor;

			continue;
		}

		fprintf(fp, "%s ", url);
		delete[] url;

		if(anchor)
		{
			fprintf(fp, "\"%s\"", anchor);
			delete[] anchor;
		}
		fprintf(fp, "\n");
	}

	ht = NULL;
	while((ht = getFrameLink(&url, ht)) != NULL)
	{
		if(!url)
		{
			if(anchor)
				delete[] anchor;
			continue;
		}

		fprintf(fp, "%s ", url);
		delete[] url;

		if(anchor)
		{
			fprintf(fp, "\"%s\"", anchor);
			delete[] anchor;
		}
		fprintf(fp, "\n");
	}


	fwrite("\n<< END TEXT DATA >>\n\n", 22, 1, fp);

	*Length = ftell(fp) - *Offset;
	
	return 1;
}
// ----------------------------------------------
// html ���� �м��Ѵ�.
// ----------------------------------------------
int HtmlParser::Ananysis()
{
	char *szTagName, *szTxt=NULL, *szVal, *p;
	HtmlTag *ht = NULL;
	TagVar *tv;

	if(!m_firstHtmlTag) 
	{
		return 0;
	}

	// META TAG �м�
	while((ht=getHtmlTag("META", ht)))
	{
		if(!(tv=ht->getTagVar("http-equiv"))) 
			continue;
		if(!(szVal=tv->getVal())) 
			continue;

		if(!strncasecmp(szVal, "description", 11)) 
		{
			if(!(tv=ht->getTagVar("content"))) 
				continue;
			if(!(szVal=tv->getVal())) 
				continue;
			rmSpecialChar(szVal);
			this->meta_abs = new char [strlen(szVal)+1];
			strcpy(this->meta_abs, szVal);
		}
		else if(!strncasecmp(szVal, "keywords", 8)) 
		{
			if(!(tv=ht->getTagVar("content"))) continue;
			if(!(szVal=tv->getVal())) continue;
			rmSpecialChar(szVal);
			this->meta_key = new char [strlen(szVal)+1];
			strcpy(this->meta_key, szVal);
		}
		else if(!strncasecmp(szVal, "Content-Type", 12)) 
		{
			if(!(tv=ht->getTagVar("content"))) 
				continue;
			if(!(szVal=tv->getVal())) 
				continue;

			szTxt = new char[strlen(szVal)+1];
			strcpy(szTxt, szVal);
			p = szVal = szTxt;

			while(1)
			{	
				szVal = getTerm(&szVal, "; ");
				if(!szVal) break;
				*(szVal++) = '\0';

				if(strchr(p, '/')) 
				{
					this->meta_type = new char [strlen(p)+1];
					strcpy(this->meta_type, p);
				}
				else if(!strncasecmp(p, "charset", 7))
				{
					p = getTerm(&p, "=");
					if(!p) break;	
					p++;
					skipBlank(&p);	

					this->meta_char = new char [strlen(p)+1];
					strcpy(this->meta_char, p);
				}

				skipBlank(&szVal);
				if(!szVal) break;	
				p = szVal;

			}
			if(szTxt) 
			{
				delete[] szTxt; 
				szTxt = NULL;
			}
		}

	} 


	// TITLE �м�
	ht = NULL;
	tv = NULL;

	if((ht=getHtmlTag("title", NULL)))
	{
		while((ht=ht->nextHtmlTag))
		{
			szTagName = ht->getTagName();
			if(!strcasecmp(szTagName, "/title"))
				break;
			if(!strcasecmp(szTagName, "TEXT_DATA")) 
			{
				szVal = ht->firstTagVar->getVal();
				if(!szVal) 
					break;

				setTitle(szVal);
			}
		}
	} 
	return 1;
}
int	HtmlParser::LoadHtml(char* szFIleName)
{
	fstream			m_fs;

	m_fs.open( szFIleName ,fstream::in);
	BOOL bResult = !m_fs;
	if(bResult == FALSE)
	{
		LONG size = 0;
		m_fs.seekg (0, ios::end);
		size = m_fs.tellg();

		char* lpTemp = new char[size];
		memset(lpTemp,0,size);
		m_fs.seekg (0, ios::beg);

		m_fs.read (lpTemp, size);

		normalizationString(lpTemp);
		exeParser(lpTemp);

		// html �м�
		Ananysis();

		// ---------------------------------------
		m_fs.close();
		SAFE_DELETE_ARRAY(lpTemp);

		return 1;
	}
	return 0;
}

int HtmlParser::addHtmlTag(HtmlTag *ht)
{
    if(ht == NULL) return 0; 

	if(!(m_lastHtmlTag))
		m_firstHtmlTag = m_lastHtmlTag = ht;
	else { 
		m_lastHtmlTag->nextHtmlTag = ht;
		m_lastHtmlTag = ht;
	}

    return 1;
}

HtmlTag * HtmlParser::parseComment(char **buf)
{
	char *begin, *end=NULL, *szVal;
	char *tmpEnd;
	HtmlTag *ht;
	TagVar *tv;
	int nSize;

	// �ּ� �����͸� �̱� ���� �ּ� Tag�� �ųʶ�	
	if(!strncmp(*buf, "!--", 3)) *buf+=3;
	begin = *buf;

	while(1)  {
		tmpEnd = getTerm(buf, "-");
		if(!tmpEnd) return NULL;

		if(!strncasecmp(*buf, "-->", 3)) {
			end = tmpEnd;
			break;
		}
		(*buf)++;
	} // end while

	*buf = tmpEnd + 3;

	if(!end) return NULL; 

	nSize = (unsigned int)(end) - (unsigned int)(begin);
	if(nSize < 1) return NULL;

// Value�� �����ϱ� ���� �޸� Ȯ��
	szVal = new char [nSize+1];
	if(!szVal) return NULL;
	strncpy(szVal, begin, nSize);
	szVal[nSize] = '\0';

	// ���ο� Tag �߰��� ���� �޸� Ȯ��	
	ht = new HtmlTag();
	if(!ht) {
		if(szVal) delete szVal;
		return NULL;
	}
	ht->setTagName("COMMENT_DATA");

	// ���ο� TagVar �߰��� ���� �޸� Ȯ��
	tv = new TagVar();
	if(!tv) {
		if(szVal) delete szVal;
		if(ht) delete ht;
	}
	tv->setVar("TEXT");
	tv->setVal(szVal);
	tv->nextTagVar = NULL;
	
	ht->addTagVar(tv);

	delete szVal;

	return ht;

}

HtmlTag * HtmlParser::parseScript(char **buf)
{
	char *begin, *end=NULL, *szVal;
	char *tmpEnd;
	HtmlTag *ht;
	TagVar *tv;
	int nSize;
	
	skipBlank(buf);

	begin = *buf;

	while(1)  {
		tmpEnd = getTerm(buf, "<");
		if(!tmpEnd) return NULL;

		if(**buf == OPEN_TAG) (*buf)++;
		skipBlank(buf);

		if(!strncasecmp(*buf, "/script", 7)) {
			end = tmpEnd;
			break;
		}
	}

	*buf = tmpEnd;

	if(!end) return NULL; 

	nSize = (unsigned int)(end) - (unsigned int)(begin);
	if(nSize < 1) return NULL;

	szVal = new char [nSize+1];
	if(!szVal) return NULL;
	strncpy(szVal, begin, nSize);
	szVal[nSize] = '\0';

	// ���ο� Tag �߰�	
	ht = new HtmlTag();
	if(!ht) {
		if(szVal) delete szVal;
		return NULL;
	}
	ht->setTagName("SCRIPT_DATA");
	ht->nextHtmlTag = NULL;

	// ���ο� TagVar �߰�	
	tv = new TagVar();
	if(!tv) {
		if(szVal) delete szVal;
		if(ht) delete ht;
		return NULL;
	}
	tv->setVar("TEXT");
	tv->setVal(szVal);
	tv->nextTagVar = NULL;
	
	ht->addTagVar(tv);

	delete szVal;

	return ht;

}

HtmlTag * HtmlParser::parseStyle(char **buf)
{
	char *begin, *end=NULL, *szVal;
	char *tmpEnd;
	HtmlTag *ht;
	TagVar *tv;
	int nSize;
	
	skipBlank(buf);

	begin = *buf;

	while(1)
	{
		tmpEnd = getTerm(buf, "<");
		if(!tmpEnd) return NULL;

		if(**buf == OPEN_TAG) (*buf)++;
		skipBlank(buf);

		if(!strncasecmp(*buf, "/style", 6)) {
			end = tmpEnd;
			break;
		}
	} // end while

	*buf = tmpEnd;

	if(!end) return NULL; 

	nSize = (unsigned int)(end) - (unsigned int)(begin);
	if(nSize < 1) return NULL;

	szVal = new char [nSize+1];
	if(!szVal) return NULL;
	strncpy(szVal, begin, nSize);
	szVal[nSize] = '\0';

	// ���ο� Tag �߰�	
	ht = new HtmlTag();
	if(!ht) {
		if(szVal) delete szVal;
		return NULL;
	}
	ht->setTagName("STYLE_DATA");
	ht->nextHtmlTag = NULL;

	// ���ο� TagVar �߰�	
	tv = new TagVar();
	if(!tv) {
		if(szVal) delete szVal;
		if(ht) delete ht;
	}
	tv->setVar("TEXT");
	tv->setVal(szVal);
	tv->nextTagVar = NULL;
	
	ht->addTagVar(tv);

	delete szVal;

	return ht;

}
// end HtmlParser::parseScript F.


HtmlTag * HtmlParser::parseTextData(char **buf)
{
	char *begin, *end, *szVal;
	HtmlTag *ht;
	TagVar *tv;
	int nSize;
	
	skipBlank(buf);
	if(**buf && **buf != OPEN_TAG)
	{
		begin = *buf;

		while((end=getTerm(buf, "<")))
		{
			(*buf)++;
			skipBlank(buf);
			if(**buf == '<') continue;
			if(whatIsTag(*buf) != INVALID_TAG) break;
		}
		*buf = end;

		if(!begin) return NULL;
		if(!end) {
			szVal = new char [strlen(begin)+1];
			if(!szVal) return NULL;
			strcpy(szVal, begin);
		}
		else {
			nSize = (unsigned int)(end) - (unsigned int)(begin);
			if(nSize < 1) return NULL;
			szVal = new char [nSize+1];
			if(!szVal) return NULL;
			strncpy(szVal, begin, nSize);
			szVal[nSize] = '\0';
		}






		transCodeSet(szVal);
		rmSpecialChar(szVal);


		// ���ο� �±� �߰�
		ht = new HtmlTag();
		if(!ht) {
			if(szVal) delete szVal;
			return NULL;
		}
		ht->setTagName("TEXT_DATA");
		ht->nextHtmlTag = NULL;

		// �±׿� �ؽ�Ʈ ������ ����
		tv = new TagVar();
		if(!tv) {
			if(szVal) delete szVal;
			if(ht) delete ht;
			return NULL;
		}
		tv->setVar("TEXT");
		tv->setVal(szVal);
		tv->nextTagVar = NULL;

		ht->addTagVar(tv);

		delete szVal;
	} // end if
	return ht;

}
// end HtmlParser::parseTextData F.


int HtmlParser::parseVarVal(HtmlTag *ht, char **buf)
{

	int nSize;
	char *begin, *end, *szVar=NULL, *szVal=NULL;
	TagVar *tv;

	if(ht == NULL || !(**buf)) return 0;


// CLOSE_TAG '>'�� ���������� ������ ���� �����Ѵ�.
// NAME=VALUE ���·� VALUE�� NULL�� �� �ִ�.
//
	while(1)
	{
		// ������ ����������� ���鹮�ڸ� �����Ѵ�.
		skipBlank(buf);

		// CLOSE_TAG���� ��� ó���Ǿ���.
		if(**buf == CLOSE_TAG) break;

		// ������ ������ �����ε� ���ο� ��ȿ�� �±װ� ������
		// CLOSE_TAG�� ������ ������ ����.
		if(**buf == OPEN_TAG) {
			begin = *buf;
			(*buf)++;
			skipBlank(buf);
			if(whatIsTag(*buf) == VALID_TAG) {
				*buf = begin;
				break;
			} // end if
			*buf = begin;
		}

		/*********************************************
		* NAME �κ� �д� ��
		*********************************************/
		begin = *buf; // NAME �� ������ġ

		while(1) {
			end = getTerm(buf, " =<>");
			if(**buf == OPEN_TAG) {
				(*buf)++;
				skipBlank(buf);
				if(whatIsTag(*buf)==VALID_TAG) {
					*buf = end;
					break;	
				} // end if
				else {
					*buf = end+1;
					continue;	
				}
			}// end if
			break;
		} // end while

		if(!end) return 0;	

		// ���ο� NAME�� ����	
		nSize = (unsigned int)(end)-(unsigned int)(begin);
		if(nSize < 1) break;
		szVar = new char [nSize+1];
		if(!szVar) return 0;
		strncpy(szVar, begin, nSize);
		szVar[nSize] = '\0'; 

		// ���ο� TagVar�� ����
		tv = new TagVar();
		if(!tv) {
			if(szVal) delete szVal;
			return 1;
		}
		tv->setVar(szVar);
		ht->addTagVar(tv);
		tv->nextTagVar = NULL;

		delete szVar;
		szVar = NULL;

		/***********************************************
		* VALUE �κ� �д� �� 
		***********************************************/
		skipBlank(buf);

		if(**buf == CLOSE_TAG) break;

		// ������ ������ �����ε� ���ο� ��ȿ�� �±װ� ������
		// CLOSE_TAG�� ������ ������ ����.
		else if(**buf == OPEN_TAG) {
			begin = *buf;
			(*buf)++;
			skipBlank(buf);
			if(whatIsTag(*buf) == VALID_TAG) {
				*buf = begin;
				break;
			} // end if
			*buf = begin;
		}
		else if(**buf != '=') {
			continue; // '='�� �ƴϸ� ���� ���� ����
		}



		// '='�� ���� �κ��� �ǳʶ�.
		(*buf)++;
		skipBlank(buf);

		begin = *buf; // VALUE �� ������ġ 

		// '�� "�� ������ ���
		if(**buf == STRING_TAG1 || **buf == STRING_TAG2) {
			(*buf)++;
			if(*begin == STRING_TAG1)
				while(1) {
					end = getTerm(buf, "'<>");
					if(!end) goto INVALID_HTML_CODE;
					if(**buf == OPEN_TAG) {
						(*buf)++;
						skipBlank(buf);
						if(whatIsTag(*buf)==VALID_TAG) {
							*buf = end;
							break;	
						} // end if
						else {
							*buf = end+1;
							continue;	
						}
					}// end if
					break;
				} // end while

		    else if(*begin == STRING_TAG2)	
				while(1) {
					end = getTerm(buf, "\"<>");
					if(!end) goto INVALID_HTML_CODE;
					if(**buf == OPEN_TAG) {
						(*buf)++;
						skipBlank(buf);
						if(whatIsTag(*buf)==VALID_TAG) {
							*buf = end;
							break;	
						} // end if
						else {
							*buf = end+1;
							continue;	
						}
					}// end if
					break;
				} // end while
		}
		else
			while(1) {
				end = getTerm(buf, " <>");
				if(!end) goto INVALID_HTML_CODE;
				if(**buf == OPEN_TAG) {
					(*buf)++;
					skipBlank(buf);
					if(whatIsTag(*buf)==VALID_TAG) {
						*buf = end;
						break;	
					} // end if
					else {
						*buf = end+1;
						continue;	
					}
				}// end if
				break;
			} // end while


		if(*end == STRING_TAG1 || *end == STRING_TAG2) {
			(*buf)++;		
		}

INVALID_HTML_CODE:
		if(*begin == STRING_TAG1 || *begin == STRING_TAG2) begin++;
		if(!end) {
			end = *buf;
			**buf = '\0';
		}


		// ���ο� VALUE ����
		nSize = (unsigned int)(end)-(unsigned int)(begin);
		if(nSize < 1) continue;
		szVal = new char [nSize+1];
		if(!szVal) {
			if(tv) delete tv;
			return 1;
		}
			
		strncpy(szVal, begin, nSize);
		szVal[nSize] = '\0';
		tv->setVal(szVal);

		delete szVal;
		szVal = NULL;

		//break;

	} // end while

	if(**buf == CLOSE_TAG) (*buf)++;
	
	return 1;

}
// end HtmlParser::parseVarVal F.


HtmlTag * HtmlParser::parseHtmlTag(char **buf)
{
    HtmlTag *ht;
	char name[NAME_SIZE+1], *tmp;
	int nIdx=0;

// Tag Name�� ������ �������� ���� ���ڸ� �����Ѵ�.
	skipBlank(buf);

/*********************************************************
* Tag Name�� �����Ѵ�.
* Tag Name�� ������ �����ų� CLOSE_TAG '>'�� �����ų�
* '\0' ���ڰ� ���ö�����.... �о �����Ѵ�.
*********************************************************/

	while(**buf != '\n' && **buf != ' ' && **buf != CLOSE_TAG && **buf)
	{
		// Tag �߰��� ���ο� Tag�� �����ϸ�...
		if(**buf == OPEN_TAG) {
			tmp = *buf;
			(*buf)++;
			skipBlank(buf);	
			if(whatIsTag(*buf) == VALID_TAG) {
				*buf = tmp;
    			ht = new HtmlTag();
			    if(!ht)  return NULL;

			// ������ HtmlTag�� �̸��� ����.
				name[nIdx] = '\0';
				ht->setTagName(name);
				ht->nextHtmlTag = NULL;
				return ht;
			} // end if

			*buf = tmp;
		} // end if

		name[nIdx++] = **buf;
		(*buf)++;

		// �̸��� NAME_SIZE���� Ŀ����� ����ó��....
		// CLOSE_TAG�� ���ö�����.. �����Ѵ�!
		if(nIdx >= NAME_SIZE) {
			skipUntilCloseTag(buf);
			return NULL;
		}
	}

    ht = new HtmlTag();
    if(!ht)  return NULL;

// ������ HtmlTag�� �̸��� ����.
	name[nIdx] = '\0';
	ht->setTagName(name);

	this->parseVarVal(ht, buf);

	return ht;

}
// end HtmlParser::parseHtmlTag F.


int HtmlParser::exeParser(char *szHtmlTxt)
{
	HtmlTag *ht;

	if(!szHtmlTxt) return 0;

// ������ ������ Tag�� �̾Ƴ�.
	while(szHtmlTxt)
	{
		if(*szHtmlTxt == '\0' || *szHtmlTxt == EOF) break;
		ht = NULL;
		skipBlank(&szHtmlTxt);

		if(!*szHtmlTxt) break;


		if(*szHtmlTxt == CLOSE_TAG) {
			szHtmlTxt++;
			skipBlank(&szHtmlTxt);
			continue;
		}
		else if(*szHtmlTxt == OPEN_TAG) {
			szHtmlTxt++;
			skipBlank(&szHtmlTxt);

			switch(whatIsTag(szHtmlTxt))
			{
				// SCRIPT TAG
				case SCRIPT_TAG:
					ht = this->parseHtmlTag(&szHtmlTxt);
					if(ht) this->addHtmlTag(ht);
					ht = this->parseScript(&szHtmlTxt);
					break;

				// STYLE TAG
				case STYLE_TAG:
					ht = this->parseHtmlTag(&szHtmlTxt);
					if(ht) this->addHtmlTag(ht);
					ht = this->parseStyle(&szHtmlTxt);
					break;
		
				// COMMENT TAG
				case COMMENT_TAG:
					ht = this->parseComment(&szHtmlTxt);
					break;
				
				// ���⿡�� VALID, INVALID ��� ó���Ѵ�. 
				case VALID_TAG:
				case INVALID_TAG:
					ht = this->parseHtmlTag(&szHtmlTxt);
					break;

				// ETC.
				default:	
					continue;
			} // end switch

		} // end if

		// OPEN_TAG�� �ƴϸ� TEXT_DATA�� �����Ѵ�.
		else
			ht = this->parseTextData(&szHtmlTxt); 

		if(ht) this->addHtmlTag(ht);
	} // end while 

    return 1;
}
// end HtmlParser::exeParser F.


void HtmlParser::print() const
{
	HtmlTag *ht = m_firstHtmlTag;
	char *szUrl;

	szUrl = this->getUrl();
//	cout << "URL: " << szUrl << ", ";
	while(ht) {

		ht->print();
		ht = ht->nextHtmlTag;

	} // end while
}
// end HtmlParser::print f.


unsigned long HtmlParser::getIp() const
{
	return this->ip;
}

/******************************************************************************
HtmlParser::setAbsPath
 ����η� �Ǿ� �ִ� URL���� ��� �����η� ��ȯ�Ѵ�.
 ��� : <A>, <AREA>, <FRAME>

input : char *pPath		parent path
******************************************************************************/
void HtmlParser::setAbsPath(char *pPath)
{
	HtmlTag *ht = NULL;
	TagVar *tv;
	char *TagName;
	char *oldUrl;
	char newUrl[4096];

	if((ht = m_firstHtmlTag) == NULL)
		return;

	while((ht = ht->nextHtmlTag) != NULL)
	{
		if((TagName = ht->getTagName()) == NULL)
			continue;
		
		if(strcasecmp(TagName, "A") == 0)
		{
			if((tv = ht->getTagVar("HREF")) == NULL) 
				continue;
		}
		else if(strcasecmp(TagName, "AREA") == 0)
		{
			if((tv = ht->getTagVar("HREF")) == NULL) 
				continue;
		}
		else if(strcasecmp(TagName, "FRAME") == 0)
		{
			if((tv = ht->getTagVar("SRC")) == NULL) 
				continue;
		}
		else 
			continue;

		if((oldUrl = tv->getVal()) == NULL) continue;
		ConcatenateUrl(pPath, oldUrl, newUrl);
		if(newUrl)
		{
			delete[] oldUrl;
			tv->setVal(newUrl);
		}
	}
}

void HtmlParser::setIp(unsigned long ip)
{
	this->ip = ip;
}

void HtmlParser::setUrl(char *url)
{
	if(!url)
	{
		cout << "URL is null" << endl;
		return;
	}

	if(m_szUrl) 
		delete[] m_szUrl;

	m_szUrl = new char[strlen(url) + 1];
	if(!m_szUrl)
	{
		cout << "Error in new method function" << endl;
		exit(1);
	}

	strcpy(m_szUrl, url);
}

void HtmlParser::setDate(char *Date)
{
	SAFE_DELETE_ARRAY(date);

	if(!Date) return;

	this->date = new char[strlen(Date) + 1];
	if(!this->date)
	{
		cout << "Error in new method function(setDate)" << endl;
		exit(1);
	}

	strcpy(this->date, Date);
}

/*****************************************************************************
HtlParser::setUrl
 �и��Ǿ� �ִ� URI�� �ϳ��� ���ļ� szUrl member�� �����Ѵ�.
input : char *hostname 		URL���� hostname
		char *identifier	�׻� '/'���丮���� �н��� ���۵Ǿ�� �Ѵ�.
		int port			URL�� port number
*****************************************************************************/
void HtmlParser::setUrl(char *hostname, char *identifier, int port)
{
	char tmp[80];
	char *p;

	sprintf(tmp, ":%d", port);

	if(!hostname)
	{
		cout << "Hostname is invalid" << endl;
		exit(1);
	}

	// identifier�� null�� ��� ��ü URI���� '/'�� �ٿ��ش�.
	if(!identifier)
	{
		p = new char[2];
		*p = '/';
		*(p + 1) = '\0';
	}
	else
	{
		p = new char[strlen(identifier) + 1];
		strcpy(p, identifier);
	}


	if(m_szUrl) delete[] m_szUrl;

	m_szUrl = new char[strlen(hostname) + strlen(p) + strlen(tmp) + 1];
	if(!m_szUrl)
	{
		cout << "Error in new method function" << endl;
		exit(1);
	}

	strcpy(m_szUrl, hostname);
	strcat(m_szUrl, tmp);
	strcat(m_szUrl, p);

	delete[] p;
}

char *HtmlParser::getUrl() const
{
	return m_szUrl;
}
// end HtmlParser::getUrl F.


/******************************************************************************
HtmlParser::getUrlAnchor

INPUT : char *url, char *anchor �� �޸� �Ҵ� ���� ���� ������ ����
		ȣ��� '&' �ٿ��� ���
		getUrlAnchor(&url, &anchor, ht); �̷�������...	

OUTPUT: URL������ Anchor ����...

RETURN: 'A' �±� �ٷ� ���� �±� - /A�� ������ �ʰ� �ٽ� A�±װ� ���ð��
		'/A' �±� - ���������� /A �±׷� �������
		�Ǵ� NULL -> �� ���� �±ױ��� ó���Ǿ��� ���...
******************************************************************************/
HtmlTag *HtmlParser::getUrlAnchor(char **url, char **anchor, HtmlTag *ht)
{
	char *szString, *tmp=NULL;
	TagVar *tv;
	HtmlTag *cur;
	int nLen;

	*url = *anchor = NULL;

	// HtmlTag ht�� ������...
	ht = getHtmlTag("A", ht);
	if(!ht) 
		return NULL;

	cur = ht;

	tv = ht->getTagVar("HREF");
	if(!tv) 
		return ht;
	szString = tv->getVal();

	// URL ������ ������...
	if(!szString) 
		return ht;

	sm_trim(szString);
	*url = this->MakeFullUrl(szString);
	if(*url == NULL)
		return ht;

	// anchor������ ī��...
	while(1)
	{
		if(!cur->nextHtmlTag) return NULL;
		szString = cur->nextHtmlTag->getTagName();
		if(!szString) 
		{
			cur = cur->nextHtmlTag;
			continue;
		}

		if(!strcasecmp(szString, "A")) 
			return cur;
		if(!strcasecmp(szString, "/A")) 
			return cur->nextHtmlTag;

		if(!strcasecmp(szString, "TEXT_DATA")) 
		{
			tv = cur->nextHtmlTag->getTagVar("TEXT");
			if(!tv) 
			{
				cur = cur->nextHtmlTag;
				continue;
			}
			szString = tv->getVal();
			nLen = 0;

			if(*anchor && szString) 
			{
				nLen = strlen(*anchor) + strlen(szString) + 2;
				tmp = new char [nLen];
				sprintf(tmp, "%s %s", *anchor, szString);
				delete [] *anchor;
				*anchor = tmp;
				tmp = NULL;
			}
			else if(!(*anchor) && szString) 
			{
				*anchor = new char [strlen(szString)+1];
				strcpy(*anchor, szString);
			}
		}
		else if(!strcasecmp(szString, "IMG")) 
		{
			tv = cur->nextHtmlTag->getTagVar("ALT");
			if(!tv) 
			{
				cur = cur->nextHtmlTag;
				continue;
			}
			szString = tv->getVal();
			nLen = 0;

			if(*anchor && szString) 
			{
				nLen = strlen(*anchor) + strlen(szString) + 2;
				tmp = new char [nLen];
				sprintf(tmp, "%s %s", *anchor, szString);
				delete [] *anchor;
				*anchor = tmp;
				tmp = NULL;
			} // end if - anchor������ �־�����....
			else if(!(*anchor) && szString) 
			{
				*anchor = new char [strlen(szString)+1];
				strcpy(*anchor, szString);
			} // end else - anchor������ ��������....
		}
		cur = cur->nextHtmlTag;
	}
}


/******************************************************************************
HtmlParser::getFrameLink

input : HtmlTag *ht
output:	char *url	�޸� �Ҵ� ���� ���� ������ ����
return: 'FRAME' �±� �ٷ� ���� �±� �ٷ� ������ �±�
		�Ǵ� NULL -> �� ���� �±ױ��� ó���Ǿ��� ���...
******************************************************************************/
HtmlTag *HtmlParser::getFrameLink(char **url, HtmlTag *ht)
{
	char *szString;
	TagVar *tv;
	HtmlTag *cur;

	*url = NULL;

	// HtmlTag ht�� ������...
	ht = getHtmlTag("FRAME", ht);
	if(!ht) 
		return NULL;

	cur = ht;

	tv = ht->getTagVar("SRC");
	if(!tv) 
		return ht;
	szString = tv->getVal();

	// URL ������ ������...
	if(!szString) 
		return ht;

	// URL ������ ī��...
	sm_trim(szString);
	*url = MakeFullUrl(szString);

	return ht;
}

/******************************************************************************
HtmlParser::MakeFullUrl

	�ҽ� ��Ʈ���� �Է¹޾� ��ü URL�� �����Ѵ�. �ҽ��� identifier�� ���� ����
	������ ��쿡 ���󼭴� �������ݱ��� ������ �� �ִ�. ���������� ���ǵ��� 
	���� ��Ʈ���� ���� identifier�� �ִ� ������ �����ϸ� ���������� ���� ���
	ȣ��Ʈ���� ������ ������ �Ѵ�.

input : char *src
return: char *	������ ��ü URL
******************************************************************************/
char *HtmlParser::MakeFullUrl(char *src)
{
	char *p;
	char *str = NULL;

	// identifier���� '#' ����
	p = strchr(src, '#');
	if(p)
		*p = 0;

	p = src;
	if(strncasecmp(p, "http://", 7) == 0)
	{
		p = p + 7;
		str = new char[strlen(p) + 1];
		strcpy(str, p);
	}
	else if(strncasecmp(p, "https://", 8) == 0)
	{
		p = p + 8;
		str = new char[strlen(p) + 1];
		strcpy(str, p);
	}
	else if(strncasecmp(p, "mailto:", 7) == 0)
	{
		return NULL;
	}
	else if(strncasecmp(p, "gopher://", 9) == 0)
	{
		return NULL;
	}
	else if(strncasecmp(p, "javascript", 10) == 0)
	{
		return NULL;
	}
	else if(strncasecmp(p, "ftp:", 4) == 0)
	{
		return NULL;
	}
	else if(strncasecmp(p, "mms://", 6) == 0)
	{
		return NULL;
	}
	else // ������̹Ƿ�
	{
		URL *u = new URL((unsigned char *)m_szUrl);

		if(u->SetIdentifier((unsigned char *)src) != 1)
		{
//			cout << ">> Path error : " << src << "\n";
			return NULL;
		}

		str = (char *)u->GetFullUrl();

		delete u;
	}

	sm_trim(str);

	return str;
}

/******************************************************************************
HtmlParser::getHtmlTag
 snode�� �ٷ� �������� szName���� ������ �±׸� ã�´�.
input : char *szName	ã���� �ϴ� Tag name
						��ҹ��ڸ� �������� �ʴ´�.
		HtmlTag *snode	��ü Html list�߿��� ã���� �ϴ� ���� ��ġ
return: HtmlTag *		!= NULL	ã�� list�� ������
						== NULL	���̻� �������� ����
******************************************************************************/
HtmlTag *HtmlParser::getHtmlTag(char *szName, HtmlTag *snode) const
{
	HtmlTag *ht;
	char *szTagName;

	if(!snode) ht = m_firstHtmlTag;
	else ht = snode->nextHtmlTag;

	if(!szName) return NULL;
	if(!ht) return NULL;

	while(ht)
	{
		szTagName = ht->getTagName();
		if(!strcasecmp(szTagName, szName)) return ht;
		ht = ht->nextHtmlTag; 
	} // end while

	return NULL;
}

/******************************************************************************
HtmlParser::getHtmlTagVal
 snode�� �ٷ� �������� szName���� ������ �±��� ������ ����� ���� ã�´�.
input : char *szTName	ã���� �ϴ� Tag name
						��ҹ��ڸ� �������� �ʴ´�.
		char *szMName	ã���� �ϴ� member�� �̸�
		HtmlTag *snode	��ü Html list�߿��� ã���� �ϴ� ���� ��ġ
output: char **val		����� ����� �����ͷ� �����ش�.
						�̹� �޸𸮻� �Ҵ�Ǿ� �ִ� ������ �ּҰ����� �����ֱ�
						������ ȣ���ڴ� ������ �޸𸮸� �Ҵ����� ���ƾ� �ϸ�,
						�翬�� �޸𸮸� ���������� ���ƾ� �Ѵ�.
return: HtmlTag *		!= NULL	ã�� list�� ������
						== NULL	���̻� �������� ����
******************************************************************************/
HtmlTag *HtmlParser::getHtmlTagVal(char *szTName, char *szMName, HtmlTag *snode, char **val) const
{
	HtmlTag *ht;
	TagVar *TagVar;

	if(!szTName) return NULL;
	if(!szMName) return NULL;

	ht = this->getHtmlTag(szTName, snode);
	if(!ht) return NULL;

	TagVar = ht->getTagVar(szMName);
	if(!TagVar) {
		*val = NULL;
		return ht;
	}

	*val = TagVar->getVal();	

	return ht;
}


HtmlTag::HtmlTag()
{
    this->nextHtmlTag = NULL;
    this->firstTagVar = NULL;
    this->lastTagVar = NULL;

	memset(this->szTagName, 0, NAME_SIZE);
}
// end HtmlTag::HtmlTag F.


HtmlTag::~HtmlTag()
{
/*
	if(this->firstTagVar) 
		delete this->firstTagVar;
	if(this->nextHtmlTag) 
		delete this->nextHtmlTag;
*/

	// ���� stack overflow �߻��ô� firstTagVar�� ������ �����ؾ� ��
	if(this->firstTagVar)
		delete this->firstTagVar;
}


int HtmlTag::setTagName(char *name)
{
	if(strlen(name) > NAME_SIZE || strlen(name) < 1) return 0;

	strcpy(this->szTagName, name);

	return 1;
}
// end HtmlTag::setTagName F.


char *HtmlTag::getTagName() const
{
	return (char *)this->szTagName;
}
// end HtmlTag::getTagName F.


int HtmlTag::addTagVar(TagVar *tv)
{
	if(!tv) return 0;

	if(!(this->lastTagVar))
		this->firstTagVar = this->lastTagVar = tv;
	else {
		this->lastTagVar->nextTagVar = tv;
		this->lastTagVar = tv;
	}

	return 1;
}
// end HtmlTag::addTagVar

void HtmlTag::print() const
{
	TagVar *tv;
	char *szVar, *szVal, *szTagName;

	szTagName = this->getTagName();
	cout << "TAG: " << szTagName << endl;
	cout << "--------------------------------------------------------" << endl;

	tv = this->firstTagVar;

	while(tv) {
		szVar = tv->getVar();
		szVal = tv->getVal();
		cout << "     " << szVar << "=" << szVal << endl;
		tv = tv->nextTagVar;
	} // end while

	cout << endl;
}
// end HtmlTag::print F.


TagVar *HtmlTag::getTagVar(char *szName) const
{
	TagVar *tv = this->firstTagVar;
	char *szVar;

	if(!szName) return NULL;
	if(!tv) return NULL;

	while(tv)
	{
		szVar = tv->getVar();
		if(!strcasecmp(szVar, szName)) return tv;
		tv = tv->nextTagVar;
	} // end while

	return NULL;

}
// end HtmlTag::getTagVar F.




TagVar::TagVar()
{
    this->nextTagVar = NULL;
	this->szVar = NULL;
	this->szVal = NULL;
}
// end TagVar F.


TagVar::~TagVar()
{
	if(this->szVar) delete [] this->szVar;
	if(this->szVal) delete [] this->szVal;
	if(this->nextTagVar) delete this->nextTagVar;
}
// end ~TagVar F.


int TagVar::setVar(char *var)
{
	int len = strlen(var);

	if(!var) return 0;

	this->szVar = new char[len+1];
	if(!this->szVar) return 0;

	strcpy(this->szVar, var);

	return 1;
}
// end TagVar::setVar F.


int TagVar::setVal(char *val)
{
	int len = strlen(val);
	if(!val) return 0;
	this->szVal = new char[len+1];
	if(!this->szVal) return 0;
	strcpy(this->szVal, val);

	return 1;
}
// end TagVar::setVal F.


char *TagVar::getVar(void) const
{
	return this->szVar;
}
// end TagVar::getVar F.


char *TagVar::getVal(void) const
{
	return this->szVal;
}
// end TagVar::getVal F.


int transCodeSet(char *buf)
{
	char *tmp;
	int i, j, idx=0, nSize, len=strlen(buf);

	tmp = new char [len+1];
	if(!tmp) return 0;

	for(i=0;i<len;i++) 
	{
		if(buf[i] == '&') 
		{
			for(j=0;j<CODE_NUM;j++) 
			{
				nSize = strlen(code[j].code);
				if(nSize == 0)
					break;
				if(!strncmp(&buf[i], code[j].code, nSize))
				{
					tmp[idx++] = code[j].dec;
					if(buf[i+nSize] == ';') i++;
					i+=nSize-1;
					break; 
				}
			}
		}
		else tmp[idx++] = buf[i];
	}

	tmp[idx] = '\0';
	strcpy(buf, tmp);

	delete tmp;

	return 1;
}
// end transCodeSet F.


char * getTerm(char **buf, char *sep)
{
	int i, len=strlen(sep);

	while(**buf)
	{
		for(i=0;i<len;i++) {
			if(**buf == sep[i])
				return(*buf);
		} // end for
		(*buf)++;
	} // end while

	return NULL;
}


// CLOSE_TAG '>'�� ���ö����� �����Ѵ�.
void skipUntilCloseTag(char **buf)
{
	while(!(**buf) && **buf != CLOSE_TAG) (*buf)++;

	return;
}
// end skipUntilCloseTag F.


// ������ �ƴ� ���ڰ� ���ö����� �����Ѵ�.
void skipBlank(char **buf)
{
	while(**buf > 0 && **buf <= ' ') (*buf)++;

	return;
}
// end skipBlank F.

int normalizationString(char *buf)
{
	int i, len;

    len = strlen(buf);
    for(i=0;i<len;i++)
        if(buf[i] > 0 && buf[i] < ' ' && buf[i] != '\n') buf[i] = ' ';

	return 1;

} // end normalizationString F.


int whatIsTag(char *buf)
{
	int i;
	char tmp, *p;

//	if(strlen(buf) < 1) return INVALID_TAG;
	if(buf[0] == '\0') return INVALID_TAG;

	if(!strncasecmp(buf, "script ", 7)) return SCRIPT_TAG;
	if(!strncasecmp(buf, "script>", 7)) return SCRIPT_TAG;
	if(!strncasecmp(buf, "style ", 6)) return STYLE_TAG;
	if(!strncasecmp(buf, "style>", 6)) return STYLE_TAG;
	if(!strncasecmp(buf, "!--", 3)) return COMMENT_TAG;

	p = buf;
	getTerm(&p, " \t\n\r<>\0");
	tmp = *p;
	*p = '\0';

	for(i=0;i<NO;i++)
	{
		if(strlen(tagList[i]) < 1) {
			*p = tmp;
			return INVALID_TAG;
		}
		// tagList�� ������������ ������ ������!
		if(!strncasecmp(buf, tagList[i], strlen(tagList[i]))) {
			*p = tmp;
			return VALID_TAG;
		}
	} // end for

	*p = tmp;
	return INVALID_TAG;

}
// end whatIsTag F.


int rmSpecialChar(char *buf)
{
	int i, j;
	int nSize;

	nSize = strlen(buf);
    // TEXT �������� Ư������ ����
    j = 0;
    for(i=0;i<nSize;i++) {
        if(buf[i] >= 0 && buf[i] < ' ') buf[i] = ' ';

        if(j==0)
            buf[j++] = buf[i];
        else {
            if(buf[j-1] != ' ' || buf[i] != ' ') buf[j++] = buf[i];

        }
    }
    buf[j] = '\0';

	return 1;
}
// end rmSpecialChar F.

/******************************************************************************
ConcatenatePath
  ���丮 �ΰ��� ���丮 ��θ� �ϳ��� ��ģ��.
  ��1) str1 = /aa/b/c/d.dat
      str2 = ./../../b.dat
      ��� : /aa/b.dat
      ���� str2�� �н��� '/'�� �����ϸ� str2�� ������ �����ش�.
  ��2) str1 = /aa/bb/cc/dd
       str2 = cc/dd/index.html
       ��� : /aa/bb/cc/dd/index.html

input : char *s1        ���� ���丮 ���
        char *s2        �߰���ų ���丮 ���
output: char *result    ��ģ ���
return: char *          result�� ���� ������
******************************************************************************/
char *ConcatenateUrl(char *s1, char *s2, char *result)
{
    char tmp[4096], *p1;

    // s2�� ó�� ���ڰ� '/'�̸� ��Ʈ�����̹Ƿ� s2�� ����� ����
    if(s2[0] == '/')
    {
        strcpy(result, s2);
        return result;
    }

	// �ι�° ��ΰ� �������ݷ� �����ϴ��� �˻�
	skipBlank(&s2);
	if(strncasecmp(s2, "mailto:", 7) == 0 ||
		strncasecmp(s2, "gopher://", 9) == 0 ||
		strncasecmp(s2, "mms://", 6) == 0 ||
		strncasecmp(s2, "ftp://", 6) == 0 ||
		strncasecmp(s2, "telnet:", 7) == 0 ||
		strncasecmp(s2, "file:", 5) == 0 ||
		strncasecmp(s2, "javascript:", 10) == 0 ||
		strncasecmp(s2, "http:", 5) == 0 ||
		strncasecmp(s2, "https:", 6) == 0)
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

    // ���� ���丮�� �ݺ��Ǵ� ���� ����
    p1 = strrchr(s2, '/');
    if(p1)
    {
        char *p;
        char tt[4096];

        strncpy(tt, s2, p1 - s2);
        tt[p1 - s2] = '\0';
        if((p = strstr(tmp, tt)) != NULL)
        {
            strcpy(p, s2);
            strcpy(result, tmp);
            return result;
        }
    }

    sprintf(result, "%s/%s", tmp, s2);

    return result;
}

#ifdef _HTMLPARSER_DEBUG

int main(int argc, char *argv[])
{
	FILE *fpIn;
	char *szTxt;
	int nSize, menu=0;
	HtmlParser *hp=NULL;

	if(argc < 2)
	{
		printf("Usage : %s test-file-name\n", argv[0]);
		return 1;
	}

	fpIn = fopen(argv[1], "r");
	if(!fpIn)
	{
		printf("Test data file�� �����ϴ�.[%s]\n", argv[1]);
		return 0;
	}

	// ������ ũ�⸦ ���ؿ�
	fseek(fpIn, 0, SEEK_END);
	nSize = ftell(fpIn);
	fseek(fpIn, 0, SEEK_SET);

	// ���� �б�
	szTxt = new char [nSize+1];
	if(!szTxt) return 0;
	fread(szTxt, nSize, 1, fpIn);
	szTxt[nSize] = '\0';

	fclose(fpIn);

	// �Ľ�	- ���� �Էºκ��� ���������� �� �� ����.
	// ��������� ���ڿ� �����ͷ� html �ҽ��� �Ѿ�� ��.
	// Ư�����ڸ� ���鹮�ڷ� ġȯ
	normalizationString(szTxt);

MAIN_MENU:
	printf("\n\n\n--------------------------------------");
	printf("\n 1. HTML �ҽ��� �Ľ��ϰ� ��� ����ϱ�");
	printf("\n 2. �޸� ���� Ȯ���ϱ�");
	printf("\n 3. URL & Anchor ���� �����ϱ�");
	printf("\n 4. ȭ�� ����");
	printf("\n 5. ������ ����");
	printf("\n 0. Exit");
	printf("\n--------------------------------------");
	printf("\n �����ϼ���: ");

	scanf("%d", &menu);

	if(menu == 1)
	{
		hp = new HtmlParser(); 
		if(!hp) {
			if(szTxt) delete szTxt;
			return 0;
		}
		hp->exeParser(szTxt);

		hp->print();

		fpIn = fopen("text.dat", "w");
		hp->writeToFile(fpIn);
		fclose(fpIn);

	} // end if 1.

	else if(menu == 2)
	{
		while(1) {
			if(hp) {
				cout << "�޸𸮿��� ���� �Ϸ�... ";
				delete hp;
			}
			hp = new HtmlParser();
			if(!hp) {
				if(szTxt) delete szTxt;
				return 0;
			}
			if((hp->exeParser(szTxt)) == 1) {
				cout << "1: ���, 2: ����... ";
				cin >> nSize;
				if(nSize == 2) break;
			}
			else {
				cout << "������ ����..." << endl;
				break;
			}
		}
	} // end if-else 2.

	else if(menu == 3)
	{
		hp = new HtmlParser();
		hp->exeParser(szTxt);

		HtmlTag *htt = hp->firstHtmlTag;
		char *url, *anchor;

		htt = NULL;
		while(1) {
/*
			url = anchor = NULL;
			htt = hp->getHtmlTag("A", htt);
			if(!htt) break;
*/
			htt = hp->getUrlAnchor(&url, &anchor, htt);
			if(!htt) break;	
			cout << "URL: " << url << endl;
			cout << "ANCHOR: " << anchor << endl;
			cout << "-------------------------------------------------------------" << endl;
			if(url) delete [] url;
			if(anchor) delete [] anchor;
		}
	} // end if-else 3.

	else if(menu == 4)
	{
		hp = new HtmlParser();
		hp->exeParser(szTxt);

		hp->writeToFile("test.txt");
	}
	else if(menu == 5)
	{
		char cmd[256];

		printf("path ? ");
		scanf("%s", cmd);

		hp = new HtmlParser();
		hp->exeParser(szTxt);

		hp->setAbsPath(cmd);
//		hp->print();
	}

	else if(menu != 0) goto MAIN_MENU;

	if(hp) delete hp;
	if(szTxt) delete [] szTxt;

}

#endif
