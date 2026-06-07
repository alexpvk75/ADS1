// ADS1.c - modello analitico deterministico con formula di Stirling

#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
// ho usato array dinamici perché MSVC non supporta i VLA (es. int A[N])

// Operatori che servono ad interpretare la stringa di input, con possibilità di personalizzazione da parte dell'utente
char OPS[4] = {'=', '{', '}', ','};
// Stringhe utilizzate nei messaggi all'utente
char *OPSnomi[4] = {"corresponsore", "terminatore aperto", "terminatore chiuso", "separatore"};
char *OPSes[4] = {"'=' '>'", 
    "'{' '(' '['",
    "'}' ')' ']'",
    "',' ';'"
};

char leggi_char() {
    char c = getchar();
    while (c == '\n') c = getchar();
    while (getchar() != '\n');
    return c;
}
// Verifica se un carattere è stato già inserito durante la personalizzazione degli operatori, da poter evitare casi come OPS = [=, =, =, =]
int gia_usato(char OPS[], int candidato) {
    for (int i = 0; i < candidato; i++)  if (OPS[i] == OPS[candidato]) return 1;
    return 0;
}
// Controllo della presenza dei tre operatori principali(corresponsore, terminatore aperto, terminatore chiuso) e del loro ordine
int controllare(char *stringa) {
    char *cr = strchr(stringa, OPS[0]);
    char *ta = strchr(stringa, OPS[1]);
    char *tc = strchr(stringa, OPS[2]);
    if ((!cr || !ta || !tc) || (!(cr < ta && ta < tc))) return 0;
    return 1;
}
// Tokenizzatore manuale, con eliminazione degli spazi all'inizio e alla fine della sottostringa estratta
char *estrarre(char *stringa, int *puntatore, char target) {
    if (!stringa || !puntatore) return NULL;
    int i = *puntatore;
    while (stringa[i] == ' ') i++;
    int start = i;
    while (stringa[i] != '\0' && stringa[i] != target) i++;
    int fine = i;
    while (fine > start && stringa[fine - 1] == ' ') fine--;
    int lun = fine - start;
    char *sottostringa = malloc(lun + 1);
    if (!sottostringa) return NULL;
    for (int j = 0; j < lun; j++) sottostringa[j] = stringa[start + j];
    sottostringa[lun] = '\0';
    if (stringa[i] == target) i++;
    *puntatore = i;
    return sottostringa;
}

int main() {
    // INIT - stabilizzazione degli operatori
    printf("\nBenvenuto su ADS1!\n");
    printf("Per inizializzare un insieme segui lo standard => A %c %ca1%c a2%c ... aN%c \n", 
            OPS[0], OPS[1], OPS[3], OPS[3], OPS[2]);
    printf("Gli elementi non devono coincidere con gli operatori.\n");
    printf("I duplicati saranno ignorati.\n");
    int el_len = 1;
    while (1){
        printf("Digita 1 per utilizzare come gli elementi i singoli simboli alfanumerici\n");
        printf("Digita 0 per utilizzare come gli elementi le stringhe alfanumeriche di lunghezza arbitraria.\n");
        printf("Scelta: ");
        char risposta = leggi_char();
        if (risposta != '0' && risposta != '1'){
            printf("Risposta non valida\n");
        } else {
            el_len = (risposta == '1') ? 1 : 0;
            break;
        }
    }
    while (1) {
        printf("\nOperatori default: "
            "\n\t%c (%s) \n\t%c (%s) \n\t%c (%s) \n\t%c (%s)\n", 
            OPS[0], OPSnomi[0], OPS[1], OPSnomi[1], OPS[2], OPSnomi[2], OPS[3], OPSnomi[3]);
        printf("Vuoi procedere con gli operatori di default? (s/n): ");
        char risposta = leggi_char();
        if (risposta == 's' || risposta == 'S') {
            printf("Operatori di default accettati.\n");
            break;
        } else if (risposta == 'n' || risposta == 'N') {
            printf("\n");
            for (int o = 0; o<4; o++) {
                OPS[o] = '\0';
                while(1){
                    printf("Stabilire %s (es. %s): ", OPSnomi[o], OPSes[o]);
                    OPS[o] = leggi_char();
                    if (gia_usato(OPS, o)) {
                        OPS[o] = '\0';
                        printf("Carattere gia inserito previamente\n");
                    } else { break;}
                }
            }
            printf("\nOperatori personalizzati accettati."
                "\n\t%c (%s) \n\t%c (%s) \n\t%c (%s) \n\t%c (%s)\n", 
            OPS[0], OPSnomi[0], OPS[1], OPSnomi[1], OPS[2], OPSnomi[2], OPS[3], OPSnomi[3]);
            printf("Nuovo standard: A %c %ca1%c a2%c ... aN%c \n", OPS[0], OPS[1], OPS[3], OPS[3], OPS[2]);
            break;
        } else  printf("Risposta non valida\n");
    }
    while(1){
        // ASGN - elaborazione della stringa di input, estrazione degli elementi dell'insieme
        char *temp = NULL; char *stringa = NULL; int capacita = 0; int lunghezza = 0;
        int ch = 0;
        printf("\nPer uscire, digita 'exit'\nAvviso importante: massima cardinalita consigliata = 25\n");
        printf("\nPrompt: ");
        int rinvio = 0;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            if (lunghezza + 1 >= capacita) { 
                capacita = (capacita == 0) ? 2 : capacita * 2;
                temp = realloc(stringa, capacita * sizeof(char));
                if (temp == NULL) {
                    free(stringa);
                    printf("Errore di allocazione della memoria.\n");
                    rinvio = 1;
                    break;
                }
                stringa = temp;
            }
            stringa[lunghezza++] = ch;
        }
        if (rinvio) continue;
        if (stringa) stringa[lunghezza] = '\0';
        if (stringa && !strcmp(stringa, "exit")) {
            free(stringa);
            printf("Arrivederci!\n"); 
            break;
        }
        if (stringa == NULL || controllare(stringa) == 0) {
            printf("Errore di inizializzazione\n");
            free(stringa);
            continue;
        }
        printf("\nElaborazione della richiesta....\n");
        int separati = 0; // serve per contare numero di separatori, da poter allocare uno spazio per gli elementi inseriti dall'utente
        for (int i = 0; i < lunghezza; i++) if (stringa[i] == OPS[3]) separati++;
        int puntatore = 0; //serve per scanning manuale della stringa
        char *denominatore = estrarre(stringa, &puntatore, OPS[0]); // nome dell'insieme
        if (denominatore == NULL) {
            free(stringa);
            printf("Errore di elaborazione del denominatore.\n"); 
            continue;
        }
        while (stringa[puntatore] != OPS[1] && stringa[puntatore] != '\0') puntatore++;
        if(stringa[puntatore] == OPS[1]) puntatore++;
        int puntatore_backup = puntatore;
        char *contenuto = estrarre(stringa, &puntatore, OPS[2]);
        if (contenuto == NULL) {
            free(denominatore);
            free(stringa); 
            printf("Errore di elaborazione della stringa"); 
            continue;
        }
        if(strlen(contenuto)==0){
            printf("Insieme vuoto\nCardinalita: 0 \nDegenerazione: 0\nIl numero di tutte le partizioni possibili: 1\n");
            free(stringa); free(denominatore); free(contenuto);
            int nuova = 0;
            while (1) {
                printf("\nVuoi eseguire un'altra computazione?(s/n): ");
                char risposta = leggi_char();
                if (risposta == 's' || risposta == 'S') {nuova = 1; break;}
                else if (risposta == 'n' || risposta == 'N') { printf("Arrivederci!\n"); return 0;}
                else printf("Risposta non valida\n");
            }
            if (nuova) continue;
        } else separati += 1;
        free(contenuto);
        puntatore = puntatore_backup;
        // CRD - calcolo della cardinalità e degenerazione
        char **insieme = (char **)calloc((separati+1), sizeof(char *));
        if (insieme == NULL) {
            free(denominatore);
            free(stringa);
            printf("Errore di allocazione di memoria");
            continue;
        }
        int N = 0;
        int degenerazione = 0;
        rinvio = 0;
        for (int e = 0; e < separati; e++) {
            char *elemento = estrarre(stringa, &puntatore, (e == separati - 1) ? OPS[2] : OPS[3]);
            if (elemento == NULL) {
                free(denominatore);
                for(int f = 0; f < separati+1; f++) free(insieme[f]);
                free(insieme);
                free(stringa);
                printf("Errore di formattazione degli elementi");
                rinvio = 1;
                break;
            }
            if (el_len ? (strlen(elemento) != 1) : (strlen(elemento) == 0)) {
                free(elemento);
                continue;
            }
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
        if (rinvio) continue;
        free(stringa);
        // output dell'insieme normalizzato (senza duplicati e seguendo la sintassi standard)
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
        // TDS - NUCLEO COMPUTAZIONALE
        // calcolo del triangolo di Stirling(uso un solo array e applico tecniche di shifting e autoaggiornamento mostrate in classe)
        printf("\nComputazione in corso...\n");
        long long *T = (long long *)calloc(N + 1, sizeof(long long));
        if (T == NULL) {
            printf("Errore di inizializzazione"); 
            free(denominatore);
            for(int f = 0; f < N; f++) free(insieme[f]);
            free(insieme); 
            continue;
        }
        T[0] = 1;
        for (int n = 1; n <= N; n++) {
            if (stampare_TDS) { 
                for (int k = 0; k < n; k++) printf("%lld ", T[k]);
                printf("\n");
            }
            if (n!=N) {
                for (int k = n; k >= 1; k--) T[k] = (k+1) * T[k] + T[k - 1];
            }
        }
        // BLL - calcolo del numero di Bell, che è obbiettivo finale
        long long B = 0;
        for (int k = 0; k < N; k++) B += T[k];
        printf("\nIl numero di tutte le partizioni possibili: %lld\n", B);
        // ESC - pulizia della memoria. scelta tra nuova computazione e uscita
        for (int f = 0; f < N; f++) {free(insieme[f]);}
        free(insieme); free(denominatore); free(T);
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