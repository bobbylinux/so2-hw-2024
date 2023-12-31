# HOMEWORK 2004 SO2

Questo programma C permette di gestire due funzionalità:
1) prendere in input un file di testo UTF-8 scritto in italiano e di modellare una tabella che per ogni parola del testo, ne fornisca le parole successive con le rispettive frequenze di apparizione.
   Questa tabella viene salvata all'interno di un file denominato output.csv.
2) prendere in input un file CSV (presumibilmente il file prodotto dall'output della prima funzionalità), un numero intero indicante un numero di parole, e opzionalmente una parola, e ne fornisca in ouput una frase casuale usando la struttura del file csv, formata dal numero di parole fornite in input, ed eventualmente a partire dalla parola successiva a quella fornita anch'essa come input.


## Descrizione del progetto

Il progetto si divide in due parti: una parte single process e una parte multi process. Alcune funzioni sono state usate da entrambe le parti del progetto. Tutti i file utilizzati da entrambe le parti del progetto sono contenute nella directory "common". Questo include anche l'header ***common.h*** usato per definire le funzioni di entrambe le parti del progetto.

La parte del progetto single process fa uso dei seguenti file:

+ **single-process-version/hw2024sp.c** - contiene il punto di ingresso della versione single process.
+ **common/read_text_file.c** - contiene la funzione ***read_text_file*** che si occupa di leggere il file di input specificato come argomento in fase di esecuzione e di restituire un array di caratteri (array di parole) che forniscono al programma una versione pulita (lowercase) di tutte le parole usabili dal software.
+ **common/list_operations.c** - contiene le funzioni utilizzate per la costruzione e la manipolazione di una lista concatenata, utilizzata per passare dall'array di parole ad una struttura più simile a quella del file di output csv finale. Questa lista concatenata contiene due strutture: 
  + word_element
  + next_word_element
  
   questo permette di contare le occorrenze e strutturare le parole successive per ogni parola del testo in input.
+ **common/round_down_to_decimal.c** - contiene la funzione ***round_down_to_decimal*** che si occupa di approssimare i numeri decimali alla seconda cifra dopo la virgola.
+ **common/to_lower.c** - contiene la funzione ***to_lower*** che si occupa di fare il lowercase di ogni parola passata in input e di restituirlo in output.
+ **common/write_output.c** - contiene la funzione ***write_output*** che si occupa prendendo una lista concatenata in input di trasformarla in un file (output.csv) secondo specifica del progetto.
+ **common/read_csv_and_build_list.c** - contiene la funzione ***read_csv_and_build_list*** che si occupa di leggere il file di input passato in argomento (formato csv) e di convertirlo in una lista concatenata.
+ **common/write_text_file.c** - contiene la funzione ***write_text_file*** che si occupa di prendere in input una lista concatenata e scrivere nel file output.txt una frase random secondo le specifiche del progetto.


La parte del progetto multi process fa uso dei seguenti file:
+ **multi-process-version/hw2024mp.c** - contiene il punto di ingresso della versione single process.
+ **multi-process-version/serialize_list.c** - contiene la funzione ***serialize_list*** che è utilizzata per trasformare la lista concatenata passata in input in una stringa dove ogni elemento della lista è separato da carattere | per permetterne l'invio nella pipe da passare al processo figlio che eseguirà il compito successivo.
+ **multi-process-version/deserialize_list.c** - contiene la funzione ***deserialize_list*** che è utilizzata per trasformare la lista serializzata con la funzione serialize_list in una lista concatenata una volta ricevuta nella pipe del processo figlio.
+ **common/read_text_file.c** - contiene la funzione ***read_text_file*** che si occupa di leggere il file di input specificato come argomento in fase di esecuzione e di restituire un array di caratteri (array di parole) che forniscono al programma una versione pulita (lowercase) di tutte le parole usabili dal software.
+ **common/list_operations.c** - contiene le funzioni utilizzate per la costruzione e la manipolazione di una lista concatenata, utilizzata per passare dall'array di parole ad una struttura più simile a quella del file di output csv finale. Questa lista concatenata contiene due strutture:
   + word_element
   + next_word_element

  questo permette di contare le occorrenze e strutturare le parole successive per ogni parola del testo in input.
+ **common/round_down_to_decimal.c** - contiene la funzione ***round_down_to_decimal*** che si occupa di approssimare i numeri decimali alla seconda cifra dopo la virgola.
+ **common/to_lower.c** - contiene la funzione ***to_lower*** che si occupa di fare il lowercase di ogni parola passata in input e di restituirlo in output.
+ **common/write_output.c** - contiene la funzione ***write_output*** che si occupa prendendo una lista concatenata in input di trasformarla in un file (output.csv) secondo specifica del progetto.
+ **common/read_csv_and_build_list.c** - contiene la funzione ***read_csv_and_build_list*** che si occupa di leggere il file di input passato in argomento (formato csv) e di convertirlo in una lista concatenata.
+ **common/write_text_file.c** - contiene la funzione ***write_text_file*** che si occupa di prendere in input una lista concatenata e scrivere nel file output.txt una frase random secondo le specifiche del progetto.

La parte multi processo di questo progetto è così strutturata:

Per l'opzione -1 che consente di trasformare un testo preso dal file txt passato come argomento in una tabella csv restituito in uscita nel file output.csv, si usano tre processi:
+ il primo processo legge il file in input txt e passa in una pipe l'array di parole al processo figlio.
+ il secondo processo riceve dalla pipe l'array di parole e lo inserirsce in una lista concatenata, che poi serializza per creare un altro processo e passare questa lista dentro un'altra pipe ad un altro processo figlio.
+ il terzo processo riceve dalla pipe la lista serializzata, la deserializza, e scrive nel file output.csv il risultato dell'operazione secondo specifica del progetto.

Per l'opzione -2 che consente di leggere il file csv passato come argomento, e generare un testo casuale usando le parole della tabella csv scritto nel file output.txt, si usano due processi:
+ il primo processo legge il file csv e lo mappa in una lista concatenata, che viene serializzata e passata tramite pipe al processo figlio.
+ il secondo processo legge la stringa e la deserializza in una lista concatenata, per poi generare una frase casuale usando le parole all'interno della lista secondo specifica del progetto.

## Installazione
```bash
make
```

## Utilizzo Single Process
```bash
# restituisce il file output.csv contenente la tabella delle parole analizzate in input.txt
$ ./hw2024sp -1 {inputfile}

# restituisce il file output.txt contenente la frase composta da 11 parole partendo dalla parola successiva a !
$ ./hw2024sp -2 {inputfile} {number_of_words} {first_word}
```
## Utilizzo Multi Process
```bash
# restituisce il file output.csv contenente la tabella delle parole analizzate in input.txt
$ ./hw2024mp -1 {inputfile}

# restituisce il file output.txt contenente la frase composta da 11 parole partendo dalla parola successiva a !
$ ./hw2024mp -2 {inputfile} {number_of_words} {first_word}
```

## Note
All'interno del progetto sono presenti due file 
+ input.txt
+ input.csv
per eseguire dei test basici, seguendo l'esempio indicato nella presentazione dell'homework si possono eseguire i seguenti comandi:

```bash
$ ./hw2024sp -1 input.txt
```
Esegue la versione single process e produce un file di output denominato ***output.csv*** contenente la tabella csv richiesta.

```bash
$ ./hw2024mp -1 input.txt
```
Esegue la versione multi process e produce un file di output denominato ***output.csv*** contenente la tabella csv richiesta.

```bash
$ ./hw2024sp -2 input.csv 11 !
```
Esegue la versione single process e produce un file di output denominato ***output.txt*** contenente la frase random.

```bash
$ ./hw2024mp -2 input.csv 11 !
```
Esegue la versione multi process e produce un file di output denominato ***output.txt*** contenente la frase random.

In entrambi i casi, se si omette la parola il testo verrà generato a partire dalla prima parola all'interno del file csv passato in input.

## Esempio:
```bash
$ ./hw2024sp -2 input.csv 11
```

```bash
$ ./hw2024mp -2 input.csv 11
```