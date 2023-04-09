#include <gl\gl.h>
#include <gl\glut.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>

#define MAX_POLYGONS 1000

typedef struct Type_Polygon{
    int sides;
    int radius;
    int cx;
    int cy;
    float red;
    float green;
    float blue;
} The_Polygon;

The_Polygon polygons[MAX_POLYGONS];
int num_polygons = 0;
int selected_polygon = -1; // no polygon is selected initially
int mouse_x = 0, mouse_y = 0;
bool isDragging = false;

void myinit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 600.0);
}

void draw_polygon(The_Polygon p)
{
    float angle = 360.0 / p.sides;
    float x, y;
    glPushMatrix();              // push the current matrix onto the stack
    glTranslatef(p.cx, p.cy, 0); // translate the matrix to the new center point
    glBegin(GL_POLYGON);
    for (int i = 0; i < p.sides; i++)
    {
        x = p.radius * cos(angle * i * 3.14159 / 180.0);
        y = p.radius * sin(angle * i * 3.14159 / 180.0);

        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix(); // pop the previous matrix from the stack
}

void keyboard(unsigned char key, int x, int y)
{
    printf("\nkey : %c\n", key);
    switch (key)
    {
    case '+':
        polygons[selected_polygon].radius += 10;
        glutPostRedisplay();
        break;
    case '-':
        if(polygons[selected_polygon].radius == 10) break;
        polygons[selected_polygon].radius -= 10;
        glutPostRedisplay();
        break;
        break;
    case '0':
        polygons[selected_polygon].sides = 3;
        glutPostRedisplay();
        break;
    case '1':
        polygons[selected_polygon].sides = 4;
        glutPostRedisplay();
        break;
    case '2':
        polygons[selected_polygon].sides = 5;
        glutPostRedisplay();
        break;
    case '3':
        polygons[selected_polygon].sides = 6;
        glutPostRedisplay();
        break;
    case '4':
        polygons[selected_polygon].sides = 7;
        glutPostRedisplay();
        break;
    case '5':
        polygons[selected_polygon].sides = 8;
        glutPostRedisplay();
        break;
    case '6':
        polygons[selected_polygon].sides = 9;
        glutPostRedisplay();
        break;
    case '7':
        polygons[selected_polygon].sides = 10;
        glutPostRedisplay();
        break;
    case '8':
        polygons[selected_polygon].sides = 100;
        glutPostRedisplay();
        break;
    case 'r': // set color to red
        polygons[selected_polygon].red = 1.0f;
        polygons[selected_polygon].green = 0.0f;
        polygons[selected_polygon].blue = 0.0f;
        glutPostRedisplay();
        break;
    case 'g': // set color to green
        polygons[selected_polygon].red = 0.0f;
        polygons[selected_polygon].green = 1.0f;
        polygons[selected_polygon].blue = 0.0f;
        glutPostRedisplay();
        break;
    case 'b': // set color to blue
        polygons[selected_polygon].red = 0.0f;
        polygons[selected_polygon].green = 0.0f;
        polygons[selected_polygon].blue = 1.0f;
        glutPostRedisplay();
        break;
    case 'd': // delete the selected polygon
        if (selected_polygon != -1)
        {
            for (int i = selected_polygon; i < num_polygons - 1; i++)
            {
                polygons[i] = polygons[i + 1];
            }
            num_polygons--;
            selected_polygon = -1;
            glutPostRedisplay();
        }
        break;
    }
}

// void mouse(int button, int state, int x, int y)
// {
//     if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//     {
//         // add a new polygon to the list
//         if (num_polygons < MAX_POLYGONS && isDragging == false)
//         {
//             polygons[num_polygons].cx = x;
//             polygons[num_polygons].cy = glutGet(GLUT_WINDOW_HEIGHT) - y;
//             polygons[num_polygons].radius = 50;
//             polygons[num_polygons].sides = 6;
//             polygons[num_polygons].red = 1.0f;
//             polygons[num_polygons].green = 0.0f;
//             polygons[num_polygons].blue = 0.0f;
//             num_polygons++;
//             glutPostRedisplay(); // tell GLUT to redraw the screen
//         }
//         for (int i = num_polygons - 1; i >= 0; i--)
//         {
//             float dx = x - polygons[i].cx;
//             float dy = y - polygons[i].cy;
//             float distance = sqrt(dx * dx + dy * dy);
//             float polygon_position = sqrt(dx * dx + dy * dy);
//             printf("x : %d\n", x+50);
//             printf("y : %d\n", y+50);
//             printf("polygon [x] : %d\n", polygons[i].cx);
//             printf("polygon [x] + 50 : %d\n", polygons[i].cx +50);
//             //printf("polygon [y] : %d\n", polygons[i].cy);
//             printf("distance : %.2f\n", distance);
//             printf("position : %.2f\n", polygon_position);
//             printf("==============================\n\n");
//             if(x>=polygons[i].cx && x<=polygons[i].cx+50){
//                 printf("Position Check : Equal!\n");
//                 selected_polygon = i;
//                 printf("selected_polygon : %d\n", selected_polygon);
//                 break;
//             }


//             if (distance <= polygons[i].radius)
//             {
//                 selected_polygon = i;
//                 break;
//             }
//             else
//             {
//                 selected_polygon = -1;
//             }
//         }
//         if (selected_polygon != -1)
//         {
//             printf("isDragging = true");
//             isDragging = true;
//         }
//     }
//     if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//     {
//         isDragging = false;
//     }
// }

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if (num_polygons < MAX_POLYGONS && isDragging == false)
        {
            polygons[num_polygons].cx = x;
            polygons[num_polygons].cy = glutGet(GLUT_WINDOW_HEIGHT) - y;
            polygons[num_polygons].radius = 50;
            polygons[num_polygons].sides = 6;
            polygons[num_polygons].red = 1.0f;
            polygons[num_polygons].green = 0.0f;
            polygons[num_polygons].blue = 0.0f;
            num_polygons++;
            glutPostRedisplay(); // tell GLUT to redraw the screen
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        for (int i = num_polygons - 1; i >= 0; i--)
        {
            float dx = x - polygons[i].cx;
            float dy = y - polygons[i].cy;
            float distance = sqrt(dx * dx + dy * dy);
            float polygon_position = sqrt(dx * dx + dy * dy);
            float current_mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y;
            if(x>=polygons[i].cx && x<=polygons[i].cx+100 && current_mouse_y >= polygons[i].cy && current_mouse_y <= polygons[i].cy+85 ){
                printf("Position Check : Equal!\n");
                selected_polygon = i;
                printf("selected_polygon : %d\n", selected_polygon);
                break;
            }
            if (distance <= polygons[i].radius)
            {
                selected_polygon = i;
                break;
            }
            else
            {
                selected_polygon = -1;
            }
        }
        if (selected_polygon != -1)
        {
            printf("isDragging = true");
            isDragging = true;
        }
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        isDragging = false;
    }
}

void motion(int x, int y)
{
    if (isDragging)
    {
        polygons[selected_polygon].cx = x;
        polygons[selected_polygon].cy = 600 - y;
        glutPostRedisplay();
    }
}