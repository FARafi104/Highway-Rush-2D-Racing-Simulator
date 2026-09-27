#include <GL/glut.h>
#include <GL/glu.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>

using namespace std;

// ===============================
// GAME VARIABLES
// ===============================

float carX = 0.0f;

float carSpeed = 0.3f;
float carSpeedZ = 0.15f;

float speedStep = 0.05f;
float minSpeed = 0.0f;
float maxSpeed = 0.5f;

float wheelAngle = 0.0f;
float cloudOffset = 0.0f;
float roadOffset = 0.0f;

float roadWidth = 8.0f;


// ===============================
// TREE
// ===============================

struct Tree
{
    float x;
    float z;
    float scale;
};

vector<Tree> trees;


// ===============================
// OBSTACLE / BLOCK
// ===============================

float obstacleX = 0.0f;
float obstacleZ = 40.0f;

bool gameOver = false;

int score = 0;


// ===============================
// DRAW GRASS
// ===============================

void drawGrass()
{
    glColor3f(0.2f, 0.65f, 0.2f);

    glBegin(GL_QUADS);

    // Left grass
    glVertex3f(-30.0f, -0.5f, -100.0f);
    glVertex3f(-4.0f, -0.5f, -100.0f);
    glVertex3f(-4.0f, -0.5f, 100.0f);
    glVertex3f(-30.0f, -0.5f, 100.0f);

    // Right grass
    glVertex3f(4.0f, -0.5f, -100.0f);
    glVertex3f(30.0f, -0.5f, -100.0f);
    glVertex3f(30.0f, -0.5f, 100.0f);
    glVertex3f(4.0f, -0.5f, 100.0f);

    glEnd();
}


// ===============================
// DRAW ROAD
// ===============================

void drawRoad()
{
    // Road
    glColor3f(0.12f, 0.12f, 0.12f);

    glBegin(GL_QUADS);

    glVertex3f(-4.0f, -0.49f, -100.0f);
    glVertex3f(4.0f, -0.49f, -100.0f);
    glVertex3f(4.0f, -0.49f, 100.0f);
    glVertex3f(-4.0f, -0.49f, 100.0f);

    glEnd();


    // Road middle lines
    glColor3f(1.0f, 1.0f, 1.0f);

    for(float z = -100.0f + roadOffset; z < 100.0f; z += 6.0f)
    {
        glBegin(GL_QUADS);

        glVertex3f(-0.08f, -0.47f, z);
        glVertex3f(0.08f, -0.47f, z);
        glVertex3f(0.08f, -0.47f, z + 3.0f);
        glVertex3f(-0.08f, -0.47f, z + 3.0f);

        glEnd();
    }
}


// ===============================
// DRAW CAR
// ===============================

void drawCar()
{
    glPushMatrix();

    glTranslatef(carX, 0.0f, -1.0f);


    // Main body
    glColor3f(0.8f, 0.05f, 0.05f);

    glPushMatrix();

    glTranslatef(0.0f, 0.45f, 0.0f);

    glScalef(2.0f, 0.5f, 3.5f);

    glutSolidCube(1.0f);

    glPopMatrix();


    // Upper body
    glColor3f(0.7f, 0.05f, 0.05f);

    glPushMatrix();

    glTranslatef(0.0f, 0.9f, 0.0f);

    glScalef(1.5f, 0.5f, 1.8f);

    glutSolidCube(1.0f);

    glPopMatrix();


    // Front windshield
    glColor3f(0.1f, 0.2f, 0.3f);

    glPushMatrix();

    glTranslatef(0.0f, 0.95f, -0.92f);

    glScalef(1.3f, 0.35f, 0.05f);

    glutSolidCube(1.0f);

    glPopMatrix();


    // Rear windshield
    glPushMatrix();

    glTranslatef(0.0f, 0.95f, 0.92f);

    glScalef(1.3f, 0.35f, 0.05f);

    glutSolidCube(1.0f);

    glPopMatrix();


    // Wheels
    glColor3f(0.02f, 0.02f, 0.02f);


    // Front left
    glPushMatrix();

    glTranslatef(-1.05f, 0.15f, -1.0f);

    glRotatef(90.0f, 0, 1, 0);

    glutSolidTorus(0.22, 0.45, 16, 16);

    glPopMatrix();


    // Front right
    glPushMatrix();

    glTranslatef(1.05f, 0.15f, -1.0f);

    glRotatef(90.0f, 0, 1, 0);

    glutSolidTorus(0.22, 0.45, 16, 16);

    glPopMatrix();


    // Rear left
    glPushMatrix();

    glTranslatef(-1.05f, 0.15f, 1.0f);

    glRotatef(90.0f, 0, 1, 0);

    glutSolidTorus(0.22, 0.45, 16, 16);

    glPopMatrix();


    // Rear right
    glPushMatrix();

    glTranslatef(1.05f, 0.15f, 1.0f);

    glRotatef(90.0f, 0, 1, 0);

    glutSolidTorus(0.22, 0.45, 16, 16);

    glPopMatrix();


    glPopMatrix();
}


// ===============================
// DRAW TREE
// ===============================

void drawTree(float x, float z, float scale)
{
    glPushMatrix();

    glTranslatef(x, 0.0f, z);

    glScalef(scale, scale, scale);


    // Tree trunk
    glColor3f(0.45f, 0.20f, 0.05f);

    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();

    glTranslatef(0.0f, 1.2f, 0.0f);

    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    gluCylinder(
        quad,
        0.3f,
        0.3f,
        2.5f,
        16,
        16
    );

    glPopMatrix();

    gluDeleteQuadric(quad);


    // Main leaves
    glColor3f(0.05f, 0.5f, 0.1f);

    glPushMatrix();

    glTranslatef(0.0f, 3.0f, 0.0f);

    glutSolidSphere(1.4f, 20, 20);

    glPopMatrix();


    // Left leaves
    glPushMatrix();

    glTranslatef(-0.8f, 2.7f, 0.0f);

    glutSolidSphere(1.0f, 20, 20);

    glPopMatrix();


    // Right leaves
    glPushMatrix();

    glTranslatef(0.8f, 2.7f, 0.0f);

    glutSolidSphere(1.0f, 20, 20);

    glPopMatrix();


    glPopMatrix();
}


// ===============================
// INITIALIZE TREES
// ===============================

void initTrees()
{
    trees.clear();

    for(int i = 0; i < 20; i++)
    {
        Tree t;

        if(i % 2 == 0)
            t.x = -7.0f;
        else
            t.x = 7.0f;

        t.z = i * 10.0f;

        t.scale = 0.8f + (i % 3) * 0.2f;

        trees.push_back(t);
    }
}


// ===============================
// DRAW CLOUD
// ===============================

void drawCloud(float x, float y, float z)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    glColor3f(1.0f, 1.0f, 1.0f);

    glutSolidSphere(1.5f, 20, 20);

    glTranslatef(1.5f, 0.0f, 0.0f);

    glutSolidSphere(1.0f, 20, 20);

    glTranslatef(-3.0f, 0.0f, 0.0f);

    glutSolidSphere(1.0f, 20, 20);

    glPopMatrix();
}


// ===============================
// CREATE RANDOM OBSTACLE
// ===============================

void resetObstacle()
{
    int lane = rand() % 3;


    // Three road lanes
    if(lane == 0)
        obstacleX = -2.0f;

    else if(lane == 1)
        obstacleX = 0.0f;

    else
        obstacleX = 2.0f;


    // Put obstacle far ahead
    obstacleZ = 40.0f + (rand() % 20);
}


// ===============================
// DRAW OBSTACLE BLOCK
// ===============================

void drawObstacle()
{
    glPushMatrix();

    glTranslatef(
        obstacleX,
        0.5f,
        obstacleZ
    );

    // Red block
    glColor3f(
        1.0f,
        0.1f,
        0.1f
    );

    glScalef(
        1.5f,
        1.0f,
        1.5f
    );

    glutSolidCube(1.0f);

    glPopMatrix();
}


// ===============================
// CHECK COLLISION
// ===============================

void checkCollision()
{
    float carZ = -1.0f;


    float distanceX =
        fabs(carX - obstacleX);

    float distanceZ =
        fabs(carZ - obstacleZ);


    // Collision area
    if(distanceX < 1.4f &&
       distanceZ < 2.0f)
    {
        gameOver = true;
    }
}


// ===============================
// DRAW TEXT
// ===============================

void drawText(
    float x,
    float y,
    const char* text
)
{
    glRasterPos2f(x, y);

    while(*text)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_HELVETICA_18,
            *text
        );

        text++;
    }
}


// ===============================
// DISPLAY
// ===============================

void display()
{
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );


    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();


    gluLookAt(
        0.0f, 3.0f, -5.0f,
        0.0f, 0.0f, 10.0f,
        0.0f, 1.0f, 0.0f
    );


    // Environment
    drawGrass();

    drawRoad();

    // Player car
    drawCar();

    // Trees
    for(size_t i = 0; i < trees.size(); i++)
    {
        float z =
            trees[i].z + roadOffset;

        if(z > 50.0f)
            z -= 200.0f;

        drawTree(
            trees[i].x,
            z,
            trees[i].scale
        );
    }


    // Clouds
    drawCloud(
        -4.0f + cloudOffset,
        6.0f,
        10.0f
    );

    drawCloud(
        3.0f + cloudOffset,
        7.0f,
        15.0f
    );


    // Obstacle
    drawObstacle();


    // ===========================
    // SCORE
    // ===========================

    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();

    gluOrtho2D(
        0,
        1024,
        0,
        768
    );


    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glLoadIdentity();


    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );


    char scoreText[50];

    sprintf(
        scoreText,
        "Score: %d",
        score
    );

    drawText(
        30,
        730,
        scoreText
    );


    // ===========================
    // GAME OVER SCREEN
    // ===========================

    if(gameOver)
    {
        glColor3f(
            1.0f,
            0.0f,
            0.0f
        );


        drawText(
            430,
            400,
            "GAME OVER"
        );


        glColor3f(
            1.0f,
            1.0f,
            1.0f
        );


        drawText(
            390,
            360,
            "Press R to Restart"
        );
    }


    glPopMatrix();

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);


    glEnable(GL_LIGHTING);


    glutSwapBuffers();
}


// ===============================
// TIMER
// ===============================

void timer(int)
{
    // Game only moves when
    // game is NOT over
    if(!gameOver)
    {
        wheelAngle -= 5.0f;


        // Clouds
        cloudOffset += 0.02f;

        if(cloudOffset > 20.0f)
            cloudOffset -= 40.0f;


        // Road movement
        roadOffset -= carSpeedZ;

        if(roadOffset < -4.0f)
            roadOffset += 4.0f;


        // Move obstacle toward player
        obstacleZ -= carSpeedZ * 10.0f;


        // If obstacle passes player
        if(obstacleZ < -10.0f)
        {
            score++;

            resetObstacle();
        }


        // Check collision
        checkCollision();
    }


    glutPostRedisplay();

    glutTimerFunc(
        16,
        timer,
        0
    );
}


// ===============================
// RESHAPE
// ===============================

void reshape(int w, int h)
{
    if(h == 0)
        h = 1;


    glViewport(
        0,
        0,
        w,
        h
    );


    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();


    gluPerspective(
        60.0,
        (float)w / (float)h,
        0.1f,
        100.0f
    );


    glMatrixMode(GL_MODELVIEW);
}


// ===============================
// ARROW KEYS
// ===============================

void specialKeys(
    int key,
    int,
    int
)
{
    // Don't allow movement
    // after game over
    if(gameOver)
        return;


    if(key == GLUT_KEY_LEFT)
        carX += carSpeed;


    if(key == GLUT_KEY_RIGHT)
        carX -= carSpeed;


    if(key == GLUT_KEY_UP)
        carSpeedZ += speedStep;


    if(key == GLUT_KEY_DOWN)
        carSpeedZ -= speedStep;


    // Speed limits
    if(carSpeedZ > maxSpeed)
        carSpeedZ = maxSpeed;


    if(carSpeedZ < minSpeed)
        carSpeedZ = minSpeed;


    // Car limits
    if(carX < -roadWidth / 2.0f + 1.0f)
        carX = -roadWidth / 2.0f + 1.0f;


    if(carX > roadWidth / 2.0f - 1.0f)
        carX = roadWidth / 2.0f - 1.0f;
}


// ===============================
// NORMAL KEYBOARD
// ===============================

void keyboard(
    unsigned char key,
    int,
    int
)
{
    // Restart
    if(key == 'r' || key == 'R')
    {
        gameOver = false;

        score = 0;

        carX = 0.0f;

        carSpeedZ = 0.15f;

        roadOffset = 0.0f;

        resetObstacle();
    }


    // ESC = Exit
    if(key == 27)
    {
        exit(0);
    }
}


// ===============================
// INITIALIZATION
// ===============================

void init()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(
        GL_FRONT,
        GL_AMBIENT_AND_DIFFUSE
    );


    glClearColor(
        0.529f,
        0.808f,
        0.922f,
        1.0f
    );


    GLfloat lightPos[] =
    {
        0.0f,
        10.0f,
        10.0f,
        1.0f
    };


    glLightfv(
        GL_LIGHT0,
        GL_POSITION,
        lightPos
    );


    // Trees
    initTrees();


    // Random number generator
    srand(
        (unsigned)time(0)
    );


    // First obstacle
    resetObstacle();
}


// ===============================
// MAIN
// ===============================

int main(
    int argc,
    char** argv
)
{
    glutInit(
        &argc,
        argv
    );


    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH
    );


    glutInitWindowSize(
        1024,
        768
    );


    glutCreateWindow(
        "Realistic Car on Endless Road"
    );


    init();


    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutSpecialFunc(specialKeys);

    glutKeyboardFunc(keyboard);


    glutTimerFunc(
        0,
        timer,
        0
    );


    glutMainLoop();


    return 0;
}
