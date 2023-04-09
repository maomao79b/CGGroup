#include <gl\gl.h>
#include <gl\glut.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>
#include "Polygon.c"

void show(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (int i = 0; i < num_polygons; i++)
    {
        glColor3f(polygons[i].red, polygons[i].green, polygons[i].blue);
        if(polygons[i].state == 'p'){
            draw_polygon(polygons[i]);
        }
        else if(polygons[i].state == 's'){
            draw_star(polygons[i]);
        }
    }
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Polygons");
    glutDisplayFunc(show);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    myinit();
    glutMainLoop();
    return 0;
}
