#include <gl\gl.h>
#include <gl\glut.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>


#define MAX_POLYGONS 1000

typedef struct Type_Polygon{
    int sides; // เก็บจำนวนมุม
    int radius; // รัศมี
    int cx; // ตำแหน่ง x
    int cy; // ตำแหน่ง y
    float red; // ช่องเก็บสีแดง
    float green; // ช่องเก็บสีเขียว
    float blue; // ช่องเก็บสีน้ำเงิน
    char state; // เก็บว่าตอนนี้วาด polygon หรือ star
} The_Polygon;

The_Polygon polygons[MAX_POLYGONS]; // array เก็บรูป
int num_polygons = 0; // จำนวนรูปที่วาด
int selected_polygon = -1; // เมื่อไม่มีรูปไหนๆเลยให้เป็น -1
bool isDragging = false; // set ค่าการคลิกเมาส์เริ่มต้นให้เป็น false
char state_shape = 'p'; // ประกาศค่าไว้เพื่อรอการใช้คีย์ใน swith case เป็น p โดยจะใช้เป็นการวาด Polygon หรือเป็น s วาดเป็นดาว

void myinit(void) // set default ให้กับโปรแกรมของ OpenGL เมื่อเริ่มทำงาน
{
    glClearColor(1.0, 1.0, 1.0, 0.0);  // ล้างพื้นหลัง
    glMatrixMode(GL_PROJECTION); //
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
    // angle คำนวณเพื่อให้ได้มุมที่ต้องวาดจุดบนจุดยอดทั้งสองสาย 
    float x, y;
    //ค่ารัสมีที่จำเป็นในการวาดสามเหลี่ยมมุมฉาก ประกอบด้วย จุดปลายด้านนอก และ จุดปลายด้านใน
    float outer_radius = p.radius; // ค่ารัสมีของจุดปลายด้านนอก เป็นรัศมีของดาว
    float inner_radius = p.radius / 2; // ค่ารัสมีของจุดปลายด้านใน เป็นรัศมีของจุดบนสองสายแฝด

    glBegin(GL_TRIANGLE_FAN); //สำหรับวาดรูปสามเหลี่ยมมุมฉาก หรือ จุดยอดดาวของแต่ละมุม 
    glVertex2f(0, 0); // จุดศูนย์กลางของดาว
    for (int i = 0; i <= p.sides * 2; i++) //วนลูปจนถึงจำนวนของด้านของดาวทั้งหมด * 2
    {
        float r = (i % 2 == 0) ? outer_radius : inner_radius; // alternate between outer and inner radius
        //คำนวณค่า r โดยสลับการกำหนด outer และ inner radius กันเป็นรอบๆ
        x = r * cos(angle * i * 3.14159 / 180.0); //คำนวณตำแหน่ง x, y ของจุด
        y = r * sin(angle * i * 3.14159 / 180.0);
        glVertex2f(x, y); // นำจุดที่คำนวณได้มาวาดเป็นเส้น เพื่อกำหนดแต่ละจุดยอดทั้งสองสาย
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

void keyboard(unsigned char key, int x, int y) //function นี้คือการเรียกใช้งานจาก Keyboard โดยจะรับค่า char key int x int y มา
{
    printf("\nkey : %c\n", key);
    switch (key)
    {
    case '+': // case + คือการเพิ่มขนาดของรูป Polygons
        polygons[selected_polygon].radius += 10; //โดยจะใช้ polygons[เลือกpolygons ที่จะเพิ่มขนาดหรือที่กำลังกดอยู่].redius ขนาด += 10  โดยจะเพิ่มขึ้นที่ละ 10
        glutPostRedisplay();
        break;
    case '-':// case - คือการลดขนาดของรูป Polygons 
        if(polygons[selected_polygon].radius == 10) break;// ห้ามให้ขนาดเล็กเกินกว่า 10 จึงได้มี if ขึ้นมา โดยจะเลือกpolygons.radius == 10 คือต้องมาค่าเท่ากับ 10
        polygons[selected_polygon].radius -= 10;//โดยจะใช้ polygons[เลือกpolygons ที่จะลดขนาดหรือที่กำลังกดอยู่].redius ขนาด -= 10  โดยจะลดขึ้นที่ละ 10
        glutPostRedisplay();
        break;
    case '0': //case 0 คือรูป 3 เหลี่ยม
        polygons[selected_polygon].sides = 3;
        glutPostRedisplay();
        break;
    case '1': //case 1 คือรูป 4 เหลี่ยม
        polygons[selected_polygon].sides = 4;
        glutPostRedisplay();
        break;
    case '2': //case 2 คือรูป 5 เหลี่ยม
        polygons[selected_polygon].sides = 5;
        glutPostRedisplay();
        break;
    case '3': //case 3 คือรูป 6 เหลี่ยม
        polygons[selected_polygon].sides = 6;
        glutPostRedisplay();
        break;
    case '4'://case 4 คือรูป 7 เหลี่ยม
        polygons[selected_polygon].sides = 7;
        glutPostRedisplay();
        break;
    case '5'://case 5 คือรูป 8 เหลี่ยม
        polygons[selected_polygon].sides = 8;
        glutPostRedisplay();
        break;
    case '6'://case 6 คือรูป 9 เหลี่ยม
        polygons[selected_polygon].sides = 9;
        glutPostRedisplay();
        break;
    case '7'://case 7 คือรูป 10 เหลี่ยม
        polygons[selected_polygon].sides = 10;
        glutPostRedisplay();
        break;
    case '8': //case 8 คือรูปวงกลม
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
        //เช็คว่ามี polygon ที่ถูกเลือกอยู่หรือไม่ โดยใช้ตัวแปร selected_polygon ถ้ามีให้ดำเนินการต่อไป ถ้าไม่มีให้ไม่ทำอะไร
        {
            for (int i = selected_polygon; i < num_polygons - 1; i++)
            //ถ้ามี polygon ที่ถูกเลือก จะเข้าลูป for เพื่อเลื่อนตำแหน่งของ polygon
            // ที่อยู่หลังจาก polygon ที่ถูกลบไป 1 ตำแหน่ง เพื่อให้ตำแหน่งของ polygon 
            //ที่เหลือในอาร์เรย์ polygons ยังคงเรียงต่อไปอย่างต่อเนื่อง
            {
                polygons[i] = polygons[i + 1];
            }
            num_polygons--;//ลบ polygon ที่ถูกเลือกออกจาก num_polygons โดยลดค่า num_polygons ลง 1 
            selected_polygon = -1;//กำหนดค่า selected_polygon เป็น -1 เพื่อยกเลิกการเลือก polygon ที่เคยถูกเลือก
            glutPostRedisplay(); //เรียกใช้ฟังก์ชัน glutPostRedisplay() 
            //เพื่อให้หน้าจอทำการ render ใหม่และแสดงผลลัพธ์ที่เปลี่ยนแปลงแล้วให้กับผู้ใช้งาน
        }
        break;
    case 'x'://case x คือ การเรียกใช้ function delete_all_polygons
        delete_all_polygons();
        break;
    case 'p': // case P Polygon คือ ทำให้วาดภาพได้ตามมุม/เหลี่ยม 
        state_shape = 'p';
        polygons[selected_polygon].state = state_shape;
        glutPostRedisplay();
        break;
    case 's': // case s Star คือ ทำให้วาดภาพ Star 
        state_shape = 's';
        polygons[selected_polygon].state = state_shape;
        glutPostRedisplay();
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){ //เงื่อนไขคลิกเมาส์ซ้าย
        if (num_polygons < MAX_POLYGONS) // เช็คจำนวนสูงสุดที่สามารถวาดภาพได้
            {
                polygons[num_polygons].cx = x; //กำหนดตำแหน่ง x ของจุดศูนย์กลางรูปที่วาดโดยรับค่ามาจากการคลิกที่เมาส์
                polygons[num_polygons].cy = glutGet(GLUT_WINDOW_HEIGHT) - y; //
                polygons[num_polygons].radius = 50;
                polygons[num_polygons].sides = 6;
                polygons[num_polygons].red = 1.0f;
                polygons[num_polygons].green = 0.0f;
                polygons[num_polygons].blue = 0.0f;
                polygons[num_polygons].state = state_shape;
                selected_polygon = num_polygons;
                num_polygons++;
                glutPostRedisplay(); // บอกให้ glut วาดภาพใหม่
            }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) // คลิกเมาส์ขวา 
    {
        for (int i = num_polygons - 1; i >= 0; i--) // รันเช็คทุกรูปที่สร้างไว้
        {
            float dx = x - polygons[i].cx;
            float dy = y - polygons[i].cy;
            float distance = sqrt(dx * dx + dy * dy); 
            float polygon_position = sqrt(dx * dx + dy * dy);
            float current_mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y; // ตำแหน่งปัจจุบันของ y ที่ต้องลบกับ glut เพราะ y แบบปกติจะเก็บค่าคนละแบบกับรูปภาพ
            if(x>=polygons[i].cx && x<=polygons[i].cx+100 && current_mouse_y >= polygons[i].cy && current_mouse_y <= polygons[i].cy+85 ){
                selected_polygon = i;
                break;
            }
            if (distance <= polygons[i].radius) // ถ้าจุดที่กดใกล้กับรูปภาพ
            {
                selected_polygon = i;
                break;
            }
            else // ถ้าจุดที่กดไม่ใช่รูปภาพ
            {
                selected_polygon = -1;
            }
        }
        if (selected_polygon != -1) 
        {
            isDragging = true;
        }
    }
    
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) //เช็คการปล่อยเมาส์ขวา
    {
        isDragging = false; //ถ้าปล่อยเมาส์แล้วจะ set ให้ค่าคลิกเมาส์ false
    }
}

void motion(int x, int y)   //เป็นฟังก์ชันที่รับค่า x,y พิกัดของเมาส์ที่เลื่อน  โดยจะทำงานเมื่อเลื่อนเมาส์
{
    if (isDragging) //ตรวจสอบการคลิกของเมาส์ โดยคลิ๊กซ้ายจะ TRUE เข้าเงื่อนไข และจะเป็น FLASE เมื่อปล่อย
    { 
        polygons[selected_polygon].cx = x;  //ถ้าเข้าเงื่อนไขจะเปลี่ยนค่าแกน x และคำนวณใหม่
        polygons[selected_polygon].cy = 600 - y; //ถ้าเข้าเงื่อนไขจะเปลี่ยนค่าแกน y และคำนวณใหม่
        glutPostRedisplay(); //เรียกใช้ฟังก์ชันหน้าจอซ้ำเพื่อให้ได้ภาพที่เปลี่ยนมุมหลายๆมุมได้
    }
}