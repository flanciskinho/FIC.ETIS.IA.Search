#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define NUM_OPERATORS 4

typedef struct tState_ {
  int cells[3][3]; // Array con lo que tiene cada puzzle en cada casilla
  int r,c; // Donde se encuentra la posicion en blanco
} tState;
