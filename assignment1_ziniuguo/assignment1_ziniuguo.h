

#ifndef ASSIGNMENT1_ZINIUGUO_ASSIGNMENT1_ZINIUGUO_H
#define ASSIGNMENT1_ZINIUGUO_ASSIGNMENT1_ZINIUGUO_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getRandom(int lower, int upper);
int get_boolean_value(const char *booleans, int index);
void update_boolean_value(char *booleans, int index, int val);
void print_boolean(char *booleans, int index);
void print_all_bits(char *booleans);

typedef struct player player;
typedef struct monster monster;

struct player {
    char *name;
    int hp;
    int attack;
    int defense;
    char *info;
    int poison_counter;
    int stun_counter;
    int silence_counter;
    int blind_counter;

    void (*attackMonster)(player *, monster *);
};

struct monster {
    int id;
    int hp;
    int attack;
    int defense;

    int (*attackPlayer)(player *, monster *);
};

char *print_class(player *p);
void printPlayerInfo(player *p);
void printMonsterInfo(monster *m);
void attackMonsterHelper(player *p, monster *m);



#endif