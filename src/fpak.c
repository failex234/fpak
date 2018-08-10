#include "fpak.h"

int main( int argc, char **argv ) {
    if (argc == 1 || !strcmp(argv[1], "--help")) {
        usage(argv[0]);
        return argc == 1;
    } else {
        if (!strcmp(argv[1], "pack") && argc > 3) {
            //Loopt durch alle angegebenen Dateien
            for (int i = 3; i < argc; i++) {
                FILE *temp = fopen(argv[i], "r");
                if (isfile(temp)) {
                    if (checkfile(getfilenamefrompath(argv[i]))) {
                        addarchivefile(newarchivefile((long *) getfilesize(temp), getfilenamefrompath(argv[i])));
                        addfiletoindex(temp);
                        fclose(temp);

                        printf("packing %s...\n", getfilenamefrompath(argv[i]));
                    } else {
                        printf("sorry but a filename can't be longer than 32 characters! %s is too long\n", argv[i]);
                        fclose(temp);
                        return 1;
                    }
                } else {
                    printf("no such file or directory %s\n", argv[i]);
                    return 1;
                }
            }

            fpak = fopen(argv[2], "wb");
            if (!isfile(fpak)) {
                printf("unable to open %s...\n", argv[2]);
                return 1;
            }

            writeheader();
            writefilestofpak();

            fclose(fpak);
            free(headertable);
            free(files);

            printf("all files were packed to %s\n", argv[2]);
        } else if (!strcmp(argv[1], "unpack") && argc > 2) {

        } else {
            if (!strcmp(argv[1], "--version")) {
                version(argv[0]);
                return 1;
            }
            usage(argv[0]);
            return 1;
        }
    }
    return 0;
}

char *getfilenamefrompath( char *filepath ) {
    int lastslash = 0;
    for (int i = 0; i < strlen(filepath); i++) {
        if (filepath[i] == pathseperator) {
            lastslash = i;
        }
    }
    return cutstring(lastslash, filepath);
}

char *cutstring(int from, char *string ) {
    char *newstring = malloc(sizeof(char) * strlen(string) - from);
    int newstringindex = 0;

    for (int i = from; i < strlen(string); i++) {
        newstring[newstringindex] = string[i];
        newstringindex++;
    }

    return newstring;
}

bool checkfile( char *filename ) {
    return strlen(filename) <= 32;
}

long getfilesize( FILE *file ) {
    int size = 0;

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    return size;
}

archivefile *newarchivefile( long *filesize, char *filename ) {
    archivefile *newfile = malloc(sizeof(archivefile));
    strcpy(newfile->filename, filename);
    newfile->filesize = filesize;

    return newfile;
}

void addarchivefile( archivefile *afile ) {
    if (afilearrlength == 0) {
        headertable = malloc(sizeof(archivefile));
        headertable[0] = afile;
        afilearrlength++;
    } else {
        headertable = realloc(headertable, sizeof(archivefile) * afilearrlength);
        headertable[afilearrlength] = afile;
        afilearrlength++;
    }
}

void addfiletoindex( FILE *file ) {
    if (currfiles == 0) {
        files = malloc(sizeof(char) * getfilesize(file));
    } else {
        files = realloc(files, sizeof(char) * (getfilesize(file) + fileslength));
    }

    char c;
    while((c = (char) fgetc(file)) != EOF) {
        files[fileslength] = c;
        fileslength++;
    }
    currfiles++;
}

void writeheader( void ) {
    //Magic Bytes
    fputs("FPAK", fpak);

    //fpak version
    char *version = malloc(sizeof(char) * 3);
    sprintf(version, "%d", VERSION);
    fputs("%d");

    //write length of array and the array itself
    //write size of all files and number of files
    //write file struct array
    char *filenum = malloc(sizeof(char) * 32);
    char *arrlen = malloc(sizeof(char) * 32);
    sprintf(filenum, "%d", currfiles);
    sprintf(arrlen, "%d", afilearrlength);

    fputs(filenum, fpak);
    fputs(arrlen, fpak);
    fwrite(headertable, sizeof(archivefile), afilearrlength, fpak);


    free(filenum);
    free(arrlen);
}

void writefilestofpak( void ) {
    fputs(files, fpak);
}

void usage ( char *prgname ) {
    printf("%s - usage\n", prgname);
    printf("%s pack <archive> [FILES...]\n", prgname);
    printf("%s unpack [-c] <archive>\n", prgname);
    printf("The oprional argument [-c] unpacks the files in a new folder\n");
    printf("\nEXAMPLES\n\n");
    printf("%s pack backup.fpak pic1.jpg pic2.jpg pic3.jpg\n", prgname);
    printf("%s unpack backup.fpak\n", prgname);
    printf("%s unpack -c backup.fpak\n", prgname);
}

void version( char *prgname ) {
    printf("%s version 0.0.1 by Felix Naumann\n", prgname);
}
