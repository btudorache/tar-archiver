# Programarea Calculatoarelor - Tema 3
Implementati un program în limbajul C care simulează un utilitar ce creează o arhivă de tipul [tar](https://www.fileformat.info/format/tar/corion.htm). Utilitarul
este reprezentat de un fisier executabil cu numele **archiver**, care atunci când rulează poate interpreta anumite
comenzi de la **standard input (stdin)**. Comenzile pe care programul poate să le interpreteze sunt:

* `create nume_arhiva nume_director` - creează o arhivă tar cu numele nume_arhivă ce contine fisiere din directorul nume_director.
* `list nume_arhiva` - afisează numele fisierelor continute în arhiva nume_arhivă.
* `extract nume_fisier nume_arhiva` - extrage continutul fisierului nume_fisier salvat în arhiva nume_arhivă.
* `exit` - încheie executia programului.

Cerintele si detaliile complete se gasesc in [enuntul problemei](https://github.com/btudorache/tar-archiver/blob/master/Programare_2019___Tema_3.pdf)

# Setup
Compilati folosind comanda `make` si rulati prin `make run`

# Notes
Programul foloseste pentru arhivare doua fisiere auxiliare:

* `usermap.txt` contine rezultatul comenzii `cat /etc/passwd`
* `files.txt` contine un subset de linii din rezultatul
aplicării comenzii `ls -la --time-style=full-iso | grep ^-` în directorul care contine fisierele
