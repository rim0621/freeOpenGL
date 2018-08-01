#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <deque>
#include<cstdlib>
#include<unistd.h>
#include<iostream>
#include<pthread.h>

using namespace std;


pthread_mutex_t mutex_lock;
int prex=90;
int pret=0;
int i=1;
int move;
deque<int> dqx;
deque<int> dqy;
deque<int> dqz;


void *getData(void *arg)	//통신으로 받는 데이터 등등... 여기서는 x , y ,z 를(y축) 받을 것... x축은 t시간(셈플링)
{
		while(1){

			pthread_mutex_lock(&mutex_lock);
			dqx.push_back((-90)+rand()%180);
			dqy.push_back((-90)+rand()%180);
			dqz.push_back((-90)+rand()%180);
			if(dqx.size()>100)
			{
				dqx.pop_front();
				dqy.pop_front();
				dqz.pop_front();
			}
			pthread_mutex_unlock(&mutex_lock);
			usleep(50000);
	}
}


/* Sample func itself 중심선 긋기 용*/
float func(float x)
{
	return x;
}

/* Function plotting func */
void draw(float (* func)(float x), float x1, float x2, float y1, float y2)
{


	glViewport(0, 0, 1000, 100);	//(전체 0~300)0부터 100까지
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix(); /* GL_MODELVIEW is default */


	glScalef(1.0 / (x2 - x1), 1.0 / (y2 - y1), 1.0);
	//비율로 들어가니깐...
	glTranslatef(0, 0, 0.0);
	glColor3f(1.0, 0.0, 1.0);

	glBegin(GL_LINE_STRIP);
	glVertex2f(0, func(90));
	glVertex2f(1000, func(90));
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(1, func(0));
	glVertex2f(1, func(180));
	glEnd();

	pthread_mutex_lock(&mutex_lock);	//데이터를 그릴때 getData에서 데이터를 수집하는걸 방지하기 위해 수집되면 에러
	for(deque<int>::iterator iter=dqz.begin();iter!=dqz.end();++iter)
	{

		glBegin(GL_LINE_STRIP);
		glVertex2f(pret, func(prex)); //pre
		glVertex2f(i, func(90+ *iter)); //90+@  next -> pre
		glEnd();
		pret=i;
		i+=10;
		prex=90+ *iter;
		glFlush();
	}
	glPopMatrix();
	pthread_mutex_unlock(&mutex_lock);

	prex=90;
	pret=0;
	i=1;

	///////////////////////////////////Viewport 1 end


	glViewport(0, 100, 1000, 100); //100부터 ~ +100개
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix(); /* GL_MODELVIEW is default */

	glScalef(1.0 / (x2 - x1), 1.0 / (y2 - y1), 1.0);
	//비율로 들어가니깐...
	glTranslatef(0, 0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINE_STRIP);
	glVertex2f(0, func(90));
	glVertex2f(1000, func(90));
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(1, func(0));
	glVertex2f(1, func(180));
	glEnd();

	pthread_mutex_lock(&mutex_lock);
	for(deque<int>::iterator iter=dqy.begin();iter!=dqy.end();++iter)
	{

		glBegin(GL_LINE_STRIP);
		glVertex2f(pret, func(prex)); //pre
		glVertex2f(i, func(90+ *iter)); //90+@  next -> pre
		glEnd();
		pret=i;
		i+=10;
		prex=90+ *iter;
		glFlush();
	}

	glPopMatrix();
	pthread_mutex_unlock(&mutex_lock);
	///////////////////////////////////Viewport 2 end


	prex=90;
	pret=0;
	i=1;


	glViewport(0, 200, 1000, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix(); /* GL_MODELVIEW is default */

	glScalef(1.0 / (x2 - x1), 1.0 / (y2 - y1), 1.0);
	//비율로 들어가니깐...
	glTranslatef(0, 0, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_LINE_STRIP);
	glVertex2f(0, func(90));
	glVertex2f(1000, func(90));
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(1, func(0));
	glVertex2f(1, func(180));
	glEnd();

	pthread_mutex_lock(&mutex_lock);
	for(deque<int>::iterator iter=dqx.begin();iter!=dqx.end();++iter)
	{

		glBegin(GL_LINE_STRIP);
		glVertex2f(pret, func(prex)); //pre
		glVertex2f(i, func(90+ *iter)); //90+@  next -> pre
		glEnd();
		pret=i;
		i+=10;
		prex=90+ *iter;
		glFlush();
	}


	glPopMatrix();
	pthread_mutex_unlock(&mutex_lock);
	usleep(1000);

	prex=90;
	pret=0;
	i=1;

	///////////////////////////////////Viewport 3 end

};





/* Redrawing func */
void redraw(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	draw(func, 0, 1000, 0, 180);

	glutSwapBuffers();
}

/* Idle proc. Redisplays, if called. */
void idle(void)
{
	glutPostRedisplay();
}

/* Key press processing */
void key(unsigned char c, int x, int y)
{
	if(c == 27) exit(0);
}

/* Window reashape */

void reshape(int w, int h)
{


	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
