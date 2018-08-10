#ifndef FPAK_FPAK_H
#define FPAK_FPAK_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#ifdef _WIN32
char pathseperator = '\\';
#else
char pathseperator = '/';
#endif

typedef struct archivefile {
    long filesize;
    char filename[32];
} archivefile;

//Globale Variablen
archivefile **headertable; //Speichert Dateigrößen und längen der Dateien nacheinander im Header.
int afilearrlength = 0; //Länge es Archivefile Array.
int currfiles = 0; //Aktuelle Anzahl aller Dateien
char *files; //Beinhaltet den Inhalt ALLER Dateien, Dynamisches Array.
long long fileslength; //Länge des file arrays
FILE *fpak; //Das Archiv, das gepackt / entpackt werden soll.
int createfolderflag = 0; //Flag, ob fürs entpacken ein neuer Ordner erstellt werden soll in welchem dann die Dateien entpackt werden.

//Fügt Dateien dem Index (Variable files) hinzu und vergrößert ihn auch automatisch.
void addfiletoindex( FILE *file );

//Erstellt eine neue ArchiveFile und gibt die Adresse der gefüllten File zurück.
archivefile *newarchivefile( long *filesize, char *filename );

//Fügt dem afile array eine afile hinzu.
void addarchivefile( archivefile *afile );

//Schreibt den Header in das Archiv. (Magic Bytes, Anzahl der Dateien, Länge Array, Array)
void writeheader( void );

//Schreibt die ganzen Dateien in das Archiv.
void writefilestofpak( void );

//Überprüft ob das Archiv gültig ist.
bool checkfpak( void );

//Überprüfr, ob der Dateiname die richtige Länge hat
bool checkfile( char *filename );

//Überprüft ob Parameter eine gültige Datei ist
bool isfile( FILE *file ) {
    return file != NULL;
}

//Gibt die Dateigröße zurück
long getfilesize( FILE *file );

//Entpackt ein Archiv.
void unpack( void );

//Zeigt die Nutzung / Hilfe an.
void usage( char *prgname );

//Zeigt das Versionsmenü an.
void version( char *prgname );

//Gibt den Dateinamen von einem Dateipfad zurück.
char *getfilenamefrompath( char *filepath );

//Entfernt einen Teil eines Strings
char *cutstring( int from, char *string );


#endif //FPAK_FPAK_H
