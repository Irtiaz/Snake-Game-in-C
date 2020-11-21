#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <allegro5/allegro_primitives.h>

#include "sketch.h"



#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define cols 40

float w;

struct cell {
	int i;
	int j;
};

typedef struct cell cell;

cell snake[cols * cols];
int snakeLength = 1;

int dir = RIGHT;

cell food;

bool over;

void move();
void generateFood();
void grow();
bool checkGameOver();


//This code in setup will be run only once
void setup() {
	srand(time(NULL));

	snake[0].i = cols / 2;
	snake[0].j = cols / 2;

	w = (float)width / cols;

	generateFood();
}


//This code in draw will run every frame
void draw() {
	al_clear_to_color(al_map_rgb(51, 51, 51));

	al_draw_filled_circle(food.j * w + w/2, food.i * w + w/2, w/2, al_map_rgb(255, 0, 0));
	al_draw_circle(food.j * w + w / 2, food.i * w + w / 2, w / 2, al_map_rgb(0, 0, 0), 1);
	
	for (int i = 0; i < snakeLength; ++i) {
		int p = snake[i].i;
		int q = snake[i].j;

		al_draw_filled_rectangle(q * w, p * w, q * w + w, p * w + w, al_map_rgb(0, 255, 0));
	}

	if (!over) move();
	else {
		if (snake[0].i < 0) snake[0].i = 0;
		else if (snake[0].i >= cols) snake[0].i = cols - 1;

		if (snake[0].j < 0) snake[0].j = 0;
		else if (snake[0].j >= cols) snake[0].j = cols - 1;
	}

	if (!over) over = checkGameOver();

	if (snake[0].i == food.i && snake[0].j == food.j) {
		grow();
		generateFood();
	}
}



void mousePressed(bool lmb) {
	//Handle mouse click events here
}
	


void keyPressed(int keyCode) {
	//Handle key press events

	if (keyCode == ALLEGRO_KEY_UP && dir != DOWN) dir = UP;
	else if (keyCode == ALLEGRO_KEY_RIGHT && dir != LEFT) dir = RIGHT;
	else if (keyCode == ALLEGRO_KEY_DOWN && dir != UP) dir = DOWN;
	else if (keyCode == ALLEGRO_KEY_LEFT && dir != RIGHT) dir = LEFT;
}



void move() {
	int prevI, prevJ;
	
	if (dir == UP || dir == DOWN) prevI = dir == UP ? snake[0].i - 1: snake[0].i + 1;
	else prevI = snake[0].i;
	
	if (dir == RIGHT || dir == LEFT) prevJ = dir == RIGHT ? snake[0].j + 1 : snake[0].j - 1;
	else prevJ = snake[0].j;

	for (int i = 0; i < snakeLength; ++i) {
		int tempI = snake[i].i;
		int tempJ = snake[i].j;

		snake[i].i = prevI;
		snake[i].j = prevJ;

		prevI = tempI;
		prevJ = tempJ;
	}
}


void grow() {
	if (snakeLength == 1) {
		if (dir == UP || dir == DOWN) {
			snake[1].j = snake[0].j;
			snake[1].i = dir == UP ? snake[0].i + 1 : snake[0].i - 1;
		}
		else {
			snake[1].i = snake[0].i;
			snake[1].j = dir == RIGHT ? snake[0].j - 1 : snake[0].j + 1;
		}
	}

	else {
		int midI = snake[snakeLength - 1].i;
		int midJ = snake[snakeLength - 1].j;
		int i1 = snake[snakeLength - 2].i;
		int j1 = snake[snakeLength - 2].j;

		int i2 = 2 * midI - i1;
		int j2 = 2 * midJ - j1;

		snake[snakeLength].i = i2;
		snake[snakeLength].j = j2;
	}
	
	++snakeLength;
}

void generateFood() {
	food.i = rand() % cols;
	food.j = rand() % cols;
}

bool checkGameOver() {
	if (snake[0].i < 0 || snake[0].i >= cols || snake[0].j < 0 || snake[0].j >= cols) return true;

	for (int i = 1; i < snakeLength; ++i) {
		if (snake[i].i == snake[0].i && snake[i].j == snake[0].j) return true;
	}

	return false;
}