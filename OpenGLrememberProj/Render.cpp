#include "Render.h"

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include <time.h>

#include <mmsystem.h>

int speed = 0;

double gameIsOver = false; //игра окончена? По ум. false
double isRestarted = false; //был рестарт?

//для передвижения камеры
double moveX1 = 0;
double moveY1 = 0;
double moveZ1 = 0;
//---

GLuint texId[10]; //массив текстур

int viewMode = 1; //режим вида. 1 - сверху, 2 - из-за головы

int Angle = 0;

int score = 0;

float limit = 10; //ограничение, регулируем с помощью O,P
float count = 0; //счетчик для рег.скор.


int N = 30, M = 20; //поле 30*20 квадратов 
int scale = 1; // размер квадрата 

int w = N * scale; // ширина поля   
int h = M * scale; // длина  поля
int size = 3;// длина змейки (изначально будет 3)

int direction = 1; //направление змейки, 1-up, 2-left, 3-down, 4-right

double vn[] = { 0,0,0 };//вектор нормали
void normal(double a1[], double b1[], double c1[], double vn[]) //функция поиска нормали (вектора b1a1 и b1c1)
{
	double a[] = { a1[0] - b1[0],a1[1] - b1[1],a1[2] - b1[2] };
	double b[] = { c1[0] - b1[0],c1[1] - b1[1],c1[2] - b1[2] };

	vn[0] = a[1] * b[2] - b[1] * a[2];
	vn[1] = -a[0] * b[2] + b[0] * a[2];
	vn[2] = a[0] * b[1] - b[0] * a[1];

	double length = sqrt(pow(vn[0], 2) + pow(vn[1], 2) + pow(vn[2], 2));

	vn[0] /= length;
	vn[1] /= length;
	vn[2] /= length;
}

class Materials { //класс материалов
public:
	void basic() //материал базовый (который был изначально)
	{
		//настройка материала
		GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
		GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
		GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
		GLfloat sh = 0.1f * 256;


		//фоновая
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		//дифузная
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//зеркальная
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

		//чтоб было красиво, без квадратиков (сглаживание освещения)
		glShadeModel(GL_SMOOTH);
		//===================================
	}
	void setka() //материал для сетки
	{
		//настройка материала
		GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
		GLfloat dif[] = { 0., 1., 0., 1. };
		GLfloat spec[] = { 0., 1., 0., 1. };
		GLfloat sh = 0.1f * 256;


		//фоновая
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		//дифузная
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//зеркальная
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

		//чтоб было красиво, без квадратиков (сглаживание освещения)
		glShadeModel(GL_SMOOTH);
		//===================================
	}
	void field() //материал для поля
	{
		//настройка материала
		GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
		GLfloat dif[] = { 1., 1., 1., 1. };
		GLfloat spec[] = { 1., 1., 1., 1. };
		GLfloat sh = 0.1f * 256;


		//фоновая
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		//дифузная
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//зеркальная
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

		//чтоб было красиво, без квадратиков (сглаживание освещения)
		glShadeModel(GL_SMOOTH);
		//===================================
	}
	void pizza() //материал для пиццы
	{
		//настройка материала
		GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
		GLfloat dif[] = { 255.0/255.0, 202.0/255.0, 134.0/255.0, 1. };
		GLfloat spec[] = { 255.0 / 255.0, 202.0 / 255.0, 134.0 / 255.0, 1. };
		GLfloat sh = 0.1f * 256;


		//фоновая
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		//дифузная
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//зеркальная
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

		//чтоб было красиво, без квадратиков (сглаживание освещения)
		glShadeModel(GL_SMOOTH);
		//===================================
	}
	void snake() //материал для змеи
	{
		//настройка материала
		GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
		GLfloat dif[] = { 153.0 / 255.0, 255.0 / 255.0, 153.0 / 255.0, 1. };
		GLfloat spec[] = { 153.0 / 255.0, 255.0 / 255.0, 153.0 / 255.0, 1. };
		GLfloat sh = 0.1f * 256;


		//фоновая
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		//дифузная
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//зеркальная
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

		//чтоб было красиво, без квадратиков (сглаживание освещения)
		glShadeModel(GL_SMOOTH);
		//===================================
	}
} materials;

class blocks { //класс отрисовки змейки
public:
	int x;
	int y;

	int angle;
	int angleTail;

	blocks()
	{
		x = 0;
		y = 0;
		angle = 0; //угол для поворота головы
		angleTail = 0; //угол для поворота хвоста
	}
	void DrawBody()
	{
		glColor3f(1.0, 0.5, 0.0); //orange
		glBegin(GL_QUADS);

		//низ
		glNormal3d(0, 0, -1);

		glTexCoord2d(0, 1);
		glVertex3f(x * scale, y * scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x * scale + scale, y * scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x * scale + scale, y * scale + scale, 0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x * scale, y * scale + scale, 0);

		//верх
		glNormal3d(0, 0, 1);

		glTexCoord2d(0,1);
		glVertex3f(x*scale, y*scale, scale*2/3.0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale + scale, y*scale, scale * 2 / 3.0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale + scale, scale * 2 / 3.0);

		//бока

		glNormal3d(0, -1, 0);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale + scale, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale, scale * 2 / 3.0);

		glNormal3d(1, 0, 0);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale + scale, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale + scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale, scale * 2 / 3.0);

		glNormal3d(0, 1, 0);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale + scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale + scale, scale * 2 / 3.0);

		glNormal3d(-1, 0, 0);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale, scale * 2 / 3.0);

		glEnd();
	}
	void DrawHead()
	{
		if (count >= limit && !gameIsOver)
		{
			switch (direction)
			{
			case 1: angle = 0; break;
			case 2: angle = 90; break;
			case 3: angle = 180; break;
			case 4: angle = -90; break;
			}
		}

		glPushMatrix();

		glTranslated(block[0].x + scale/2.0, block[0].y + scale/2.0, 0); 
		glRotated(angle, 0, 0, 1); 
		glTranslated(-block[0].x - scale / 2.0, -block[0].y - scale / 2.0, 0);

		glColor3f(0.0, 0.5, 1.0); //lightblue
		glBegin(GL_QUADS);
		//низ

		glNormal3d(0, 0, -1);

		glTexCoord2d(77.0 / 512.0, 0);
		glVertex3f(x * scale, y * scale, 0);
		glTexCoord2d(176.0 / 512.0, 0);
		glVertex3f(x * scale + scale, y * scale, 0);
		glTexCoord2d(151.0 / 512.0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale + scale - scale/4.0, y * scale + scale, 0);
		glTexCoord2d(102.0 / 512.0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale + scale/4.0, y * scale + scale, 0);

		//верх
		double _A[3];
		double _B[3];
		double _C[3];

		_A[0] = x * scale + scale;
		_A[1] = y * scale;
		_A[2] = scale * 2 / 3.0;

		_B[0] = x * scale;
		_B[1] = y * scale;
		_B[2] = scale * 2 / 3.0;

		_C[0] = x * scale;
		_C[1] = y * scale + scale;
		_C[2] = scale * 2 / 6.0;

		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale, y*scale, scale * 2 / 3.0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale + scale, y*scale, scale * 2 / 3.0);
		glTexCoord2d(192.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x * scale + scale - scale / 4.0, y * scale + scale, scale * 2 / 6.0);
		glTexCoord2d(64.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x * scale + scale / 4.0, y * scale + scale, scale * 2 / 6.0);

		//бока

		//---зад---
		_A[0] = x * scale + scale;
		_A[1] = y * scale;
		_A[2] = scale * 2 / 3.0;

		_B[0] = x * scale + scale;
		_B[1] = y * scale;
		_B[2] = 0;

		_C[0] = x * scale;
		_C[1] = y * scale;
		_C[2] = 0;

		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(0, 1);
		glVertex3f(x * scale, y * scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x * scale + scale, y * scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale, scale * 2 / 3.0);

		//---правая щека---
		_A[0] = x * scale + scale - scale / 4.0;
		_A[1] = y * scale + scale;
		_A[2] = 0;

		_B[0] = x * scale + scale;
		_B[1] = y * scale;
		_B[2] = 0;

		_C[0] = x * scale + scale;
		_C[1] = y * scale;
		_C[2] = scale * 2 / 3.0;

		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(253.0 / 512.0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale + scale, y * scale, 0);
		glTexCoord2d(151.0 / 512.0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale + scale - scale / 4.0, y * scale + scale, 0);
		glTexCoord2d(151.0 / 512.0, 1 - 380.0 / 512.0);
		glVertex3f(x * scale + scale - scale / 4.0, y * scale + scale, scale * 2 / 6.0);
		glTexCoord2d(253.0 / 512.0, 1 - 347.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale, scale * 2 / 3.0);
		
		//---перед---
		_A[0] = x * scale + scale / 4.0;
		_A[1] = y * scale + scale;
		_A[2] = scale * 2 / 6.0;

		_B[0] = x * scale + scale / 4.0;
		_B[1] = y * scale + scale;
		_B[2] = 0;

		_C[0] = x * scale + scale - scale / 4.0;
		_C[1] = y * scale + scale;
		_C[2] = 0;

		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(102.0 / 512.0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale + scale - scale / 4.0, y * scale + scale, 0);
		glTexCoord2d(151.0 / 512.0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale + scale / 4.0, y * scale + scale, 0);
		glTexCoord2d(151.0 / 512.0, 1 - 380.0 / 512.0);
		glVertex3f(x * scale + scale / 4.0, y * scale + scale, scale * 2 / 6.0);
		glTexCoord2d(102.0 / 512.0, 1 - 380.0 / 512.0);
		glVertex3f(x * scale + scale - scale / 4.0, y * scale + scale, scale * 2 / 6.0);

		//---левая щека---
		_A[0] = x * scale;
		_A[1] = y * scale;
		_A[2] = 0;

		_B[0] = x * scale + scale / 4.0;
		_B[1] = y * scale + scale;
		_B[2] = 0;

		_C[0] = x * scale + scale / 4.0;
		_C[1] = y * scale + scale;
		_C[2] = scale * 2 / 6.0;

		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale, y * scale, 0);
		glTexCoord2d(102.0 / 512.0, 1 - 412.0 / 512.0);
		glVertex3f(x * scale + scale / 4.0, y * scale + scale, 0);
		glTexCoord2d(102.0 / 512.0, 1 - 380.0 / 512.0);
		glVertex3f(x * scale + scale / 4.0, y * scale + scale, scale * 2 / 6.0);
		glTexCoord2d(0, 1 - 347.0 / 512.0);
		glVertex3f(x*scale, y*scale, scale * 2 / 3.0);

		glEnd();

		glPopMatrix();
	}

	void DrawTail()
	{
		if (block[size - 1].y < block[size - 2].y)
			angleTail = 0;
		if (block[size - 1].y > block[size - 2].y)
			angleTail = 180;
		if (block[size - 1].x < block[size - 2].x)
			angleTail = -90;
		if (block[size - 1].x > block[size - 2].x)
			angleTail = 90;

		glPushMatrix();

		glTranslated(block[size-1].x + scale / 2.0, block[size - 1].y + scale / 2.0, 0);
		glRotated(angleTail, 0, 0, 1);
		glTranslated(-block[size - 1].x - scale / 2.0, -block[size - 1].y - scale / 2.0, 0);


		glColor3f(1.0, 0.5, 0.0); //orange
		glBegin(GL_QUADS);

		//низ
		glNormal3d(0, 0, -1);

		glTexCoord2d(0, 1);
		glVertex3f(x * scale + scale/2.0, y * scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x * scale + scale - scale / 2.0, y * scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x * scale + scale, y * scale + scale, 0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x * scale, y * scale + scale, 0);

		//верх
		double _A[] = { x*scale, y*scale + scale, scale * 2 / 3.0 };
		double _B[] = { x*scale + scale, y*scale + scale, scale * 2 / 3.0 };
		double _C[] = { x*scale + scale, y*scale + scale, 0};
		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale + scale/2.0, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale + scale - scale/2.0, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale + scale, scale * 2 / 3.0);

		//бока
		glColor3f(1.0, 0.5, 0.0); //orange

		//правая сторона хвоста
		_A[0] = x*scale + scale;
		_A[1] = y*scale + scale;
		_A[2] = scale * 2 / 3.0;

		_B[0] = x*scale + scale;
		_B[1] = y*scale + scale;
		_B[2] = 0;

		_C[0] = x*scale + scale - scale / 2.0;
		_C[1] = y*scale;
		_C[2] = 0;

		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale + scale - scale / 2.0, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale + scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale - scale / 2.0, y*scale, 0);

		//перед хвоста змеи
		glNormal3d( 0,1,0);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale + scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale, y*scale + scale, scale * 2 / 3.0);

		//левая сторона хвоста
		_A[0] = x*scale + scale / 2.0;
		_A[1] = y*scale;
		_A[2] = 0;

		_B[0] = x*scale;
		_B[1] = y*scale + scale;
		_B[2] = 0;

		_C[0] = x*scale;
		_C[1] = y*scale + scale;
		_C[2] = scale * 2 / 3.0;

		normal(_A, _B, _C, vn);
		glNormal3dv(vn);

		glTexCoord2d(0, 1);
		glVertex3f(x*scale + scale / 2.0, y*scale, 0);
		glTexCoord2d(256.0 / 512.0, 1);
		glVertex3f(x*scale, y*scale + scale, 0);
		glTexCoord2d(256.0 / 512.0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale, y*scale + scale, scale * 2 / 3.0);
		glTexCoord2d(0, 1 - 256.0 / 512.0);
		glVertex3f(x*scale + scale / 2.0, y*scale, 0);

		glEnd();
		glPopMatrix();
	}

}block[30 * 20];

class DrawBlock { //класс отрисовки блоков
public:
	int x, y; //координаты блока
	void Cobbles() //блок брусчатки
	{
		double m = scale * 0;
		double A[] = { x + m,y + m,0 - scale };
		double B[] = { x + scale - m,y + m,0 - scale };
		double C[] = { x + scale - m,y + scale - m,0 - scale };
		double D[] = { x + m,y + scale - m,0 - scale };
		double A1[] = { x + m,y + m,0 };
		double B1[] = { x + scale - m,y + m,0 };
		double C1[] = { x + scale - m,y + scale - m,0 };
		double D1[] = { x + m,y + scale - m,0 };

		glColor3d(0.0, 1.0, 0.0); //green
		glBegin(GL_TRIANGLES);
			////низ
			normal(D, A, B, vn);
			glNormal3dv(vn);

			glTexCoord2d(0, 512.0 / 512.0);
			glVertex3dv(A);
			glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(B);
			glTexCoord2d(0, (512.0-128.0) / 512.0);
			glVertex3dv(D);

			normal(B, C, D, vn);
			glNormal3dv(vn);

			glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(B);
			glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(C);
			glTexCoord2d(0, (512.0 - 128.0) / 512.0);
			glVertex3dv(D);
			//верх
			normal(B1, A1, D1, vn);
			glNormal3dv(vn);

			glTexCoord2d(256.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(A1);
			glTexCoord2d(384.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(B1);
			glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(D1);

			normal(D1, C1, B1, vn);
			glNormal3dv(vn);

			glTexCoord2d(384.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(B1);
			glTexCoord2d(384.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(C1);
			glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(D1);
			//бока
			//A-A1-D1-D
			normal(D1, A1, A, vn);
			glNormal3dv(vn);

			glTexCoord2d(384.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(A);
			glTexCoord2d(384.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(A1);
			glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(D1);

			normal(A, D, D1, vn);
			glNormal3dv(vn);

			glTexCoord2d(384.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(A);
			glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(D1);
			glTexCoord2d(256.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(D);
			//A-A1-B1-B
			normal(A1, B1, B, vn);
			glNormal3dv(vn);

			glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(B);
			glTexCoord2d(256.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(B1);
			glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(A1);

			normal(B, A, A1, vn);
			glNormal3dv(vn);

			glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(B);
			glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
			glVertex3dv(A1);
			glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(A);
			//B-B1-C1-C
			normal(B1, C1, C, vn);
			glNormal3dv(vn);

			glTexCoord2d(128.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(C);
			glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(C1);
			glTexCoord2d(0, (512.0 - 128.0) / 512.0);
			glVertex3dv(B1);

			normal(C, B, B1, vn);
			glNormal3dv(vn);

			glTexCoord2d(128.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(C);
			glTexCoord2d(0, (512.0 - 128.0) / 512.0);
			glVertex3dv(B1);
			glTexCoord2d(0, (512.0 - 256.0) / 512.0);
			glVertex3dv(B);
			//C-C1-D1-D
			normal(C1, D1, D, vn);
			glNormal3dv(vn);

			glTexCoord2d(256.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(D);
			glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(D1);
			glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(C1);

			normal(D, C, C1, vn);
			glNormal3dv(vn);

			glTexCoord2d(256.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(D);
			glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
			glVertex3dv(C1);
			glTexCoord2d(128.0 / 512.0, (512.0 - 256.0) / 512.0);
			glVertex3dv(C);
		glEnd();
	}
} drawBlock;

void DrawField() // функция, которая отрисовывает поле 
{
	materials.field();

	glBindTexture(GL_TEXTURE_2D, texId[0]);

	for (drawBlock.x = 0; drawBlock.x < w; drawBlock.x += scale)
	{
		for (drawBlock.y = 0; drawBlock.y < h; drawBlock.y += scale)
		{
			drawBlock.Cobbles();
		}
	}

	if (gameIsOver)
	{
		drawBlock.x = block[0].x;
		drawBlock.y = block[0].y;
		drawBlock.Cobbles();
	}
}

void DrawCylinder() // функция, которая отрисовывает цилиндр 
{
	materials.field();
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	
	GLfloat theta;
	GLfloat pi = acos(-1.0); //пи
	GLfloat r = 2 * w; // радиус
	double center[] = { w/2.0,h/2.0,r };

	//низ цилиндра
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 0, 1);

	glTexCoord2d(0.5, 0.5);
	glVertex3f(center[0], center[1], - center[2]);

	for (GLfloat a = 0.0f; a <= 360.0f; a++) {
		theta = pi * a / 180.0f;

		GLdouble _x = 0.5 + 0.5 * cos(theta);
		GLdouble _y = 0.5 + 0.5 * sin(theta);

		glTexCoord2d(_x, _y);
		glVertex3f(r * cos(theta) + center[0], r * sin(theta) + center[1], -center[2]);
	}
	glEnd();

	//бока цилиндра
	glBindTexture(GL_TEXTURE_2D, texId[3]);

	glBegin(GL_QUAD_STRIP);


	for (GLfloat a = 0.0f; a <= 360.0f; a++) {
		theta = pi * a / 180.0f;

		double _low1[] = { r * cos(theta) + center[0], r * sin(theta) + center[1], -center[2] };
		double _up1[] = { r * cos(theta) + center[0], r * sin(theta) + center[1], center[2] };

		GLfloat _theta = pi * (a + 1.0) / 180.0f;
		double _up2[] = { r * cos(_theta) + center[0], r * sin(_theta) + center[1], center[2] };

		normal(_up2, _up1, _low1, vn);
		glNormal3dv(vn);

		glTexCoord2d(a / 512.0, 1.0);

		glVertex3dv(_up1);

		glTexCoord2d(a / 512.0, 1.0 - 360.0 / 512.0);

		glVertex3dv(_low1);
	}
	glEnd();
	
}


void DrawSnake()
{
	materials.snake();

	glBindTexture(GL_TEXTURE_2D, texId[2]);

	block[0].DrawHead(); //рисуем голову
	
	for (int i = 1; i < size-1; i++) //рисуем тело
	{
		block[i].DrawBody();
	}

	block[size - 1].DrawTail(); //рисуем хвост
}

class Boxes //класс для коробок-препятствий
{
public:
	float x;
	float y;

	Boxes()
	{
		x = rand() % N; //координата x еды в рандомном месте 
		y = rand() % M; //координата y еды в рандомном месте
	}

	void spawn()
	{
		int flag = 1; //изначально 1, чтобы зайти в первый раз в while
		while (flag == 1) //пока коробка спавнется в точке, где змейка
		{
			x = rand() % N; //координата x коробки в рандомном месте 
			y = rand() % M; //координата y коробки в рандомном месте

			flag = 0; //сбрасываем

			for (int i = 0; i < size; i++)
			{
				if (block[i].x == x && block[i].y == y)
					flag = 1; //коробка появилась в змейке
			}

			for (int i = 0; i < 5; i++)
			{
				if (boxes[i].x == x && boxes[i].y == y)
					flag = 1; //коробка появилась в коробке
			}
		}
	}

	void draw()
	{
		materials.field();

		glBindTexture(GL_TEXTURE_2D, texId[5]); //текстуры коробки

		double m = scale * 0;
		double A[] = { x + m,y + m,0 };
		double B[] = { x + scale - m,y + m,0 };
		double C[] = { x + scale - m,y + scale - m,0 };
		double D[] = { x + m,y + scale - m,0 };
		double A1[] = { x + m,y + m,0 + scale };
		double B1[] = { x + scale - m,y + m,0 + scale };
		double C1[] = { x + scale - m,y + scale - m,0 + scale };
		double D1[] = { x + m,y + scale - m,0 + scale };

		glColor3d(0.0, 1.0, 0.0); //green
		glBegin(GL_TRIANGLES);
		////низ
		normal(D, A, B, vn);
		glNormal3dv(vn);

		glTexCoord2d(0, 512.0 / 512.0);
		glVertex3dv(A);
		glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(B);
		glTexCoord2d(0, (512.0 - 128.0) / 512.0);
		glVertex3dv(D);

		normal(B, C, D, vn);
		glNormal3dv(vn);

		glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(B);
		glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(C);
		glTexCoord2d(0, (512.0 - 128.0) / 512.0);
		glVertex3dv(D);
		//верх
		normal(B1, A1, D1, vn);
		glNormal3dv(vn);

		glTexCoord2d(256.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(A1);
		glTexCoord2d(384.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(B1);
		glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(D1);

		normal(D1, C1, B1, vn);
		glNormal3dv(vn);

		glTexCoord2d(384.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(B1);
		glTexCoord2d(384.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(C1);
		glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(D1);
		//бока
		//A-A1-D1-D
		normal(D1, A1, A, vn);
		glNormal3dv(vn);

		glTexCoord2d(384.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(A);
		glTexCoord2d(384.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(A1);
		glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(D1);

		normal(A, D, D1, vn);
		glNormal3dv(vn);

		glTexCoord2d(384.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(A);
		glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(D1);
		glTexCoord2d(256.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(D);
		//A-A1-B1-B
		normal(A1, B1, B, vn);
		glNormal3dv(vn);

		glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(B);
		glTexCoord2d(256.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(B1);
		glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(A1);

		normal(B, A, A1, vn);
		glNormal3dv(vn);

		glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(B);
		glTexCoord2d(128.0 / 512.0, 512.0 / 512.0);
		glVertex3dv(A1);
		glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(A);
		//B-B1-C1-C
		normal(B1, C1, C, vn);
		glNormal3dv(vn);

		glTexCoord2d(128.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(C);
		glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(C1);
		glTexCoord2d(0, (512.0 - 128.0) / 512.0);
		glVertex3dv(B1);

		normal(C, B, B1, vn);
		glNormal3dv(vn);

		glTexCoord2d(128.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(C);
		glTexCoord2d(0, (512.0 - 128.0) / 512.0);
		glVertex3dv(B1);
		glTexCoord2d(0, (512.0 - 256.0) / 512.0);
		glVertex3dv(B);
		//C-C1-D1-D
		normal(C1, D1, D, vn);
		glNormal3dv(vn);

		glTexCoord2d(256.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(D);
		glTexCoord2d(256.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(D1);
		glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(C1);

		normal(D, C, C1, vn);
		glNormal3dv(vn);

		glTexCoord2d(256.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(D);
		glTexCoord2d(128.0 / 512.0, (512.0 - 128.0) / 512.0);
		glVertex3dv(C1);
		glTexCoord2d(128.0 / 512.0, (512.0 - 256.0) / 512.0);
		glVertex3dv(C);
		glEnd();

	}
}boxes[5];

class food //класс для еды, которую ест змейка
{
public:
	float x;
	float y;

	food()
	{
		srand(time(0));
		x = rand() % N; //координата x еды в рандомном месте 
		y = rand() % M; //координата y еды в рандомном месте
	}

	void spawn()
	{
		int flag = 1; //изначально 1, чтобы зайти в первый раз в while
		while (flag == 1) //пока пицца спавнется в точке, где змейка
		{
			x = rand() % N; //координата x еды в рандомном месте 
			y = rand() % M; //координата y еды в рандомном месте

			flag = 0; //сбрасываем

			for (int i = 0; i < size; i++)
			{
				if (block[i].x == x && block[i].y == y)
					flag = 1; //еда появилась в змейке
			}

			for (int i = 0; i < size; i++)
			{
				if (boxes[i].x == x && boxes[i].y == y)
					flag = 1; //еда появилась в коробке
			}
		}		
	}
	
	void draw()
	{
		materials.pizza();

		glBindTexture(GL_TEXTURE_2D, texId[1]); //текстуры пиццы
		
		double center[] = { x + scale / 2.0, y + scale / 2.0,0.6*scale };
		GLfloat theta;
		GLfloat pi = acos(-1.0); //пи
		GLfloat radius = scale / 2.0; // радиус

		glColor3f(1.0f, 0.0f, 0.0f); //красный
		//верх пиццы
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 0, 1);

		glTexCoord2d(85.0 / 512.0, 1 - 85.0 / 512.0);
		glVertex3f(center[0], center[1], center[2]);

		for (GLfloat a = 0.0f; a <= 360.0f; a++) {
			theta = pi * a / 180.0f;

			GLdouble _x = 85.0 / 512.0 + 85.0 / 512.0 * cos(theta);
			GLdouble _y = 1 - 85.0 / 512.0 + 85.0 / 512.0 * sin(theta);

			glTexCoord2d(_x, _y);
			glVertex3f(radius * cos(theta) + center[0], radius * sin(theta) + center[1], center[2]);
		}
		glEnd();

		//низ пиццы
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 0, -1);

		glTexCoord2d(255.0 / 512.0, 1 - 85.0 / 512.0);
		glVertex3f(center[0], center[1], center[2] - 0.1 * scale);

		for (GLfloat a = 0.0f; a <= 360.0f; a++) {
			theta = pi * a / 180.0f;

			GLdouble _x = 255.0 / 512.0 + 85.0 / 512.0 * cos(theta);
			GLdouble _y = 1 - 85.0 / 512.0 + 85.0 / 512.0 * sin(theta);

			glTexCoord2d(_x, _y);
			glVertex3f(radius * cos(theta) + center[0], radius * sin(theta) + center[1], center[2] - 0.1 * scale);
		}
		glEnd();

		//бока пиццы
		glBegin(GL_QUAD_STRIP);
		

		for (GLfloat a = 0.0f; a <= 360.0f; a++) {
			theta = pi * a / 180.0f;

			double _low1[] = { radius * cos(theta) + center[0], radius * sin(theta) + center[1], center[2] - 0.1 * scale };
			double _up1[] = { radius * cos(theta) + center[0], radius * sin(theta) + center[1], center[2] };

			GLfloat _theta = pi * (a + 1.0) / 180.0f;
			double _up2[] = { radius * cos(_theta) + center[0], radius * sin(_theta) + center[1], center[2] };

			normal(_low1, _up1, _up2, vn);
			glNormal3dv(vn);

			glTexCoord2d(a / 512.0, 1.0 - 170.0 / 512.0);

			glVertex3f(radius * cos(theta) + center[0], radius * sin(theta) + center[1], center[2]);

			glTexCoord2d(a / 512.0, 1.0 - 210.0 / 512.0);

			glVertex3f(radius * cos(theta) + center[0], radius * sin(theta) + center[1], center[2] - 0.1 * scale);
		}
		glEnd();
	}
	void animate()
	{
		glPushMatrix(); //вращение пиццы
			glTranslated(food1.x + scale / 2.0, food1.y + scale / 2.0, 0);
			glRotated(Angle++, 0, 0, 1);
			glTranslated(-food1.x - scale / 2.0, -food1.y - scale / 2.0, 0);
			food1.draw(); //рисуем еду
		glPopMatrix();
	}
}food1;

class Game { //класс "игры"
public:
	void gameover() //функция конца игры
	{
		gameIsOver = true;

		moveX1 = 0;
		moveY1 = 0;
		moveZ1 = 0;
		viewMode = 1;

		PlaySound(TEXT("main.wav"), NULL, SND_ASYNC);
	}
	void restart() //функция рестарта
	{
		PlaySound(NULL, NULL, SND_ASYNC);

		size = 3; //сбрасываем длину змейки

				  //все части змейки в начальную точку
		block[0].x = 0;
		block[0].y = 0;
		block[1].x = 0;
		block[1].y = 0;
		block[2].x = 0;
		block[2].y = 0;

		direction = 1; //направление головы наверх

		score = 0; //сброс очков

		food1.spawn(); //респавн еды

		isRestarted = true;

		gameIsOver = false;
	}
} game;

void Move() //для движения змейки
{	
	for (int i = size; i > 0; i--) //двигаем все блоки тела змейки следом за головой
	{
		block[i].x = block[i - 1].x;
		block[i].y = block[i - 1].y;
	}	
	
	switch (direction) //в зависимости от направления движения перемещаем голову в нужную сторону на 1 клетку
	{
		
	case 1: block[0].y += scale; break; //вверх
	case 2: block[0].x -= scale; break; //влево
	case 3: block[0].y -= scale; break; //вниз
	case 4: block[0].x += scale; break; //вправо
	}

	if ((block[0].x == food1.x) && (block[0].y == food1.y)) //если змейка съела еду
	{
		PlaySound(TEXT("coin.wav"), NULL, SND_ASYNC);

		if (limit >= 0)
			limit -= 0.1;
		size++; //длина змейки +1
		food1.spawn(); //респавн еды 
		score+=10;
	}

	//в зависимости от режима вида, смотрим либо за головой змейки, либо сверху
	if (viewMode == 1)
	{
		moveX1 = 0;
		moveY1 = 0;
		moveZ1 = 0;
	}
	else if (viewMode == 2)
	{
		moveX1 = w / 2.0 - scale / 2.0 - block[0].x;
		moveY1 = h / 2.0 - scale / 2.0 - block[0].y;
		moveZ1 = 0 - 15;
	}
	//--------

	//Проверка, не вышла ли змейка за пределы поля
	if (block[0].x < 0)
		game.gameover();
	if (block[0].x > N - 1)
		game.gameover();
	if (block[0].y < 0)
		game.gameover();
	if (block[0].y > M - 1)
		game.gameover();
	


	for (int i = 1; i < size; i++) //Проверка, не укусила ли змейка себя
	{
		if ((block[0].x == block[i].x) && (block[0].y == block[i].y)) //если укусила
		{
			game.gameover();
		}
	}

	for (int i = 0; i < 5; i++) //Проверка, не врезалась ли змейка в коробку
	{
		if ((block[0].x == boxes[i].x) && (block[0].y == boxes[i].y)) //если врезалась
		{
			game.gameover();
		}
	}
}

bool textureMode = true;
bool lightMode = true;

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 25;
		fi1 = acos(-1) / 2;
		fi2 = acos(-1) / 2;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{

		if (gameIsOver) //если игра окончена
		{
			fi1 += 0.01;
			fi2 = acos(-1) * 10.0 / 180.0;
		}
		else if (isRestarted)
		{
			fi1 = acos(-1) * 90.0 / 180.0;
			fi2 = acos(-1) * 90.0 / 180.0;
			isRestarted = false;
		}

		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(moveX1, moveY1, 0);

		//ограничение поворота камеры, чтобы камера не уходила под поверхность
		if (fi2 <= acos(-1) * 10.0 / 180.0)
			fi2 = acos(-1) * 10.0 / 180.0;
		else if (fi2 >= acos(-1) * 90.0 / 180.0)
			fi2 = acos(-1) * 90.0 / 180.0;
		//----

		double posZ;

		if (camDist*sin(fi2) <= 2)
			posZ = 2;
		else
			posZ = camDist*sin(fi2);

		//----

		

		pos.setCoords(moveX1 + camDist*cos(fi2)*cos(fi1),
			moveY1 + camDist*cos(fi2)*sin(fi1),
			posZ);

		
		if (pos.X() > 1.5 * w)
			pos = Vector3(1.4 * w, pos.Y(), pos.Z());
		else if (pos.X()<-1.5 * w)
			pos = Vector3(-1.4* w, pos.Y(), pos.Z());
		if (pos.Y() > 1.5 * w)
			pos = Vector3(pos.X(), 1.4 * w, pos.Z());
		else if (pos.Y()<-1.5 * w)
			pos = Vector3(pos.X(), -1.4 * w, pos.Z());
		if (pos.Z() > 1.5 * w)
			pos = Vector3(pos.X(), pos.Y(), 1.4 * w);
		else if (pos.Z()<-1.5 * w)
			pos = Vector3(pos.X(), pos.Y(), -1.4 * w);

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(w / 2.0, h / 2.0, 25.0);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		//s.scale = s.scale*0.08;
		s.scale = s.scale * 0;
		s.Show();
	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой правой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;
	
	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = acos(-1) / 2;
		camera.fi2 = acos(-1) / 2;
		camera.camDist = 25;

		light.pos = Vector3(w / 2.0, h / 2.0, 25.0);

	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

	if (key == 'W' && direction != 3 && viewMode == 1)
	{
		if (block[0].y + scale != block[1].y) direction = 1;
	}
	if (key == 'A' && direction != 4 && viewMode == 1)
	{
		if (block[0].x - scale != block[1].x) direction = 2;
	}
	if (key == 'S' && direction != 1 && viewMode == 1)
	{
		if (block[0].y - scale != block[1].y) direction = 3;
	}
	if (key == 'D' && direction != 2 && viewMode == 1)
	{
		if (block[0].x + scale != block[1].x) direction = 4;
	}

	if (key == 'Q')
	{
		switch (direction)
		{
		case 1: if (block[0].x - scale != block[1].x) direction = 2; break;
		case 2: if (block[0].y - scale != block[1].y) direction = 3; break;
		case 3: if (block[0].x + scale != block[1].x) direction = 4; break;
		case 4: if (block[0].y + scale != block[1].y) direction = 1; break;
		}
	}
	if (key == 'E')
	{
		switch (direction)
		{
		case 1: if (block[0].x + scale != block[1].x) direction = 4; break;
		case 2: if (block[0].y + scale != block[1].y) direction = 1; break;
		case 3: if (block[0].x - scale != block[1].x) direction = 2; break;
		case 4: if (block[0].y - scale != block[1].y) direction = 3; break;
		}
	}

	if (key == 'O') //< понижение скорости
	{ 
		if (limit < 30)
		{
			speed--;
			limit += 5;
		}
	}
	if (key == 'P') //> повышение скорости
	{      
		if (limit >= 0)
		{
			speed++;
			limit -= 5;
		}
	}

	if (key == 'V' && viewMode == 1) //вид сверху, по ум.
	{
		viewMode = 2;
	}

	else if (key == 'V' && viewMode == 2) //вид из-за головы
	{
		viewMode = 1;
	}

	if (key == 'M' && gameIsOver == true) //рестарт
	{
		game.restart();
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}




//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);


	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;

						  //массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	glGenTextures(6, texId); //менять 1 параметр по количеству текстур

	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId[0]);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//текстура пиццы
	*texarray; //я
	*texCharArray; //я

	OpenGL::LoadBMP("pizza.bmp", &texW, &texH, &texarray); //я
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray); //я

	glBindTexture(GL_TEXTURE_2D, texId[1]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray); //я

	free(texCharArray); //я
	free(texarray); //я
					 //наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//текстура змейки
	*texarray; //я
	*texCharArray; //я

	OpenGL::LoadBMP("snake.bmp", &texW, &texH, &texarray); //я
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray); //я

	glBindTexture(GL_TEXTURE_2D, texId[2]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray); //я

	free(texCharArray); //я
	free(texarray); //я
					//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//текстура для цилиндра
	*texarray; //я
	*texCharArray; //я

	OpenGL::LoadBMP("greenstones.bmp", &texW, &texH, &texarray); //я
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray); //я

	glBindTexture(GL_TEXTURE_2D, texId[3]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray); //я

	free(texCharArray); //я
	free(texarray); //я
					//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//текстура для монстра на дно цилиндра
	*texarray; //я
	*texCharArray; //я

	OpenGL::LoadBMP("monster.bmp", &texW, &texH, &texarray); //я
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray); //я

	glBindTexture(GL_TEXTURE_2D, texId[4]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray); //я

	free(texCharArray); //я
	free(texarray); //я
					//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//текстура для коробок-препятствий
	*texarray; //я
	*texCharArray; //я

	OpenGL::LoadBMP("box.bmp", &texW, &texH, &texarray); //я
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray); //я

	glBindTexture(GL_TEXTURE_2D, texId[5]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray); //я

	free(texCharArray); //я
	free(texarray); //я
					//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//===конец текстур===

	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH);


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
}









void Display() //функция общей отрисовки 
{
	DrawCylinder(); //цилиндр (внешнее пространство)

	DrawField(); //рисуем поле

	DrawSnake(); //рисуем змейку

	for (int i = 0; i < 5; i++) //рисуем коробки
	{
		boxes[i].draw();
	}

	food1.animate(); //анимированная пицца
}

void Render(OpenGL *ogl)
{       	
	
	
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	materials.basic();

	//Прогать тут  

	
	glRotated(180, 0, 0, 1);
	glTranslated(-N / 2, -M / 2, 0);
	

	
	Display();

	if (count < limit)
	{
		count++;
	}
	else
	{
		if (!gameIsOver) //если игра не окончена
		{
			Move();
		}

		count = 0;
	}


	



	//текст сообщения вверху слева, если надоест - закоментировать, или заменить =)
	if (gameIsOver && block[0].x != block[1].x != block[0].y != block[1].y)
	{
		char c[500];  //максимальная длина сообщения
		sprintf_s(c, "(T)Текстуры - %d (L)Свет - %d\n"
			"F - переместить свет в точку камеры\n"
			"R - установить камеру и свет в начальное положение\n"
			"Управление:\nO/P - замедление/ускорение,\n W,A,S,D - движение\n Q,E - поворот\n V - смена вида\n"
			"SCORE: %d\n"
			"SPEED: %d\n"
			"GAME OVER!\n M - рестарт", textureMode, lightMode, score, speed);
		ogl->message = std::string(c);
	}
	else
	{
		char c[500];  //максимальная длина сообщения
		sprintf_s(c, "(T)Текстуры - %d (L)Свет - %d\n"
			"F - переместить свет в точку камеры\n"
			"R - установить камеру и свет в начальное положение\n"
			"Управление:\nO/P - замедление/ускорение,\n W,A,S,D - движение\n Q,E - поворот\n V - смена вида\n"
			"\n SCORE: %d\n"
			"SPEED: %d", textureMode, lightMode, score, speed);
		ogl->message = std::string(c);
	}
	
	
	////текст сообщения вверху слева, если надоест - закоментировать, или заменить =)
	//char c[350];  //максимальная длина сообщения
	//sprintf_s(c, "(T)Текстуры - %d\n(L)Свет - %d\n\nУправление:\n"
	//	"O/P - замедление/ускорение,\n W,A,S,D - движение\n Q,E - поворот\n"
	//	"R - установить камеру и свет в начальное положение\n"
	//	"F - переместить свет в точку камеры\n"
	//	"SCORE: %d\n"
	//	"LIMIT: %f\n"
	//	"COUNT: %f", textureMode, lightMode, score, limit, count);
	//ogl->message = std::string(c);




}   //конец тела функции

