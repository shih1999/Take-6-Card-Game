#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "interface.h"
#include "40711014E.h"

char com[11][8] = {"0", "  YOU ", " CP1 ", " CP2 ", " CP3 ", " CP4 ", " CP5 ", " CP6 ", " CP7 ", " CP8 ", " CP9 "};

// struct for piles
typedef struct _sPiles {
	int pilenum;	// number of cards in the pile
	int pileheads;	// total heads in the pile
	int card[6];
	int last;
} sPiles;
// struct for players
typedef struct _sPlayers {
	int card[11];
	int head[11];
	int position[11];
	int heads;
} sPlayers;

// icon for heads
void numtoicon(int num) {
	switch(num)
	{
		case 1:
			printf("| -₁- ");
			break;
		case 2:
			printf("| -₂- ");
			break;
		case 3:
			printf("| -₃- ");
			break;
		case 4:
			printf("| -₄- ");
			break;
		case 5:
			printf("| -₅- ");
			break;
		case 6:
			printf("| -₆- ");
			break;
		case 7:
			printf("| -₇- ");
			break;
	}	
}

// simple swap
void simple_swap (int *x, int *y) {
	int temp = *x; 
    *x = *y; 
    *y = temp;
}

// sort the cads for player ( small to big ) 
void player_sort(int c, int num, sPlayers player[]) {
	int temp;
	for (int h = 1; h <= c-1; h++) {
		int min = h;
		for (int k = h+1; k <= c; k++) {
			if (player[num].card[k] <= player[num].card[min])
				min = k;
		}
		/*
		swap2((&player[num].card[h]),(&player[num].card[min]));
		swap2((&player[num].head[h]),(&player[num].head[min]));
		*/
		temp = player[num].card[h];
		player[num].card[h] = player[num].card[min];
		player[num].card[min] = temp;

		temp = player[num].head[h];
		player[num].head[h] = player[num].head[min];
		player[num].head[min] = temp;
	} 
}

// detect keyboard event
// source: https://www.twblogs.net/a/5cb92631bd9eee0f00a235fd
int scanKeyboard() {
	int in;
	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0,&stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
 
	in = getchar();
 
	tcsetattr(0,TCSANOW,&stored_settings);
	return in;
}

// shuffle cards
void shufflecard(int arr[]) {
	int i, temp, num;
	// put numbers in
	for ( i = 1; i <= 104; i++) {
		arr[i] = i;
	}
	// shuffle
	for ( i = 1; i <= 104; i++) {
		num = rand() % (104-1+1)+1;
		//num = rand() % 104;
		temp = arr[i];
		arr[i] = arr[num];
		arr[num] = temp;
	}
}

// deal the cards 
void dealcard(int c, int num, int cards[], sPlayers player[], sPiles pile[]) {	// n: number of players
	int give = 1;
	for (int i = 1; i <= num; i++) {
		for (int j = 1; j <= c; j++) {
			player[i].card[j] = cards[give];
			player[i].head[j] = value(cards[give]);
			give++;
		}
		/*
		printf("player %d \n", i);
		for (int j = 1; j <= c; j++) {
			printf("%d ", player[i].card[j]);
		}
		printf("\n");
		*/
		player_sort(c, i, player);
		/*
		for (int j = 1; j <= c; j++) {
			printf("%d ", player[i].card[j]);
		}
		printf("\n");
		*/
	}
	for (int i = 1; i <= 4; i++) {
		pile[i].card[1] = cards[give];
		pile[i].last = cards[give];
		pile[i].pilenum ++;
		pile[i].pileheads += value(cards[give]);
		give++;
	}
}

// welcome : enter or quit
int welcome() {
	char state;
	EnterOrQuit();

	while (1) {
		//printf("%d",scanKeyboard());
		int press = scanKeyboard();
		
		if (press == 10) {
			enterGame();
			return 1;
		}
		else if (press == 32) {
			quitGame();
			return 0;
		}
		else {
			printf("\n                  Invalid Choice ! Please Press Again.\n");
		}
	}
}

// select number of components & agent
int menu(int *num, int *a, int *l) {
	menu_num();
	while (1) {
		printf("                            your chioce: ");
		scanf("%d", num);
		if ( *num >=1 && *num <= 9) {
			*num = *num+1;
			printf("\n");
			break;
		}
		else
			printf("\n                  Invalid Choice ! Please Choose Again.\n");
	}
	menu_level();
	while (1) {
		printf("                            your chioce: ");
		scanf("%d", l);
		if ( *l == 1 || *l == 2) {
			printf("\n");
			break;
		}
		else
			printf("\n                  Invalid Choice ! Please Choose Again.\n");
	}
	menu_agent();
	while (1) {
		printf("                            your chioce: ");
		scanf("%d", a);
		if ( *a == 1 || *a == 2) {
			printf("\n");
			break;
		}
		else
			printf("\n                  Invalid Choice ! Please Choose Again.\n");
	}
	startGame();
}

// get card num at the choosen position
int getcard(int c, int choice, int p, sPlayers player[]) {
	int pose = 1;
	int find;
	for (int i = 1; i <= c; i++) {
		if (player[p].position[i] == choice) {
			find = player[p].card[i];
			player[p].card[i] = 0;
			break;
		}
	}
	for (int i = 1; i <= c; i++) {
		if (player[p].card[i] != 0) {
			player[p].position[i] = pose;
			pose++;
		}
		else
			player[p].position[i] = 0;
	}
	/*
	for (int i = 1; i <= c; i++) {
		printf("%d ", player[p].position[i]);
	}
	printf("\n");
	*/
	return find;
}

// print current status & let the player choose one card
int player_choose(int c, int r, int num, sPlayers player[], sPiles pile[]) {
	int piletotal;
	int choice;
	int choosen;
	printf("........................................................................\n\n");
	printf("                            ⚔️  ROUND %02d ⚔️\n\n", r);

	// print every players' bull heads
	printf(" | PLAYER |");
	for (int i = 1; i <= num; i++) {
		printf("%s", com[i]);
	}
	printf("\n | HEADS  | ");
	for (int i = 1; i <= num; i++) {
		printf(" %03d ", player[i].heads);
	}
	printf("\n\n\n");

	// print the four piles of cards
	for (int i = 1; i <= 4; i++) {
		piletotal = pile[i].pilenum;
		// printf("total: %d last: %d\n", piletotal, pile[i].last);
		printf("  PILE %d ➮   ", i);
		for (int j = 1; j <= piletotal; j++) {
			numtoicon(value(pile[i].card[j]));
		}
		printf("|\n             ");
		for (int i = 1; i <= piletotal; i++) {
		printf("|     ");
		}
		printf("|\n             ");
		for (int j = 1; j <= piletotal; j++) {
			printf("| %3d ", pile[i].card[j]);
		}
		printf("|\n\n");
	}

	// print the players cards
	printf("\n\n             ");
	for (int i = 1; i <= c; i++) {
		if (player[1].card[i] != 0) {
			numtoicon(player[1].head[i]);
		}
	}
	printf("|\n  YOUR CARDS ");
	for (int i = 1; i <= c+1-r; i++) {
		printf("|     ");
	}
	printf("|\n             ");
	for (int i = 1; i <= c; i++) {
		if (player[1].card[i] != 0) {
			printf("| %3d ", player[1].card[i]);
		}
	}

	// let the player choose one card
	printf("|\n\n\n");
	printf("                    ? which card do you want to use ?\n");
	printf("                 ( enter the position of the card 1-%d )\n\n", c+1-r);
	while (1) {
		printf("                            your chioce: ");
		scanf("%d", &choice);
		if ( choice >=1 && choice <= c+1-r) {
			printf("\n\n");
			break;
		}
		else
			printf("\n                  Invalid Choice ! Please Choose Again.\n");
	}
	choosen = getcard(c, choice, 1, player);
	return choosen;
}

// components pick card [agent 1] ( use random )
int pickcard_1(int c, int r, int num, sPlayers player[], sPiles pile[]) {
	int choice;
	int choosen;
	choice = rand() % ((c+1-r)-1+1)+1;
	//printf("ran %d \n", choice);
	choosen = getcard(c, choice, num, player);
	return choosen;
}

// components pick card [agent 2] ( use card order : from small to big )
int pickcard_2(int c, int num, sPlayers player[], sPiles pile[]) {
	int choice;
	int choosen;
	choice = 1;
	choosen = getcard(c, choice, num, player);
	return choosen;
}

// place card into pile
void place_card(int no, int p, sPiles pile[], sPlayers player[]) {
	int right = 0;	// the right pile
	int found = 0;
	int choice;	// which pile to take
	int order[4][2];

	if (no > pile[1].last) {
		right = 1;
	}
	for (int i = 2; i <= 4; i++) {
		if (no > pile[i].last && pile[i].last > pile[right].last) {
			right = i;
		}
	}
	// if the card couldn't be put into any pile
	if (right == 0 ) {
		if (p == 1) {	// player
			printf("                    ? which pile do you want to take ?\n");
			printf("                 ( enter the position of the pile 1-4 )\n");
			while (1) {
				printf("                            your chioce: ");
				scanf("%d", &choice);
				if ( choice >=1 && choice <= 4) {
					printf("\n");
					break;
				}
				else
					printf("\n                  Invalid Choice ! Please Choose Again.\n");
			}
		}
		else {			// components
			choice = 1;
			for (int i = 2; i <= 4; i++) {
				if (pile[i].pileheads < pile[choice].pileheads)
					choice = i;
			}
		}
		// empty the pile & put new card in
		player[p].heads = player[p].heads + pile[choice].pileheads;
		for (int i = 1; i <= pile[choice].pilenum; i++) {
			pile[choice].card[i] = 0;
		}
		pile[choice].pileheads = value(no);
		pile[choice].pilenum = 1;
		pile[choice].card[1] = no;
		pile[choice].last = no;
	}
	// if the pile the card should be put in already have 5 cards
	else if (pile[right].pilenum == 5) {
		for (int i = 1; i <= 5; i++) {
			pile[right].card[i] = 0;
		}
		pile[right].pileheads = value(no);
		pile[right].pilenum = 1;
		pile[right].card[1] = no;
		pile[right].last = no;
	}
	else {
		pile[right].pilenum++;
		pile[right].card[pile[right].pilenum] = no;
		pile[right].pileheads = pile[right].pileheads + value(no);
		pile[right].last = no;
	}
}

int main() {
	char com[11][8] = {"0", " YOU ", " CP1 ", " CP2 ", " CP3 ", " CP4 ", " CP5 ", " CP6 ", " CP7 ", " CP8 ", " CP9 "};
	sPiles piles[5];
	sPlayers players[11];
	int cards[104];
	int n;
	int cpp;	// cards per person
	int agent;	// agent level
	int level;	// game level
	int rank[11][2];

	srand((unsigned) time(NULL) + getpid());	// for random

	while(1) 
	{
	if ( welcome() == 1) {	//enter
		for (int i = 1; i <= 4; i++) {
		piles[i].pilenum = 0;
		piles[i].pileheads = 0;
		}
		menu(&n, &agent, &level);
		if (level == 1) cpp = 5;
		else if (level == 2) cpp = 10;
		//printf("%d %d\n", n, agent);
		for (int i = 1; i <= n; i++) {
			players[i].heads = 0;
			for (int j = 1; j <= cpp; j++) {
				players[i].position[j] = j;
			}
		}
		shufflecard(cards);
		dealcard(cpp, n, cards, players, piles);

		// 10 rounds
		for (int round = 1; round <= cpp; round++) {
			int out[11][2] = {0};
			for (int p = 1; p <= n; p++) {
				out[p][1] = p;
			}
			// choose cards
			out[1][0] = player_choose(cpp, round, n, players, piles);
			for (int p = 2; p <= n; p++) {
				if (agent == 1) {
					out[p][0] = pickcard_1(cpp, round, p, players, piles);
				}
				if (agent == 2) {
					out[p][0] = pickcard_2(cpp, p, players, piles);
				}
			}
			// print everyones' choice
			printf(" [ PLAYER ] ");
			for (int i = 1; i <= n; i++) {
			printf("%s", com[i]);
			}
			printf("\n [ CHIOCE ] ");
			for (int i = 1; i <= n; i++) {
				printf(" %3d ", out[i][0]);
			}
			printf("\n\n");

			// sort the cards
			for (int i = 1; i <= n-1; i++) {
				int min = i;
				for (int j = i+1; j <= n; j++) {
					if (out[j][0] <= out[min][0])
					min = j;
				}
				simple_swap(&out[i][0], &out[min][0]);
				simple_swap(&out[i][1], &out[min][1]);
			}
			/*
			for (int i = 1; i <= n; i++) {
				printf("%d %d\n", out[i][0], out[i][1]);
			}
			*/
			for (int i = 1; i <=n; i++) {
				place_card(out[i][0], out[i][1], piles, players);
			}
		}
		for (int i = 1; i <= n; i++) {
			rank[i][0] = players[i].heads;
			rank[i][1] = i;
		}
		for (int i = 1; i <= n-1; i++) {
			int min = i;
			for (int j = i+1; j <= n; j++) {
				if (rank[j][0] <= rank[min][0])
				min = j;
			}
			simple_swap(&rank[i][0], &rank[min][0]);
			simple_swap(&rank[i][1], &rank[min][1]);
		}
		// print ranking
		ranking();
		int nowrank = 1;
		int nowhead = rank[1][0];
		printf("      ◦           %02d            %s         🐮 x %03d           ◦\n", nowrank, com[rank[1][1]], rank[1][0]);
		for (int i = 2; i <= n; i++) {
			if (rank[i][0] != nowhead) {
				nowhead = rank[i][0];
				nowrank++;

			}
			printf("      ◦           %02d            %s         🐮 x %03d           ◦\n", nowrank, com[rank[i][1]], rank[i][0]);
		}
		Gameend();
		// playagain or quit
		printf("\n\n");
		printf("             	     ~ press \"tab\" to play again ~\n");
		printf("             	       ~ press \"space\" to quit ~\n");

		while (1) {
			int again = scanKeyboard();
		
			if (again == 9) {
				break;
		 	}
			else if (again == 32) {
				quitGame();
				return 0;
			}
			else {
				printf("\n                  Invalid Choice ! Please Press Again.\n");
			}
		}
	}
	else
		return 0;	//quit
	}
}