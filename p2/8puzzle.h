#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define NUM_OPERATORS 4

#define N 3

typedef struct tState_ {
  int cells[N][N];
  int row[N*N],col[N*N];
} tState;
