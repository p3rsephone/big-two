#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_MAX_ESTADO      1024

#define FORMATO             "q=h1?%llx_h2?%llx_h3?%llx_h4?%llx_t1?%d_t2?%d_t3?%d_t4?%d_sel?%llx_lPlay?%llx_lPlayr?%d_act?%c_PASS?%d_b1?%llx_b2?%llx_b3?%llx"

#define QUERY               "%s?q=h1?%llx_h2?%llx_h3?%llx_h4?%llx_t1?%d_t2?%d_t3?%d_t4?%d_sel?%llx_lPlay?%llx_lPlayr?%d_act?%c_PASS?%d_b1?%llx_b2?%llx_b3?%llx"
/**
URL da CGI
*/
#define SCRIPT		"http://127.0.0.1/cgi-bin/big2"
/**
URL da pasta com as cartas e com os botões
*/
#define BARALHO		"http://127.0.0.1/cards"
/**
Ordem dos naipes
*/
#define NAIPES		"DCHS"
/**
Ordem dos valores das cartas
*/
#define VALORES		"3456789TJQKA2"

/**
          Estrutura de dados onde estão armazenados (e vão sendo
          atualizados) todas as informações relativas ao estado 
          do jogo, respetivamente:mãos dos jogadores;tamanho de
          cada mão;cartas selecionadas pelo jogador;ultima jogada;
          ultimo jogador;ultima ação;numero de jogadores que passaram
          ultimas jogadas dos bots;
*/
typedef struct {
  long long unsigned  int mao[4];
  int tamanho[4];
  long long unsigned int selecao;
  long long unsigned int uJogada;
  int uJogador;
  char acao;
  int nPass;
  long long unsigned int bots[3];
} ESTADO;

void baralha (long long unsigned int *n, int* m);int le_estado(char *estado, long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,long long unsigned int* bots, int* pass);
char *da_estado(ESTADO mao);int indice(int naipe, int valor);int contaUns (long long unsigned int n);long long unsigned int add_carta_indice(long long unsigned int m, int indice);
long long unsigned int add_carta(long long unsigned int ESTADO, int naipe, int valor);long long unsigned int rem_carta_indice(long long unsigned int m, int indice);
long long unsigned int rem_carta(long long unsigned int ESTADO, int naipe, int valor);int carta_existe(long long unsigned int ESTADO, int naipe, int valor);
void imprime_botoes(ESTADO m); void imprime_carta(char const *path, int x, int y, ESTADO e, int naipe, int valor, int mao);void imprime(char const *path, ESTADO e,int w);void imprime_bots(ESTADO e);
long long unsigned int sel_carta(ESTADO e, int naipe, int valor);long long unsigned int del_cartas(long long unsigned int m, long long unsigned int cartas);void jogar(long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int* pass, long long unsigned int* bots);
int validaJogada(ESTADO e);int mesmoNum(long long unsigned int cartas); int proxJogador (int uj);void jogaUma (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int* pass,long long unsigned int* bots);
void jogaDuas (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int* pass, long long unsigned int* bots);int indiNumCarta (long long unsigned int cartas,int n);int valor_indice (int ind);int naipe_indice (int ind);
void jogaTres (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int*pass, long long unsigned int* bots);
void jogaMjogada (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int* pass, long long unsigned int* bots);
void inicializa ( long long unsigned int *mao, int* t,long long unsigned int* ujogada, int* ujogador ,char* c, long long unsigned int* bots);void win (int winner);
int carta_existe_indice(long long unsigned int m, int indice);int numCartasValor (long long unsigned int mao,int valor);int combinacao3 (long long unsigned int mao);int combinacao2 (long long unsigned int mao);
long long unsigned int hint (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao);long long unsigned int ajudaUma (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao);
long long unsigned int ajudaDuas (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao);long long unsigned int ajudaTres (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao);
long long unsigned int ajudaMJogada (long long unsigned int sel, long long unsigned int mao);

/** \brief Função principal

          Função principal do programa que inicializa a estrutura 
          de dados toda a zero, lê da query o estado e atualiza a 
          estrutura com as informações atuais do mesmo. 
          Função que vai imprimir o código html para desenhar as 
          cartas e toda mesa de jogo propriamente dita.
 */
int main() {

    ESTADO e;
    e.mao[0] = 0; e.mao[1] = 0; e.mao[2] = 0; e.mao[3] = 0;
    e.tamanho[0] = 0; e.tamanho[1] = 0; e.tamanho[2] = 0; e.tamanho[3] = 0;
    e.selecao=0; e.uJogada=0; e.uJogador=3;
    e.bots[0] = 0; e.bots[1] = 0; e.bots[2] = 0;
    e.acao='n';
    e.nPass=0;
    int winner = 4;
    char *estado;

    printf("Content-Type: text/html; charset=utf-8\n\n");

    le_estado(getenv("QUERY_STRING"), e.mao, e.tamanho,&e.selecao, &e.uJogada, &e.uJogador, &e.acao, e.bots, &e.nPass);
    if (e.uJogador != 3) jogar(e.mao, e.tamanho, &e.selecao, &e.uJogada, &e.uJogador, &e.acao, &e.nPass, e.bots);
    if (e.nPass==3) {e.uJogada=0;e.nPass=0;}
    if (e.tamanho[0] == 0) winner = 0; else if (e.tamanho[1] == 0) winner =1; else if (e.tamanho[2] == 0) winner = 2; else if (e.tamanho[3] == 0) winner = 3;
    estado = da_estado(e);
    imprime(BARALHO,e,winner);

    return 0;
}


int le_estado(char *estado, long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,long long unsigned int* bots, int* pass) {
    if(estado == NULL || strlen(estado) == 0) {
        baralha(mao,t);
        inicializa(mao,t,ujogada,ujogador,c,bots);

        return sscanf(estado, FORMATO, mao, mao+1, mao+2, mao+3, t, t+1, t+2, t+3,sel,ujogada,ujogador,c,pass,bots,bots+1,bots+2);
    }
    return sscanf(estado, FORMATO, mao, mao+1, mao+2, mao+3, t, t+1, t+2, t+3,sel,ujogada,ujogador,c,pass,bots,bots+1,bots+2);
}

void inicializa (long long unsigned int *mao, int* t,long long unsigned int* ujogada, int* ujogador ,char* c, long long unsigned int* bots) {
   int jog;
   int i=0;
   int naipe;
   for (jog=1;jog<=3 && !(carta_existe(mao[jog],0,0));jog++);
   if (jog<4) {
      for (naipe = 0;naipe<=3 && i<3;naipe++){
            if (carta_existe(mao[jog],naipe,0)) {mao[jog]=rem_carta(mao[jog],naipe,0);*ujogada=add_carta(*ujogada,naipe,0);bots[jog-1]=add_carta(bots[jog-1],naipe,0);t[jog]--;}
      }
      bots[jog-1]=*ujogada;
   }
   *ujogador = jog;
   *c = 'j';
 }


char *da_estado(ESTADO e) {
    char *estado = NULL;
    estado = (char *) malloc (TAM_MAX_ESTADO);
    sprintf(estado, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.tamanho[0], e.tamanho[1], e.tamanho[2], e.tamanho[3],e.selecao,e.uJogada,e.uJogador,e.acao,e.nPass,e.bots[0],e.bots[1],e.bots[2]);
    return estado;
}

/**
Devolve o índice da carta
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/
int indice(int naipe, int valor) {
	return 4*valor + naipe;
}

/** \brief Devolve o número de "uns" (cartas) que uma mão possui
@param mão 	   Mão da qual se pretende saber o número de cartas
@return		O número de cartas existentes na mão
*/
int contaUns (long long unsigned int n) {
    int x=0, i;
    for(i =0; n>0;i++){
        x+=(n%2);
        n/=2;
    }
    return x;
}

/** \brief Adiciona uma carta a uma mão, dado o indice

@param mão    A mão destino
@param indíce    O indíce da carta (inteiro de 0 a 51)
@return    A nova mão
*/
long long unsigned int add_carta_indice(long long unsigned int m, int indice) {
	return m | ((long long unsigned int) 1 << indice);
}

/** \brief Adiciona uma carta a uma mão

@param mão    A mão destino
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return    A nova mão
*/
long long unsigned int add_carta(long long unsigned int m, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return m | ((long long unsigned int) 1 << idx);
}

/** \brief Remove uma carta a uma mão, dado o indice

@param mão    A mão destino
@param indíce    O indíce da carta (inteiro de 0 a 51)
@return    A nova mão
*/
long long unsigned int rem_carta_indice(long long unsigned int m, int indice) {
	return m & ~((long long unsigned int) 1 << indice);
}

/** \brief Remove uma carta a uma mão

@param mão    A mão destino
@param naipe    O naipe da carta (inteiro entre 0 e 3)
@param valor    O valor da carta (inteiro entre 0 e 12)
@return    A nova mão
*/
long long unsigned int rem_carta(long long unsigned int m, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return m & ~((long long unsigned int) 1 << idx);
}

/** \brief Remove varias cartas de uma mão

@param mão    A mão donde se vão remover as cartas
@param cartas    A mão com as cartas a remover
@return    A mão origem alterada
*/
long long unsigned int del_cartas(long long unsigned int cartas, long long unsigned int m) {
	return m & ~(cartas);
}

/** \brief Seleciona ou remove da seleção uma carta 

@param ESTADO    O estado do jogo
@param naipe    O naipe da carta (inteiro entre 0 e 3)
@param valor    O valor da carta (inteiro entre 0 e 12)
@return    A mão seleção alterada
*/
long long unsigned int sel_carta(ESTADO e, int naipe, int valor) {
    int idx = indice(naipe, valor);
    return (e.selecao) ^ ((long long unsigned int) 1 << idx);
}

/** \brief Verifica se uma carta pertence a uma mão, dado o indíce

@param mão    A mão objetivo
@param índice    O índice da carta cuja existência se pretende verificar
@return    1 se a carta existe e 0 caso contrário
*/
int carta_existe_indice(long long unsigned int m, int indice) {
  return (m >> indice) & 1;
}

/** \brief Verifica se uma carta pertence a uma mão

@param mão    A mão objetivo
@param naipe    O naipe da carta (inteiro entre 0 e 3)
@param valor    O valor da carta (inteiro entre 0 e 12)
@return    1 se a carta existe e 0 caso contrário
*/
int carta_existe(long long unsigned int m, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (m >> idx) & 1;
}

/** \brief Imprime o html correspondente ao botão passar e ao botão jogar
@param ESTADO    O estado do jogo
*/
void imprime_botoes(ESTADO e) {
    char scriptPassa[10240];
    sprintf(scriptPassa, QUERY, SCRIPT, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.tamanho[0], e.tamanho[1], e.tamanho[2], e.tamanho[3],e.selecao,e.uJogada,0,'p',(++e.nPass),e.bots[0],e.bots[1],e.bots[2]);
    printf("<a xlink:href = \"%s\"><image x = \"590\" y=\"650\" height=\"40\" width=\"206\" xlink:href = http://127.0.0.1/cards/Passar.png /></a>\n", scriptPassa);
    if ((e.selecao != 0) && (validaJogada(e))) {
        char scriptJoga[10240];
        sprintf(scriptJoga, QUERY, SCRIPT, del_cartas(e.selecao, e.mao[0]), e.mao[1], e.mao[2], e.mao[3], contaUns(del_cartas(e.selecao, e.mao[0])), e.tamanho[1], e.tamanho[2], e.tamanho[3],(unsigned long long) 0,e.selecao,0,'j',0,e.bots[0],e.bots[1],e.bots[2]);
        printf("<a xlink:href = \"%s\"><image x=\"590\" y=\"700\" height=\"40\" width=\"206\" xlink:href = http://127.0.0.1/cards/Jogar.png /></a>\n", scriptJoga);
    }
    if (e.selecao == 0){
    	char scriptHint[10240];
    	sprintf(scriptHint,QUERY,SCRIPT,e.mao[0],e.mao[1], e.mao[2], e.mao[3], e.tamanho[0], e.tamanho[1], e.tamanho[2], e.tamanho[3],hint(e.selecao,e.uJogada,e.mao[0]),e.uJogada,e.uJogador,e.acao,0,e.bots[0],e.bots[1],e.bots[2]);
        printf("<a xlink:href = \"%s\"><image x=\"590\" y=\"750\" height=\"40\" width=\"206\" xlink:href = http://127.0.0.1/cards/hint.png /></a>\n", scriptHint);
    }
}

/** \brief Imprime o html correspondente a uma carta

@param path	o URL correspondente à pasta que contém todas as cartas
@param x A coordenada x da carta
@param y A coordenada y da carta
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/
//@param jogador  ??  

void imprime_carta(char const *path, int x, int y, ESTADO e, int naipe, int valor, int m) {
	char const *suit = NAIPES;
	char const *rank = VALORES;
	char script[10240];
    if (m==0) {
        sprintf(script, QUERY, SCRIPT, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.tamanho[0], e.tamanho[1], e.tamanho[2], e.tamanho[3],sel_carta(e, naipe, valor),e.uJogada,e.uJogador,e.acao,e.nPass,e.bots[0],e.bots[1],e.bots[2]);
    	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, path, rank[valor], suit[naipe]);
    }
    else if (m==7) {
    	printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", x, y, path, rank[valor], suit[naipe]);
    }
    else {
    	printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/back.png\" /></a>\n", x, y, path);
    }

}

/**

*/
void imprime(char const *path, ESTADO e, int w) {
	int n, v, m;
	int x, y;
    int X[7]={220,680,220,40,500,360,150};
    int Y[7]={660,200,20,200,340,160,340};
	printf("<svg height = \"800\" width = \"800\">\n");
	printf("<rect x = \"0\" y = \"0\" height = \"800\" width = \"800\" style = \"fill:#007700\"/>\n");

    for (m=0; m<4;m++) {
        x=X[m],y=Y[m];
	    for(v = 0; v < 13; v++) {
		    for(n = 0; n < 4; n++) {
			    if(carta_existe(e.mao[m], n, v)) {
                    if(m%2==0) x+=20;
                    else y+=20;
                    if (carta_existe(e.selecao,n,v)) imprime_carta (path, x, y-20, e, n, v,m);
                    else imprime_carta(path,x,y,e,n,v,m);
                }
		    }
        }
    }
    
    imprime_botoes(e);
    
    for(m=0;m<3;m++) {
        x=X[4+m],y=Y[4+m];
        if (e.bots[m]!=0) {
            for (v=0;v<13;v++) {
                for (n=0;n<4;n++) {
                    if (carta_existe(e.bots[m],n,v)) {
                        imprime_carta(path,x,y,e,n,v,7);
                        x+=20;
                    }
                }
            }
        }
        else if (e.tamanho[0]!=13) {
            printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/pass.png\" /></a>\n", x, y, path);
        }
    }
    win(w);
    printf("</svg>\n");
}

void win (int winner) {
    if (winner!=4) {
        if (winner==0) printf("<image x = \"0\" y = \"0\" height = \"800\" width = \"800\" xlink:href = \"%s/winner.gif\" /></a>\n", BARALHO);
        if (winner>0 && winner<4) printf("<image x = \"0\" y = \"0\" height = \"800\" width = \"800\" xlink:href = \"%s/loser.gif\" /></a>\n", BARALHO);
        printf("<a xlink:href = \"%s\"><image x=\"400\" y=\"650\" height=\"80\" width=\"412\" xlink:href = \"%s/again.png\" /></a>\n", SCRIPT, BARALHO);
    }
}

void baralha (long long unsigned int* m, int* n) {
    int naipe,val,r;
    srandom(time(NULL));
    for (naipe=0;naipe<4;naipe++) {
        for (val=0;val<13;val++) {
            r=random() %4;
            if (n[r] < 13) {
                m[r]=add_carta (m[r],naipe,val);
                n[r]++;
            } else val--;
        }
    }
}

int proxJogador (int uj){
	if (uj==3) return 0;
	else return (uj+1);
}

long long unsigned int hint (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao){
int numCartas;
numCartas = contaUns(uJogada);
    switch(numCartas) {
        case 1:
            return (ajudaUma (sel,uJogada,mao));
            break;
        case 2:
            return (ajudaDuas (sel,uJogada,mao));
            break;
        case 3:
            return (ajudaTres (sel,uJogada,mao));
            break;
        default:
            return (ajudaMJogada (sel,mao));

      }
}

long long unsigned int ajudaUma (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao){
   int i;
   int ultcarta = indiNumCarta(uJogada,1);
   for (i=ultcarta+1;!carta_existe_indice(mao,i) && i<52;i++);
   if (i==52) return 0;
   else {sel = add_carta_indice (sel,i);
         return sel;}
}

long long unsigned int ajudaDuas (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao){
   int naipe;        
   int ultMCarta = indiNumCarta(uJogada,2); //indice da carta mais alta
   int nm = naipe_indice (ultMCarta);
   int vm = valor_indice (ultMCarta);
   if (nm!=3 && numCartasValor(mao,vm)==2){
       for(naipe = 0; naipe<4 ;naipe++)
          if (carta_existe(mao,naipe,vm)) sel = add_carta(sel,naipe,vm);
       return sel;}
   else {
     int v;
     for(v=vm+1;v<13 && numCartasValor(mao,v)<2;v++);
     if (v==13) return 0;
     else {
        int i=0; //vai contar o numero de cartas ja na selecao
        for (naipe = 0;naipe<4 && i<2;naipe++)
           if (carta_existe(mao,naipe,v)) {sel = add_carta(sel,naipe,v);i++;}
         return sel;
     }
   	}
}

long long unsigned int ajudaTres (long long unsigned int sel, long long unsigned int uJogada, long long unsigned int mao){
   int ultMCarta = indiNumCarta(uJogada,3); //indice da carta mais alta
   int vm = valor_indice (ultMCarta);
   int v;
   for(v=vm+1;v<13 && numCartasValor(mao,v)<3;v++);
   if (v==13) return 0;
   else {
       int i=0; //vai contar o numero de cartas ja na selecao
      int naipe;
       for (naipe = 0;naipe<4 && i<3;naipe++)
           if (carta_existe(mao,naipe,v)) {sel = add_carta(sel,naipe,v);i++;}
       return sel;
   	}
}

long long unsigned int ajudaMJogada (long long unsigned int sel, long long unsigned int mao){
    if (combinacao3(mao)!=0) {
      int val = combinacao3(mao);
      int naipe;
      int i=0;
      for (naipe = 0;naipe<4 && i<3;naipe++)
         if (carta_existe(mao,naipe,val)) {sel = add_carta(sel,naipe,val);i++;}
     } 
    else if (combinacao2(mao)!=0){
      int i=0; //vai contar o numero de cartas ja na selecao
      int naipe;
      int val = combinacao2 (mao);
      for (naipe = 0;naipe<4 && i<2;naipe++)
         if (carta_existe(mao,naipe,val)) {sel = add_carta(sel,naipe,val);i++;}
    }
    else {
        int i;
        for (i=0;i<52 && !carta_existe_indice(mao,i);i++);
        sel = add_carta_indice (sel,i);
    }
    return sel;	
}

void jogar(long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int* pass,long long unsigned int* bots) {
    int numCartas;
    while (*ujogador!=3) {
        if (*pass==3) {*ujogada=0;*pass=0;numCartas=0;}
        numCartas = contaUns(*ujogada);
        *sel = 0;
        switch(numCartas) {
            case 1:
                jogaUma(mao, t, sel, ujogada, ujogador, c,pass, bots);
                break;
            case 2:
                jogaDuas(mao, t, sel, ujogada, ujogador, c,pass, bots);
                break;
            case 3:
                jogaTres(mao,t,sel,ujogada,ujogador,c,pass,bots);
                break;
            default:
                jogaMjogada(mao, t, sel, ujogada, ujogador, c,pass, bots);
        }
    }
}

void jogaMjogada (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int* pass, long long unsigned int* bots){
    int jogA = proxJogador(*ujogador);
    long long unsigned int maoA = mao[jogA];
    if (combinacao3(maoA)!=0) {
        int val = combinacao3(maoA);
        int naipe;
        int i=0;
        for (naipe = 0;naipe<4 && i<3;naipe++)
            if (carta_existe(maoA,naipe,val)) {*sel = add_carta(*sel,naipe,val);i++;}
        mao[jogA] = del_cartas(*sel,mao[jogA]);
        *ujogada = *sel;
        *sel = 0;
        t[jogA]-=3;
        *ujogador = jogA;
        *c = 'j';
        *pass = 0;
        bots[jogA-1] = *ujogada;
    }
    else if (combinacao2(maoA)!=0){
        int i=0; //vai contar o numero de cartas ja na selecao
        int naipe;
        int val = combinacao2 (maoA);
            for (naipe = 0;naipe<4 && i<2;naipe++)
                if (carta_existe(maoA,naipe,val)) {*sel = add_carta(*sel,naipe,val);i++;}
                mao[jogA] = del_cartas(*sel,mao[jogA]);
                *ujogada = *sel;
                *sel = 0;
                t[jogA]-=2;
                *ujogador = jogA;
                *c = 'j';
                *pass = 0;
                bots[jogA-1] = *ujogada;
    }
    else {
        int i;
        for (i=0;i<52 && !carta_existe_indice(maoA,i);i++);
        *sel = add_carta_indice (*sel,i);
        mao[jogA] = rem_carta_indice(mao[jogA],i);
        t[jogA]--;
        *ujogada = *sel;
        *sel = 0;
        *ujogador = jogA;
        *c = 'j';
        *pass = 0;
        bots[jogA-1] = *ujogada;
    }
}

void jogaUma (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int* pass, long long unsigned int* bots) {
    int i;
    int jogA = proxJogador(*ujogador);
    long long unsigned int maoA = mao[jogA];
    int ultcarta = indiNumCarta(*ujogada,1);
    for (i=ultcarta+1;!carta_existe_indice(maoA,i) && i<52;i++);
    if (i==52) {
        *c = 'p';*ujogador = jogA;(*pass)++;
        bots[jogA-1] = 0;
    }
    else {
        *sel = add_carta_indice (*sel,i);
        mao[jogA] = rem_carta_indice(mao[jogA],i);
        t[jogA]--;
        *ujogada = *sel;
        *sel = 0;
        *ujogador = jogA;
        *c = 'j';
        *pass = 0;
        bots[jogA-1] = *ujogada;
    }
}


void jogaDuas (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int*pass,long long unsigned int* bots) {
    int naipe;
    int jogA = proxJogador(*ujogador);
    long long unsigned int maoA = mao[jogA];
    int ultMCarta = indiNumCarta(*ujogada,2); //indice da carta mais alta
    int nm = naipe_indice (ultMCarta);
    int vm = valor_indice (ultMCarta);
    if (nm!=3 && numCartasValor(maoA,vm)==2){
        for(naipe = 0; naipe<4 ;naipe++)
        if (carta_existe(maoA,naipe,vm)) *sel = add_carta(*sel,naipe,vm);
        mao[jogA] = del_cartas(*sel,mao[jogA]);
        *ujogada = *sel;
        *sel = 0;
        t[jogA]-=2;
        *ujogador = jogA;
        *c = 'j';
        *pass = 0;
        bots[jogA-1] = *ujogada;
    }
    else {
        int v;
        for(v=vm+1;v<13 && numCartasValor(maoA,v)<2;v++);
   	    if (v==13) {
            *c = 'p';*ujogador = jogA;(*pass)++;
            bots[jogA-1] = 0;
        }
   	    else {
            int i=0; //vai contar o numero de cartas ja na selecao
            for (naipe = 0;naipe<4 && i<2;naipe++)
                if (carta_existe(maoA,naipe,v)) {*sel = add_carta(*sel,naipe,v);i++;}
                mao[jogA] = del_cartas(*sel,mao[jogA]);
                *ujogada = *sel;
                *sel = 0;
                t[jogA]-=2;
                *ujogador = jogA;
                *c = 'j';
                *pass = 0;
                bots[jogA-1] = *ujogada;
            }
        }
}

void jogaTres (long long unsigned int *mao, int* t, long long unsigned int* sel,long long unsigned int* ujogada, int* ujogador ,char* c,int*pass,long long unsigned int* bots) {
    int jogA = proxJogador(*ujogador);
    long long unsigned int maoA = mao[jogA];
    int ultMCarta = indiNumCarta(*ujogada,3); //indice da carta mais alta
    int vm = valor_indice (ultMCarta);
    int v;
    for(v=vm+1;v<13 && numCartasValor(maoA,v)<3;v++);
    if (v==13) {
        *c = 'p';*ujogador = jogA;(*pass)++;
        bots[jogA-1] = 0;
    }
    else {
        int i=0; //vai contar o numero de cartas ja na selecao
        int naipe;
        for (naipe = 0;naipe<4 && i<3;naipe++)
            if (carta_existe(maoA,naipe,v)) {*sel = add_carta(*sel,naipe,v);i++;}
        mao[jogA] = del_cartas(*sel,mao[jogA]);
        *ujogada = *sel;
        *sel = 0;
        t[jogA]-=3;
        *ujogador = jogA;
        *c = 'j';
        *pass=0;
        bots[jogA-1] = *ujogada;
    }
}

int combinacao3 (long long unsigned int mao){
  int val;
  for (val=0;val<13 && numCartasValor(mao,val) < 3;val++);
  if (val==13) return 0;
  else return val;
}

int combinacao2 (long long unsigned int mao){
  int val;
  for (val=0;val<13 && numCartasValor(mao,val) < 2;val++);
  if (val==13) return 0;
  else return val;
}

/** Dado uma mao da-nos o indice do elemento n pertencente à mão(o primeiro elemento é o 1;)*/
int indiNumCarta (long long unsigned int cartas,int n){
  int i=0;
  while(n>0 && cartas>0){
    while(cartas%2 == 0) {cartas/=2;i++;}
    n--;
    if (n==0) return i;
    else {cartas/=2;i++;}
  }
  return -1;
}

int numCartasValor (long long unsigned int mao,int valor){
  int naipe;
  int num = 0;
  for (naipe = 0; naipe<4; naipe++){
    if (carta_existe(mao,naipe,valor)) num++;
  }
  return num;
}

int valor_indice (int ind){
 return (ind/4);
}

int naipe_indice (int ind) {
 return (ind%4);
}

int validaJogada(ESTADO e) {
    int numCartas=contaUns(e.uJogada);
    switch (numCartas) {
        case 1:
            if ((contaUns(e.selecao) == 1) && (e.selecao>e.uJogada)) return 1;
            else return 0;
            break;
        case 2:
            if((contaUns(e.selecao) == 2) && (e.selecao>e.uJogada) && mesmoNum(e.selecao)) return 1;
            else return 0;
            break;
        case 3:
            if((contaUns(e.selecao) == 3) && (e.selecao>e.uJogada) && mesmoNum(e.selecao)) return 1;
            else return 0;
            break;
        case 4:
            return 0;
            break;
        default:
            if (contaUns(e.selecao)==1) return 1;
            else if (mesmoNum(e.selecao)) return 1;
            else return 0;
    }
}

int mesmoNum(long long unsigned int cartas) {
    int x,i;
    for(i=0;cartas>0 && (cartas%2!=1);i++,cartas/=2);
    x=i/4;
    for(;cartas>0;i++,cartas/=2) if (cartas%2==1 && i/4!=x) return 0;
    return 1;
}
