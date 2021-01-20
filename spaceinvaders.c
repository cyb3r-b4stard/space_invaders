/*
 * Source: https://youtu.be/k6rTH77tML0
 */
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <windows.h>
#include <time.h>
const int sizey = 23;
const int sizex = 40;
// char world[sizey][sizex];
char world[23][40];
const char player = 'A';
const char playerLaser = '^';
const char enemy = 'M';
const char enemyShielded = 'O';
const char enemyLaser = 'U';
const char explosion = 'X';

char action();

int enemyWalkOnLaser(int y, int x) {
    int score = 0;
    if (world[y][x] == enemy) {world[y][x] = explosion; score += 50;}
    if (world[y][x] == enemyShielded) {world[y][x] = 'M'; score += 50;}
    return score;
}


int game() {
	int x, y, yi;
	int score = 0, bonus = 0;
	int victory = 1;
	int laserReady = 1;
	int enemyReady = 0;

	srand(time(NULL));
	/*welcome screen*/
	printf("\n \n     Welcome soldier! \n \n \n \n");
	Sleep(1000);
	printf(
			"  Brave the COMMAND PROMPT INVADERS and come back a hero. \n \n \n \n");
	Sleep(1000);
	printf("               Good luck.");
	Sleep(1000);
	printf("\n \n \n \n Press any key to start.");
	//getch();

	/*initialise world*/
	int totalEnemies = 0;
	for (x = 0; x < sizex; x++) {
		for (y = 0; y < sizey; y++) {
			if ((y + 1) % 2 == 0 && y < 7 && x > 4 && x < sizex - 5
					&& x % 2 == 0) {
				world[y][x] = enemy;
				totalEnemies++;
			} else if ((y + 1) % 2 == 0 && y >= 7 && y < 9 && x > 4
					&& x < sizex - 5 && x % 2 == 0) {
				world[y][x] = enemyShielded;
				totalEnemies = totalEnemies + 2;
			} else {
				world[y][x] = ' ';
			}
		}
	}
	world[sizey - 1][sizex / 2] = player;
	int i = 1;
	char direction = 'l';
	char keyPress;
	int currentEnemies = totalEnemies;
	while (currentEnemies > 0 && victory) {
		int drop = 0;
		int enemySpeed = 1 + 10 * currentEnemies / totalEnemies;
		laserReady++;

        /*laser time*/
		for (x = 0; x < sizex; x++) {
			for (y = sizey - 1; y >= 0; y--) {
				if (i % 2 == 0 && world[y][x] == enemyLaser
                               && (world[y + 1][x] != enemy
                               && world[y + 1][x] != enemyShielded
                               && world[y + 1][x] != playerLaser)) {
					world[y + 1][x] = enemyLaser;
					world[y][x] = ' ';
				} else if (i % 2 == 0 && world[y][x] == enemyLaser
						&& (world[y + 1][x] == enemy
								| world[y + 1][x] == enemyShielded
								| world[y + 1][x] == playerLaser)) {
					world[y][x] = ' ';
					if (world[y + 1][x] == playerLaser) world[y+1][x]=' ';
				}
			}
		}
		for (x = 0; x < sizex; x++) {
			for (y = 0; y < sizey; y++) {
				if ((i % 5) == 0
						&& (world[y][x] == enemyShielded | world[y][x] == enemy)
						&& (rand() % 15) > 13
						&& world[y + 1][x] != playerLaser) {
					for (yi = y + 1; yi < sizey; yi++) {
						if (world[yi][x] == enemy
								| world[yi][x] == enemyShielded) {
							enemyReady = 0;
							break;
						}
						enemyReady = 1;
					}
					if (enemyReady) {
						world[y + 1][x] = enemyLaser;
					}
				}
				if (world[y][x] == playerLaser && world[y - 1][x] == enemy) {
					world[y][x] = ' ';
					world[y - 1][x] = explosion;
					currentEnemies--;
					score = score + 50;
				} else if (world[y][x] == playerLaser
						&& world[y - 1][x] == enemyShielded) {
					world[y][x] = ' ';
					world[y - 1][x] = enemy;
					currentEnemies--;
					score = score + 50;
				} else if (world[y][x] == playerLaser
						&& world[y - 1][x] == enemyLaser) {
					world[y][x] = ' ';
					world[y - 1][x] = ' ';
				} else if (world[y][x] == explosion) {
					world[y][x] = ' ';
				} else if ((i + 1) % 2 == 0 && world[y][x] == enemyLaser
						&& world[y + 1][x] == player) {
					world[y + 1][x] = explosion;
					world[y][x] = ' ';
					victory = 0;
				} else if (world[y][x] == playerLaser
						&& world[y - 1][x] != enemyLaser) {
					world[y][x] = ' ';
					world[y - 1][x] = playerLaser;
				}
			}
		}

		/*update enemy direction*/
		for (y = 0; y < sizey; y++) {
			if (world[y][0] == enemy) {
				direction = 'r';
				drop = 1;
				break;
			}
			if (world[y][sizex - 1] == enemy) {
				direction = 'l';
				drop = 1;
				break;
			}
		}

		/*update board*/
		if (i % enemySpeed == 0) {
			if (direction == 'l') {
				for (x = 0; x < sizex - 1; x++) {
					for (y = 0; y < sizey; y++) {
						if (drop
								&& (world[y - 1][x + 1] == enemy
										|| world[y - 1][x + 1] == enemyShielded)) {
                            if (world[y][x] == playerLaser) {
                                score += enemyWalkOnLaser(y - 1, x + 1);
                                currentEnemies--;
                            }
							world[y][x] = world[y - 1][x + 1];
							world[y - 1][x + 1] = ' ';
						} else if (!drop
								&& (world[y][x + 1] == enemy
										|| world[y][x + 1] == enemyShielded)) {
                            if (world[y][x] == playerLaser) {
                                score += enemyWalkOnLaser(y, x + 1);
                                currentEnemies--;
                            }
							world[y][x] = world[y][x + 1];
							world[y][x + 1] = ' ';
						}
					}
				}
			} else {
				for (x = sizex; x > 0; x--) {
					for (y = 0; y < sizey; y++) {
						if (drop
								&& (world[y - 1][x - 1] == enemy
										|| world[y - 1][x - 1] == enemyShielded)) {
                            if (world[y][x] == playerLaser) {
                                score += enemyWalkOnLaser(y - 1, x - 1);
                                currentEnemies--;
                            }
							world[y][x] = world[y - 1][x - 1];
							world[y - 1][x - 1] = ' ';
						} else if (!drop
								&& (world[y][x - 1] == enemy
										|| world[y][x - 1] == enemyShielded)) {
                            if (world[y][x] == playerLaser) {
                                score += enemyWalkOnLaser(y, x - 1);
                                currentEnemies--;
                            }
							world[y][x] = world[y][x - 1];
							world[y][x - 1] = ' ';
						}
					}
				}
			}
			for (x = 0; x < sizex; x++) {
				if (world[sizey - 1][x] == enemy || world[sizey - 1][x] == enemyShielded) {
					victory = 0;
				}
			}
		}
		if (victory == 0) {
            break;
		}

        /*Check for win condition by Danil Davydyan. Because win-checks implemented just don't function properly*/
		int currentEnemies = 0;
        for (int i = 0; i < sizey; i++) {
            for (int j = 0; j < sizex; j++) {
                if (world[i][j] == enemy || world[i][j] == enemyShielded) {
                    currentEnemies++;
                }
            }
        }
        if (currentEnemies == 0) {
            victory = 1;
            break;
        }

		/*display world*/
		system("cls");
		printf("     SCORE:    %d", score);
		printf("\n");
		for (y = 0; y < sizey; y++) {
			printf("|");
			for (x = 0; x < sizex; x++) {
				printf("%c", world[y][x]);
			}
			printf("|");
			printf("\n");
		}
		keyPress = action();

        int causedSuicide = 0;

		if (keyPress == 'a') {
			for (x = 0; x < sizex - 1; x = x + 1) {
				if (world[sizey - 1][x + 1] == player) {
				    if (world[sizey - 1][x] == enemyLaser) {
                        causedSuicide = 1;
                        break;
                    }
					world[sizey - 1][x] = player;
					world[sizey - 1][x + 1] = ' ';
				}
			}
		}

		if (keyPress == 'd') {
			for (x = sizex - 1; x > 0; x = x - 1) {
				if (world[sizey - 1][x - 1] == player) {
				    if (world[sizey - 1][x] == enemyLaser) {
                        causedSuicide = 1;
                        break;
                    }
					world[sizey - 1][x] = player;
					world[sizey - 1][x - 1] = ' ';
				}
			}
		}
		if (keyPress == 'm' && laserReady > 2) {
			for (x = 0; x < sizex; x = x + 1) {
				if (world[sizey - 1][x] == player) {
					world[sizey - 2][x] = playerLaser;
					laserReady = 0;
				}
			}
		}
		if (causedSuicide == 1) {
            victory = 0;
            break;
		}
		i++;
		Sleep(100);
	}
	system("cls");
	printf("     SCORE:    %d", score);
	printf("\n");
	for (y = 0; y < sizey; y++) {
		printf("|");
		for (x = 0; x < sizex; x++) {
			printf("%c", world[y][x]);
		}
		printf("|");
		printf("\n");
	}
	Sleep(1000);
	system("cls");
	if (victory != 0) {
		printf(
				"\n \n \n \n \n \n               CONGRATULATIONS! \n \n \n \n \n");
		Sleep(1000);
		printf("\n \n               Score: %d", score);
		Sleep(1000);
		bonus = totalEnemies * 20 - i;
		printf("\n \n               Bonus: %d", bonus);
		Sleep(1000);
		printf("\n \n               Total Score: %d", score + bonus);
		printf("\n \n \n \n               Well done");
		Sleep(1000);
		printf(", Hero.");
		Sleep(1000);
	} else {
		printf("\n \n \n \n \n \n               You have failed.");
		Sleep(1000);
		printf("\n \n               Final Score: %d", score);
	}
	return score + bonus;
}

int main() {
	int new, score = 0, k = 0;
	for (int i = 0; i < 100; i++) {
		new = game();
		if (new <= 3750)
			k++;
		score += new;
	}
	printf("The average is: %d", score/100);
	printf("\nThe percentage of wins: %d", (100-k));
}


char action() {
	// The only information you can use for your decision is
	// the char world[23][40] variable,
	// which you should NOT update, but ONLY read.

	char keyPress = ' ';
	static char direction = 'r';
	static int laserReady = 1;
	static long int cycle = 1;
	int left = 0, right = 0, first = 1;
	int playerPosition, non_null, line, cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, found = 0;
	static int position;
	int arr[4];
	laserReady++;

	printf("%d", cycle);

	for (int x = 0; x < sizex; ++x) {
		for (int y = 0; y < sizey; ++y) {
			if ((world[y][x] == 'M' || world[y][x] == 'O') && first == 1) {
				left = x;
				first = 0;
			} else if ((world[y][x] == 'M' || world[y][x] == 'O') && first == 0) {
				right = x-3;
			}
		}
	}
	first = 1;


	if (world[sizey-1][right] == player) direction = 'l';
	if (world[sizey-1][left] == player) direction = 'r';

	if (cycle < 12) {
		if (direction == 'r') {
			if ((cycle % 12) == 1) {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && !(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) 
						&& world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
						keyPress = 'd';
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser) || (world[sizey-2][x] == enemyLaser))) {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} else {
							if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
								keyPress = 'd';
							else if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser&& x != left && world[sizey-1][x-1] != enemyLaser)
								keyPress = 'a';
						}
					} 
				}
			} else {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser) || (world[sizey-2][x] == enemyLaser))) {
						if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
							keyPress = 'd';
						else if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser&& x != left && world[sizey-1][x-1] != enemyLaser)
							keyPress = 'a';
						else {
							if (laserReady > 2) {
								keyPress = 'm';
								laserReady = 0;
							} 
						}
					}
					else {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} 
					}
				}
			}
		} else if (direction == 'l') {
			if ((cycle % 12) == 1) {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && !(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) 
						&& world[sizey-2][x-1] != enemyLaser && x != left && world[sizey-1][x-1] != enemyLaser)
						keyPress = 'a';
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser) || (world[sizey-2][x] == enemyLaser))) {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} else {
							if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
								keyPress = 'd';
							else if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser&& x != left && world[sizey-1][x-1] != enemyLaser)
								keyPress = 'a';
						}
					} 
				}
			} else {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser & cycle % 2 == 1) || (world[sizey-2][x] == enemyLaser))) {
						if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser && x != left && world[sizey-1][x-1] != enemyLaser)
							keyPress = 'a';
						else if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
							keyPress = 'd'; 
						else {
							if (laserReady > 2) {
								keyPress = 'm';
								laserReady = 0;
							} 
						}
					}
					else {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} 
					}
				}
			}
		}
	} else if (cycle >= 12 && cycle <= 115) {
		if (direction == 'r') {
			if ((cycle % 12) == 1 || (cycle % 12) == 2) {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && !(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) 
						&& world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
						keyPress = 'd';
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser) || (world[sizey-2][x] == enemyLaser))) {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} else {
							if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
								keyPress = 'd';
							else if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser&& x != left && world[sizey-1][x-1] != enemyLaser)
								keyPress = 'a';
						}
					} 
				}
			} else {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser) || (world[sizey-2][x] == enemyLaser))) {
						if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
							keyPress = 'd';
						else if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser&& x != left && world[sizey-1][x-1] != enemyLaser)
							keyPress = 'a';
						else {
							if (laserReady > 2) {
								keyPress = 'm';
								laserReady = 0;
							} 
						}
					}
					else {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} 
					}
				}
			}
		} else if (direction == 'l') {
			if ((cycle % 12) == 1 || (cycle % 12) == 2) {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && !(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) 
						&& world[sizey-2][x-1] != enemyLaser && x != left && world[sizey-1][x-1] != enemyLaser)
						keyPress = 'a';
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser) || (world[sizey-2][x] == enemyLaser))) {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} else {
							if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
								keyPress = 'd';
							else if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser && x != left && world[sizey-1][x-1] != enemyLaser)
								keyPress = 'a';
						}
					} 
				}
			} else {
				for (int x = 0; x < sizex; x++) {
					if (world[sizey-1][x] == player && ((world[sizey-3][x] == enemyLaser & cycle % 2 == 1) || (world[sizey-2][x] == enemyLaser))) {
						if (!(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x-1] != enemyLaser && x != left && world[sizey-1][x-1] != enemyLaser)
							keyPress = 'a';
						else if (!(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1) && world[sizey-2][x+1] != enemyLaser && x != right && world[sizey-1][x+1] != enemyLaser)
							keyPress = 'd'; 
						else {
							if (laserReady > 2) {
								keyPress = 'm';
								laserReady = 0;
							} 
						}
					}
					else {
						if (laserReady > 2) {
							keyPress = 'm';
							laserReady = 0;
						} 
					}
				}
			}
		}
	} else if (cycle > 115 && cycle <= 250) {
		if (world[sizey-1][8] != player) {
			for (int x = 0; x < sizex; x++) {
				if (world[sizey-1][x] == player && !(world[sizey-3][x-1] == enemyLaser && cycle % 2 == 1)  
					&& world[sizey-2][x-1] != enemyLaser && x != 0 && world[sizey-1][x-1] != enemyLaser)
					keyPress = 'a';
			}	
		} else {
			if (laserReady > 2) {
				keyPress = 'm';
				laserReady = 0;
			}
		}
	} else if (cycle > 250) {
		if (world[sizey-1][sizex-12] != player) {
			for (int x = 0; x < sizex; x++) {
				if (world[sizey-1][x] == player && !(world[sizey-3][x+1] == enemyLaser && cycle % 2 == 1)  
					&& world[sizey-2][x+1] != enemyLaser && x != sizex-1 && world[sizey-1][x+1] != enemyLaser)
					keyPress = 'd';
			}	
		} else {
			if (laserReady > 2) {
				keyPress = 'm';
				laserReady = 0;
			}
		}
	}
	cycle++;
	return keyPress;
}