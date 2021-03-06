/* directory2gophermap.c
 *
 * copyright Stefan Straubinger am 23.Oktober2016
 *
 *
 * "ls -b > lsb" lists the filenames in the directory and stores it in a file called lsb.
 * Blanks are with escape sequence "\ ". so to make a valid filename delete backslashes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000 /* max. linelength*/

#define DEBUG 0

/* ls -b outputs filenames in the current directory,
 * There is a blackslash in front of each space character.
 * This function also removes annoying trailing newline,
 * which is here often used as a work around!
 */
void deletebackslash(char *dateiname){
    long i, j;
    char myline[MAXLEN] = "";
    for (i = 0, j = 0; dateiname[i]!='\n' &&dateiname[i] != '\0'; i++) {
        if (dateiname[i] == '\\')
            continue;
        myline[j] = dateiname[i];
        j++;
    }
    myline[j]  = '\0';
    strcpy(dateiname, myline);
}

/* this function if for debugging purpose only
 * There were newline at the end of the filenames.
 */
void showlastchar(char *dateiname) {
    long i;
    for (i = 0; dateiname[i] != '\0'; i++) /* goes through the array till end. Why not use length or sizeof function?*/
        ;
    printf("***%c***(%ld)\n", dateiname[i-1], i);
}

int main()
{
    FILE *fp;               /* file pointer for output */
    FILE *fp_element;       /* file pointer for reading bookmark-files entry */
    FILE *fp_infile;

    const char IPADDRESS[] = "45.32.xxx.xxx";
    const char PORT[] = "70";
    char dateiname[MAXLEN] = "\0";      /* filename is the entry of the current line in lsb*/
    const char pfad[MAXLEN]="/home/gast/Desktop/c-test/gopher-helper-programs/\0"; /* directory of bookmark files */
    char pfadmitdatei[MAXLEN] = "\0";   /* combination of directory and filename, so the full PATH of current bookmark file */

    printf("directory2gophermap!\n");

    fp_infile = fopen("lsb", "r");      /* lsb file contains a list of files in the directory generated by ls -b > lsb */

    fp = fopen("gophermap.directory","w"); /* filepointer for output */
    if (fp == NULL) {
        fprintf(stderr, "could not generate output file pointer!\n");
        return 1;
    }

    fprintf(fp,"Meine Bilder vom Stocker Kogel.\n");
    while ( 1==1 )
        if (fgets(dateiname, MAXLEN, fp_infile)) {
            deletebackslash(dateiname);
            /* gophermap file format:
             * iIntotext\tbeliebig\tbeliebig\tbeliebig\n
             * hTitle\t/URL:http://example.org/\t45.32.xxx.xxx\t70\n
             * IImage other than gif\tImage.jpeg\t45.32.xxx.xxx\t70\n
             */
            fprintf(fp,"I%s\t%s\t%s\t%s\n", dateiname, dateiname, IPADDRESS, PORT);
            deletebackslash(dateiname);             /* remove trailing \n*/
            /* delete pfadmitdatei, copy const PATH, and filename*/
            strcpy(pfadmitdatei, "\0");
            strcpy(pfadmitdatei, pfad);
            strcat(pfadmitdatei, dateiname);
        }
        else { //the End of fp
            fprintf(stderr, "Fehler bei lesen von dateiname oder Dateiende?\n");
            fprintf(fp, "i\tfake\t(NULL)\t0\n");
            fprintf(fp, "i______________________________________________________________________\tinfo\tnull.host\t1\n");
            fprintf(fp, "i                   Gophered by pygopherd/2.0.16 on Void/x86_64 4.8.3_1\tinfo\tnull.host\t1\n");
            break;
        }
    fcloseall();
    return 0;
}
