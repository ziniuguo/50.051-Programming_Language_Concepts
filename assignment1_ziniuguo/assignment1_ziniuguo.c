
#include "assignment1_ziniuguo.h"

int getRandom(int lower, int upper) {
    int num = (rand() %
               (upper - lower + 1)) + lower;
    return num;
}

int get_boolean_value(const char *booleans, int index) {
    return (*booleans >> (7 - index)) & 1 ? 1 : 0;
}

void update_boolean_value(char *booleans, int index, int val) {
    if (val != 0 && val != 1) {
        val = 1;

    }
    if (val == 0) {
        (*booleans) = (*booleans) & ~(1 << (7 - index));
    } else {
        (*booleans) = (*booleans) | (1 << (7 - index));
    }
}

void print_boolean(char *booleans, int index) {
    printf("boolean %d: %d\n",
           index, get_boolean_value(booleans, index));
}

void print_all_bits(char *booleans) {
    int i;
    for (i = 0; i <= 7; i++) {
        printf("%d", get_boolean_value(booleans, i));
    }
    printf("\n");
}


char *print_class(player *p) {
    char *infoBooleans = p->info;
    int id1 = (get_boolean_value(infoBooleans, 6));
    int id2 = (get_boolean_value(infoBooleans, 7));
    if (get_boolean_value(infoBooleans, 4) == 1) {
        if (id1 == 0 && id2 == 0) {
            return "Warrior";
        } else if (id1 == 0) {
            return "Rogue";
        } else if (id2 == 0) {
            return "Monk";
        } else {
            return "Barbarian";
        }
    }
    if (get_boolean_value(infoBooleans, 4) == 0) {
        /* index 5 is unused! */
        if (id1 == 0 && id2 == 0) {
            return "Mage";
        } else if (id1 == 0) {
            return "Priest";
        } else if (id2 == 0) {
            return "Druid";
        } else {
            return "Paladin";
        }
    }
    return "error";
}

void printPlayerInfo(player *p) {
    char *infoBooleans = p->info;
    printf("name: %s\n", p->name);
    printf("HP: %d\n", p->hp);
    printf("attack: %d\n", p->attack);
    printf("defense: %d\n", p->defense);
    print_all_bits(infoBooleans);
    printf("status: ");
    if (get_boolean_value(infoBooleans, 0) == 1) {
        printf("Poisoned ");
    }
    if (get_boolean_value(infoBooleans, 1) == 1) {
        printf("Stunned ");
    }
    if (get_boolean_value(infoBooleans, 2) == 1) {
        printf("Silent ");
    }
    if (get_boolean_value(infoBooleans, 3) == 1) {
        printf("Blinded ");
    }
    printf("\n");
    printf("class: %s\n\n", print_class(p));
}

void printMonsterInfo(monster *m) {
    if (m == NULL) {
        printf("No such monster exists...\n");
        return;
    }
    printf("id: %d\n", m->id);
    printf("hp: %d\n", m->hp);
    printf("attack: %d\n", m->attack);
    printf("defense: %d\n\n", m->defense);
}

void attackMonsterHelper(player *p, monster *m) {
    char *infoBooleans = p->info;
    int atk = p->attack;
    int rnd = getRandom(1, 10);
    int def = m->defense;
    int dmg = atk + rnd - def;
    if (get_boolean_value(infoBooleans, 1) == 1) {
        /* player is stunned */
        printf("this player is stunned and not able to attack...\n");
        return;
    }

    if (get_boolean_value(p->info, 4) == 1) {
        /* physical */
        if (get_boolean_value(infoBooleans, 3) == 1) {
            /* blinded */
            if (rand() % 2 == 0) {
                printf("this physical player is blinded and unluckily not able to attack...\n");
                /* 50 percent chance */
                return;
            }
        }
    }
    if (get_boolean_value(p->info, 4) == 0) {
        /* magical */
        if (get_boolean_value(infoBooleans, 2) == 1) {
            /* silenced */
            if (rand() % 2 == 0) {
                /* 50 percent chance */
                printf("this magical player is silenced and unluckily not able to attack...\n");
                return;
            }
        }
    }

    printf("dmg caused to monster: %d\n", dmg);
    printf("curr monster's HP: %d -> ", m->hp);
    if (dmg > 0) {
        m->hp -= dmg;
    }
    printf("%d\n\n", m->hp);
}

int attackPlayerHelper(player *p, monster *m) {
    int debuff_index;
    int def = p->defense;
    int rnd = getRandom(1, 5);
    int atk = m->attack;
    int dmg = atk + rnd - def;
    if (get_boolean_value(p->info, 0) == 1) {
        dmg = dmg * 1.25;
    }
    printf("dmg caused to player: %d\n", dmg);
    printf("curr Player's HP: %d\n", p->hp);
    if (getRandom(1, 4) != 1) {
        /* 25 percent chance attack */
        if (dmg > 0) {
            p->hp -= dmg;
        }
        if (getRandom(5, 8) == 8) {
            /* 25 percent chance debuff player*/
            debuff_index = getRandom(9, 12);
            if (debuff_index == 9) {
                p->poison_counter = 3;
                update_boolean_value(p->info, 0, 1);
                printf("player is poisoned!\n");
            } else if (debuff_index == 10) {
                p->stun_counter = 3;
                update_boolean_value(p->info, 1, 1);
                printf("player is stunned!\n");
            } else if (debuff_index == 11) {
                p->silence_counter = 3;
                update_boolean_value(p->info, 2, 1);
                printf("player is silenced!\n");
            } else {
                p->blind_counter = 3;
                update_boolean_value(p->info, 3, 1);
                printf("player is blinded!\n");

            }
        }
    }
    printf("hp after attacked: %d\n\n", p->hp);


    return 0;
}

player *newPlayer(char *name) {
    /* no debuff at the beginning! */
    int i;
    player *new_player = malloc(sizeof(player));
    new_player->name = name;
    new_player->attack = getRandom(30, 50);
    new_player->defense = getRandom(5, 10);
    new_player->hp = getRandom(1500, 2000);

    new_player->poison_counter = 0;
    new_player->stun_counter = 0;
    new_player->silence_counter = 0;
    new_player->blind_counter = 0;

    new_player->info = malloc(sizeof(char));
    *(new_player->info) = 8;
    for (i = 7; i >= 4; i--) {
        /* randomise new player */
        update_boolean_value(new_player->info, i, getRandom(0, 1));
    }

    /* link func */
    new_player->attackMonster = attackMonsterHelper;
    return new_player;
}

monster *newMonster() {
    monster *new_monster;
    new_monster = malloc(sizeof(monster));
    new_monster->hp = getRandom(4000, 5000);
    new_monster->attack = getRandom(20, 45);
    new_monster->defense = getRandom(5, 10);

    /* link func */
    new_monster->attackPlayer = attackPlayerHelper;
    return new_monster;
}

int main() {
    int p_idx;
    int m_idx;

    int max_monster_index = 2;
    int max_player_index = 3;
    int monster_i_to_attack;
    int player_i_to_attack;
    monster *currMonster;
    player *currPlayer;

    player *player0 = newPlayer("Lin Gao");
    player *player1 = newPlayer("Winnie the Ping");
    player *player2 = newPlayer("Chia-Yu the social credit grinder");
    player *player3 = newPlayer("LeeHsienLoong");
    player **players = malloc(sizeof(player *) * 10);


    monster *monster0 = newMonster();
    monster *monster1 = newMonster();
    monster *monster2 = newMonster();
    monster **monsters = malloc(sizeof(monster *) * 10);

    players[0] = player0;
    players[1] = player1;
    players[2] = player2;
    players[3] = player3;

    monsters[0] = monster0;
    monsters[1] = monster1;
    monsters[2] = monster2;

    monster0->id = 1004890;
    monster1->id = 1004891;
    monster2->id = 1001234;

    printf("initial players info\n");
    printPlayerInfo(player0);
    printPlayerInfo(player1);
    printPlayerInfo(player2);
    printPlayerInfo(player3);

    while (1) {
        printf("????????????????????? PLAYER'S ROUND\n");
        for (p_idx = 0; p_idx <= max_player_index; p_idx++) {
            if (max_monster_index < 0) {
                break;
            }
            monster_i_to_attack = getRandom(0, max_monster_index);
            currMonster = monsters[monster_i_to_attack];
            currPlayer = players[p_idx];
            printf("======= PLAYER %s attacking MONSTER %d =======\n", currPlayer->name, currMonster->id);
            currPlayer->attackMonster(currPlayer, currMonster);
            if (currMonster->hp <= 0) {
                printf("!!!!!!! MONSTER defeated id: %d !!!!!!!\n\n", currMonster->id);
                free(currMonster);
                while (monster_i_to_attack < max_monster_index) {
                    monsters[monster_i_to_attack] = monsters[monster_i_to_attack + 1];
                    monster_i_to_attack += 1;
                }
                max_monster_index -= 1;
            }
            /* clear status */
            if (currPlayer->poison_counter > 0) {
                currPlayer->poison_counter -= 1;
                if (currPlayer->poison_counter == 0) {
                    /* change bit */
                    update_boolean_value(currPlayer->info, 0, 0);
                    printf("poison expired!\n");
                }
            }
            if (currPlayer->stun_counter > 0) {
                currPlayer->stun_counter -= 1;
                if (currPlayer->stun_counter == 0) {
                    update_boolean_value(currPlayer->info, 1, 0);
                    printf("stun expired!\n");
                }
            }
            if (currPlayer->silence_counter > 0) {
                currPlayer->silence_counter -= 1;
                if (currPlayer->silence_counter == 0) {
                    update_boolean_value(currPlayer->info, 2, 0);
                    printf("silence expired!\n");
                }
            }
            if (currPlayer->blind_counter > 0) {
                currPlayer->blind_counter -= 1;
                if (currPlayer->blind_counter == 0) {
                    update_boolean_value(currPlayer->info, 3, 0);
                    printf("blind expired!\n");
                }
            }
        }
        if (max_monster_index < 0) {
            printf("PLAYERS WON!!!!!!!\n");
            break;
        }

        printf("????????????????????? MONSTER'S ROUND\n");
        for (m_idx = 0; m_idx <= max_monster_index; m_idx++) {
            if (max_player_index < 0) {
                break;
            }
            player_i_to_attack = getRandom(0, max_player_index);
            currPlayer = players[player_i_to_attack];
            currMonster = monsters[m_idx];
            printf("======= MONSTER %d attacking PLAYER %s =======\n", currMonster->id, currPlayer->name);
            currMonster->attackPlayer(currPlayer, currMonster);
            if (currPlayer->hp <= 0) {
                printf("!!!!!!! PLAYER eliminated name: %s !!!!!!!\n\n", currPlayer->name);
                free(currPlayer);
                while (player_i_to_attack < max_player_index) {
                    players[player_i_to_attack] = players[player_i_to_attack + 1];
                    player_i_to_attack += 1;
                }
                max_player_index -= 1;
            }
        }
        if (max_player_index < 0) {
            printf("MONSTERS WON!!!!!!!\n");
            break;
        }
    }

    free(players);
    free(monsters);
    return 0;
}