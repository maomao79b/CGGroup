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
    char state;
} The_Polygon;

The_Polygon polygons[MAX_POLYGONS];
int num_polygons = 0;
int selected_polygon = -1; // no polygon is selected initially
int mouse_x = 0, mouse_y = 0;
bool isDragging = false;
char state_shape = 'p';

void myinit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 600.0);
}

void draw_polygon(The_Polygon p) //สร้างรูปหลายเหลี่ยม พารามิเตอร์ที่รับคือ The_polygon ที่เก็บข้อมูลต่างๆที่ใช้ในการคำนวณสำหรับการวาดรูป
{
    float angle = 360.0 / p.sides; // คำนวณมุมแต่ละด้านของ polygon เพื่อใช้ในการคำนวณค่า x,y ของแต่ละจุดบน polygon
    float x, y;
    glPushMatrix();              // push the current matrix onto the stack
    glTranslatef(p.cx, p.cy, 0); // translate the matrix to the new center point
    glBegin(GL_POLYGON);
    for (int i = 0; i < p.sides; i++) // โดย side คือ input ที่เป็นตัวบ่งบอกว่าต้องการให้รูปภาพมีกี่จุดหรือกี่เหลี่ยม สมมติ side = 4 จะได้เป็น x1,x2,x3,x4
    {
        x = p.radius * cos(angle * i * 3.14159 / 180.0); // คำนวณค่า x ของแต่ละจุดบน polygon
        y = p.radius * sin(angle * i * 3.14159 / 180.0); // คำนวณค่า y ของแต่ละจุดบน polygon

        glVertex2f(x, y); // นำจุดที่คำนวณได้มาวาดเป็นเส้น ซึ่งจะต่อกันแบบ ddaline โดยจุดสุดท้ายนั้นจะเป็นจุดเริ่มต้นเสมอ ซึ่งจะทำให้เกิดรูป 2D ขึ้น
    }
    glEnd();
    glPopMatrix(); // pop เมทริกซ์ก่อนหน้าออกจาก stack
    //pop การเคลื่อนที่และการหมุนของเมทริกซ์กลับออกจาก stack ด้วย glPopMatrix() 
    //โดยทำให้หน้าจอกลับไปเป็นสภาพเดิมก่อนที่จะวาดรูปหลายเหลี่ยมนี้ขึ้นมา
}
void draw_star(The_Polygon p) // สร้างรูปดาว โดยที่พารามิเตอร์คือแบบเดียวกับ polygon จะทำงานก็ต่อเมื่อกด s บน keyboard
{
    //ในการวาดรูปทรงเหลี่ยมโดยใช้ค่า radius, cx (จุดศูนย์กลาง x),
    //cy (จุดศูนย์กลาง y) ที่กำหนดใน The_Polygon ที่เป็น parameter ของฟังก์ชันนี้
    glPushMatrix(); //เก็บ matrix ปัจจุบันลงใน stack
    glTranslatef(p.cx, p.cy, 0); //ย้ายจุดศูนย์กลางของดาว (p.cx, p.cy) ไปยังจุดเริ่มต้นที่จะวาด
    float angle = 360.0 / (p.sides * 2); //คำนวณหาค่ามุมฉากของจุดยอดดาว คำนวณหาแค่ครั้งเดียวเพราะทุกจุดยอดจะมีมุมที่เท่ากัน
    float x, y;
    float outer_radius = p.radius; // ค่ารัสมีของจุดปลายด้านนอก ใช้ในการ
    float inner_radius = p.radius / 2; // ค่ารัสมีของจุดปลายด้านใน

    glBegin(GL_TRIANGLE_FAN); //สำหรับวาดรูปสามเหลี่ยมมุมฉาก หรือ จุดยอดดาวของแต่ละมุม 
    glVertex2f(0, 0);
    for (int i = 0; i <= p.sides * 2; i++) //วนลูปจนถึงจำนวนของด้านของดาวทั้งหมด * 2
    {
        float r = (i % 2 == 0) ? outer_radius : inner_radius; // alternate between outer and inner radius
        //คำนวณค่า r โดยสลับการกำหนด outer และ inner radius กันเป็นรอบๆ
        x = r * cos(angle * i * 3.14159 / 180.0); //คำนวณตำแหน่ง x, y ของจุด
        y = r * sin(angle * i * 3.14159 / 180.0);
        glVertex2f(x, y); // นำจุดที่คำนวณได้มาวาดเป็นเส้น ซึ่งจะต่อกันแบบ ddaline โดยจุดสุดท้ายนั้นจะเป็นจุดเริ่มต้นเสมอ ซึ่งจะทำให้เกิดรูป 2D ขึ้น
    }
    glEnd();
    glPopMatrix(); // pop เมทริกซ์ก่อนหน้าออกจาก stack
    //pop การเคลื่อนที่และการหมุนของเมทริกซ์กลับออกจาก stack ด้วย glPopMatrix() 
    //โดยทำให้หน้าจอกลับไปเป็นสภาพเดิมก่อนที่จะวาดรูปหลายเหลี่ยมนี้ขึ้นมา
}

void delete_all_polygons() // function ที่เอาไว้สำหรับ Clear Polygon ทั้งหมดในหน้าจอ
{
    num_polygons = 0; //กำหนดค่า num_polygons เป็น 0 เพื่อเริ่มการนับ Polygon ใหม่
    selected_polygon = -1; //กำหนดค่า selected_polygon เป็น -1 เพื่อไม่มี polygon ใดที่ถูกเลือกไว้เริ่มต้น
    for (int i = 0; i < MAX_POLYGONS; i++) //ทำการ loop ข้อมูลใน polygons ตามจำนวน MAX_POLYGONS ซึ่งกำหนดไว้ค่าคงที่ 1000
    {                           //ส่วน attribute ด้านในตั้งค่า Default หรือ 0 
        polygons[i].sides = 0; 
        polygons[i].radius = 0;
        polygons[i].cx = 0;
        polygons[i].cy = 0;
        polygons[i].red = 0.0;
        polygons[i].green = 0.0;
        polygons[i].blue = 0.0;
        polygons[i].state = state_shape;
    }
    glFlush(); //เรียกใช้ glFlush() เพื่อเคลียร์ buffer และอัปเดตผลลัพธ์ของการแสดงผล OpenGL 
    glutPostRedisplay();// เพื่อร้องขอให้ภาพถูกแสดงผลใหม่
}

void keyboard(unsigned char key, int x, int y)
{
    printf("\nkey : %c\n", key);
    switch (key)
    {
    case '+': // case + คือการเพิ่มขนาดของรูป Polygons
        polygons[selected_polygon].radius += 10;
        glutPostRedisplay();
        break;
    case '-':// case - คือการลดขนาดของรูป Polygons
        if(polygons[selected_polygon].radius == 10) break;
        polygons[selected_polygon].radius -= 10;
        glutPostRedisplay();
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
        polygons[selected_polygon].sides = 100; //ใช้ค่า 100 กับ polygon ที่มีรูปแบบเป็นวงกลม (circle) ก็จะสามารถวาดรูปวงกลมขึ้นมาได้ เนื่องจากวงกลมมีจำนวนด้านไม่จำกัด
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
    case 'x':
        delete_all_polygons();
        break;
    case 'p':
        state_shape = 'p';
        polygons[selected_polygon].state = state_shape;
        glutPostRedisplay();
        break;
    case 's':
        state_shape = 's';
        polygons[selected_polygon].state = state_shape;
        glutPostRedisplay();
        break;
    }
}

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
                polygons[num_polygons].state = state_shape;
                selected_polygon = num_polygons;
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

void motion(int x, int y)   //เป็นฟังก์ชันที่รับค่า x,y พิกัดของเมาส์ที่เลื่อน  โดยจะทำงานเมื่อเลื่อนเมาส์
{
    if (isDragging) //ตรวจสอบการคลิกของเมาส์ โดยคลิ๊กซ้ายจะ TRUEเข้าเงื่อนไข และจเป็น FLASEเมื่อปล่อย
    { 
        polygons[selected_polygon].cx = x;  //ถ้าเข้าเงื่อนไขจะเปลี่ยนค่าแกน x
        polygons[selected_polygon].cy = 600 - y; //ถ้าเข้าเงื่อนไขจะเปลี่ยนค่าแกน y และคำนวณใหม่
        glutPostRedisplay();
    }
}