#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "number.h"
#include "search.h"

#include <stdbool.h>

static int  init_number = 789;

static int goal_number = 243;

#define MAX_TABU 10
/* Lista de numeros tabu */
/* Se requiere que esten ordenados para que la busqueda dicotomica tenga efecto */
static int tabu [MAX_TABU] = {
		244,
		253,
		254,
		343,
		344,
		353,
		778,
		779,
		679,
		689
};

static int h0(tSearchNode *n) ;/* SIEMPRE DEVUELVE CERO */
static int h1(tSearchNode *n) ;/* NUMERO DE CIFRAS MAL COLOCADAS */
static int h2(tSearchNode *n) ;/* DISTANCIA MANHATAN */

int h(tSearchNode *n) {
	return h1(n); 
}

/* realiza una busqueda dicotomia para ver si existe el dato de entrada es un valor dentro del array de tabu */
static bool is_tabu(const int *array,const int length, const int value) {
	int lower, center, top;
	
	lower = 0;
	top = length-1;
//printf("%d{\n", value);	
	while (lower <= top) {
		center = ((int) (top - lower) /2) + lower;
//printf("%3d",center);
		if (array[center] == value)
			return true;
		
		if (value < array[center])
			top = center - 1;
		else
			lower = center + 1;
	}
//printf("}\n");
	return false;
}

/* Para crear el nodo en el que comenzamos */
tState *initialState() {
  tState *s = (tState *)malloc(sizeof(tState));
  if (s == NULL) {
  	perror("initialState, malloc");
  	exit(EXIT_FAILURE);
  }
  
  s->number = init_number;
  s->modify = NOT_MODIFY;
  
  return s;
}

/* Para comprobar si se llego a un estado objetivo */
int goalTest(tState *s) {
	return (s->number == goal_number);
}
static char * print_op(const unsigned op);
/* Para comprobar que operaciones se pueden ejecutar */
int executable(unsigned op,tState *s) {
	
	int aux;
	switch(op) {
		case ADD1:
			if ((s->modify == UNIT) || ((s->number % 10) >= 9)) /* La cifra ya ha sido modificada con anterioridad */
				return false;
			else
				aux = s->number +1;
			break;
		case ADD10:
			if ((s->modify == TEN) || ((s->number % 100) >= 90))
				return false;
			else
				aux = s->number +10;
			break;
		case ADD100:
			if ((s->modify == HUNDRED) || ((s->number % 1000) >= 900))
				return false;
			else
				aux = s->number +100;
			break;
		case SUB1:
			if ((s->modify == UNIT)  || ((s->number % 10) < 1))
				return false;
			else
				aux = s->number -1;
			break;
		case SUB10:
			if ((s->modify == TEN) || ((s->number % 100) < 10))
				return false;
			else
				aux = s->number -10;
			break;
		case SUB100:
			if ((s->modify == HUNDRED)  || ((s->number % 1000) < 100))
				return false;
			else
				aux = s->number -100;
			break;
		default :
			return false;
	}
	/* La operacion se podrá ejecutar si el numero no es un tabu */
	//printf("%3d (%s)-> %3d->%s\n", s->number, print_op(op), aux, is_tabu(tabu, MAX_TABU, aux)?"TABU":"OK");
	//return true;
	return !is_tabu(tabu, MAX_TABU, aux);
}

/* Realiza una operacion sobre el estado */
tState *successorState(unsigned op,tState *old) {
  tState *result = (tState *)malloc(sizeof(tState));
  
  if (result == NULL) {
  	perror("successorState, malloc");
  	exit(EXIT_FAILURE);
  }
  
  /* Copy result from old */
  memcpy(result,old,sizeof(tState));
 
  switch(op) {
    case ADD1:   result->number++;
    				  result->modify = UNIT;
    				  break;
    case ADD10:  result->number += 10;
    			 	  result->modify = TEN;
    				  break;
    case ADD100: result->number += 100;
    			     result->modify = HUNDRED;
    			     break;
    case SUB1:   result->number--;      
    				  result->modify = UNIT;
    				  break;
    case SUB10:  result->number = result->number - 10;
    				  result->modify = TEN;
    				  break;
    case SUB100: result->number = result->number - 100;
         		  result->modify = HUNDRED;
         		  break;
  }
  
  return result;
}

/* Lo que cuesta realizar una operacion */
int cost(unsigned op,tState *s) {
// Todas los movimientos tienen coste uno
  return 1;
}

//Comprueba si dos estados son iguales
int stateEqual(tState *s1,tState *s2) {
  return (memcmp(s1,s2,sizeof(tState))==0);
}

/*--- HEURÍSTICAS -----------------------------------------------*/

/* Numero de movimientos que quedan por hacer (si los movimientos no tienen restricciones) */
static int h2(tSearchNode *n) {
	int cnt = 0;
	int aux_number;
	int aux_goal;
	tState *s = n->state;
	
	//Miro las unidades;
	aux_number = s->number % 10;
	aux_goal = goal_number % 10;
	cnt += abs(aux_number - aux_goal);
	
	//Miro las decenas
	aux_number = s->number - aux_number;
	aux_goal = goal_number - aux_goal;
	aux_number = aux_number % 100;
	aux_goal = aux_goal % 100;
	cnt += abs(aux_number - aux_goal);
	
	//Miro las centenas
	aux_number = s->number / 100;
	aux_goal = goal_number / 100;
	cnt += abs(aux_number - aux_goal);
	
	return cnt;
}

/* Cantidad de cifras mal colocadas */
static int h1(tSearchNode *n) {
	int cnt = 0;
	int aux_number;
	int aux_goal;
	tState *s = n->state;
	
	//Miramos las unidades
	aux_number = s->number % 10;
	aux_goal = goal_number % 10;
	if (aux_number != aux_goal)
		cnt++;
		
	//Miro las decenas
	aux_number = s->number - aux_number;
	aux_goal = goal_number - aux_goal;
	aux_number = aux_number % 100;
	aux_goal = aux_goal % 100;
	if (aux_number != aux_goal)
		cnt++;
		
	//Miro las centenas
	aux_number = s->number / 100;
	aux_goal = goal_number / 100;
	if (aux_number != aux_goal)
		cnt++;
		
	return cnt;
}

/* valor cero */
static int h0(tSearchNode *n) {
	return 0;
}

/*--- DISPLAY FUNCTIONS -----------------------------------------------*/

static char *print_modify(const int modify) {
	switch (modify) {
		case NOT_MODIFY:
			return "NOT MODIFY";
		case UNIT:
			return "UNIT";
		case TEN:
			return "TEN";
		case HUNDRED:
			return "HUNDRED";
		
		default:
			return "UNKNOW";
	}
}

void showState(tState *s) {
  printf("%3d %s", s->number, print_modify(s->modify));
}

static char * print_op(const unsigned op) {
  switch(op) {
    case ADD1:        return "+  1";
    case ADD10:      return "+ 10";
    case ADD100:    return "+100";
    case SUB1:        return "-  1";
    case SUB10:      return "- 10";
    case SUB100:    return "-100";
    
    default :
    	return "UNKNOW";
  }
	
}

void showOperator(unsigned op) {
	printf("%s\n", print_op(op));
}
