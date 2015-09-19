#include <stdio.h>
#include <string.h>
#include "domain.h"
#include "search.h"

int incr[NUM_OPERATORS][2] = {
  {0,1},
  {1,0},
  {1,1},
  {0,2},
  {2,0}
};

tState *initialState() {
  tState *s = (tState *)malloc(sizeof(tState));
  s->num[MIS][LEFT]=3;
  s->num[CAN][LEFT]=3;
  s->num[MIS][RIGHT]=0;
  s->num[CAN][RIGHT]=0;
  s->boatbank=LEFT;
  return s;
}

int goalTest(tState *s) {
  return s->num[MIS][RIGHT]==3 && s->num[CAN][RIGHT]==3;
}

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

int cost(unsigned op,tState *s) {
  return 1;
}

int stateEqual(tState *s1,tState *s2) {
  int i,j;
  
  if (memcmp(s1,s2,sizeof(tState))==0) return 1;
  return 0;
}


/* We remove the constraint of missionaries outnumbered by cannibals */
/* We maintain that the boat must be moved by 1 or 2 persons         */

int h(tSearchNode *n) {
  int nleft = n->state->num[MIS][LEFT] +
              n->state->num[CAN][LEFT];
  int initmovs=0;	      

  if (nleft==0) return 0;
  
  /* If boat at right, we start moving one person to the left */
  /* Moving 2 persons would make things worse                 */
  if (n->state->boatbank==RIGHT) {
    initmovs=1; nleft++;
  }  
  
  /* If 2 persons at LEFT, we just make one movement to the RIGHT */  
  if (nleft<=2) return initmovs + 1;
  
  /* If more than 2 persons, we use one of them to transport nleft-2 */
  /* in 2 movements each one. The last 2 persons go together and     */
  /* need not to get back to the left                                */
  return initmovs + (nleft-2)*2 + 1;  
}

/*--- DISPLAY FUNCTIONS -----------------------------------------------*/

void showState(tState *s) {
  int i;
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
/*    printf("%d%d %d%d %c\n",
    s->num[MIS][LEFT],
    s->num[CAN][LEFT],
    s->num[MIS][RIGHT],
    s->num[CAN][RIGHT],
    (s->boatbank==LEFT)?'L':'R');*/
}

void showOperator(unsigned op) {
  printf("M%dC%d",incr[op][MIS],incr[op][CAN]);
}
