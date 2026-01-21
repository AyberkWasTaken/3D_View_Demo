#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define PI 3.14159265358979323846
#define FPS 60
#define TIMER_PERIOD (1000/FPS)
#define GRAVITY 0.01f

int gameWidth = 960, gameHeight = 720;
float mouseSensitivity = 0.001f;
bool up = false, down = false, left = false, right = false, space=false, crouch=false, Jump = false;

int frameCount = 0;
int fps = 0;
int previousTime = 0;

typedef struct {
	float x, y, z;
} coord;

typedef struct {
	float x, y;
} coord2D;

coord2D cameraFOV = { 0.0f , 0.0f };
coord2D cameraSpeed = { 0.0f , 0.0f };

coord cameraPos = { 0.0f, 0.0f, 5.0f };

coord square[8] = {{-0.25, -0.25, 0.25},{-0.25, 0.25, 0.25},{0.25, 0.25, 0.25},{0.25, -0.25, 0.25},{-0.25, -0.25, -0.25},{-0.25, 0.25, -0.25},{0.25, 0.25, -0.25},{0.25, -0.25, -0.25}};

#define CAR_V_COUNT 42  
#define CAR_E_COUNT 74  

coord carVertices[CAR_V_COUNT] = {
	{-0.35, -0.25, -0.9}, { 0.35, -0.25, -0.9}, 
	{-0.35, -0.25,  0.9}, { 0.35, -0.25,  0.9}, 
	{-0.40, -0.10, -0.8}, { 0.40, -0.10, -0.8}, 
	{-0.40, -0.10,  0.8}, { 0.40, -0.10,  0.8}, 

	{-0.40,  0.05, -0.3}, { 0.40,  0.05, -0.3}, 
	{-0.40,  0.05,  0.4}, { 0.40,  0.05,  0.4}, 
	{-0.35,  0.05,  0.9}, { 0.35,  0.05,  0.9}, 

	{-0.30,  0.25, -0.1}, { 0.30,  0.25, -0.1}, 
	{-0.30,  0.25,  0.25},{ 0.30,  0.25,  0.25},

	{-0.41, -0.25, -0.6}, {-0.41, -0.05, -0.6}, {-0.41, -0.05, -0.4}, {-0.41, -0.25, -0.4},
	{ 0.41, -0.25, -0.6}, { 0.41, -0.05, -0.6}, { 0.41, -0.05, -0.4}, { 0.41, -0.25, -0.4},
	{-0.41, -0.25,  0.4}, {-0.41, -0.05,  0.4}, {-0.41, -0.05,  0.6}, {-0.41, -0.25,  0.6},
	{ 0.41, -0.25,  0.4}, { 0.41, -0.05,  0.4}, { 0.41, -0.05,  0.6}, { 0.41, -0.25,  0.6},

	{-0.35,  0.15,  0.85}, { 0.35,  0.15,  0.85}, 
	{-0.40,  0.20,  0.95}, { 0.40,  0.20,  0.95}, 

	{-0.20, -0.05, -0.86}, { 0.20, -0.05, -0.86}  
};

int carEdges[CAR_E_COUNT][2] = {
	{0,1}, {1,3}, {3,2}, {2,0},
	{8,9}, {9,11}, {11,13}, {13,15}, {15,14}, {14,12}, {12,10}, {10,8},
	{0,8}, {1,9}, {4,10}, {5,11}, {6,12}, {7,13}, {2,14}, {3,15},
	{0,4}, {4,6}, {6,2}, {1,5}, {5,7}, {7,3},


	{16,17}, {17,19}, {19,18}, {18,16}, 
	{10,16}, {11,17}, 
	{12,18}, {13,19}, 

	{20,21}, {21,22}, {22,23}, {23,20}, 
	{24,25}, {25,26}, {26,27}, {27,24}, 
	{28,29}, {29,30}, {30,31}, {31,28},
	{32,33}, {33,34}, {34,35}, {35,32},
	{20,22}, {21,23}, {24,26}, {25,27}, {28,30}, {29,31}, {32,34}, {33,35},

	{14,36}, {15,37},
	{36,37}, 
	{36,38}, {37,39}, {38,39},
	{38,14}, {39,15}, 

	{8,40}, {40,41}, {41,9}, 
	{0,40}, {1,41} 
};

int edges[12][2] = {
	{0,1}, {1,2}, {2,3}, {3,0}, 
	{4,5}, {5,6}, {6,7}, {7,4}, 
	{0,4}, {1,5}, {2,6}, {3,7}  
};

coord floorQuad[4] = {
	{-5.0f, -1.0f, -3.0f}, 
	{-5.0f, -1.0f, 5.0f},  
	{5.0f, -1.0f, 5.0f},   
	{5.0f, -1.0f, -3.0f}   
};

void vprint(float x, float y, void* font, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}

void drawCircle(float x, float y, float r) {
	float angle;
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
	for(int i = 0; i < 100; i++) {
		angle = 2 * i * PI / 100;
		glVertex2f(x + r * cos(angle),y + r * sin(angle));
	}
	glEnd();
}

void drawSquare(coord p1,coord p2) {
	glColor3ub(255, 255, 255);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y );
	glVertex2f(p2.x, p2.y );
	glEnd();
}

coord translateZ(coord p, float dz) {
	coord temp;
	temp.x = p.x;
	temp.y = p.y;
	temp.z = p.z + dz;
	return temp;
}

coord rotateXZ(coord p, float angle) {
	coord temp;
	float c = cos(angle);
	float s = sin(angle);
	temp.x = p.x * c - p.z * s;
	temp.z = p.x * s + p.z * c;
	temp.y = p.y;
	return temp;
}

coord rotateYZ(coord p, float angle) {
	coord temp;
	float c = cos(angle);
	float s = sin(angle);
	temp.y = p.y * c - p.z * s;
	temp.z = p.y * s + p.z * c;
	temp.x = p.x;
	return temp;
}

coord angleCalc(coord p,float angle){
	coord temp;
	temp.x = p.x * sin(angle);
	temp.z = p.z * cos(angle);
	temp.y = p.y;
	return temp;
}

coord project(coord coordinate){
	coord temp;
	temp.x = coordinate.x / coordinate.z;
	temp.y = coordinate.y / coordinate.z;
	return temp;
}

float angle = 0.0f;


void calculateFPS() {

	frameCount++;

	int currentTime = glutGet(GLUT_ELAPSED_TIME);

	if (currentTime - previousTime > 1000) {
		fps = frameCount;
		previousTime = currentTime;
		frameCount = 0;
	}
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);

	float floorSize = 10.0f;
	float step = 0.5f;

	for (float x = -floorSize; x < floorSize; x += step) {
		for (float z = -floorSize; z < floorSize; z += step) {

			coord p[4];
			p[0].x = x;        p[0].y = -1.0f; p[0].z = z;
			p[1].x = x;        p[1].y = -1.0f; p[1].z = z + step;
			p[2].x = x + step; p[2].y = -1.0f; p[2].z = z + step;
			p[3].x = x + step; p[3].y = -1.0f; p[3].z = z;

			int pointsBehind = 0;

			for (int i = 0; i < 4; i++) {

				p[i].x += cameraPos.x;
				p[i].y += cameraPos.y;
				p[i].z += cameraPos.z;
				p[i] = rotateXZ(p[i], cameraFOV.x);
				p[i] = rotateYZ(p[i], cameraFOV.y);

				if (p[i].z <= 0.1f) pointsBehind++;
			}

			if (pointsBehind > 0) continue;
			if (((int)x + (int)z) % 2 == 0) glColor3ub(100, 100, 100);
			else glColor3ub(150, 150, 150);
			if ((x == 0.0f && z == -4.0f))
				glColor3ub(255, 0, 0);

			for (int i = 0; i < 4; i++) {
				coord proj = project(p[i]);
				glVertex2f(proj.x, proj.y);
			}
		}
	}
	glEnd();

	glColor3ub(255, 0, 0);
	glLineWidth(2.0); 
	glBegin(GL_QUADS); 


	for (int i = 0; i < CAR_E_COUNT; i++) {
	
		coord p1 = carVertices[carEdges[i][0]];
		coord p2 = carVertices[carEdges[i][1]];
    
		p1 = rotateXZ(p1, angle * 0.5);
		p1.x += cameraPos.x;
		p1.y += cameraPos.y;
		p1.z += cameraPos.z;
		p1 = rotateXZ(p1, cameraFOV.x);
		p1 = rotateYZ(p1, cameraFOV.y);
		float z1 = p1.z;
		p1 = project(p1);             

		p2 = rotateXZ(p2, angle*0.5);
		p2.x += cameraPos.x;
		p2.y += cameraPos.y;
		p2.z = p2.z + cameraPos.z;
		p2 = rotateXZ(p2, cameraFOV.x);
		p2 = rotateYZ(p2, cameraFOV.y);
		float z2 = p2.z;
		p2 = project(p2);

		if (z1 <= 0.1f || z2 <= 0.1f) {
			continue; 
		}

		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();

	calculateFPS();

	glColor3ub(0, 255, 0);

	vprint(-1.3f, 0.85f, GLUT_BITMAP_HELVETICA_18, "FPS: %d", fps);

	vprint(-1.3, 0.9f, GLUT_BITMAP_HELVETICA_18, "Camera position: x=%.2f, y=%.2f, z=%.2f Angle: X = %.2f, Y = %.2f", -cameraPos.x, cameraPos.y, -cameraPos.z, cameraFOV.x, cameraFOV.y);
	glutSwapBuffers();
}

void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	float dt = 1.0f / FPS;
	angle += 2 * PI * dt / 5;

	if (up) {
		cameraPos.z -= 0.1f * cos(cameraFOV.x);
		cameraPos.x -= 0.1f * sin(cameraFOV.x);
	}
	if (down) {
		cameraPos.z += 0.1f * cos(cameraFOV.x);
		cameraPos.x += 0.1f * sin(cameraFOV.x);
	}
	if (left) {
		cameraPos.z -= 0.1f * sin(cameraFOV.x);
		cameraPos.x += 0.1f * cos(cameraFOV.x);
	}
	if (right){
		cameraPos.z += 0.1f * sin(cameraFOV.x);
		cameraPos.x -= 0.1f * cos(cameraFOV.x);
	}
	if (crouch){
		if (cameraPos.y < 0.1f)
			cameraPos.y += 0.1f;
	}

	cameraSpeed.y += GRAVITY;
	
	cameraPos.y += cameraSpeed.y;

	if (cameraPos.y >= -0.05f) {
		cameraPos.y = 0.0f;
		cameraSpeed.y = 0.0f;
		Jump = false;
	}

	cameraPos.y += cameraSpeed.y;

	glutPostRedisplay();
}

void onResize(int w, int h) {
	gameWidth = w;
	gameHeight = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w,
			1.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
	else
		glOrtho(-1.0 * (GLfloat)w / (GLfloat)h,
			1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -1.0, 1.0);

//	glOrtho(-w / 2.0, w / 2.0, -h / 2.0, h / 2.0, -2000.0, 2000.0);

	glMatrixMode(GL_MODELVIEW);
}

void onMouseMove(int x, int y) {
	int centerX = gameWidth / 2;
	int centerY = gameHeight / 2;

	if (x == centerX && y == centerY) return;

	int deltaX = x - centerX;
	int deltaY = y - centerY;

	cameraFOV.x += deltaX * mouseSensitivity;
	cameraFOV.y += -deltaY * mouseSensitivity;

	if (cameraFOV.x < -2 * PI)
		cameraFOV.x += 2 * PI;
	else if (cameraFOV.x > 2 * PI)
		cameraFOV.x -= 2 * PI;
	if (cameraFOV.y < -PI / 2)
		cameraFOV.y = -PI / 2;
	else if (cameraFOV.y > PI / 2)
		cameraFOV.y = PI / 2;

	glutWarpPointer(centerX, centerY);
}

void onKeyDown(unsigned char key, int x, int y) {

	if (key == 'w' || key == 'W') {
		up = true;
	}
	else if (key == 's' || key == 'S') {
		down = true;
	}
	else if (key == 'a' || key == 'A') {
		left = true;
	}
	else if (key == 'd' || key == 'D') {
		right = true;
	}
	else if (key == 'c' || key == 'C') {
		crouch = true;
	}
	else if (key == 32 && Jump == false) {
		space = true;
		cameraSpeed.y = -0.13f;
		Jump = true;
	}
	else if (key == 27) {
		exit(0);
	}

	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {

	if (key == 'w' || key == 'W') {
		up = false;
	}
	else if (key == 's' || key == 'S') {
		down = false;
	}
	else if (key == 'a' || key == 'A') {
		left = false;
	}
	else if (key == 'd' || key == 'D') {
		right = false;
	}
	else if (key == 'c' || key == 'C') {
		crouch = false;
	}
	else if (key == 32) {
		space = false;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(gameWidth, gameHeight);
	glutInitWindowPosition( 400, 20);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("3D World");

	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(gameWidth/2, gameHeight/2);
	glutPassiveMotionFunc(onMouseMove);

	glutDisplayFunc(display); 
	glutReshapeFunc(onResize);

	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	glutMainLoop();
	return 0;
}