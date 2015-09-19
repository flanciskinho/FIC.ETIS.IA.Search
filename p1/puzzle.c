#include "puzzle.h"
#include "search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
//Es la que dio el profesor como ejemplo
static int initcells[3][3] = 
{
  {1,3,0},
  {4,2,6},
  {7,5,8}
};
*/


static int initcells[3][3] = 
{
  {6,1,4},
  {3,5,8},
  {7,2,0}
};


/*Tambien hay que cambiar la posicion de la casilla blanca, de codigo 0,  en el codigo de la funcion initialState() */

/* estado objetivo si la suma resulta par */
static int goalcells_pair[3][3] = 
{
  {1,2,3},
  {4,5,6},
  {7,8,0}
};

/* estado objetivo si la suma resulta impar */
static int goalcells_odd[3][3] = 
{
  {1,2,3},
  {8,0,4},
  {7,6,5}
};

static int goalcells[3][3];

/* Funcion auxiliar para determinar la paridad del 8puzzle */
static int counting( int *array, int index) {
	if (array[index] == 0) /* casilla en blanco */
		return 0;	
	if (array[index] == 1) /* no los hay más pequeños que el uno */
		return 0;
		
	int i = index +1;
	int value = array[index];
	int cnt = 0;
	
	for (; i < 9; i++) {
		if (array[i] == 0)/* Si es un cero es la casilla en blanco */
			continue;
		if (array[i] < value)/* Si es más pequeño... pues a contar */
			cnt++;
	}
			
	return cnt;
}

/* Funcion que determina en que conjunto esta el problema del 8puzzle (el de los pares o impares) */
static int what_parity(int cells[][3]) {
	int i, j, cnt;	
	int aux[9];
	
	/* transformo el array bidimensional en un array unidimensional */
	for (cnt = i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			aux[cnt++] = cells[i][j];
	
	/* voy mirando las casillas que despues de i son más pequeñas */
	cnt = 0;
	for (i = 0; i < 9; i++)
		cnt += counting(aux, i);
		
	return (cnt % 2 == 0);
}

/* Copia un array de valores en el estado objetivo */
static void copy_goal_cells(int cells[][3]) {
	int i,j;
	for (i=0;i<3;i++)
		for (j=0;j<3;j++)
			goalcells[i][j] = cells[i][j];	
}

/* funcion que se encarga de darle valor a las celdas del estado objetivo */
static void create_goal_cells(int cells[][3]) {
	if (what_parity(cells))
		copy_goal_cells(goalcells_pair);
	else
		copy_goal_cells(goalcells_odd);
	
}

static void print_cells(int cells[][3]) {
  int i,j;
  printf("goalcells : \n");
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++)
      printf("%d  ",cells[i][j]);
    printf("\n");
  }
}

tState *initialState() {//inicializa las celdas a procesar con la disposicion inicial de las celdas en el vector initcells
  tState *s = (tState *)malloc(sizeof(tState));
  int i,j;
  for (i=0;i<3;i++)
    for (j=0;j<3;j++)
      s->cells[i][j]=initcells[i][j];
  //s->r=0; s->c=2;  /*posicion de la casilla blanca, de codigo 0, en el estado inicial*/
  
  /* buscar la posicion en blanco */
  for (i = 0; i < 3; i++)
  	for (j = 0; j < 3; j++)
  		if (s->cells[i][j] == 0) {
  			s->r = i; s->c=j;/* posicion en blanco */
  			i = 3; j = 3;
  		}
  
  create_goal_cells(initcells);
  
  //print_cells(goalcells);
  
  return s;
}

int goalTest(tState *s) {
  int i,j;
  
  for (i=0;i<3;i++)
    for (j=0;j<3;j++)
      if (s->cells[i][j]!=goalcells[i][j]) return 0;
  return 1;
}

int executable(unsigned op,tState *s) {
  switch(op) {
    case UP:    return s->r>0;
    case DOWN:  return s->r<2;
    case LEFT:  return s->c>0;
    case RIGHT: return s->c<2;
  }
}

tState *successorState(unsigned op,tState *old) {
  tState *result = (tState *)malloc(sizeof(tState));
  
  /* Copy result from old */
  memcpy(result,old,sizeof(tState));
 
  switch(op) {
    case UP: result->r--; break;
    case DOWN: result->r++; break;
    case LEFT: result->c--; break;
    case RIGHT: result->c++; break;
  }
  result->cells[result->r][result->c]=0;
  result->cells[old->r][old->c]=old->cells[result->r][result->c];
  
  return result;
}

int cost(unsigned op,tState *s) {
  return 1;
}

int stateEqual(tState *s1,tState *s2) {
  return (memcmp(s1,s2,sizeof(tState))==0);
}

/*--- DISPLAY FUNCTIONS -----------------------------------------------*/

void showState(tState *s) {
  int i,j;
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++)
      printf("%d  ",s->cells[i][j]);
    printf("\n");
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