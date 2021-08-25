/*
	Microsoft Visual Studio Community 2019
	Author - Gautam Thareja
*/

#include<GL/glut.h>
#include<iostream>
#include <math.h>
#include<windows.h>
#include<vector>
#include<ctime>
#include<unordered_map>
#include<map>
using namespace std;

int score = 0;
int ptsz = 43;
bool lt = false;
bool rt = false;
bool down = false;
bool r = false;
int y;
float rd, gr, bl;

map<pair<int, int>, vector<float>> grid;

int ht[9] = { 50,50,50,50,50,50,50,50,50 };
int colCount = 5;
//float red[9] = { 1.0,1.0,1.0,0.45,0.01,0.20,0.40,0.60,0.80 };
//float green[9] = { 0.12,0.58,0.83,0.8,0.25,0.40,0.60,0.80,0.20 };
//float blue[9] = { 0.07,0.10,0,0.23,0.67,0.60,0.80,0.50,0.40 };
float red[5] = { 1.0,1.0,1.0,0.45098,0.0117647 };
float green[5] = { 0.121569,0.588235,0.839216,0.8,0.25098 };
float blue[5] = { 0.0705882,0.1009804,0,0.231373,0.678431 };
int tow, towi, col;
int blockNo = 0;
bool newBlock = true;
bool gameOver = false;

//Function Declaration
void checkSurroundings(int[], float, float, float, int, int, int);

void drawCurrentBlock() {
	if (gameOver)
		return;

	if (newBlock) {
		cout << "Score = " << score << endl;
		cout << "drawing new block\n";
		blockNo++;
		y = 600;
		srand((unsigned)time(0) + blockNo);
		col = rand() % colCount;
		towi = rand() % 9;
		tow = (towi + 1) * 45;

		rd = red[col], gr = green[col], bl = blue[col];

		cout << tow << " " << y << endl;
		cout << rd << " " << gr << " " << bl << endl;
		glBegin(GL_POINTS);
		glColor3f(rd, gr, bl);
		glVertex2f(tow, y);
		glEnd();
		glFlush();
		newBlock = false;
	}
	else {
		//cout << "drawing current block\n";
		//cout << towi << " " << y << endl;
		//cout << rd << " " << gr << " " << bl << endl;
		tow = (towi + 1) * 45;
		glBegin(GL_POINTS);
		glColor3f(rd, gr, bl);
		glVertex2f(tow, y);
		glEnd();
		glFlush();
	}
}

void drawGrid() {
	//cout << "drawing grid\n";
	if (gameOver)
		return;
	glBegin(GL_POINTS);
	for (int i = 45; i < 450; i += 45) {
		for (int j = 50; j < 600; j += 45) {
			float rd1 = grid[{i, j}][0];
			float gr1 = grid[{i, j}][1];
			float bl1 = grid[{i, j}][2];
			glColor3f(rd1, gr1, bl1);
			glVertex2f(i, j);
		}
	}
	glEnd();
	glFlush();
}

void shiftDownByOne(int ht[], int towi, int tow, int y) {
	float color1[3];
	for (int i = y; i < ht[towi]; i += 45) {
		glReadPixels(tow, i + 45, 1.0, 1.0, GL_RGB, GL_FLOAT, color1);
		grid[{tow, i}] = { color1[0], color1[1], color1[2] };
		if (color1[0] == 0 && color1[1] == 0 && color1[2] == 0)
			break;
	}

	drawGrid();

	for (int i = y; i < ht[towi]; i += 45) {
		glReadPixels(tow, i, 1.0, 1.0, GL_RGB, GL_FLOAT, color1);
		if (color1[0] == 0 && color1[1] == 0 && color1[2] == 0)
			break;
		checkSurroundings(ht, color1[0], color1[1], color1[2], towi, tow, i);
	}
}

void shiftDownByTwo(int ht[], int towi, int tow, int y) {
	float color1[3];
	int count = 0;
	for (int i = y; i < ht[towi]; i += 45) {
		glReadPixels(tow, i + 2 * 45, 1.0, 1.0, GL_RGB, GL_FLOAT, color1);
		grid[{tow, i}] = { color1[0], color1[1], color1[2] };
		if (color1[0] == 0 && color1[1] == 0 && color1[2] == 0) {
			count++;
			if (count == 2)
				break;
		}
	}

	drawGrid();

	for (int i = y; i < ht[towi]; i += 45) {
		glReadPixels(tow, i, 1.0, 1.0, GL_RGB, GL_FLOAT, color1);
		if (color1[0] == 0 && color1[1] == 0 && color1[2] == 0)
			break;
		checkSurroundings(ht, color1[0], color1[1], color1[2], towi, tow, i);
	}
}

int checkBottom(int ht[], float rd, float gr, float bl, int towi, int tow, int y) {
	if (y > 51) {
		float color[3];
		glReadPixels(tow, y - 45, 1.0, 1.0, GL_RGB, GL_FLOAT, color);

		color[0] = (int)(color[0] * 100);
		color[1] = (int)(color[1] * 100);
		color[2] = (int)(color[2] * 100);

		if (color[0] == (int)(rd * 100) && color[1] == (int)(gr * 100) && color[2] == (int)(bl * 100)) {
			cout << "bottom matched\n";
			score++;
			shiftDownByTwo(ht, towi, tow, y - 45);
			ht[towi] -= 45;
			/*if (ht[towi] < 50)
				ht[towi] = 50;*/
			return 1;
		}
	}
	return 0;
}

int checkRight(int ht[], float rd, float gr, float bl, int towi, int tow, int y) {
	if (towi > 9) {
		return 0;
	}
	float color[3];
	glReadPixels(tow + 45, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);

	color[0] = (int)(color[0] * 100);
	color[1] = (int)(color[1] * 100);
	color[2] = (int)(color[2] * 100);

	if (color[0] == (int)(rd * 100) && color[1] == (int)(gr * 100) && color[2] == (int)(bl * 100)) {
		score++;
		cout << "right matched\n";
		shiftDownByOne(ht, towi, tow, y);
		shiftDownByOne(ht, towi + 1, tow + 45, y);
		ht[towi + 1] -= 45;
		/*if (ht[towi + 1] < 50)
			ht[towi + 1] = 50;*/
		return 1;
	}
	return 0;
}

int checkLeft(int ht[], float rd, float gr, float bl, int towi, int tow, int y) {
	if (towi < 0) {
		return 0;
	}
	float color[3];
	glReadPixels(tow - 45, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);

	color[0] = (int)(color[0] * 100);
	color[1] = (int)(color[1] * 100);
	color[2] = (int)(color[2] * 100);

	if (color[0] == (int)(rd * 100) && color[1] == (int)(gr * 100) && color[2] == (int)(bl * 100)) {
		cout << "left matched\n";
		score++;
		shiftDownByOne(ht, towi, tow, y);
		shiftDownByOne(ht, towi - 1, tow - 45, y);
		ht[towi - 1] -= 45;
		if (ht[towi - 1] < 50)
			ht[towi - 1] = 50;
		return 1;
	}
	return 0;
}

void checkSurroundings(int ht[], float rd, float gr, float bl, int towi, int tow, int y) {
	int bt = checkBottom(ht, rd, gr, bl, towi, tow, y);
	int rt = checkRight(ht, rd, gr, bl, towi, tow, y);
	int lt = checkLeft(ht, rd, gr, bl, towi, tow, y);

	if (bt || rt || lt) {
		score++;
		ht[towi] -= 45;
	}
}

void drop() {
	if (y > ht[towi]) {
		y--;
		for (int i = 0; i < 10000 - blockNo * 100; i++)
			Sleep(0.9);
		glutPostRedisplay();
	}
}

void specialInput(int key, int x, int y)
{
	switch (key){
	case GLUT_KEY_DOWN:
		//do something here
		//cout << "from SpecialInput: down pressed\n";
		down = true;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		//do something here
		//cout << "from SpecialInput: left pressed\n";
		lt = true;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		//do something here
		//cout << "from SpecialInput: right pressed\n";
		rt = true;
		glutPostRedisplay();
		break;
	case GLUT_KEY_HOME:
		//cout << "from SpecialInput: home pressed\n";
		r = true;
		glutPostRedisplay();
		break;
	}
}

void restart() {
	for (int i = 45; i < 450; i += 45) {
		ht[i / 45 - 1] = 50;
		for (int j = 50; j < 600; j += 45) {
			grid[{i, j}] = { 0.0,0.0,0.0 };
		}
	}
}

void game() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawGrid();
	drawCurrentBlock();

	if (rt) {
		if (towi < 8 && ht[towi + 1] < y) {
			towi++;
			tow += 45;
		}
		rt = false;
	}
	if (lt) {
		if (towi > 0 && ht[towi - 1] < y) {
			towi--;
			tow -= 45;
		}
		lt = false;
	}
	if (down) {
		y -= 45;
		if (y < ht[towi]) {
			y = ht[towi];
		}
		down = false;
	}
	if (r) {
		cout << "\nDo you want to restart the game? [y/n]\n";
		char ans;
		cin >> ans;
		if (ans == 'y') {
			cout << "\n\nRestarting the game.\n";
			restart();
			newBlock = true;
			score = 0;
			blockNo = 0;
		}
		r = false;
	}

	if (y == ht[towi]) {
		cout << "\nprev tower height = " << ht[towi] << endl;
		cout << "block dropped\n";
		grid[{tow, ht[towi]}] = { rd,gr,bl };
		drawGrid();
		ht[towi] += 45;
		checkSurroundings(ht, rd, gr, bl, towi, tow, y);
		cout << "new tower height = " << ht[towi] << endl << endl;
		newBlock = true;
		glutPostRedisplay();
	}

	if (ht[towi] > 600 && !gameOver) {
		cout << "Game Over!\n";
		cout << "Score = " << score << endl;
		glBegin(GL_POINTS);
		for (int i = 45; i < 450; i += 45) {
			for (int j = 50; j < 600; j += 45) {
				if (!(grid[{i, j}][0] == 0.0 && grid[{i, j}][1] == 0.0 && grid[{i, j}][2] == 0.0)) {
					glColor3f(1.0, 1.0, 1.0);
					glVertex2f(i, j);
				}
			}
		}
		glEnd();
		glFlush();
		gameOver = true;
	}
}

void myInit() {
	//initializing grid
	for (int i = 45; i < 450; i += 45) {
		for (int j = 50; j < 600; j += 45) {
			grid[{i, j}] = { 0.0,0.0,0.0 };
		}
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPointSize(ptsz);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 450, 0.0, 600);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(450, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Mini Tetris");
	glutDisplayFunc(game);
	glutSpecialFunc(specialInput);
	glutIdleFunc(drop);
	myInit();
	glutMainLoop();
}
