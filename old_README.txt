Tudorache Bogdan-Mihai 314CA

Programarea calculatoarelor - Tema #3 Fisiere, Stringuri si Structuri

Task 0 - archiver:
    
    Programul ruleaza intr-un ciclu while care se inchide la primirea comenzii "exit". Dupa rularea programului, din linia de comanda se citeste cate o linie si fiecare argument este delimitat cu ajutorul functiei strtok. daca comanda primita nu respecta limitele impuse (primul cuvant nu este "create", "list" sau "extract" urmate de argumentele necesare) se afiseaza "> Wrong command!", si altfel se executa comanda. Programul recunoaste daca se afla in limitele impuse de cele 3 comenzi prin impartirea liniei in string-uri separate, numararea cuvintelor si utilizarea functiei strcmp. 

Task 1 - create:

    Implementarea functiei "create" implica doua parti importante: construirea campului header pentru fiecare fisier ce trebuie arhivat in parte si creearea prorpiu-zisa a arhivei. Pentru completarea celor 14 campuri din header se folosesc fisierele files.txt si usermap.txt. Structura header-ului trebuie sa fie exact la fel ca cea aflata in documentatia arhivei .tar, altfel nu va fi recunoscuta.
    Initial se numara numarul de linii din fisierul "files.txt" (numarul de fisiere) iar completarea header-ului si scrierea in arhiva octet cu octet se face pentru fiecare fisier in parte.

    Majoritatea campurilor se completeaza usor, deschizand cele doua fisiere auxiliare, citind linie cu linie cu fgets, delimitand string-urile necesare cu strtok, si eventual transformand valorile in octal cu sprintf (au fost folosite functiile "get_info", "get_uid_gid"). Campurile a caror completare a fost mai complexa sunt "mode", "mtime" si "chksum". 

    Pentrul campul "mode" am extras din fisierul files.txt cele trei permisiuni pentru "user", "group", si "others", formate din maxim 3 caractere, prin functia "extract_perm", si am echivalat aceste permisiuni in valorile lor specifice din octal (read - 4, write - 2, execute - 1, si toate cele 8 combinatii) cu ajutorul functiei "set_permissions". Cu aceste trei valori am construit valoarea necesara campului mode prin concatenare (cu ajutorul functiei "perm_final").

    Campul "mtime" se completeaza cu ajutorul functiilor si tipurilor de date introduse de biblioteca time.h. Am extras timpul in formatul necesar din fisierul files.txt cu ajutorul functiei "get_time" (strtok si concatenari). In functia "mtime" se efectueaza operatiile necesare pentru a salva campul "mtime" din header. Salvarea timpului in formatul necesar in structura tm specifica bibliotecii time.h se face cu ajutorul functiei "strptime". Obtinerea timpului in secunde din structura tm se face prin functia "mktime". Aceasta valoare se transforma in octal si se salveaza in header.

    Calcularea chksum-ului consta in calcularea tuturor octetilor din header. Suma se realizeaza prin declararea unui pointer de tipul char "fortat" sa arate catre primul octet al structurii header, se parcurg pe rand cei 512 octeti si se aduna pe rand. Se construieste campul "chksum" dupa structura ceruta de arhiva .tar, asa cum se face si pentru alte cateva campuri din header (adaugarea unor zero-uri la inceput si adaugarea unor spatii goale la final).
    
    Scrierea datelor din fisierul ales pentru arhivare in arhiva propriu-zisa se face cu functia "data_write". Pentru fiecare fisier in parte, se scrie header-ul in arhiva si se scrie octet cu octet continutul fisierului in arhiva. In cazul in care dimensiunea fisierului nu e multiplu de 512 (asa cum impune formatul arhivei .tar), arhiva se va umple de zero-uri pana cand size-ul va fi multiplu dorit.
    Finalizarea procesului de creeare a arhivei se face prin functia "ending". Prin aceasta are loc scrierea unui bloc de 512 de octeti de zero la finalul arhivei, inchiderea fisierelor ramase deschise si eliberarea memoriei alocate dinamic.
    
Task 2 - list:

    Implementarea functiei programului "list" se face cu ajutorul functiei scrise in C "list", care primeste ca argument numele arhivei. Se deschide arhiva pentru citire binara, si intr-un ciclu while se citeste campul "name" din header (primii 100 de octeti). Ciclul se inchide atunci cand se ajunge la blocul de 512 de octeti de 0. se afiseaza numele fisierului anterior citit, se face un "salt" pana la campul size cu ajutorul functiei "fseek", se citeste valoarea lui size si se transforma in decimal (ea era scrisa in header in octal) si se face "saltul" la urmatorul fisier din arhiva prin functiile "fseek" si "get_jump". Functia "get_jump" intoarce valoare dependenta de "size" care trebuie "sarita" pentru a ajunge la urmatorul fisier din cadrul arhivei. La finalul ciclului while se inchide arhiva din care se citeste.

Task 3 - extract:
    
    Implementarea acestei functii se face asemanator cu functia "list". Functia scrisa in C "extract" primeste ca argumente numele fisierului care trebuie arhivat si numele arhivei. Se deschide arhiva pentru citire binara. Se citeste campul "name" din header (primii 100 de octeti) intr-un ciclu while care se inchide atunci cand se ajunge la blocul de 512 octeti de 0. Se face un "salt" in arhiva la campul "size", se citeste valoarea lui "size" si se transforma in decimal. Se face alt "salt" pana la inceputul propriu-zis al fisierului din arhiva. Daca numele fisierului este egal cu cel specificat in linia de comanda, se va extrage noul fisier, altfel se va cauta in continuare un fisier cu numele cautat. Dupa selectarea fisierului cautat si pozitionarea la inceputul propriu-zis al datelor fisierului, se creeaza fisierul extras si se copiaza octet cu octet datele din arhiva in fisierul extras. La finalul se inchid cele doua fisiere.
