# ADS1 - DOCUMENTAZIONE TECNICA

ADS1 è uno dei modelli dell’Algoritmo di Stirling, il cui compito è determinare il numero totale di partizioni di un insieme.

Un insieme è una collezione definita di oggetti, detti elementi, riferibili a entità astratte o reali. In un insieme non sono ammessi duplicati.
Un insieme può contenere sottoinsiemi, cioè sotto-collezioni di elementi appartenenti all’insieme originale.

Una partizione è una raccolta di sottoinsiemi, detti blocchi, che coprono l'insieme originale senza sovrapposizioni e senza elementi ripetuti tra i blocchi.
Il numero totale di partizioni di un insieme di n elementi è dato dal numero di Bell B_n
Il numero di modi per partizionare un insieme di n elementi in esattamente k blocchi è dato dal numero di Stirling di seconda specie.

ADS1 utilizza metodi computazionali basati su formule ricorsive per calcolare i numeri di Stirling e, di conseguenza, i numeri di Bell in modo analitico e deterministico.
Nel corso dell’anno scolastico 2025/2026 ho implementato ADS1 nei linguaggi C e Python.
Il programma, oltre a includere il nucleo computazionale, integra anche un’interfaccia CLI che consente l’interazione con l’utente e semplifica l’assegnazione degli insiemi da analizzare.
## DESCRIZIONE ALGORITMICA

Il programma è composto dalle seguenti procedure:
- INIT: inizializza la sintassi del'input, ovvero gli operatori con cui l'utente comunica l'insieme da elaborare
- ASGN: assegnazione/acquizione e elaborazione/interpretazione della stringa, inserita dall’utente, che rappresenta l'insieme 
- CRD: calcolo della cardinalità dell’insieme
- TDS: il calcolo dei numeri di Stirling di seconda specie, utilizzati per determinare il numero di Bell
- BLL: restituzione del numero di Bell
- ESC: procedura tecnica di deallocazione della memoria (nel caso del C) e gestione della terminazione del programma o dell’avvio di una nuova computazione
Dopo INIT, il programma continua l’esecuzione a meno che l’utente non interrompa il flusso durante ASGN o ESC.

Ogni fase usa funzioni di supporto per compiti specifici. In particolare:
- `leggi_char`: legge un singolo carattere dall'utente in modo sicuro.
- `gia_usato`: verifica se un carattere è stato già inserito durante la personalizzazione degli operatori, da poter evitare casi come OPS = [=, =, =, =]
- `controllare`: controlla la presenza dei tre operatori principali(corresponsore, terminatore aperto, terminatore chiuso) e il loro ordine
- `estrarre`: funzione di parsing ovvero di tokenizzazione manuale, con eliminazione degli spazi all'inizio e alla fine della sottostringa estratta

## INTERFACCIA CLI

La CLI guida l'utente durante l'esecuzione del programma.
Le funzionalita principali sono:
- configurare gli operatori;
- leggere l'insieme da elaborare;
- validare l'input;
- mostrare i risultati;
- decidere se eseguire un'altra computazione.

La sintassi adottata da ADS1 per la rappresentazione degli insiemi è composta da quattro operatori:
- corresponsore: separa il denominatore(nome dell'insieme) dal contenuto dell'insieme
- terminatore aperto: identifica l'inizio dell'elenco degli elementi
- terminatore chiuso: identifica la fine dell'elenco degli elementi
- separatore: distingue un elemento dal successivo

Nella configurazione predefinita tali operatori corrispondono rispettivamente ai simboli:
=  {  }  ,
e consentono di rappresentare un insieme nella forma:
    A = {a1, a2, ... aN}
dove A rappresenta il denominatore dell'insieme e a1, a2 .. aN rappresentano i suoi elementi.
ADS1 consente all'utente di personalizzare ciascun operatore durante la fase di inizializzazione, mantenendo invariata la struttura logica dell'input
    Esempio: A > [a1; a2; ... aN]

Durante l'esecuzione, ADS1 utilizza un sistema di conferma basato sulle risposte s/n (sì/no) per le operazioni che richiedono una scelta da parte dell'utente:
- accettazione degli operatori predefiniti
- avvio di una nuova computazione
- terminazione del programma
In caso di risposta non valida, il sistema richiede nuovamente l'inserimento senza interrompere l'esecuzione.
Gli input non conformi vengono rifiutati e non vengono inoltrati alle procedure di elaborazione.
Per consentire l'interruzione immediata dell'applicazione, ADS1 riconosce inoltre il comando 'exit' digitabile durante la fase di acquisizione dell'insieme.

Dopo la fase di acquisizione, il programma mostra l'insieme normalizzato senza duplicati e con la sintassi corretta.
Vengono inoltre visualizzate:
- cardinalità effettiva dell'insieme
- degenerazione dell'insieme (numero di duplicati rimossi)
- numero totale delle partizioni possibili

Prima dell'esecuzione del nucleo computazionale, l'utente può scegliere se stampare il triangolo di Stirling generato durante il calcolo

## NUCLEO COMPUTAZIONALE

L'elemento centrale di ADS1 responsabile della costruzione del triangolo di Stirling e della determinazione del numero di Bell.
Dal punto di vista matematico, l'algoritmo implementa la relazione ricorsiva dei numeri di Stirling di seconda specie:
$$S(n, k) = k * S(n-1, k) + S(n-1, k-1)$$

Nel codice vengono usati tecniche di shifting e autoaggiornamento, anziché la ricorsione vera.
Si usa un solo array dinamico $T$ di dimensione $N+1$.
L'array viene aggiornato in ordine decrescente di $k$ per mantenere i valori precedenti.

L'array inizia con $T[0] = 1$
Per ogni $n$ da 1 a $N$, il programma calcola:
$$T[k] = (k + 1) * T[k] + T[k - 1]$$
per $k$ che va da $n$ a 1.

Alla fine, il numero di Bell è la somma di tutti i valori dell'array T nell'ultima configurazione.

## PRESTAZIONI DEL CODICE

### Prestazioni numeriche (complessita)

Il calcolo principale usa operazioni semplici:
- moltiplicazioni;
- addizioni;
- assegnazioni.

Il tipo usato per i risultati e `long long`.
TDS fa crescere il tempo di esecuzione piu o meno come $N^2$.

### Prestazioni operative

Il programma segue questo flusso:
INIT -> ASGN -> CRD -> TDS -> BLL -> ESC

Ogni procedura ha un lavoro preciso e passa il risultato alla fase successiva.
L'esecuzione è sequenziale e non usa parallelismo.

### Prestazioni tecniche

#### Tempo di esecuzione

Le procedure piu esigenti sono:
- CRD, che confronta gli elementi per eliminare i duplicati;
- TDS, che aggiorna l'array per calcolare i numeri di Stirling.

Nel caso peggiore, entrambe le procedure crescono in modo quadratico.

##### Uso della memoria

La memoria cresce in modo lineare con il numero di elementi.
Il programma usa un solo array dinamico $T$ di lunghezza $N+1$, quindi non costruisce una matrice completa.

##### Uso della CPU

Il codice è single-threaded.
Non ci sono costi di sincronizzazione.
La CPU lavora di piu quando l'insieme contiene piu elementi.

## COME AVVIARE IL PROGRAMMA

### 1) Con GCC (su Linux)

- Apri una shell o un terminale
- Entra nella cartella che contiene `main.c`.
- Usa il compilatore GCC per generare l'eseguibile:
    ``gcc main.c -o ads1``
- Se la compilazione ha successo, avvia il programma con:
    ``./ads1``

### 2) Con CL (MSVC su Windows)

- Apri il "Developer Command Prompt for Visual Studio".
- Entra nella cartella che contiene `main.c`.
- Compila con il comando:
    ``cl main.c /Fe:ads1.exe``
- Se la compilazione ha successo, esegui il programma con:
    ``ads1.exe``
