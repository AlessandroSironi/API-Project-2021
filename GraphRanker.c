/* 
    PROGETTO API 2021 - GraphRanker

    Sironi Alessandro
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct
{
    int num_grafo; // Indice Grafo
    int somma;     // Somma dei cammini più brevi tra il nodo 0 e tutti gli altri nodi del grafo
} Grafi;

int d; // Numero di nodi dei grafi
int k; // Lunghezza della classifica
int heapsize;
int pienezza_heap;
int id_heap_analizzato;
char token[12];

// AggiungiGrafo
void AggiungiGrafo (int d, int buffer_size, Grafi classifica[]);
// TopK
void TopK (Grafi classifica[]);

// Dijkstra
unsigned int dijkstra (unsigned int matrice_pesi[d][d]);
int calcola_distanza_minima(unsigned int distanze[], int W[]);

// Librerie C
unsigned int my_atoi(char * cifra);
char * my_strtok(char *str, char delim);

// Max Heap
void costruisci_max_heap(Grafi vettore[]);
void max_heapify(Grafi vettore[], int n);
int left(int i);
int right(int i);
int parent(int i);
int max (Grafi vettore[]);
void swap (int * a, int * b);

int main()
{
    char comando;

    int trash_int;

    int prima_topk = 1; //Bool

    // GESTIONE INPUT
    trash_int = scanf("%d %d", &d, &k);
    if(trash_int)
    {
        trash_int = trash_int + 1;
    }
    heapsize = k;

    id_heap_analizzato = 0;
    pienezza_heap = 0;

    int buffer_size = 65535;
    char buffer[buffer_size+1];

    Grafi classifica[k];

    while (fgets (buffer, buffer_size, stdin))
    {
        comando = buffer[0];
        switch (comando)
        {
        case 'A':
            //CHIAMATA AGGIUNGI_GRAFO
            AggiungiGrafo(d, buffer_size, classifica);
            break;
        case 'T':
            //CHIAMATA TOPK
            if (prima_topk == 0)
            {
                printf("\n");
            }

            if (prima_topk == 1)
            {
                prima_topk = 0;
            }
            TopK(classifica);
            break;
        
        default:
            break;
        }
    }
    
    return 0;
}

void AggiungiGrafo (int d, int buffer_size, Grafi classifica[])
{
    unsigned int pesi_grafo[d][d];
    char riga[buffer_size+1];
    char * token;
    char delim = ',';
    unsigned int somma_cammini_grafo;

    int i;
    int j;
    int b;
    
    char * trash;

    for (i = 0; i < d; i++) //Per ogni riga della matrice dei pesi...
    {
        trash = fgets (riga, buffer_size, stdin);
        if(trash)
        {
            trash +=1;
        }
        
        j = 0;

        token = my_strtok(riga, delim);
    
        pesi_grafo[i][j] = my_atoi(token);
        j = 1;
        while (token != NULL && j < d)
        {
            token = my_strtok(NULL, delim);
            pesi_grafo[i][j] = my_atoi(token);
            j++;
        }
    }

    somma_cammini_grafo = dijkstra(pesi_grafo);

    if (pienezza_heap < k)
    {
        classifica[pienezza_heap].num_grafo = id_heap_analizzato;
        classifica[pienezza_heap].somma = somma_cammini_grafo;
        pienezza_heap++;
    } else {
        if (somma_cammini_grafo < max(classifica))
        {
            classifica[0].somma = classifica[k-1].somma;
            classifica[0].num_grafo = classifica[k-1].num_grafo;
            max_heapify(classifica, 0);

            classifica[k-1].somma = somma_cammini_grafo;
            classifica[k-1].num_grafo = id_heap_analizzato;

            b = k-1;

            while(b > 0 && classifica[parent(b)].somma < somma_cammini_grafo)
            {
                swap(&classifica[parent(b)].somma, &classifica[b].somma);
                swap(&classifica[parent(b)].num_grafo, &classifica[b].num_grafo);
                b = parent(b);
            }
            max_heapify(classifica, 2);
        }
    }
    id_heap_analizzato++;
}

unsigned int my_atoi (char * cifra)
{
    unsigned int cifra_int = 0;
    if (cifra != NULL)
    {
        for (int i = 0; cifra[i] != '\0' && cifra[i] != ',' && cifra[i] != '\n'; i++)
        {
            cifra_int *= 10; // Sistemo zeri
            cifra_int += (cifra[i] - '0'); // Valore tabella ASCII
        }
        return cifra_int;
    }
    return -1;
}

char * my_strtok(char *str, char delim)
{
    static char * input = NULL;
    int i;

    if (str != NULL)
    {
        input = str;
    }

    if (input == NULL) // Sono alla fine
    {
        return NULL;
    }
    
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] != delim)
        {
            token[i] = input[i];
        } else // Sono arrivato al delimitatore
        {
            token[i] = '\0';
            input = input + i + 1; // Algebra puntatori
            return token;
        }
    }
    
    input = NULL;
    token[i] = '\0';
   
    return token;    
}

void costruisci_max_heap(Grafi vettore[])
{
    for (int i = (heapsize/2)-1; i >= 0; i--)
    {
        max_heapify(vettore, i);
    }
}

void max_heapify(Grafi vettore[], int n)
{
    int sinistra;
    sinistra = left(n);
    int destra;
    destra = right(n);
    int posmax = 0;

    if (sinistra < heapsize && vettore[sinistra].somma > vettore[n].somma) 
    {
        posmax = sinistra;
    } else 
    {
        posmax = n;
    } 

    if (destra < heapsize && vettore[destra].somma > vettore[posmax].somma) 
    {
        posmax = destra;
    }

    if (posmax != n)
    {
        swap(&vettore[n].somma, &vettore[posmax].somma);
        swap(&vettore[n].num_grafo, & vettore[posmax].num_grafo);
        max_heapify(vettore, posmax);
    }
}

int left(int i)
{
    return (2*i + 1);
}
int right(int i)
{
    return (2*i + 2);
}
int parent(int i)
{
    return ((i-1)/2);
}

int max (Grafi vettore[])
{
    return vettore[0].somma;
}

void swap (int * a, int * b)
{
    int park = *a; 
    *a = *b; 
    *b = park; 
}

unsigned int dijkstra (unsigned int matrice_pesi[d][d]) //restituisce la somma degli elementi di distanze[]. Sorgente è sempre nodo 0.
{
    int sorgente = 0;
    unsigned int distanze[d];
    int W[d]; // Boolean.
    int vertice;
    unsigned int somma_pesi = 0;

    for (int i = 0; i < d; i++)
    {
        distanze[i] = INT_MAX;
        W[i] = 0;
    }

    distanze[sorgente]=0;

    for (int i = 0; i < d-1; i++)
    {
        vertice = calcola_distanza_minima(distanze, W);
        W[vertice] = 1;

        for (int j = 0; j < d; j++)
        {
            if (   distanze[vertice] != INT_MAX
                && matrice_pesi[vertice][j]
                && distanze[vertice] + matrice_pesi[vertice][j] < distanze[j]
                && !W[j])
            {
                distanze[j] = distanze[vertice] + matrice_pesi[vertice][j];
            }
        }
    }

    for (int i = 0; i < d; i++)
    {
        if (distanze[i] == INT_MAX) distanze[i] = 0; //BugFix per nodi scollegati.
        somma_pesi = somma_pesi + distanze[i];
    }
    return somma_pesi;
}

int calcola_distanza_minima(unsigned int distanze[], int W[])
{
    int min = INT_MAX;
    int posizione_minore = -1;

    for (int i = 0; i < d; i++)
    {
        if (distanze[i] <= min && W[i] == 0)
        {
            posizione_minore = i;
            min = distanze[posizione_minore];
        }
    }
    return posizione_minore;
}

void TopK(Grafi classifica[])
{
    int contatore;

    if (pienezza_heap < k)
    {
        contatore = pienezza_heap;
    } else
    {
        contatore = k;
    }

    if (contatore != 0) 
    {
        printf("%d", classifica[0].num_grafo);
    }

    for (int i = 1; i < contatore; i++)
    {
        printf(" %d", classifica[i].num_grafo);
    }
}
