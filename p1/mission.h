#define MIS 0
#define CAN 1
#define LEFT 0
#define RIGHT 1

#define M0C1 0
#define M1C0 1
#define M1C1 2
#define M2C0 3
#define M0C2 4
#define NUM_OPERATORS 5

typedef struct tState_ {
  int num[2][2];
  int boatbank;
} tState;
