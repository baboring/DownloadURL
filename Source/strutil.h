#ifndef __STRUTIL_H_
#define __STRUTIL_H_

#define MAXLINE		1024

char *sm_rtrim(char *str);
char *sm_ltrim(char *str);
char *sm_trim(char *str);

char *sm_rtrim_pos(char *str);
char *sm_ltrim_pos(char *str);

char *sm_strtoupper(char *src, char *dest);
char *sm_strtolower(char *src, char *dest);

int getline(char *src, char *dst);
int getword(char *src, char *dst);
int getword1(char *src, char *dst, char sep);
int getword2(char *src, char *dst, char sep);
int getwordex(char *src, char *dst);

char *StringCopy(char **dst, char *src);
char *StringCat(char **dst, char *src);

int ParseUrlString(char *Url, char **host, char **id, int *port);
char *ConcatenatePath(char *s1, char *s2, char *result);
void CorrectPath(char *src, char *dst);
char *sm_DeleteChar(char *str, char DelChar);
char *sm_DeleteCharStr(char *str, char *DelChar);

int strncasecmp(char *src, char *dst, int size);
int strcasecmp(char *src, char *dst);

#endif
