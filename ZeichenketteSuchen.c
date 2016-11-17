/* file: ZeichenketteSuchen.c
 *
 * Copyright (C) am 6.November2016 Stefan Straubinger
 * This program is free software. Use it only under the acceptance that
 * ************* IT COMES WITH NO WARRANTY WHATSOEVER *****************
 * You can redistribute and/or modify it under the TERMS AND CONDITIONS of the
 * Do What The Fuck You Want To Public License, Version 2, as 
 * published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
 *
 * A simple "Find and Replace" piece of code.
 * Usefull for changing IP_address in a gophermap file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 120       /* maximum length of Input Line*/

#define DEBUG 0


long mygetline(char s[], long lim);

int main()
{
    int i;
    char * needle = "127.0.0.1";            // searching for needle
    char IP_address []= "44.143.58.106";    // replace needle with IP_address
    char haystack [MAXLEN] = "";            // a line of the input text,
    char buffer [MAXLEN] = "";              // copy of input line
    char leer [MAXLEN] = "";
    char * locator;


    //printf("Replace String with another!\n");

    while(mygetline(haystack, MAXLEN) ) {
        while (locator = strstr(haystack,needle))
        {
            if (DEBUG) printf("-----------------------------------------\n%p\n--->%s<---\n",locator, locator);
            if (DEBUG) printf("%s", haystack);

            if (strlen(needle) == strlen(IP_address))
                strncpy(locator, IP_address, strlen(IP_address));
            if (strlen(needle) > strlen(IP_address)) {
                strncpy(locator, IP_address, strlen(IP_address));
                strcpy(buffer, locator);
                strcpy(locator+strlen(IP_address), buffer + strlen(needle));
            }
            if (strlen(needle) < strlen(IP_address)) {
                strcpy(buffer, locator);
                memmove(locator + (strlen(IP_address) - strlen(needle)),buffer,strlen(buffer) );
                strncpy(locator, IP_address, strlen(IP_address));
            }
            for (i=0; i< MAXLEN; i++) //delete buffer.
                buffer[i] = '\0';

        }
        printf("%s", haystack);

    }
    printf("\n");

    return 0;
}



/* getline: read a line into s
 * return: length, is 0 if EOF
 */
long mygetline(char s[], long lim) {
  long c, i;
  for (i = 0; (i < lim - 1) && ((c=getchar()) != EOF) && (c != '\n'); i++) {
    s[i] = c;
  }

  if (c == '\n') {
    s[i] = c;
    i++;
  }

  s[i] = '\0';
  return i;
} // end mygetline
