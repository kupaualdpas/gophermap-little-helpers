/* bookmarks2gophermap.c
 *
 * copyright Stefan Straubinger am 23.Oktober2016
 *
 * For each of my bookmarks I added an file which name should be the clickable link title.
 * It contains in first line the URL, in the 2nd the date of meant visit,
 * and the rest is further description of the URL
 *
 * "ls -b > lsb" lists the filenames in the directory and stores it in a file called lsb.
 * Blanks are with escape sequence "\ ". so to make a valid filename delete backslashes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

    char dateiname[MAXLEN] = "\0";      /* filename is the entry of the current line in lsb*/
    const char pfad[MAXLEN]="/home/gast/Desktop/c-test/gopher-helper-programs/\0"; /* directory of bookmark files */
    char pfadmitdatei[MAXLEN] = "\0";   /* combination of directory and filename, so the full PATH of current bookmark file */
    char zeile[MAXLEN] = "\0";          /* current line of bookmark file content, like URL, date, description */

    const char IP_ADDRESS[] = "45.32.xxx.xxx"

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = gmtime (&rawtime);       /* the current time and date in GMT */

    printf("bookmarks2gophermap!\n");

    fp_infile = fopen("lsb", "r");      /* lsb file contains a list of files in the directory generated by ls -b > lsb */

    fp = fopen("gophermap.bookmarks","w"); /* filepointer for output */
    if (fp == NULL) {
        fprintf(stderr, "could not generate output file pointer!\n");
        return 1;
    }

    fprintf(fp,"Meine Bookmarks mit Stand vom %s %s.\n Liste erzeugt mit ls -b > lsb und bookmarks2gophermap, einem selbst\n geschriebenen C-Programm\n", asctime(timeinfo), timeinfo->tm_zone);
    while ( 1==1 )
        if (fgets(dateiname, MAXLEN, fp_infile)) {
            deletebackslash(dateiname);
            /* gophermap file format:
             * iIntotext\tbeliebig\tbeliebig\tbeliebig\n
             * hTitle\t/URL:http://example.org/\t45.32.xxx.xxx\t70\n
             */
            fprintf(fp,"h%s\t", dateiname);
            if (DEBUG) showlastchar(dateiname);
            deletebackslash(dateiname);             /* remove trailing \n*/
            if (DEBUG) showlastchar(dateiname);
            /* delete pfadmitdatei, copy const PATH, and filename*/
            strcpy(pfadmitdatei, "\0");
            strcpy(pfadmitdatei, pfad);
            if (DEBUG) showlastchar(pfadmitdatei);
            strcat(pfadmitdatei, dateiname);
            if (DEBUG) printf("\"%s\" pfadmitdatei\n", pfadmitdatei);

            /* Open bookmark file and get all infos out.
             */
            if ((fp_element = fopen(pfadmitdatei,"r")) == NULL){
                fprintf(stderr, "Fehler beim Lesen der Datei pfadmitdatei bei fp_element\n"); /* eventually skip bookmark files is ok*/
            }
            else {
                fgets(zeile, MAXLEN, fp_element);
                deletebackslash(zeile); //delete trailing newline \n
                if (DEBUG) printf("\"%s\"", zeile);
                fprintf(fp, "/URL:%s\t%s\t70\n", zeile, IP_ADDRESS); //URL

                //Stand und Beschreibung als Info
                while (fgets(zeile, MAXLEN, fp_element) != NULL){
                    deletebackslash(zeile);
                    fprintf(fp, "i%s\tfake\t(NULL)\t0\n", zeile);
                }
                fclose(fp_element);
            }

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