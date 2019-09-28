#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define command_size 51
#define id_size 51

typedef struct relazione{
  char complemento[id_size];
  char tipo[id_size];
  struct relazione * next;
}relazione_tipo;

typedef struct listaRelazioni{
  relazione_tipo * in;
  relazione_tipo * out;
}listaRelazioni_tipo;

typedef struct nodo{
  char nome[id_size];
  struct nodo * padre;
  struct nodo * destro;
  struct nodo * sinistro;
  listaRelazioni_tipo rel;
  //0 -> nero; 1 -> rosso;
  int colore;
}nodo_tipo;

typedef struct albero{
  nodo_tipo * radice;
  nodo_tipo * sentinella;
}albero_tipo;

typedef struct nomiMax{
  char nome[id_size];
  struct nomiMax * next;
}nomiMax_tipo;

typedef struct tipoRelazione{
  char tipo[id_size];
  int maxCount;
  nomiMax_tipo * nomeMaxCount;
  struct tipoRelazione * next;
}tipoRelazione_tipo;

typedef struct contatoreLocale{
  char tipo[id_size];
  //nomiMax_tipo * nomi;
  int numero;
  struct contatoreLocale * next;
}contatoreLocale_tipo;

void readOneParameter(char * mem);
void readThreeParameters(char * p1, char * p2, char * p3);
void addent(albero_tipo * albero, nodo_tipo * nodoNuovo);
void printNodes(nodo_tipo * albero);
nodo_tipo * createNode(char nome[id_size]);
void rotateLeft(albero_tipo * albero, nodo_tipo * pivot);
void rotateRight(albero_tipo * albero, nodo_tipo * pivot);
void balanceTree(albero_tipo * albero, nodo_tipo * alice);
int countRightRabbitSteps(nodo_tipo * radice, int steps);
int countLeftRabbitSteps(nodo_tipo * radice, int steps);
nodo_tipo * cercaNodo(albero_tipo * albero, char * nodo);
void addrel(albero_tipo * albero, tipoRelazione_tipo * * listaTipiRelazione, char * origine, char * destinazione, char * tipo);
nodo_tipo * treeMin(nodo_tipo * radice, nodo_tipo * sentinella);
nodo_tipo * treeMax(nodo_tipo * radice, nodo_tipo * sentinella);
nodo_tipo * successore(albero_tipo * albero, nodo_tipo * nodo);
nodo_tipo * predecessore(albero_tipo * albero, nodo_tipo * nodo);
relazione_tipo * eliminaElementoListaRelazioni(listaRelazioni_tipo * rel, char * tipo, char * nome, int verso);
void delrel(albero_tipo * albero, char * origine, char * destinazione, char * tipo, tipoRelazione_tipo * * listaTipi);
void report(albero_tipo * albero, tipoRelazione_tipo * relazioni);
void addTipoRelazione(tipoRelazione_tipo * * testa, tipoRelazione_tipo * elemento);
void addRelazione(relazione_tipo * *testa, relazione_tipo * elemento);
void addNomeMaxCount(nomiMax_tipo * * testa, nomiMax_tipo * elemento);
void deleteFixup(albero_tipo * albero, nodo_tipo * nodo);
void delent(albero_tipo * albero, char * nome, tipoRelazione_tipo * * listaTipi);
void eliminaRelazioni(nodo_tipo * radice, char * nome, albero_tipo * albero);
void updateReport(nodo_tipo * radice, tipoRelazione_tipo * * listaTipi, nodo_tipo * sentinella);
void rimuoviTipi(tipoRelazione_tipo * * listaTipi);
void eliminaUno(relazione_tipo * * testa_p, char * nome);
void eliminaComplemento(relazione_tipo * * testa, char * complemento, char * tipo);

int main(){
  char com[command_size];
  char origin[id_size];
  char destination[id_size];
  char relationship[id_size];
  char name[id_size];

  tipoRelazione_tipo * listaTipiRelazione = NULL;

  albero_tipo albero;
  albero.sentinella = malloc(sizeof(nodo_tipo));
  strcpy(albero.sentinella->nome, "sentinella");
  albero.sentinella->colore = 0;
  albero.sentinella->padre = albero.sentinella;
  albero.sentinella->sinistro = albero.sentinella;
  albero.sentinella->destro = albero.sentinella;
  albero.radice = albero.sentinella;
  if (EOF != scanf("%s", com)) {
    do {
      if (0 == strcmp(com, "addrel")) {
        //printf("addrel\n");
        //read entity names and relationship id and invoke addrel function
        readThreeParameters(origin, destination, relationship);
        addrel(&albero, &listaTipiRelazione, origin, destination, relationship);
      }
      else if (0 == strcmp(com, "delrel")) {
        //printf("delrel\n");
        //read entity names and relationship id and invoke delrel function
        readThreeParameters(origin, destination, relationship);
        delrel(&albero, origin, destination, relationship, &listaTipiRelazione);
      }
      else if (0 == strcmp(com, "addent")) {
        //printf("addent\n");
        //read entity name and invoke addent function
        readOneParameter(name);
        addent(&albero, createNode(name));
      }
      else if (0 == strcmp(com, "delent")) {
        //printf("delent\n");
        //read entity name and invoke delent function
        readOneParameter(name);
        delent(&albero, name, &listaTipiRelazione);
      }
      else if (0 == strcmp(com, "report")) {
        //printf("report\n");
        //invoke report function
        //printNodes(albero.radice);
        report(&albero, listaTipiRelazione);
        //printNodes(albero.radice);
      }
      else if (0 == strcmp(com, "end")) {
        //terminate
        break;
      }
      else{
        //log and go on
      }
    } while(EOF != scanf("%s", com));
  }
  return(0);
}

//##############################################################################
/*auxiliary function definitions*/
//##############################################################################
void readOneParameter(char * mem){
  scanf("%*c");
  scanf("\"%[^\"]s", mem);
  scanf("%*c%*c");
}
//##############################################################################
void readThreeParameters(char * p1, char * p2, char * p3){
  scanf("%*c");
  scanf("\"%[^\"]s", p1);
  scanf("%*c%*c");
  scanf("\"%[^\"]s", p2);
  scanf("%*c%*c");
  scanf("\"%[^\"]s", p3);
  scanf("%*c%*c");
}
//##############################################################################
//insert node in tree
void addent(albero_tipo * albero, nodo_tipo * nodoNuovo){
  nodo_tipo * scout;
  nodo_tipo * puntatore;
  int confronto;
  scout = albero->radice;
  puntatore = albero->sentinella;
  while(scout != albero->sentinella){
    puntatore = scout;
    confronto = strcmp(nodoNuovo->nome, scout->nome);
    if(0 > confronto){
      scout = scout->sinistro;
    }
    else if(0 < confronto){
      scout = scout->destro;
    }
    else{
      return;
    }
  }
  nodoNuovo->padre = puntatore;
  if(puntatore == albero->sentinella){
    albero->radice = nodoNuovo;
  }
  else if(0 > strcmp(nodoNuovo->nome, puntatore->nome)){
    puntatore->sinistro = nodoNuovo;
  }
  else{
    puntatore->destro = nodoNuovo;
  }
  nodoNuovo->sinistro = albero->sentinella;
  nodoNuovo->destro = albero->sentinella;
  nodoNuovo->colore = 1;
  //then fix the tree structure
  balanceTree(albero, nodoNuovo);
}
//##############################################################################
//print all node values
void printNodes(nodo_tipo * radice){
  relazione_tipo * cursore;
  if(0 != strcmp(radice->nome, "sentinella")){
    printNodes(radice->sinistro);
    //--------------------------------------------------------------------------
    printf("%s : ", radice->nome);
    cursore = radice->rel.in;
    while(cursore != NULL){
      printf("[%s - %s] ", cursore->complemento, cursore->tipo);
      cursore = cursore->next;
    }
    cursore = radice->rel.out;
    while(cursore != NULL){
      printf("{%s - %s} ", cursore->complemento, cursore->tipo);
      cursore = cursore->next;
    }
    printf("\n");
    //--------------------------------------------------------------------------
    printNodes(radice->destro);
  }
  return;
}
//##############################################################################
//create a new node to insert in the tree
nodo_tipo * createNode(char nomeNodo[id_size]){
  nodo_tipo * p_nuovoNodo;
  p_nuovoNodo = malloc(sizeof(nodo_tipo));
  p_nuovoNodo->padre = NULL;
  p_nuovoNodo->sinistro = NULL;
  p_nuovoNodo->destro = NULL;
  p_nuovoNodo->colore = 0;
  p_nuovoNodo->rel.in = NULL;
  p_nuovoNodo->rel.out = NULL;
  strcpy(p_nuovoNodo->nome, nomeNodo);
  return(p_nuovoNodo);
}
//##############################################################################
//rotate tree
void rotateLeft(albero_tipo * albero, nodo_tipo * pivot){
  nodo_tipo * rotore;
  rotore = pivot->destro;
  pivot->destro = rotore->sinistro;
  if(rotore->sinistro != albero->sentinella){
    rotore->sinistro->padre = pivot;
  }
  rotore->padre = pivot->padre;
  if(pivot->padre == albero->sentinella){
    albero->radice = rotore;
  }
  else if(pivot == pivot->padre->sinistro){
    pivot->padre->sinistro = rotore;
  }
  else{
    pivot->padre->destro = rotore;
  }
  rotore->sinistro = pivot;
  pivot->padre = rotore;
}

void rotateRight(albero_tipo * albero, nodo_tipo * pivot){
  nodo_tipo * rotore;
  rotore = pivot->sinistro;
  pivot->sinistro = rotore->destro;
  if(rotore->destro != albero->sentinella){
    rotore->destro->padre = pivot;
  }
  rotore->padre = pivot->padre;
  if(pivot->padre == albero->sentinella){
    albero->radice = rotore;
  }
  else if(pivot == pivot->padre->sinistro){
    pivot->padre->sinistro = rotore;
  }
  else{
    pivot->padre->destro = rotore;
  }
  rotore->destro = pivot;
  pivot->padre = rotore;
}
//##############################################################################
//balance tree
void balanceTree(albero_tipo * albero, nodo_tipo * alice){
  nodo_tipo * bob, * carol;
  if(alice == albero->radice){
    albero->radice->colore = 0;
  }
  else{
    bob = alice->padre;
    if(bob->colore == 1){
      if(bob == bob->padre->sinistro){
        carol = bob->padre->destro;
        if(carol->colore == 1){
          carol->colore = 0;
          bob->colore = 0;
          bob->padre->colore = 1;
          balanceTree(albero, bob->padre);
        }
        else{
          if(alice == bob->destro){
            alice = bob;
            rotateLeft(albero, alice);
            bob = alice->padre;
          }
          bob->colore = 0;
          bob->padre->colore = 1;
          rotateRight(albero, bob->padre);
        }
      }
      else{
        carol = bob->padre->sinistro;
        if(carol->colore == 1){
          carol->colore = 0;
          bob->colore = 0;
          bob->padre->colore = 1;
          balanceTree(albero, bob->padre);
        }
        else{
          if(alice == bob->sinistro){
            alice = bob;
            rotateRight(albero, alice);
            bob = alice->padre;
          }
          bob->colore = 0;
          bob->padre->colore = 1;
          rotateLeft(albero, bob->padre);
        }
      }
    }
  }
}
//##############################################################################
//print a tree on console

//##############################################################################
//add relationship between two entities
void addrel(albero_tipo * albero, tipoRelazione_tipo * * listaTipiRelazione, char * origine, char * destinazione, char * tipo){
  tipoRelazione_tipo * cursoreTipi = *listaTipiRelazione;
  int tipoEsistente = 0;
  int relEsistente = 0;
  relazione_tipo * cursore;
  nodo_tipo * nodoOrigine;
  nodo_tipo * nodoDestinazione;
  relazione_tipo * nuovaRelazione;
  int contatoreLocale;
  nomiMax_tipo * nomiMax_p;
  nomiMax_tipo * nomiMax_p1;
  nomiMax_tipo * nuovoNodoNomeMax;

  while(cursoreTipi != NULL){
    if(0 == strcmp(tipo, cursoreTipi->tipo)){
      tipoEsistente = 1;
      break;
    }
    cursoreTipi = cursoreTipi->next;
  }

  nodoOrigine = cercaNodo(albero, origine);
  nodoDestinazione = cercaNodo(albero, destinazione);
  if(nodoOrigine != albero->sentinella && nodoDestinazione != albero->sentinella){
    cursore = nodoDestinazione->rel.in;
    relEsistente = 0;
    contatoreLocale = 0;

    if(tipoEsistente == 0){
      tipoRelazione_tipo * nuovoTipo = malloc(sizeof(tipoRelazione_tipo));
      strcpy(nuovoTipo->tipo, tipo);
      nuovoTipo->maxCount = 0;
      nuovoTipo->nomeMaxCount = NULL;
      addTipoRelazione(listaTipiRelazione, nuovoTipo);
    }
    else{
      while(cursore != NULL){
        if(0 == strcmp(cursore->tipo, tipo)){
          contatoreLocale++;
          if(0 == strcmp(cursore->complemento, origine)){
            relEsistente = 1;
          }
        }
        cursore = cursore->next;
      }
    }

    if(relEsistente == 0){
      nuovaRelazione = malloc(sizeof(relazione_tipo));
      strcpy(nuovaRelazione->complemento, origine);
      strcpy(nuovaRelazione->tipo, tipo);
      addRelazione(&(nodoDestinazione->rel.in), nuovaRelazione);

      nuovaRelazione = malloc(sizeof(relazione_tipo));
      strcpy(nuovaRelazione->complemento, destinazione);
      strcpy(nuovaRelazione->tipo, tipo);
      addRelazione(&(nodoOrigine->rel.out), nuovaRelazione);

      contatoreLocale++;

      cursoreTipi = *listaTipiRelazione;
      while(cursoreTipi != NULL){
        if(0 == strcmp(cursoreTipi->tipo, tipo)){
          if(contatoreLocale > cursoreTipi->maxCount){
            nomiMax_p = cursoreTipi->nomeMaxCount;
            while(nomiMax_p != NULL){
              nomiMax_p1 = nomiMax_p->next;
              free(nomiMax_p);
              nomiMax_p = nomiMax_p1;
            }
            nuovoNodoNomeMax = malloc(sizeof(nomiMax_tipo));
            strcpy(nuovoNodoNomeMax->nome, destinazione);
            nuovoNodoNomeMax->next = NULL;
            cursoreTipi->nomeMaxCount = nuovoNodoNomeMax;
            cursoreTipi->maxCount = contatoreLocale;
          }
          else if(contatoreLocale == cursoreTipi->maxCount){
            nuovoNodoNomeMax = malloc(sizeof(nomiMax_tipo));
            strcpy(nuovoNodoNomeMax->nome, destinazione);
            //-----------------------------------------------------
            /*nuovoNodoNomeMax->next = cursoreTipi->nomeMaxCount;
            cursoreTipi->nomeMaxCount = nuovoNodoNomeMax;*/
            addNomeMaxCount(&(cursoreTipi->nomeMaxCount), nuovoNodoNomeMax);
            //-----------------------------------------------------
          }
          break;
        }
        cursoreTipi = cursoreTipi->next;
      }
    }
    
  }
}
//##############################################################################
//cerca un nodo in un albero
nodo_tipo * cercaNodo(albero_tipo * albero, char * nodo){
  nodo_tipo * cursore = albero->radice;
  int confronto;
  while(cursore != albero->sentinella){
    confronto = strcmp(nodo, cursore->nome);
    if(0 == confronto){
      return cursore;
    }
    else if(0 > confronto){
      cursore = cursore->sinistro;
    }
    else{
      cursore = cursore->destro;
    }
  }
  return albero->sentinella;
}
//##############################################################################
//rimuovi una entità dall'albero
void delent(albero_tipo * albero, char * nome, tipoRelazione_tipo * * listaTipi){
  //printf("deleting %s\n", nome);
  nodo_tipo * nodo = cercaNodo(albero, nome);
  nodo_tipo * nodoDaEliminare;
  nodo_tipo * nodoSostituto;

  if(nodo != albero->sentinella){
    if(nodo->sinistro == albero->sentinella || nodo->destro == albero->sentinella){
      nodoDaEliminare = nodo;
    }
    else{
      nodoDaEliminare = successore(albero, nodo);
    }
    if(nodoDaEliminare->sinistro != albero->sentinella){
      nodoSostituto = nodoDaEliminare->sinistro;
    }
    else{
      nodoSostituto = nodoDaEliminare->destro;
    }
    nodoSostituto->padre = nodoDaEliminare->padre;
    if(nodoDaEliminare->padre == albero->sentinella){
      albero->radice = nodoSostituto;
    }
    else if(nodoDaEliminare == nodoDaEliminare->padre->sinistro){
      nodoDaEliminare->padre->sinistro = nodoSostituto;
    }
    else{
      nodoDaEliminare->padre->destro = nodoSostituto;
    }
    if(nodoDaEliminare != nodo){
      strcpy(nodo->nome, nodoDaEliminare->nome);
      nodo->rel.in = nodoDaEliminare->rel.in;
      nodo->rel.out = nodoDaEliminare->rel.out;
    }
    if(nodoDaEliminare->colore == 0){
      //delete fixup
      deleteFixup(albero, nodoSostituto);
    }
    free(nodoDaEliminare);
  }

  //elimina tutte le relazioni che originavano nel nodo eliminato
  eliminaRelazioni(albero->radice, nome, albero);
  

  tipoRelazione_tipo * cursoreTipi;
  nomiMax_tipo * cursoreNomi;
  cursoreTipi = *listaTipi;
  while(cursoreTipi != NULL){
    cursoreTipi->maxCount = 0;
    cursoreNomi = cursoreTipi->nomeMaxCount;
    while(cursoreNomi != NULL){
      cursoreTipi->nomeMaxCount = cursoreTipi->nomeMaxCount->next;
      free(cursoreNomi);
      cursoreNomi = cursoreTipi->nomeMaxCount;
    }
    cursoreTipi = cursoreTipi->next;
  }

  updateReport(albero->radice, listaTipi, albero->sentinella);

  cursoreTipi = *listaTipi;
  tipoRelazione_tipo * tmp, * prox;
  if((*listaTipi) != NULL){

    if((*listaTipi)->maxCount == 0){
      *listaTipi = (*listaTipi)->next;
      free(cursoreTipi);
    }
    else{
      while(cursoreTipi->next != NULL){
        prox = cursoreTipi->next;
        if(prox->maxCount == 0){
          cursoreTipi->next = prox->next;
          free(prox);
        }
        cursoreTipi = prox;
      }
    }
  }

  //printNodes(albero->radice);
  rimuoviTipi(listaTipi);
}
//##############################################################################
//trova il minimo nell'albero
nodo_tipo * treeMin(nodo_tipo * radice, nodo_tipo * sentinella){
  nodo_tipo * min = radice;
  while(min->sinistro != sentinella){
    min = min->sinistro;
  }
  return min;
}
//trova il massimo nell'albero
nodo_tipo * treeMax(nodo_tipo * radice, nodo_tipo * sentinella){
  nodo_tipo * max = radice;
  while(max->destro != sentinella){
    max = max->destro;
  }
  return max;
}
//trova il successore del nodo nell'albero
nodo_tipo * successore(albero_tipo * albero, nodo_tipo * nodo){
  if(nodo->destro != albero->sentinella){
    return(treeMin(nodo->destro, albero->sentinella));
  }
  nodo_tipo * padre;
  padre = nodo->padre;
  while(padre != albero->sentinella && nodo == padre->destro){
    nodo = padre;
    padre = nodo->padre;
  }
  return padre;
}
//trova il predecessore del nodo nell'albero
nodo_tipo * predecessore(albero_tipo * albero, nodo_tipo * nodo){
  if(nodo->sinistro != albero->sentinella){
    return(treeMax(nodo->sinistro, albero->sentinella));
  }
  nodo_tipo * padre;
  padre = nodo->padre;
  while(padre != albero->sentinella && nodo == padre->sinistro){
    nodo = padre;
    padre = nodo->padre;
  }
  return padre;
}
//##############################################################################
//rimuovi una relazione tra due entità dell'albero
void delrel(albero_tipo * albero, char * origine, char * destinazione, char * tipo, tipoRelazione_tipo * * listaTipi){
  int relEsistente = 0;
  nodo_tipo * nodoOrigine;
  nodo_tipo * nodoDestinazione;
  relazione_tipo * relIn, * relOut;
  relazione_tipo * cursore;
  tipoRelazione_tipo * cursoreTipi;
  nomiMax_tipo * cursoreNomi;
  nodoOrigine = cercaNodo(albero, origine);
  nodoDestinazione = cercaNodo(albero, destinazione);
  relIn = NULL;
  relOut = NULL;
  if(nodoOrigine != albero->sentinella && nodoDestinazione != albero->sentinella){
    cursore = nodoOrigine->rel.out;
    while(cursore != NULL){
      if(0 == strcmp(destinazione, cursore->complemento)){
        if(0 == strcmp(tipo, cursore->tipo)){
          relOut = cursore;
        }
      }
      cursore = cursore->next;
    }
    cursore = nodoDestinazione->rel.in;
    while(cursore != NULL){
      if(0 == strcmp(origine, cursore->complemento)){
        if(0 == strcmp(tipo, cursore->tipo)){
          relIn = cursore;
        }
      }
      cursore = cursore->next;
    }
  }
  else{
    return;
  }
  if(relIn != NULL && relOut != NULL){
    eliminaComplemento(&(nodoOrigine->rel.out), relOut->complemento, relOut->tipo);
    eliminaComplemento(&(nodoDestinazione->rel.in), relIn->complemento, relOut->tipo);
  }
  else{
    return;
  }
  //---------------------------------------------------------------------------
  cursoreTipi = *listaTipi;
  while(cursoreTipi != NULL){
    if(0 == strcmp(cursoreTipi->tipo, tipo)){
      cursoreNomi = cursoreTipi->nomeMaxCount;
      if(cursoreNomi->next == NULL){
        
        tipoRelazione_tipo * cursoreTipi;
        nomiMax_tipo * cursoreNomi;
        cursoreTipi = *listaTipi;
        while(cursoreTipi != NULL){
          cursoreTipi->maxCount = 0;
          cursoreNomi = cursoreTipi->nomeMaxCount;
          while(cursoreNomi != NULL){
            cursoreTipi->nomeMaxCount = cursoreTipi->nomeMaxCount->next;
            free(cursoreNomi);
            cursoreNomi = cursoreTipi->nomeMaxCount;
          }
          cursoreTipi = cursoreTipi->next;
        }

        updateReport(albero->radice, listaTipi, albero->sentinella);

        cursoreTipi = *listaTipi;
        tipoRelazione_tipo * tmp, * prox;
        if((*listaTipi) != NULL){

          if((*listaTipi)->maxCount == 0){
            *listaTipi = (*listaTipi)->next;
            free(cursoreTipi);
          }
          else{
            while(cursoreTipi->next != NULL){
              prox = cursoreTipi->next;
              if(prox->maxCount == 0){
                cursoreTipi->next = prox->next;
                free(prox);
              }
              cursoreTipi = prox;
            }
          }
        }

      }
      else{
        nomiMax_tipo * prox;
        prox = cursoreNomi->next;
        if(0 == strcmp(cursoreNomi->nome, destinazione)){
          cursoreTipi->nomeMaxCount = cursoreTipi->nomeMaxCount->next;
          free(cursoreNomi);
        }
        else{
          while(prox != NULL){
            if(0 == strcmp(prox->nome, destinazione)){
              cursoreNomi->next = prox->next;
              free(prox);
            }
            cursoreNomi = cursoreNomi->next;
            if(cursoreNomi == NULL){
              prox = NULL;
            }
            else{
              prox = cursoreNomi->next;
            }
          }
        }
      }
      break;
    }
    cursoreTipi = cursoreTipi->next;
  }
  //---------------------------------------------------------------------------
}
//##############################################################################
//elimina un elemento da una lista di relazioni
relazione_tipo * eliminaElementoListaRelazioni(listaRelazioni_tipo * rel, char * tipo, char * nome, int verso){
  relazione_tipo * result;
  relazione_tipo * cursore;
  relazione_tipo * scout;
  relazione_tipo * * testa;
  if(0 == verso){
    testa = &(rel->in);
  }
  else{
    testa = &(rel->out);
  }
  cursore = *testa;
  if(cursore != NULL){
    scout = cursore->next;
    if(0 == strcmp(cursore->complemento, nome) && 0 == strcmp(cursore->tipo, tipo)){
      *testa = scout;
      //free(cursore);
      result = cursore;
    }
  }
  else{
    scout = NULL;
    result = NULL;
  }
  while(scout != NULL){
    if(0 == strcmp(scout->complemento, nome) && 0 == strcmp(scout->tipo, tipo)){
      cursore->next = scout->next;
      //free(scout);
      result = scout;
      break;
    }
    cursore = scout;
    scout = cursore->next;
  }
  return result;
}
//##############################################################################
//magari facciamola più semplice
void eliminaComplemento(relazione_tipo * * testa, char * complemento, char * tipo){
  if(*testa == NULL){
    return;
  }
  relazione_tipo * c, * s;
  c = *testa;
  if(0 == strcmp((*testa)->complemento, complemento)){
    if(0 == strcmp((*testa)->tipo, tipo)){
      (*testa) = (*testa)->next;
      free(c);
    }
  }
  else{
    s = c->next;
    while(s != NULL){
      if(0 == strcmp(s->complemento, complemento)){
        if(0 == strcmp(s->tipo, tipo)){
          c->next = s->next;
          free(s);
          break;
        }
      }
      c = s;
      s = s->next;
    }
  }
}
//##############################################################################
void report(albero_tipo * albero, tipoRelazione_tipo * relazioni){
  tipoRelazione_tipo * cursore;
  nomiMax_tipo * cursoreNomi;
  cursore = relazioni;
  if(cursore == NULL){
    fputs("none\n", stdout);
  }
  else{
    while(cursore != NULL){
      printf("\"%s\"", cursore->tipo);
      cursoreNomi = cursore->nomeMaxCount;
      while(cursoreNomi != NULL){
        printf(" \"%s\"", cursoreNomi->nome);
        cursoreNomi = cursoreNomi->next;
      }
      printf(" %d", cursore->maxCount);
      cursore = cursore->next;
      if(cursore == NULL){
        fputs(";\n", stdout);
      }
      else{
        fputs("; ", stdout);
      }
    }
  }
}
//##############################################################################
//delete fixup
void deleteFixup(albero_tipo * albero, nodo_tipo * nodo){
  nodo_tipo * fratello;
  if(nodo->colore == 1 || nodo == albero->sentinella){
    nodo->colore = 0;
  }
  else{
    //--------------------------------------------------------------------------
    if(nodo == nodo->padre->sinistro){
      fratello = nodo->padre->destro;
      if(fratello->colore == 1){
        fratello->colore = 0;
        nodo->padre->colore = 1;
        rotateLeft(albero, nodo->padre);
        fratello = nodo->padre->destro;
      }
      if(fratello->destro->colore == 0 && fratello->sinistro->colore == 0){
        fratello->colore = 1;
        deleteFixup(albero, nodo->padre);
      }
      else{
        if(fratello->destro->colore == 0){
          fratello->sinistro->colore = 0;
          fratello->colore = 1;
          rotateRight(albero, fratello);
          fratello = nodo->padre->destro;
        }
        else{
          fratello->colore = nodo->padre->colore;
          nodo->padre->colore = 0;
          fratello->destro->colore = 0;
          rotateLeft(albero, nodo->padre);
        }
      }
    }
    //--------------------------------------------------------------------------
    else{
      fratello = nodo->padre->sinistro;
      if(fratello->colore == 1){
        fratello->colore = 0;
        nodo->padre->colore = 1;
        rotateRight(albero, nodo->padre);
        fratello = nodo->padre->sinistro;
      }
      if(fratello->sinistro->colore == 0 && fratello->destro->colore == 0){
        fratello->colore = 1;
        deleteFixup(albero, nodo->padre);
      }
      else{
        if(fratello->sinistro->colore == 0){
          fratello->destro->colore = 0;
          fratello->colore = 1;
          rotateLeft(albero, fratello);
          fratello = nodo->padre->sinistro;
        }
        else{
          fratello->colore = nodo->padre->colore;
          nodo->padre->colore = 0;
          fratello->sinistro->colore = 0;
          rotateLeft(albero, nodo->padre);
        }
      }
    }
    //--------------------------------------------------------------------------
  }
}
//##############################################################################
//aggiungi elemento a lista dinamica in ordine
void addTipoRelazione(tipoRelazione_tipo * * testa, tipoRelazione_tipo * elemento){
  tipoRelazione_tipo * curr, * pross;
  if(*testa != NULL){
    if(0 > strcmp(elemento->tipo, (*testa)->tipo)){
      elemento->next = *testa;
      *testa = elemento;
    }
    else{
      curr = *testa;
      pross = curr->next;
      int done = 0;
      while(pross != NULL){
        if(strcmp(elemento->tipo, curr->tipo) > 0 && 0 > strcmp(elemento->tipo, pross->tipo)){
          elemento->next = pross;
          curr->next = elemento;
          done = 1;
          break;
        }
        curr = pross;
        pross = curr->next;
      }
      if(done == 0){
        elemento->next = NULL;
        curr->next = elemento;
        done = 1;
      }
    }
  }
  else{
    elemento->next = NULL;
    *testa = elemento;
  }
}
//##############################################################################
//aggiungi nuova relazione in ordine
void addRelazione(relazione_tipo * * testa, relazione_tipo * elemento){
  relazione_tipo * curr, * pross;
  if(*testa != NULL){
    if(0 > strcmp(elemento->complemento, (*testa)->complemento)){
      elemento->next = *testa;
      *testa = elemento;
    }
    else{
      curr = *testa;
      pross = curr->next;
      int done = 0;
      while(pross != NULL){
        if(strcmp(elemento->complemento, curr->complemento) > 0 && 0 > strcmp(elemento->complemento, pross->complemento)){
          elemento->next = pross;
          curr->next = elemento;
          done = 1;
          break;
        }
        curr = pross;
        pross = curr->next;
      }
      if(done == 0){
        elemento->next = NULL;
        curr->next = elemento;
        done = 1;
      }
    }
  }
  else{
    elemento->next = NULL;
    *testa = elemento;
  }
}
//##############################################################################
void addNomeMaxCount(nomiMax_tipo * * testa, nomiMax_tipo * elemento){
  nomiMax_tipo * curr, * pross;
  if(*testa != NULL){
    if(0 > strcmp(elemento->nome, (*testa)->nome)){
      elemento->next = *testa;
      *testa = elemento;
    }
    else{
      curr = *testa;
      pross = curr->next;
      int done = 0;
      while(pross != NULL){
        if(strcmp(elemento->nome, curr->nome) > 0 && 0 > strcmp(elemento->nome, pross->nome)){
          elemento->next = pross;
          curr->next = elemento;
          done = 1;
          break;
        }
        curr = pross;
        pross = curr->next;
      }
      if(done == 0){
        elemento->next = NULL;
        curr->next = elemento;
        done = 1;
      }
    }
  }
  else{
    elemento->next = NULL;
    *testa = elemento;
  }
}
//##############################################################################
//scorri l'albero e aggiorna max count e i loro nomi per ogni tipo di relazione
void updateReport(nodo_tipo * radice, tipoRelazione_tipo * * listaTipi, nodo_tipo * sentinella){
  if(radice->sinistro != sentinella){
    updateReport(radice->sinistro, listaTipi, sentinella);
  }
  //-------------------------------------------------------------------------------
  
  contatoreLocale_tipo * testaContatori = NULL;
  contatoreLocale_tipo * nuovoContatore = NULL;
  tipoRelazione_tipo * cursoreTipi;
  nomiMax_tipo * cursoreNomi;
  relazione_tipo * cursoreRelazioni;

  cursoreTipi = *listaTipi;
  while(cursoreTipi != NULL){
    nuovoContatore = malloc(sizeof(contatoreLocale_tipo));
    nuovoContatore->numero = 0;
    strcpy(nuovoContatore->tipo, cursoreTipi->tipo);
    nuovoContatore->next = testaContatori;
    testaContatori = nuovoContatore;

    cursoreTipi = cursoreTipi->next;
  }

  cursoreRelazioni = radice->rel.in;
  while(cursoreRelazioni != NULL){
    nuovoContatore = testaContatori;
    while(nuovoContatore != NULL){
      if(0 == strcmp(nuovoContatore->tipo, cursoreRelazioni->tipo)){
        nuovoContatore->numero++;
        break;
      }
      nuovoContatore = nuovoContatore->next;
    }
    cursoreRelazioni = cursoreRelazioni->next;
  }
  
  cursoreTipi = *listaTipi;
  while(cursoreTipi != NULL){
    nuovoContatore = testaContatori;
    while(nuovoContatore != NULL){
      if(0 == strcmp(nuovoContatore->tipo, cursoreTipi->tipo)){
        if(nuovoContatore->numero > cursoreTipi->maxCount){
          cursoreTipi->maxCount = nuovoContatore->numero;
          if(cursoreTipi->nomeMaxCount != NULL){
            cursoreNomi = cursoreTipi->nomeMaxCount;
            while(cursoreNomi != NULL){
              cursoreTipi->nomeMaxCount = cursoreTipi->nomeMaxCount->next;
              free(cursoreNomi);
              cursoreNomi = cursoreTipi->nomeMaxCount;
            }
          }
          cursoreTipi->nomeMaxCount = malloc(sizeof(nomiMax_tipo));
          strcpy(cursoreTipi->nomeMaxCount->nome, radice->nome);
          cursoreTipi->nomeMaxCount->next = NULL;
        }
        else if(nuovoContatore->numero == cursoreTipi->maxCount){
          if(nuovoContatore->numero > 0){
            cursoreNomi = malloc(sizeof(nomiMax_tipo));
            strcpy(cursoreNomi->nome, radice->nome);
            cursoreNomi->next = NULL;
            addNomeMaxCount(&(cursoreTipi->nomeMaxCount), cursoreNomi);
          }
        }
        break;
      }
      nuovoContatore = nuovoContatore->next;
    }
    cursoreTipi = cursoreTipi->next;
  }

  nuovoContatore = testaContatori;
  while(nuovoContatore != NULL){
    testaContatori = testaContatori->next;
    free(nuovoContatore);
    nuovoContatore = testaContatori;
  }
  //-------------------------------------------------------------------------------
  if(radice->destro != sentinella){
    updateReport(radice->destro, listaTipi, sentinella);
  }
}
//##############################################################################
//elimina relazioni con orignine in argomento
void eliminaRelazioni(nodo_tipo * radice, char * nome, albero_tipo * albero){
  if(radice->sinistro != albero->sentinella){
    eliminaRelazioni(radice->sinistro, nome, albero);
  }
  //----------------------------------------------------------------------------
  //****************************************************************************
  eliminaUno(&(radice->rel.in), nome);
  eliminaUno(&(radice->rel.out), nome);
  //****************************************************************************  
  //----------------------------------------------------------------------------
  if(radice->destro != albero->sentinella){
    eliminaRelazioni(radice->destro, nome, albero);
  }
}
//##############################################################################
//smetti di monitorare le relazioni non istanziate
void rimuoviTipi(tipoRelazione_tipo * * listaTipi){
  tipoRelazione_tipo * cursoreTipi;
  cursoreTipi = *listaTipi;
  tipoRelazione_tipo * tmp, * prox;
  if((*listaTipi) != NULL){

    if((*listaTipi)->maxCount == 0){
      *listaTipi = (*listaTipi)->next;
      free(cursoreTipi);
    }
    else{
      while(cursoreTipi->next != NULL){
        prox = cursoreTipi->next;
        if(prox->maxCount == 0){
          cursoreTipi->next = prox->next;
          free(prox);
        }
        cursoreTipi = prox;
      }
    }
  }
}

//##############################################################################
//elimina un elemento da una lista di relazioni (quello che ha come complemento l'argomento)
void eliminaUno(relazione_tipo * * testa_p, char * nome){
  relazione_tipo * c, * s;
  c = *testa_p;
  if(c == NULL){
    return;
  }
  if(0 == strcmp(c->complemento, nome)){
    *testa_p = (*testa_p)->next;
    free(c);
    eliminaUno(testa_p, nome);
  }
  else{
    s = c->next;
    while(s != NULL){
      if(0 == strcmp(s->complemento, nome)){
        c->next = s->next;
        free(s);
        eliminaUno(testa_p, nome);
        break;
      }
      else{
        c = s;
        s = s->next;
      }
    }
  }
}
//##############################################################################

