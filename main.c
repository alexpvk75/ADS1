#include <stdio.h> // Per input/output
#include <string.h> // Per funzioni di manipolazione delle stringhe come strcmp e strchr
#include <stdlib.h> // Per funzioni di allocazione e deallocazione della memoria

//operatori default
char OPS[4] = {'=', '{', '}', ','};
// Stringhe descrittive per gli operatori, utilizzate nei messaggi all'utente
char *OPSnomi[4] = {"corresponsore", "terminatore aperto", "terminatore chiuso", "separatore"};
char *OPSes[4] = {"'=', '>'", 
    "'{', '(', '['",
    "'}', ')', ']'",
    "',' ';'"
};

// Funzione per leggere un carattere, ignorando eventuali spazi o newline
char leggi_char() {
    char c = getchar();
    while (c == '\n') c = getchar();
    while (getchar() != '\n');
    return c;
}
// Funzione per verificare se un carattere è stato già inserito durante la personalizzazione degli operatori
int gia_usato(char OPS[], int candidato) {
    for (int i = 0; i < candidato; i++)  if (OPS[i] == OPS[candidato]) return 1;
    return 0;
}
// Funzione per controllare se la stringa di input rispetta il formato richiesto (denominatore, terminatore aperto, terminatore chiuso)
// Quindi verifica sia la presenza dei tre operatori principali che il loro ordine corretto nella stringa
int controllare(char *stringa) {
    char *cr = strchr(stringa, OPS[0]);
    char *ta = strchr(stringa, OPS[1]);
    char *tc = strchr(stringa, OPS[2]);
    if ((!cr || !ta || !tc) || (!(cr < ta && ta < tc))) return 0;
    return 1;
}
// Funzione per estrarre una sottostringa delimitata da un carattere terminatore specifico, aggiornando il puntatore di posizione nella stringa originale
char *estrarre(char *stringa, int *puntatore, char termine) {
    if (!stringa || !puntatore) return NULL;
    int i = *puntatore;
    while (stringa[i] == ' ') i++;
    int start = i;
    while (stringa[i] != '\0' && stringa[i] != termine) i++;
    int fine = i;
    while (fine > start && stringa[fine - 1] == ' ') fine--;
    int lun = fine - start;
    char *pezzo = malloc(lun + 1);
    if (!pezzo) return NULL;
    for (int j = 0; j < lun; j++) pezzo[j] = stringa[start + j];
    pezzo[lun] = '\0';
    if (stringa[i] == termine) i++;
    *puntatore = i;
    return pezzo;
}
// NUCLEO COMPUTAZIONALE
// Funzione che sfrutta i numeri di Stirling di seconda specie nel modo non convenzionale per generare il 'triangolo di Stirling'
// e calcolare il numero di Bell, ovvero il numero di tutti partizioni possibili di un insieme di N elementi
// con la minima complessità computazionale e di memoria possibile, con output del triangolo se richiesto dall'utente
long long bell_stirling(long long N, long long **T, int pr) {
    for (int n = 0; n < N; n++) {
        T[n][0] = 1;
        for (int k = n; k >= 1; k--) {
            T[n][k] = (k + 1) * T[n][k] + T[n][k - 1];
        }
        for (int k = 0; k <= n; k++) {
            if (pr) printf("%lld ", T[n][k]);
            if (n + 1 < N) T[n + 1][k] = T[n][k];
        }
        if (pr) printf("\n");
    }
    long long B = 0;
    for (int k = 0; k < N; k++) B += T[N-1][k];
    return B;
}

int main() {
    // INIT - stabilizzazione degli operatori
    printf("Benvenuto su ADS1!\n");
    while (1) {
        printf("Operatori default: "
            "\n\t%c (%s) \n\t%c (%s) \n\t%c (%s) \n\t%c (%s)\n", 
            OPS[0], OPSnomi[0], OPS[1], OPSnomi[1], OPS[2], OPSnomi[2], OPS[3], OPSnomi[3]);
        printf("Vuoi procedere con operatori default? (s/n): ");
        char risposta = leggi_char();
        if (risposta == 's' || risposta == 'S') {
            printf("Operatori default accettati.\n");
            break;
        } else if (risposta == 'n' || risposta == 'N') {
            for (int o = 0; o<4; o++) {
                OPS[o] = '\0';
                printf("Stabilire %s (es. %s): ", OPSnomi[o], OPSes[o]);
                OPS[o] = leggi_char();
                while (gia_usato(OPS, o)) {
                    printf("Carattere gia usato, inserire un altro carattere: ");
                    OPS[o] = leggi_char();
                }
            }
            printf("Operatori personalizzati accettati."
                "\n\t%c (%s) \n\t%c (%s) \n\t%c (%s) \n\t%c (%s)\n", 
            OPS[0], OPSnomi[0], OPS[1], OPSnomi[1], OPS[2], OPSnomi[2], OPS[3], OPSnomi[3]);
            break;
        } else  printf("Risposta non valida\n");
    }
    while(1){
        // ASGN - prodecura principale di input dell'utente
        char *temp = NULL;
        char *stringa = NULL;
        int capacita = 0;
        int lunghezza = 0;
        int ch = 0;
        printf("\nPer uscire, digita 'exit'\n");
        printf("Per inizializzare insieme segui lo standard => A %c %ca%c b%c c%c \n", 
            OPS[0], OPS[1], OPS[3], OPS[3], OPS[2]);
        printf("Gli elementi possono essere stringhe di caratteri alfanumerici, ma non possono contenere gli operatori.\n");
        printf("Per separare gli elementi usa '%c'\n", OPS[3]);
        printf("\nPrompt: ");
        int rinvio = 0;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            if (lunghezza + 1 >= capacita) { 
                capacita = (capacita == 0) ? 2 : capacita * 2;
                temp = realloc(stringa, capacita * sizeof(char));
                if (temp == NULL) {
                    free(stringa);
                    printf("Errore: memoria insufficiente.\n");
                    rinvio = 1;
                    break;
                }
                stringa = temp;
            }
            stringa[lunghezza++] = ch;
        }
        if (rinvio) continue;
        if (stringa) stringa[lunghezza] = '\0';
        if (stringa && strcmp(stringa, "exit") == 0) {
            free(stringa);
            printf("Arrivederci!\n");
            break;
        }
        if (stringa == NULL || controllare(stringa) == 0) {
            printf("Errore di inizializzazione\n");
            free(stringa);
            continue;
        }
        // CRD - procedura di elaborazione della stringa di input, estrazione del denominatore e degli elementi dell'insieme, 
        // rimozione dei duplicati e calcolo della cardinalità e degenerazione
        printf("\nElaborazione della richiesta....\n");
        int separati = 0;
        for (int i = 0; i < lunghezza; i++) if (stringa[i] == OPS[3]) separati++;
        int puntatore = 0;
        char *denominatore = estrarre(stringa, &puntatore, OPS[0]);
        if (denominatore == NULL) { printf("Errore di formattazione del denominatore.\n"); continue;}
        while (stringa[puntatore] != OPS[1] && stringa[puntatore] != '\0') puntatore++;
        if(stringa[puntatore] == OPS[1]) puntatore++;
        int puntatore_backup = puntatore;
        char *contenuto = estrarre(stringa, &puntatore, OPS[2]);
        if (contenuto == NULL) { printf("Errore di elaborazione della stringa"); continue;}
        if(strlen(contenuto)==0){
            printf("Insieme vuoto\nCardinalita: 0 \nDegenerazione: 0\nIl numero di tutti partizioni possibili: 1\n");
            free(stringa); free(denominatore); free(contenuto);
            while (1) {
                printf("\nVuoi eseguire un'altra computazione?(s/n): ");
                char risposta = leggi_char();
                if (risposta == 's' || risposta == 'S') break;
                else if (risposta == 'n' || risposta == 'N') { printf("Arrivederci!\n"); return 0;} 
                else printf("Risposta non valida\n");
            }
        } else separati += 1;
        free(contenuto);
        puntatore = puntatore_backup;
        char **insieme = (char **)calloc((separati+1), sizeof(char *));
        int N = 0;
        int degenerazione = 0;
        for (int e = 0; e < separati; e++) {
            char *elemento = estrarre(stringa, &puntatore, (e == separati - 1) ? OPS[2] : OPS[3]);
            if (elemento == NULL) { printf("Errore di formattazione degli elementi"); continue;}
            if (strlen(elemento) == 0) { free(elemento); continue;}
            int duplicato = 0;
            for (int i = 0; i < N; i++) {
                if (strcmp(insieme[i], elemento) == 0) {
                    duplicato = 1;
                    break;
                }
            }
            if (!duplicato) insieme[N++] = elemento;
            else { free(elemento); degenerazione++; }
        }
        free(stringa);
        printf("%s %c %c", denominatore, OPS[0], OPS[1]);
        for (int i = 0; i < N; i++) {
            printf("%s", insieme[i]);
            if (i < N - 1) printf("%c ", OPS[3]);
        }
        printf("%c\n", OPS[2]);
        printf("Cardinalita: %d \nDegenerazione(duplicati): %d\n", N, degenerazione);
        int stampare_TDS = 0;
        while(1) {
            printf("Vuoi stampare il triangolo di Stirling? (s/n): ");
            char risposta = leggi_char();
            if (risposta == 's' || risposta == 'S') {
                stampare_TDS = 1;
                break;
            } else if (risposta == 'n' || risposta == 'N') break;
            else printf("Risposta non valida\n");
        }
        // TDS + BLL - computazione del numero di Bell tramite triangolo di Stirling
        printf("\nComputazione in corso...\n");
        long long **T = (long long **)malloc(N * sizeof(long long *));
        if (T == NULL) return 1;
        for (int i = 0; i < N; i++) T[i] = (long long *)calloc(N, sizeof(long long));
        long long B = bell_stirling(N, T, stampare_TDS);
        printf("\nIl numero di tutti partizioni possibili: %lld\n", B);
        // FIN - procedura di pulizia della memoria e richiesta di nuova computazione
        for (int f = 0; f < N; f++) {free(insieme[f]); free(T[f]);}
        free(insieme); free(T); free(denominatore);
        while (1) {
            printf("\nVuoi eseguire un'altra computazione?(s/n): ");
            char risposta = leggi_char();
            if (risposta == 's' || risposta == 'S') break;
            else if (risposta == 'n' || risposta == 'N') { printf("Arrivederci!\n"); return 0;} 
            else printf("Risposta non valida\n");
        }
    }
    return 0;
}