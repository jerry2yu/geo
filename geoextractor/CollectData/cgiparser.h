char upcase(char c);
int hex2dec(char c);

/**
 *
 */
void GetStringComponent(char *name, char *s, char *t);

/**
 * get all remaind strings start from "name", from s, and save it to t
 */
void GetStringComponent2(const char *name, char *s, char *t);

/**
 * Usage : URLencode(string to encode);
 * Return: Pointer of encoded str which is memory allocated.
 * Do    : Encode string.
 */
char *URLencode(char *str);

/**
 * Usage : URLdecode(query pointer);
 * Return: Pointer of query string.
 * Do    : Decode query string.
 */
void URLdecode(char *s);

