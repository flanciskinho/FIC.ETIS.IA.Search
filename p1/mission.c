#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mission.h"
#include "search.h"

/* Posibles movimientos con la barca
 * {x,y}   0<=x+y<=2
 * x -> numero de misioneros que van en la barca
 * y -> numero de canibales que van en la barca
 */
int incr[NUM_OPERATORS][2] = {
  {0,1},
  {1,0},
  {1,1},
  {0,2},
  {2,0}
};

/* Estado inicial de la barca */
/* Al principio están todos a la izquierda */
tState *initialState() {
  tState *s = (tState *)malloc(sizeof(tState));
  s->num[MIS][LEFT]=3;
  s->num[CAN][LEFT]=3;
  s->num[MIS][RIGHT]=0;
  s->num[CAN][RIGHT]=0;
  s->boatbank=LEFT;
  return s;
}

/* Estado que se quiere lograr (todos los individuos a la derecha) */
int goalTest(tState *s) {
  return s->num[MIS][RIGHT]==3 && s->num[CAN][RIGHT]==3;
}

/* Parte en la que en la que hay más caníbales que misioneros */
/* No se puede llegar a este estado */
static int outnumbered(tState *s,int bank) {
  return s->num[MIS][bank]>0 && s->num[CAN][bank] > s->num[MIS][bank];
}

int executable(unsigned op,tState *s) {
  tState *result=successorState(op,s);

  int ok= result->num[MIS][LEFT]>=0 &&
	  result->num[CAN][LEFT]>=0 &&
	  result->num[MIS][RIGHT]>=0 &&
	  result->num[CAN][RIGHT]>=0 &&
	  !outnumbered(result,LEFT) &&
	  !outnumbered(result,RIGHT);
  free(result);
  return ok;	  	  
}

/* Para crear el siguiente estado */
tState *successorState(unsigned op,tState *old) {
  tState *result = (tState *)malloc(sizeof(tState));
  
  /* Copy result from old */
  memcpy(result,old,sizeof(tState));
  
  result->boatbank = 1 - old->boatbank;
  result->num[MIS][old->boatbank] -= incr[op][MIS];
  result->num[CAN][old->boatbank] -= incr[op][CAN];
  result->num[MIS][result->boatbank] += incr[op][MIS];
  result->num[CAN][result->boatbank] += incr[op][CAN];

  return result;
}

/* El coste de todas las operaciones es uno */
int cost(unsigned op,tState *s) {
  return 1;
}

/* Comprueba si dos estados son iguales */
int stateEqual(tState *s1,tState *s2) {
  int i,j;
  
  if (memcmp(s1,s2,sizeof(tState))==0) return 1;
  return 0;
}


/*--- DISPLAY FUNCTIONS -----------------------------------------------*/

void showState(tState *s) {
  int i;
/*  
  for (i=1;i<=3;i++)
    printf("%c ",(i<=s->num[MIS][LEFT])?'m':'-');
  for (i=1;i<=3;i++)
    printf("%c ",(i<=s->num[CAN][LEFT])?'c':'-');
  
  if (s->boatbank==LEFT)
    printf(" \\__/~~~~~~~~~~~ ");
  else
    printf(" ~~~~~~~~~~~\\__/ ");
  for (i=1;i<=3;i++)
    printf("%c ",(i<=s->num[MIS][RIGHT])?'m':'-');
  for (i=1;i<=3;i++)
    printf("%c ",(i<=s->num[CAN][RIGHT])?'c':'-');
  printf("\n");
  */
    printf("%d%d %d%d %c\n",
    s->num[MIS][LEFT],
    s->num[CAN][LEFT],
    s->num[MIS][RIGHT],
    s->num[CAN][RIGHT],
    (s->boatbank==LEFT)?'L':'R');
}

void showOperator(unsigned op) {
  printf("M%dC%d",incr[op][MIS],incr[op][CAN]);
}
