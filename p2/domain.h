#define ADD1		0
#define ADD10		1
#define ADD100		2
#define SUB1		3
#define SUB10		4
#define SUB100		5

#define NUM_OPERATORS 6

enum digit {
	NOT_MODIFY, /* Para el inicio, cuando aún no se a modificado ninguno */
	UNIT, /* unidad */
	TEN, /* decena */
	HUNDRED /* centena */
};

/* Estado en el que permanecemos */
typedef struct tState_ {
  int number; /* numero en el que estamos */
  int modify; /* guardamos en que cifra a sido la última modificación */
} tState;
