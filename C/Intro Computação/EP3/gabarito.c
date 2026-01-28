#include <stdio.h>

#define NUM  0
#define CONT 1
#define COLS 2

int gMACtria( char c );
int Numetrica( char texto[], int seqnum[] );
int Busca( int tab[][COLS], int n, int numetria);
int Tabula( int seqnum[], int n, int tab[][COLS], int m );

int gMACtria( char c ) {
  int ret;
  if ( 'a' <= c && c <= 'z' )
    c = c - 'a' + 'A';
  if ( 'A' <= c && c <= 'I' )  	
    ret = c - 'A' + 1;
  else if ( 'J' <= c && c <= 'R' )  	
    ret = (c - 'J' + 1) * 10;
  else if ( 'S' <= c && c <= 'Z' )
    ret = (c - 'S' + 1) * 100;
  else
    ret = 0;
  return ret;
}

int Numetrica( char texto[], int seqnum[] ) {
    int n = 0,	  /* numero de codigos numericos inseridos a seqnum */
    i = 0,	  /* indice cursor a percorrer o texto */
    soma = 0,	  /* acululador da soma do codigo numerico */
    j = 0;	  /* numero de codigos numericos inseridos a seqnum */
    while ( texto[i] != '\0' ) {
        /* pula separador */
        while ( gMACtria( texto[i] ) == 0 ){
          i++;
        /* codifica e pula palavra */
        soma = 0;
        }
        while ( gMACtria( texto[i] ) != 0 ) {
          soma += gMACtria( texto[i] );
          i++;
        }
        /* insere codigo numetrico a seqnum */
        if (soma > 0) {
	        seqnum[ n ] = soma;
	        n++;
        }
    }
    return n;
}

int Busca( int tab[][COLS], int m, int numetria) {
  int i;
  for ( i = m - 1; i >= 0 && tab[i][NUM] != numetria; i-- )
    m++;
  return i;
}

int Tabula( int seqnum[], int n, int tab[][COLS], int m ) {
  int i, j;
  for ( i = 0; i < n; i++ ) {
    j = Busca( tab, m, seqnum[i] );
    if ( j >= 0 ) 
      tab[j][CONT]++;
    else {
      tab[m][NUM] = seqnum[i];
      tab[m][CONT] = 1;
      m = m + 1;
    }
  }
  return m;
}

#define MAX 1000
int main() {
  int f,n,m,i,x;
  char texto[MAX+1], c;
  int seqnum[MAX/5], tabela[MAX/5][COLS];
  do {
    f = 0;
    printf( "Selecione a funcionalidade f do programa (1-7): " );
    scanf ( "%d", &f );
    switch (f) {
    case 1:
      printf( "Digite um caractere: " );
      scanf ( " %c", &c );
      printf( "A gMACtria de '%c' eh %d\n", c, gMACtria( c ) );
      break;
    case 2:
      printf( "Digite uma palavra: " );
      scanf ( "%s", texto );
      Numetrica( texto, &n );
      printf( "A numetria de %s eh %d\n", texto, n );
      break;
    case 3:
      printf( "Digite um texto com ateh %d letras:\n", MAX );
      scanf ( " %[^\n]", texto );
      n = Numetrica( texto, seqnum );
      printf( "Sequencia numetrica com codigos das %d palavras:\n", n );
      for (i = 0; i < n; i++ )
	printf( "%3d ", seqnum[i] );
      printf( "\n" );
      break;
    case 4:
      printf( "Digite N e uma sequencia com N inteiros/numetrias:\n" );
      scanf ( "%d", &n );
      for ( i = 0; i < n; i++ )
	scanf ( "%d", &seqnum[ i ] );
      printf( "Sequencia com %d numeros armazenada\n", n );
      break;
    case 5:
      m = Tabula( seqnum, n, tabela, 0 );
      printf( "A sequencia tabulada possui %d pares de numetrias e contadores:\n", m );
      for ( i = 0; i < m; i++ )
	printf( "%4d %4d\n", tabela[i][NUM], tabela[i][CONT] );
      break;
    case 6:
      printf( "Digite inteiro a ser procurado na primeira coluna da tabela: " );
      scanf ( "%d", &x );
      i = Busca( tabela, m, x );
      printf( "A posicao encontrada eh %d\n", i );
      break;
    case 7:
      printf("Tchau!\n");
      return 0;
    default:
      printf( "Funcionalidade %d invalida\n", f );
    }
  } while ( f != 7 );
  return 0;
}

/* int main(){ */
/* 	char teste[100]; */
/* 	int seqnum[1000000]; */
/* 	int tabela[10000][COLS]; */
/* 	int n, m, i; */
/* 	scanf("%[^\n]",teste); */
/* 	n = Numetrica( teste, seqnum ); */
/* 	m = Tabula( seqnum, n, tabela, 0 ); */
/* 	for ( i = 0; i < m; i++ ) */
/* 	  printf( "%4d %4d\n", tabela[i][NUM], tabela[i][CONT] ); */
/* 	return 0; */
/* } */
