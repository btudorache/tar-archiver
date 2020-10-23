// Tudorache Bogdan-Mihai 314CA
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// citire linie cu linie
void read_line(char line[512]) {
    fgets(line, 512, stdin);
    int size = strlen(line);
    line[size-1] = '\0';
}

/* citire linie cu linie dintr-un fisier si creearea unei copii
   pentru utilizare viitoare */
void read_line_file(char line[512], FILE *files, char cpy[512]) {
    fgets(line, 512, files);
    int size = strlen(line);
    line[size-1] = '\0';
    strcpy(cpy, line);
}

/* numararea argumentelor din linia de comanda si salvarea
   fiecarui argument in parte la valoarea unor pointeri */
int word_count_args(char line[512], char **command, char **arg1, char **arg2) {
    int cnt = 0;
    char *pch = strtok(line, " ");
    *command = pch; // salvarea primului argument
    while (pch != NULL) {
        cnt++;
        pch = strtok(NULL, " ");
        if (cnt == 1) {
            *arg1 = pch; // salvarea argumentului 2
        } else if (cnt == 2) {
            *arg2 = pch; // salvarea argumentului 3
        }
    }
    return cnt;
}

// extragerea datei ultimei modificari din fisierul "files.txt"
void get_time(char line[512], char **date, char **clock, char time[40]) {
    int cnt = 0;
    char *pch = strtok(line, " ");
    while (cnt != 6) {
        cnt++;
        pch = strtok(NULL, " ");
        if (cnt == 5) {
            *date = pch; // salvarea datei
        } else if (cnt == 6) {
            *clock = pch; // salvarea timpului
        }
    }
    char date_temp[40], clock_temp[40];
    strcpy(date_temp, *date);
    strcpy(clock_temp, *clock);

    // eliminarea valorilor de dupa vigrula de dupa secunde
    clock_temp[strlen(clock_temp)-10] = 0;

    // concatenarea timpului in formatul necesar in variabila "time"
    strcpy(time, date_temp);
    strcat(time, " ");
    strcat(time, clock_temp);
}

// structura headerului fisierului .tar
union record {
    char charptr[512];
    struct header {
        char name[100];
        char mode[8];
        char uid[8];
        char gid[8];
        char size[12];
        char mtime[12];
        char chksum[8];
        char typeflag;
        char linkname[100];
        char magic[8];
        char uname[32];
        char gname[32];
        char devmajor[8];
        char devminor[8];
    } header;
};

// numararea liniilor dintr-un fisier
int files_counter(FILE *files, char line[512]) {
    int cnt = 0;
    while (fgets(line, 512, files) != NULL) {
        cnt++;
    }
    return cnt;
}

/* extragerea unor informatii necesare pentru headerul fisierului
   .tar si salvarea lor in structura specifica */
void get_info(union record *data, char line[512], int i, char *uid, char *gid) {
    int cnt = 0;
    char *pch = strtok(line, " ");
    char *owner_name, *owner_group, *size, *name;
    while (cnt != 8) {
        cnt++;
        pch = strtok(NULL, " ");
        if (cnt == 2) {
            owner_name = pch; // extragerea campului "uname" din files.txt
        } else if (cnt == 3) {
            owner_group = pch;// extragerea campului "gname" din files.txt
        } else if (cnt == 4) {
            size = pch;       // extragerea campului "size" din files.txt
        } else if (cnt == 8) {
            name = pch;       // extragerea campului "name" din files.txt
        }
    }
    // transformarea in int a char-ului "size"
    int int_size = atoi(size);
    // transformarea in octal a valorii lui "size"
    sprintf(size, "%o", int_size);
    // salvarea campului "name" in header
    strcpy(data[i].header.name, name);
    // salvarea campului "linkname" in header
    strcpy(data[i].header.linkname, name);
    // salvarea campului "uname" in header
    strcpy(data[i].header.uname, owner_name);
    // salvarea campului "gname" in header
    strcpy(data[i].header.gname, owner_group);
    /* setarea valorii "chksum" in spaces
       pentru calcularea chksum-ului propriu zis */
    strcpy(data[i].header.chksum, "       ");
    data[i].header.chksum[7] = ' ';
    // salvarea campului "typeflag" in header
    data[i].header.typeflag = '0';
    // salvarea campului "magic" in header
    strcpy(data[i].header.magic, "GNUtar ");

    /* constuirea campului "size" dupa structura ceruta de formatul .tar 
       (concatenarea unor zero-uri la inceput) */
    char fsize[12];
    strcpy(fsize, "0");
    for (int i = 0; i < 12 - (int)strlen(size) - 2; i++) {
        strcat(fsize, "0");
    }
    strcat(fsize, size);
    // salvarea campului "size" in header
    strcpy(data[i].header.size, fsize);

    /* constuirea campurilor "uid" si "gid" dupa structura ceruta de formatul .tar 
       (concatenarea unor zero-uri la inceput) */
    char fuid[8], fgid[8];
    strcpy(fuid, "0");
    strcpy(fgid, "0");
    for (int i = 0; i < 8 - (int)strlen(uid) - 2; i++) {
        strcat(fuid, "0");
    }
    strcat(fuid, uid);
    for (int i = 0; i < 8 - (int)strlen(gid) - 2; i++) {
        strcat(fgid, "0");
    }
    strcat(fgid, gid);

    // salvarea campului "uid" in header
    strcpy(data[i].header.uid, fuid);
    // salvarea campului "gid" in header
    strcpy(data[i].header.gid, fgid);
}

// extragerea campurilor "uid" si "gid" din fisierul "usermap.txt"
void get_uid_gid(char usermap[512], char **uid, char **gid) {
    int cnt = 0;
    char *pch = strtok(usermap, ":");
    while (cnt != 3 && pch != NULL) {
        cnt++;
        pch = strtok(NULL, ":");
        if (cnt == 2) {
            *uid = pch; // extragerea campului "uid" din "usermap.txt"
        } else if (cnt == 3) {
            *gid = pch; // extragerea campului "gid" din "usermap.txt"
        }
    }
    // transformarea char-ului "uid" in int
    int int_uid = atoi(*uid);
    // transformarea char-ului "gid" in int
    int int_gid = atoi(*gid);
    // scrierea int-ului "uid" in octal si transformare la loc in char
    sprintf(*uid, "%o", int_uid);
    // scrierea int-ului "gid" in octal si transformare la loc in char
    sprintf(*gid, "%o", int_gid);
}

// extragerea permisiunilor din fisierul "files.txt"
void extract_perm(char line[512], char **user, char **group, char **others) {
    int cnt = 0;
    char *pch = strtok(line, "-");
    *user = pch; // extragerea permisiunii pentru "user"
    while (cnt != 2) {
        cnt++;
        pch = strtok(NULL, "-");
        if (cnt == 1) {
            *group = pch; // extragerea permisiunii pentru "group"
        } else if (cnt == 2) {
            *others = pch; // extragerea permisiunii pentru "others"
        }
    }
}

// echivalarea permisiunilor in valorile lor specifice din octal
void set_permissions(char **permission) {
    if (strcmp(*permission, "x") == 0) {
        strcpy(*permission, "1");
    } else if (strcmp(*permission, "w") == 0) {
        strcpy(*permission, "2");
    } else if (strcmp(*permission, "wx") == 0) {
        strcpy(*permission, "3");
    } else if (strcmp(*permission, "r") == 0) {
        strcpy(*permission, "4");
    } else if (strcmp(*permission, "rx") == 0) {
        strcpy(*permission, "5");
    } else if (strcmp(*permission, "rw") == 0) {
        strcpy(*permission, "6");
    } else if (strcmp(*permission, "rwx") == 0) {
        strcpy(*permission, "7");
    }
}

/* stabilirea permisiunilor prin echivalarea in valorile lor specifice 
   din octal si construirea campului ce va fi salvat in header */
void perm_final(char **u, char **g, char **o, union record *data, int i) {
    set_permissions(u); // echivalare in octal pentru "read"
    set_permissions(g); // echivalare in octal pentru "write"
    set_permissions(o); // echivalare in octal pentru "execute"

    /* construirea campului "mode" dupa structura ceruta de arhiva
       .tar si de cerinta prin adaugarea unor zero-uri la inceput */
    char permissions_final[8] = "0000";
    strcat(permissions_final, *u);
    strcat(permissions_final, *g);
    strcat(permissions_final, *o);

    // salvarea campului "mode" in header
    strcpy(data[i].header.mode, permissions_final);
}

// iesirea fortata din program in cazul in care nu se poate deschide un fisier
void file_error(FILE *file) {
    if (file == NULL) {
        printf("> Failed!\n");
        exit(-1);
    }
}

/* citire linie cu linie din fisierul "usermap.txt" pana 
   la linia unde se afla "uid"-ul si "gid"-ul userului curent (bogdan) */
void get_usermap_line(char line[512], FILE *usermap) {
    for (int i = 0; i < 46; i++) {
        fgets(line, 512, usermap);
    }
    int size = strlen(line);
    line[size-1] = '\0';
}

// scrierea datelor din fisierul ales pentru arhivare in arhiva propriu-zisa
void data_write(FILE *file, FILE *archive, union record *data, int i) {
    // scrierea header-ului in arhiva
    fwrite(&data[i], sizeof(union record), 1, archive);
    char buffer, zero = 0;
    // scrierea octet cu octet a fisierului in arhiva
    while (fread(&buffer, sizeof(char), 1, file)) {
        fwrite(&buffer, sizeof(char), 1, archive);
    }
    fclose(file);
    int size;
    sscanf(data[i].header.size, "%o", &size);
    /* in cazul in care dimensiunea fisierului nu e multiplu de 512, arhiva 
       se va umple cu zero-uri pana cand size-ul va fi multiplu dorit */
    int remaining = 512 - (size % 512);
    if (remaining != 512) {
        for (int i = 0; i < remaining; i++) {
            fwrite(&zero, sizeof(char), 1, archive);
        }
    }
}

/* scrierea unui bloc de 512 de octeti de zero la finalul arhivei, inchiderea
 fisierelor ramase deschise si eliberarea memoriei alocata dinamic */
void ending(FILE *archive, FILE *files, FILE *usermap, union record *data) {
    char zero = 0;
    // scrierea blocului de zero
    for (int i = 0; i < 512; i++) {
        fwrite(&zero, sizeof(char), 1, archive);
    }
    fclose(files);
    fclose(usermap);
    fclose(archive);
    printf("> Done!\n");
    free(data);
}

// calcularea chksum-ului (suma octetilor din header)
void chksum(union record *data, int i) {
    int sum = 0;
    // calcularea sumei
    char *pnt =(char *)data[i].charptr;
    for (int j = 0; j < 512; j++) {
        sum = sum + pnt[j];
    }
    char string_sum[8];
    // transformarea valorii in octal
    sprintf(string_sum, "%o", sum);
    char fstring_sum[8];
    /* construirea campului "chksum" dupa structura ceruta de arhiva .tar 
       (adaugarea unor zero-uri la inceput si niste spatii goale la final) */
    strcpy(fstring_sum, "0");
    for (int i = 0; i < 8 - (int)strlen(string_sum) - 3; i++) {
        printf("%d \n", i);
        strcat(fstring_sum, "0");
    }
    strcat(fstring_sum, string_sum);
    strcpy(data[i].header.chksum, fstring_sum);
    int size = strlen(fstring_sum);
    for (int j = size + 1; j < 8; j++) {
        data[i].header.chksum[j] = ' ';
    }
}

/* concatenarea unor siruri de caractere pentru a forma 
   numele fisierelor ce urmeaza a fi deschide */
void concat_files(char file1[512], char file2[512], char *argument) {
    strcpy(file1, argument);
    strcat(file1, "files.txt");
    strcpy(file2, argument);
    strcat(file2, "usermap.txt");
}

// concatenarea numelui fisierului care trebuie arhivat pentru a fi deschis
void concat_name(char data_line[512], char *arg2, union record *data, int i) {
    strcpy(data_line, arg2);
    strcat(data_line, data[i].header.name);
}

/* numararea liniilor din fisierul files.txt si 
   pozitionarea "cursorului" la inceputul fisierului */
void count_and_setCursor(int *cnt, char linie[512], FILE *files) {
    *cnt = files_counter(files, linie);
    fseek(files, 0, SEEK_SET);
}

/* salvarea timpului in secunde cu ajutorul 
   functiilor si uneltelor din biblioteca time.h */
void mtime(struct tm tm, time_t t, char time[40], int i, union record *data) {
    /* salvarea timpului in formatul necesar 
       in structura tm specifica bibliotecii time.h */
    strptime(time, "%Y-%m-%d %T", &tm);
    // obtinerea timpului in secunde
    t = mktime(&tm);
    char o_time[40];
    // transformarea valorii timpului in octal
    sprintf(o_time, "%lo", t);
    // salvarea campului "mtime" in header
    strcpy(data[i].header.mtime, o_time);
}

/* functie care intoarce valoarea dependenta de "size" care trebuie 
   sarita pentru a ajunge la urmatorul fisier in cadrul arhivei */
int get_jump(int size) {
    if (size % 512 == 0) {
        return size;
    } else {
        return size + (512 - size % 512);
    }
}

// implementarea functiei "list"
void list(char *argument) {
    // deschiderea arhivei pentru citire
    FILE *archive = fopen(argument, "rb");
    file_error(archive);
    char name[100], size[12];
    int size_dec;
    while (1) {
        // citirea campului "name" din header (primii 100 de octeti)
        fread(&name, sizeof(char), 100, archive);
        // ciclul se inchide atunci cand se ajunge la blocul de 512 octeti de 0
        if (name[0] == '\0') {
            break;
        }
        // afisarea numelui fisierului
        printf("> %s\n", name);
        // "salt" in arhiva pana la campul "size"
        fseek(archive, 24, SEEK_CUR);
        // citirea valorii lui "size"
        fread(&size, sizeof(char), 12, archive);
        // transformarea valorii lui "size" in decimal
        sscanf(size, "%o", &size_dec);
        // salt pana la urmatorul fisier din arhiva
        size_dec = get_jump(size_dec);
        fseek(archive, 376 + size_dec, SEEK_CUR);
    }
    // inchiderea arhivei
    fclose(archive);
}

// implementarea functiei "extract"
void extract(char *arg1, char *arg2) {
    // deschiderea arhivei pentru citire
    FILE *archive = fopen(arg2, "rb");
    file_error(archive);
    char name[100], size[12], buffer;
    int size_dec, size_aux, ok = 0;
    while (1) {
        // citirea campului "name" din header (primii 100 de octeti)
        fread(&name, sizeof(char), 100, archive);
        // ciclul se inchide atunci cand se ajunge la blocul de 512 octeti de 0
        if (name[0] == '\0') {
            break;
        }
        // "salt" in arhiva pana la campul "size"
        fseek(archive, 24, SEEK_CUR);
        // citirea valorii lui "size"
        fread(&size, sizeof(char), 12, archive);
        // transformarea valorii lui "size" in decimal
        sscanf(size, "%o", &size_dec);
        // "salt" pana la inceputul prorpiu-zis al fisierului din arhiva
        fseek(archive, 376, SEEK_CUR);
        /* daca numele fisierului este egal cu cel specificat in linia de 
           comanda, se va extrage noul fisier, altfel se va cauta in 
           continuare un fisier cu numele cautat */
        if (strcmp(name, arg1) == 0) {
            ok = 1;
            break;
        }
        size_aux = get_jump(size_dec);
        fseek(archive, size_aux, SEEK_CUR);
    }
    char new_file[108];
    // creearea numelui fisierului extras
    strcpy(new_file, "extracted_");
    strcat(new_file, name);
    // creearea propriu-zisa a fisierului extras
    FILE *file = fopen(new_file, "wb");
    file_error(file);
    // copierea octet cu octet din arhiva in fisierul extras
    for (int i = 0; i < size_dec; i++) {
            fread(&buffer, sizeof(char), 1, archive);
            fwrite(&buffer, sizeof(char), 1, file);
        }
    if (ok == 0) {
        printf("> File not extracted!\n");
    } else {
        printf("> Done!\n");
    }
    // inchiderea fisierelor
    fclose(file);
    fclose(archive);
}

int main() {
    int arguments, run = 1, cnt = 0;
    char *command, *arg1, *arg2, *uid, *gid, line[512];
    /* programul ruleaza intr-un ciclu while care 
       se inchide pana la primirea comenzii "exit" */
    while (run) {
        /* se citeste o linie si se imparte in argumente; daca linia 
           nu respecta structura impusa se va afisa "> Wrong command!" */
        read_line(line);
        arguments = word_count_args(line, &command, &arg1, &arg2);
        // programul se inchide la primirea comenzii "exit"
        if (strcmp(command, "exit") == 0 && arguments == 1) {
            run = 0;
        // executatrea comenzii "create"
        } else if (strcmp(command, "create") == 0 && arguments == 3) {
            char myFiles[512], line[512], myUsermap[512], lineUsermap[512];
            union record *data = NULL;

            // deschiderea fisierelor necesare pentru a creea arhiva
            concat_files(myFiles, myUsermap, arg2);
            FILE *files = fopen(myFiles, "r");
            file_error(files);
            FILE *archive = fopen(arg1, "wb");
            file_error(archive);
            FILE *usermap = fopen(myUsermap, "r");
            file_error(usermap);

            // extragerea uid-ului si gid-ului din usermap.txt
            get_usermap_line(lineUsermap, usermap);
            get_uid_gid(lineUsermap, &uid, &gid);

            /* numararea liniilor din files.txt (numarul de fisiere ce urmeaza
             a fi arhivate) si plasarea "cursorului" la inceputul fisierului */
            count_and_setCursor(&cnt, line, files);

            // alocare dinamica a vectorului de uniuni
            data = malloc(cnt * sizeof(union record));
            // transformarea fiecarui octet al vectorului in 0
            memset(data, 0, cnt * sizeof(union record));

            char *read, *write, *exec, line_data[512], *date, *clock, cpy[512];
            // arhivarea fisier cu fisier
            for (int i = 0; i < cnt; i++) {
                char time[40];
                // construirea header-ului prin apelarea functiilor necesare
                // citirea unei linii din files.txt
                read_line_file(line, files, cpy);
                // extragerea si salvarea unor informatii (nu toate)
                get_info(data, line, i, uid, gid);
                // extragerea permisiunilor
                extract_perm(line, &read, &write, &exec);
                // salvarea permisiunilor
                perm_final(&read, &write, &exec, data, i);

                // extragerea timpului
                get_time(cpy, &date, &clock, time);
                struct tm tm = {0};
                time_t t = 0;
                // prelucrarea si salvarea timpului
                mtime(tm, t, time, i, data);

                /* construirea si salvarea 
                   chksum-ului (suma octetilor din header) */
                chksum(data, i);


                concat_name(line_data, arg2, data, i);
                // deschiderea fisierului cu datele propriu-zise
                FILE *date = fopen(line_data, "rb");
                file_error(date);
                // scrierea in arhiva
                data_write(date, archive, data, i);
            }
        /* finalizarea arhivei (inchiderea fisierelor, eliberarea memoriei 
           alocate dinamic, scrierea unui bloc de 512 octeti de zero) */
        ending(archive, files, usermap, data);
        // executarea comenzii "list"
        } else if (strcmp(command, "list") == 0 && arguments == 2) {
            list(arg1);
        // executarea comenzii "extract"
        } else if (strcmp(command, "extract") == 0 && arguments == 3) {
            extract(arg1, arg2);
        } else {
            printf("Wrong command!\n");
        }
    }
    return 0;
}
