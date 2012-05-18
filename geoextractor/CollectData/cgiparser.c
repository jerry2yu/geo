#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgiparser.h"

char upcase(char c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return c;
}

int hex2dec(char c)
{
	if (c >= '0' && c <= '9')
		return (int) c - '0';
	else
	{
		c = upcase(c);
		if (c >= 'A' && c <= 'F')
			return (int) (10 + c - 'A');
		else
			return 0;
	}
}

void ChangeChars2(char *s)
{
	char t[2000];
	int i=0, j=0;
	
	while(s[i] != '\0')
	{
		if (s[i] == '+')
		{
			//t[j++] = ' ';
			t[j++] = s[i];
			i++;
		}
		else if (s[i] != '%')
			t[j++] = s[i++];
		else
		{
			t[j] = hex2dec(s[i+1]) * 16 + hex2dec(s[i+2]);
			if (t[j] == 10 || t[j]==13)
				t[j]='+';
			if (t[j] == ' ')
				t[j]='+';
			j++;
			i += 3;
		}
	}
	t[j++] = '\0';
	strcpy(s,t);
}
void GetStringComponent(char *name, char *s, char *t)
{
	char name2[500];
	char *p;
        if (s==NULL)
          printf("s is null");

	strcpy(name2, name);
	strcat(name2, "=");
	p = strstr(s, name2);
	if (p == NULL)
	{
		t[0] = '\0';
		return;
	}
	else
	{
		int i = 0;
		p += strlen(name2);
		while (*p != '&' && *p != '\0')
			t[i++] = *p++;
		t[i++] = '\0';
	}
	URLdecode(t);
}

void GetStringComponent2( const char *name, char *s, char *t)
{
	char name2[500];
	char *p;
        if (s==NULL)
          printf("s is null");

	strcpy(name2, name);
	strcat(name2, "=");
	p = strstr(s, name2);
	if (p == NULL)
	{
		t[0] = '\0';
		return;
	}
	else
	{
		int i = 0;
		p += strlen(name2);
		while (*p != '\0')
			t[i++] = *p++;
		t[i++] = '\0';
	}
	ChangeChars2(t);
}

/**********************************************
** Usage : _x2c(HEX up character, HEX low character);
** Return: Hex value which is changed.
** Do    : Change two hex character to one hex value.
**********************************************/
char _x2c(char hex_up, char hex_low) 
{
  char digit;

  digit = 16 * (hex_up >= 'A' ? ((hex_up & 0xdf) - 'A') + 10 : (hex_up - '0'));
  digit += (hex_low >= 'A' ? ((hex_low & 0xdf) - 'A') + 10 : (hex_low - '0'));

  return (digit);
}

/**********************************************
** Usage : qURLencode(string to encode);
** Return: Pointer of encoded str which is memory allocated.
** Do    : Encode string.
**********************************************/
char *URLencode(char *str) {
  char *encstr, buf[2+1];
  unsigned char c;
  int i, j;

  if(str == NULL) return NULL;
  if((encstr = (char *)malloc((strlen(str) * 3) + 1)) == NULL) return NULL;

  for(i = j = 0; str[i]; i++) {
    c = (unsigned char)str[i];
         if((c >= '0') && (c <= '9')) encstr[j++] = c;
    else if((c >= 'A') && (c <= 'Z')) encstr[j++] = c;
    else if((c >= 'a') && (c <= 'z')) encstr[j++] = c;
    else if((c == '@') || (c == '.') || (c == '/') || (c == '\\')
         || (c == '-') || (c == '_') || (c == ':') ) encstr[j++] = c;
    else {
      sprintf(buf, "%02x", c);
      encstr[j++] = '%';
      encstr[j++] = buf[0];
      encstr[j++] = buf[1];
    }
  }
  encstr[j] = '\0';

  return encstr;
}

void URLdecode(char *s) {
	char t[2000];
	int i=0, j=0;
	
	while(s[i] != '\0')
	{
		if (s[i] == '+')
		{
			t[j++] = ' ';
			i++;
		}
		else if (s[i] != '%')
			t[j++] = s[i++];
		else
		{
			t[j] = hex2dec(s[i+1]) * 16 + hex2dec(s[i+2]);
			if (t[j] == 10 || t[j]==13)
				t[j]=' ';
			j++;
			i += 3;
		}
	}
	t[j++] = '\0';
	strcpy(s,t);
}
