#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

//#define DEBUG
//#define PERSPECTIVE

static int Width = 200;
static int Height = 200;

int wnd_width, wnd_height;
int mouse_down_x, mouse_down_y, mouse_move_x, mouse_move_y;


char DrawMethod = 2;
GLenum displaymode = GL_LINE;
void menuEvents(int select)
{
	if (select <= 4){
		DrawMethod = select;
		glutPostRedisplay();
	}
	else
	{
		switch (select)
		{
		case 5:
			displaymode = GL_LINE;
			break;
		case 6:
			displaymode = GL_POINT;
			break;
		case 7:
			displaymode = GL_FILL;
		default:
			break;
		}
		glutPostRedisplay();
	}
}

void CreateMenu()
{
	glutCreateMenu(menuEvents);
	glutAddMenuEntry("四边形带", 4);
	glutAddMenuEntry("三角形带", 2);
	glutAddMenuEntry("三角形", 1);
	glutAddMenuEntry("四边形", 3);
	glutAddMenuEntry("边模式", 5);
	glutAddMenuEntry("点模式", 6);
	glutAddMenuEntry("填充模式", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#define Pi (3.141592653)

float R = 1;
float level = 18;
int NumOfSphere = 1;


void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);
	
	glPolygonMode(GL_FRONT_AND_BACK, displaymode);

}

void displayMethod()
{
	init();

	int viewport_column = 0;
	int viewport_row = 0;

	for (int num = 0; num < NumOfSphere; num++)
	{

		if ((viewport_row+1)*Width>wnd_width)
		{
			viewport_row = 0;
			viewport_column++;
		}

		glViewport(Width*(viewport_row++), Height*viewport_column, Width, Height);

		float AngleAcc = Pi / level;
		float phi = AngleAcc;

		float theta = 0;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, R, 0);
		for (theta = 0; theta < 2 * Pi; theta += AngleAcc)
		{
			glVertex3f(R*sin(phi)*cos(theta), R*cos(phi), R*sin(phi)*sin(theta));
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, -R, 0);
		for (theta = 0; theta < 2 * Pi; theta += AngleAcc)
		{
			glVertex3f(R*sin(phi)*cos(theta), -R*cos(phi), R*sin(phi)*sin(theta));
		}
		glEnd();

		switch (DrawMethod)
		{
		case 1:
			glBegin(GL_TRIANGLES);
			while (phi < Pi)
			{
				for (theta = 0; theta < 2 * Pi; theta +=AngleAcc)
				{
					glVertex3f(R*sin(theta)*sin(phi),R*cos(phi),R*cos(theta)*sin(phi));
					glVertex3f(R*sin(theta)*sin(phi+AngleAcc),R*cos(phi+AngleAcc),R*cos(theta)*sin(phi+AngleAcc));
					glVertex3f(R*sin(theta + AngleAcc)*sin(phi),R*cos(phi),R*cos(theta + AngleAcc)*sin(phi));
				}
				phi += AngleAcc;
			}
			glEnd();
			break;
		case 2:
			glBegin(GL_TRIANGLE_STRIP);
		
			while (phi < Pi)
			{
				for (theta = 0; theta < 2 * Pi; theta +=AngleAcc)
				{
					glVertex3f(R*sin(theta)*sin(phi),R*cos(phi),R*cos(theta)*sin(phi));
					glVertex3f(R*sin(theta)*sin(phi+AngleAcc),R*cos(phi+AngleAcc),R*cos(theta)*sin(phi+AngleAcc));
				}
				phi += AngleAcc;
			}
			glEnd();
			break;
		case 3:
			glBegin(GL_QUADS);
			while (phi < Pi)
			{
				for (theta = 0; theta < 2 * Pi; theta +=AngleAcc)
				{
					glVertex3f(R*sin(theta)*sin(phi),R*cos(phi),R*cos(theta)*sin(phi));
					glVertex3f(R*sin(theta)*sin(phi+AngleAcc),R*cos(phi+AngleAcc),R*cos(theta)*sin(phi+AngleAcc));
					glVertex3f(R*sin(theta+AngleAcc)*sin(phi + AngleAcc),R*cos(phi +AngleAcc),R*cos(theta + AngleAcc)*sin(phi + AngleAcc));
					glVertex3f(R*sin(theta+AngleAcc)*sin(phi),R*cos(phi),R*cos(theta+AngleAcc)*sin(phi));
				}
				phi += AngleAcc;
			}
			glEnd();
			break;
		case 4:
			glBegin(GL_QUAD_STRIP);
			while (phi < Pi)
			{
				for (theta = 0; theta < 2 * Pi; theta +=AngleAcc)
				{
					glVertex3f(R*sin(theta)*sin(phi), R*cos(phi), R*cos(theta)*sin(phi));
					glVertex3f(R*sin(theta)*sin(phi+AngleAcc), R*cos(phi+AngleAcc),R*cos(theta)*sin(phi+AngleAcc));
				}
				phi += AngleAcc;
			}
			glEnd();
			break;
		default:
			break;
		}
	}
	glutSwapBuffers();
	glFlush();
}

void MotionMethod(int x, int y)
{
	mouse_move_x = x - mouse_down_x;
	mouse_move_y = y - mouse_down_y;
	glLoadIdentity();
	glRotated(mouse_move_x, 0, 1, 0);
	glRotated(mouse_move_y, 1, 0, 0);
	glutPostRedisplay();
}

void MouseMethod(int Mouse, int State, int x, int y)
{
	if (Mouse == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
	{
		mouse_down_x = x;
		mouse_down_y = y;
	}
}

void keyboardMethod(unsigned char key, int x, int y)
{
	static float ScaleRate = 0.2;

	glLoadIdentity();
	switch (key)
	{
	case 's'://smaller
		R *= (1.0 - ScaleRate);
		break;
	case 'a'://bigger
		//glScalef(0.5, .5, .5);
		R = (R * (1.0 + ScaleRate)) < 1 ? R * (1.0 + ScaleRate) : 1;
		break;
	case 'w'://draw less lines
		level = level - 1 < 2 ? 2 : level - 1;
		break;
	case 'q'://draw more lines
		level += 1;
		break;
	case 'z'://add a sphere
		NumOfSphere++;
		break;
	case 'x'://delete a sphere until one last
		NumOfSphere = NumOfSphere - 1 < 1 ? 1 : NumOfSphere - 1;
		break;
	case '\b'://reset
		glLoadIdentity();
	
	

	}
	glutPostRedisplay();

}

void reshapeMethod(int width, int height)
{
	wnd_width = width;
	wnd_height = height;
	glutPostRedisplay();
}

int main(int argc, char* argv [])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ball");
	CreateMenu();
	glutDisplayFunc(&displayMethod);
	glutMouseFunc(&MouseMethod);
	glutMotionFunc(&MotionMethod);
	glutKeyboardFunc(&keyboardMethod);
	glutReshapeFunc(&reshapeMethod);
	glutMainLoop();
	return 0;
}