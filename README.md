# HOMEWORK 2004 SO2

Questo programma C permette di gestire due funzionalità:
1) prendere in input un file di testo UTF-8 scritto in italiano e di modellare una tabella che per ogni parola del testo, ne fornisca le parole successive con le rispettive frequenze di apparizione.
   Questa tabella viene salvata all'interno di un file denominato output.csv.
2) prendere in input un file CSV (presumibilmente il file prodotto dall'output della prima funzionalità), un numero intero indicante un numero di parole, e opzionalmente una parola, e ne fornisca in ouput una frase casuale usando la struttura del file csv, formata dal numero di parole fornite in input, ed eventualmente a partire dalla parola successiva a quella fornita anch'essa come input.

Il software fa uso della libreria ***[GNU GSL](https://www.gnu.org/software/gsl/)*** per la generazione di numeri pseudo-casuali.

## Installazione
```bash
make
```

## Utilizzo Single Process
```bash
# restituisce il file output.csv contenente la tabella delle parole analizzate in input.txt
$ ./hw2024sp -1 input.txt

# restituisce il file output.txt contenente la frase composta da 11 parole partendo dalla parola successiva a !
$ ./hw2024sp -2 output.csv 11 !
```
## Utilizzo Multi Process
```bash
# restituisce il file output.csv contenente la tabella delle parole analizzate in input.txt
$ ./hw2024mp -1 input.txt

# restituisce il file output.txt contenente la frase composta da 11 parole partendo dalla parola successiva a !
$ ./hw2024mp -2 output.csv 11 !
```