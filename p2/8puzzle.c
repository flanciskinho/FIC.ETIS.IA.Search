#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "domain.h"
#include "search.h"


static int initcells[N][N] = 
{
  {2,7,3},
  {4,1,6},
  {8,0,5}
};

/* estado objetivo si la suma es par */
static int goalcells_pair[N][N] = 
{
  {1,2,3},
  {4,5,6},
  {7,8,0}
};

/* estado objetivo si la suma resulta impar */
static int goalcells_odd[N][N] = 
{
  {1,2,3},
  {8,0,4},
  {7,6,5}
};

/* Dependiendo del problema se podrá llegar a un estado final u a otro */
static int goalcells[N][N];

static int h0(tSearchNode *n) ;/* SIEMPRE DEVUELVE CERO */
static int h1(tSearchNode *n) ;/* NUMERO DE CIFRAS MAL COLOCADAS */
static int h2(tSearchNode *n) ;/* DISTANCIA MANHATAN */

int h(tSearchNode *n) {
	return h1(n); 
}


static tState *getState(int cells[N][N]) {
  tState *s = (tState *)malloc(sizeof(tState));
  int i,j,c;
  for (i=0;i<N;i++)
    for (j=0;j<N;j++) {
      c=s->cells[i][j]=cells[i][j];/* c es el valor que tiene el tablero en la posición (i,j) */
      s->row[c]=i; /* Guardamos la fila en la que está la posicion c en el tablero */
      s->col[c]=j; /* lo mismo pero con la columna */
    }
  return s;
}

/* Funcion auxiliar para determinar la paridad del 8puzzle */
static int counting( int *array, int index) {
	if (array[index] == 0) /* casilla en blanco */
		return 0;	
	if (array[index] == 1) /* no los hay más pequeños que el uno */
		return 0;
		
	int i = index +1;
	int value = array[index];
	int cnt = 0;
	
	for (; i < N*N; i++) {
		if (array[i] == 0)/* Si es un cero es la casilla en blanco */
			continue;
		if (array[i] < value)/* Si es más pequeño... pues a contar */
			cnt++;
	}
			
	return cnt;
}

/* Funcion que determina en que conjunto esta el problema del 8puzzle (el de los pares o impares) */
static int what_parity(int cells[][N]) {
	int i, j, cnt;	
	int aux[N*N];
	
	/* transformo el array bidimensional en un array unidimensional */
	for (cnt = i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			aux[cnt++] = cells[i][j];
	
	/* voy mirando las casillas que despues de i son más pequeñas */
	cnt = 0;
	for (i = 0; i < N*N; i++)
		cnt += counting(aux, i);
		
	return (cnt % 2 == 0);
}

/* Copia un array de valores en el estado objetivo */
static void copy_goal_cells(int cells[][N]) {
	int i,j;
	for (i=0;i<N;i++)
		for (j=0;j<N;j++)
			goalcells[i][j] = cells[i][j];	
}

/* funcion que se encarga de darle valor a las celdas del estado objetivo */
static void create_goal_cells(int cells[][N]) {
	if (what_parity(cells))
		copy_goal_cells(goalcells_pair);
	else
		copy_goal_cells(goalcells_odd);
	
}

/* Es el que devuelve el estado inicial de las celdas */
tState *initialState() {
	/* porque no, aquí ponemos ya cual será el estado objetivo que tendrá que alcanzar el problema */
	create_goal_cells(initcells);
  return getState(initcells);
}

int goalTest(tState *s) {
  int i,j;
  
  for (i=0;i<N;i++)
    for (j=0;j<N;j++)
      if (s->cells[i][j]!=goalcells[i][j]) return 0;
  return 1;
}

int executable(unsigned op,tState *s) {
  switch(op) {
    case UP:    return s->row[0]>0;
    case DOWN:  return s->row[0]<2;
    case LEFT:  return s->col[0]>0;
    case RIGHT: return s->col[0]<2;
  }
}

tState *successorState(unsigned op,tState *old) {
  int tile;
  tState *result = (tState *)malloc(sizeof(tState));
  
  /* Copy result from old */
  memcpy(result,old,sizeof(tState));
 
  switch(op) {
    case UP: result->row[0]--; break;
    case DOWN: result->row[0]++; break;
    case LEFT: result->col[0]--; break;
    case RIGHT: result->col[0]++; break;
  }
  
  /* Get the moved tile */
  tile=old->cells[result->row[0]][result->col[0]];
  
  /* Put a 0 in the new position of the hole */
  result->cells[result->row[0]][result->col[0]]=0;
  
  /* Put the tile in the old position of the hole */
  result->cells[old->row[0]][old->col[0]]=tile;
  
  /* Update row and col for the tile */
  result->row[tile]=old->row[0];
  result->col[tile]=old->col[0];
  
  return result;
}

int cost(unsigned op,tState *s) {
  return 1;
}

static tState *goal = NULL;

/* valor cero */
static int h0(tSearchNode *n) {
	return 0;	
}

/* Numero de celdas mal posicionadas */
static int h1(tSearchNode *n) {
	int cnt=0;
	int c;
	tState *s=n->state;
	
	if (goal==NULL) goal=getState(goalcells);
	
	for (c=1;c<N*N;c++)
		if (s->row[c] != goal->row[c] || s->col[c] != goal->col[c])
		cnt++;
		
	return cnt;
}

/* Distancia manhatan */
static int h2(tSearchNode *n) {
  int sum=0,c;
  tState *s=n->state;

  if (goal==NULL) goal=getState(goalcells);
  
  for (c=1;c<N*N;c++)
    sum += abs(s->row[c]-goal->row[c]) + abs(s->col[c]-goal->col[c]);
    
  return sum;
}

int stateEqual(tState *s1,tState *s2) {
  return (memcmp(s1,s2,sizeof(tState))==0);
}

/*--- DISPLAY FUNCTIONS -----------------------------------------------*/

/*
void showState(tState *s) {
  int i,j;
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++)
      printf("%d  ",s->cells[i][j]);
    printf("\n");
  }
}
*/

void showState(tState *s) {
  int i,j;
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++)
      printf("%d",s->cells[i][j]);
    printf(";");
  }
}


void showOperator(unsigned op) {
  switch(op) {
    case UP:    printf("UP");break;
    case DOWN:  printf("DOWN");break;
    case LEFT:  printf("LEFT");break;
    case RIGHT: printf("RIGHT");break;
  }
}
