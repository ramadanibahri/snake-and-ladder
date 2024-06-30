#include <windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include<iostream>
#include <cmath>
#endif

using namespace std;

// Global Variabel
int * snakes;
int * ladders;

int scoreP1 = 1;
int scoreP2 = 1;
int turn = 1;
float observerX = 0.5;
float observerY = 0.3;

float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;

float centerX = 6.0;
float centerY = 3.5;
float centerZ = 0.0;

float saveRotationX = 0.0;
float saveRotationY = 0.0;
float saveRotationZ = 0.0;

bool isChangeView = false;
bool currentView = false;

int mode = 1;

float obsX = 0.0;
float obsY = 0.0;
float obsZ = 0.0;

bool gameOver = false;
bool isRotate = false;

boolean diff = true, spec = true, amb = true;
float theta = 0.0;
int mouseButton = GLUT_LEFT_BUTTON;

// Pattern tile
void drawTile(int rows, int cols) {
    float tileDepth = 0.1;
    float transparency = 1.0;

    if(isChangeView) {
        transparency = 0.3;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if((i + j) % 2 == 0) {
                // Hijau
                glColor4f(0.0, 0.7, 0.4, transparency);
            } else {
                // Putih
                glColor4f(1.0, 1.0, 1.0, transparency);
            }

            float x = j;
            float y = i;

            // Front
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 0.0, -1.0);
                glVertex3f(x, y, 0.0);
                glVertex3f(x + 1.0, y, 0.0);
                glVertex3f(x + 1.0, y + 1.0, 0.0);
                glVertex3f(x, y + 1.0, 0.0);
            glEnd();

            // back
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 0.0, 1.0);
                glVertex3f(x, y, tileDepth);
                glVertex3f(x + 1.0, y, tileDepth);
                glVertex3f(x + 1.0, y + 1.0, tileDepth);
                glVertex3f(x, y + 1.0, tileDepth);
            glEnd();

            // left
            glBegin(GL_POLYGON);
                glNormal3f(-1.0, 0.0, 0.0);
                glVertex3f(x, y, 0.0);
                glVertex3f(x, y + 1.0, 0.0);
                glVertex3f(x, y + 1.0, tileDepth);
                glVertex3f(x, y, tileDepth);
            glEnd();

            // right
            glBegin(GL_POLYGON);
                glNormal3f(1.0, 0.0, 0.0);
                glVertex3f(x + 1.0, y, 0.0);
                glVertex3f(x + 1.0, y + 1.0, 0.0);
                glVertex3f(x + 1.0, y + 1.0, tileDepth);
                glVertex3f(x + 1.0, y, tileDepth);
            glEnd();

            // top
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 1.0, 0.0);
                glVertex3f(x, y + 1.0, 0.0);
                glVertex3f(x + 1.0, y + 1.0, 0.0);
                glVertex3f(x + 1.0, y + 1.0, tileDepth);
                glVertex3f(x, y + 1.0, tileDepth);
            glEnd();

            // bottom
            glBegin(GL_POLYGON);
                glNormal3f(0.0, -1.0, 0.0);
                glVertex3f(x, y, 0.0);
                glVertex3f(x + 1.0, y, 0.0);
                glVertex3f(x + 1.0, y, tileDepth);
                glVertex3f(x, y, tileDepth);
            glEnd();
        }
    }
}

void drawBorderTile(float tileWidth, float tileHeight, float borderDepth) {
    glColor3f(0.5, 0.5, 0.0);

    // Border Left
    glBegin(GL_POLYGON); // Front face
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-0.1, 0, 0);
        glVertex3f(0.1, 0, 0);
        glVertex3f(0.1, tileHeight, 0);
        glVertex3f(-0.1, tileHeight, 0);
    glEnd();

    glBegin(GL_POLYGON); // Back face
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-0.1, 0, borderDepth);
        glVertex3f(0.1, 0, borderDepth);
        glVertex3f(0.1, tileHeight, borderDepth);
        glVertex3f(-0.1, tileHeight, borderDepth);
    glEnd();

    glBegin(GL_POLYGON); // Left face
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(-0.1, 0, 0);
        glVertex3f(-0.1, tileHeight, 0);
        glVertex3f(-0.1, tileHeight, borderDepth);
        glVertex3f(-0.1, 0, borderDepth);
    glEnd();

    glBegin(GL_POLYGON); // Right face
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(0.1, 0, 0);
        glVertex3f(0.1, tileHeight, 0);
        glVertex3f(0.1, tileHeight, borderDepth);
        glVertex3f(0.1, 0, borderDepth);
    glEnd();

    glBegin(GL_POLYGON); // Top face
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(-0.1, tileHeight, 0);
        glVertex3f(0.1, tileHeight, 0);
        glVertex3f(0.1, tileHeight, borderDepth);
        glVertex3f(-0.1, tileHeight, borderDepth);
    glEnd();

    glBegin(GL_POLYGON); // Bottom face
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(-0.1, 0, 0);
        glVertex3f(0.1, 0, 0);
        glVertex3f(0.1, 0, borderDepth);
        glVertex3f(-0.1, 0, borderDepth);
    glEnd();

    // Border Top
    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(0.0, tileHeight + 0.1, 0.0);
        glVertex3f(0.0, tileHeight, 0.0);
        glVertex3f(tileWidth, tileHeight, 0.0);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, tileHeight + 0.1, borderDepth);
        glVertex3f(0.0, tileHeight, borderDepth);
        glVertex3f(tileWidth, tileHeight, borderDepth);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(0.0, tileHeight + 0.1, 0.0);
        glVertex3f(0.0, tileHeight, 0.0);
        glVertex3f(0.0, tileHeight, borderDepth);
        glVertex3f(0.0, tileHeight + 0.1, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, tileHeight, 0.0);
        glVertex3f(tileWidth, tileHeight, 0.0);
        glVertex3f(tileWidth, tileHeight, borderDepth);
        glVertex3f(0.0, tileHeight, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(tileWidth, tileHeight, 0.0);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, 0.0);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, borderDepth);
        glVertex3f(tileWidth, tileHeight, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, 0.0);
        glVertex3f(0.0, tileHeight + 0.1, 0.0);
        glVertex3f(0.0, tileHeight + 0.1, borderDepth);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, borderDepth);
    glEnd();

    // Border Right
    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(tileWidth, -0.1, 0.0);
        glVertex3f(tileWidth + 0.1, 0.0, 0.0);
        glVertex3f(tileWidth + 0.1, 7.1, 0.0);
        glVertex3f(tileWidth, tileHeight + 0.1, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(tileWidth, -0.1, borderDepth);
        glVertex3f(tileWidth + 0.1, 0.0, borderDepth);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, borderDepth);
        glVertex3f(tileWidth, tileHeight + 0.1, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(tileWidth, -0.1, 0.0);
        glVertex3f(tileWidth + 0.1, 0.0, 0.0);
        glVertex3f(tileWidth + 0.1, 0.0, borderDepth);
        glVertex3f(tileWidth, -0.1, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(tileWidth + 0.1, 0.0, 0.0);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, 0.0);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, borderDepth);
        glVertex3f(tileWidth + 0.1, 0.0, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, 0.0);
        glVertex3f(tileWidth, tileHeight + 0.1, 0.0);
        glVertex3f(tileWidth, tileHeight + 0.1, borderDepth);
        glVertex3f(tileWidth + 0.1, tileHeight + 0.1, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(tileWidth, tileHeight + 0.1, 0.0);
        glVertex3f(tileWidth, -0.1, 0.0);
        glVertex3f(tileWidth, -0.1, borderDepth);
        glVertex3f(tileWidth, tileHeight + 0.1, borderDepth);
    glEnd();

    // Border Bottom
    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-0.1, 0.0, 0.0);
        glVertex3f(tileWidth, 0.0, 0.0);
        glVertex3f(tileWidth + 0.1, -0.1, 0.0);
        glVertex3f(0.0, -0.1, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-0.1, 0.0, borderDepth);
        glVertex3f(tileWidth, 0.0, borderDepth);
        glVertex3f(tileWidth + 0.1, -0.1, borderDepth);
        glVertex3f(0.0, -0.1, 0.2);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(-0.1, 0.0, 0.0);
        glVertex3f(tileWidth, 0.0, 0.0);
        glVertex3f(tileWidth, 0.0, borderDepth);
        glVertex3f(-0.1, 0.0, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(tileWidth, 0.0, 0.0);
        glVertex3f(tileWidth + 0.1, -0.1, 0.0);
        glVertex3f(tileWidth + 0.1, -0.1, borderDepth);
        glVertex3f(tileWidth, 0.0, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(tileWidth + 0.1, -0.1, 0.0);
        glVertex3f(0.0, -0.1, 0.0);
        glVertex3f(0.0, -0.1, borderDepth);
        glVertex3f(tileWidth + 0.1, -0.1, borderDepth);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(0.0, -0.1, 0.0);
        glVertex3f(-0.1, 0.0, 0.0);
        glVertex3f(-0.1, 0.0, borderDepth);
        glVertex3f(0.0, -0.1, borderDepth);
    glEnd();
}

// Border of track
void drawTrack(int w, int x, int wx, float borderWidth, float borderDepth) {
    // Black
    glColor3f(0.5, 0.5, 0.0);

    for (int i = 1; i <= x; i++) {
        if (i % 2 == 0) {
            // Right to Left
            // Front
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 0.0, -1.0);
                glVertex3f(w, i * wx, 0);
                glVertex3f(wx, i * wx, 0);
                glVertex3f(wx, i * wx - borderWidth, 0);
                glVertex3f(w, i * wx - borderWidth, 0);
            glEnd();

            // Back
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 0.0, 1.0);
                glVertex3f(w, i * wx, borderDepth);
                glVertex3f(wx, i * wx, borderDepth);
                glVertex3f(wx, i * wx - borderWidth, borderDepth);
                glVertex3f(w, i * wx - borderWidth, borderDepth);
            glEnd();

            // Left
            glBegin(GL_POLYGON);
                glNormal3f(-1.0, 0.0, 0.0);
                glVertex3f(wx, i * wx, 0);
                glVertex3f(wx, i * wx, borderDepth);
                glVertex3f(wx, i * wx - borderWidth, borderDepth);
                glVertex3f(wx, i * wx - borderWidth, 0);
            glEnd();

            // Right
            glBegin(GL_POLYGON);
                glNormal3f(1.0, 0.0, 0.0);
                glVertex3f(w, i * wx, 0);
                glVertex3f(w, i * wx, borderDepth);
                glVertex3f(w, i * wx - borderWidth, borderDepth);
                glVertex3f(w, i * wx - borderWidth, 0);
            glEnd();

            // Top
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 1.0, 0.0);
                glVertex3f(w, i * wx, 0);
                glVertex3f(wx, i * wx, 0);
                glVertex3f(wx, i * wx, borderDepth);
                glVertex3f(w, i * wx, borderDepth);
            glEnd();

            // Bottom
            glBegin(GL_POLYGON);
                glNormal3f(0.0, -1.0, 0.0);
                glVertex3f(w, i * wx - borderWidth, 0);
                glVertex3f(wx, i * wx - borderWidth, 0);
                glVertex3f(wx, i * wx - borderWidth, borderDepth);
                glVertex3f(w, i * wx - borderWidth, borderDepth);
            glEnd();
        } else {
            // Left to Right
            // Front
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 0.0, -1.0);
                glVertex3f(0, i * wx, 0);
                glVertex3f(w - wx, i * wx, 0);
                glVertex3f(w - wx, i * wx - borderWidth, 0);
                glVertex3f(0, i * wx - borderWidth, 0);
            glEnd();

            // Back
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 0.0, 1.0);
                glVertex3f(0, i * wx, borderDepth);
                glVertex3f(w - wx, i * wx, borderDepth);
                glVertex3f(w - wx, i * wx - borderWidth, borderDepth);
                glVertex3f(0, i * wx - borderWidth, borderDepth);
            glEnd();

            // Left
            glBegin(GL_POLYGON);
                glNormal3f(-1.0, 0.0, 0.0);
                glVertex3f(0, i * wx, 0);
                glVertex3f(0, i * wx, borderDepth);
                glVertex3f(0, i * wx - borderWidth, borderDepth);
                glVertex3f(0, i * wx - borderWidth, 0);
            glEnd();

            // Right
            glBegin(GL_POLYGON);
                glNormal3f(1.0, 0.0, 0.0);
                glVertex3f(w - wx, i * wx, 0);
                glVertex3f(w - wx, i * wx, borderDepth);
                glVertex3f(w - wx, i * wx - borderWidth, borderDepth);
                glVertex3f(w - wx, i * wx - borderWidth, 0);
            glEnd();

            // Top
            glBegin(GL_POLYGON);
                glNormal3f(0.0, 1.0, 0.0);
                glVertex3f(0, i * wx, 0);
                glVertex3f(w - wx, i * wx, 0);
                glVertex3f(w - wx, i * wx, borderDepth);
                glVertex3f(0, i * wx, borderDepth);
            glEnd();

            // Bottom
            glBegin(GL_POLYGON);
                glNormal3f(0.0, -1.0, 0.0);
                glVertex3f(0, i * wx - borderWidth, 0);
                glVertex3f(w - wx, i * wx - borderWidth, 0);
                glVertex3f(w - wx, i * wx - borderWidth, borderDepth);
                glVertex3f(0, i * wx - borderWidth, borderDepth);
            glEnd();
        }
    }
}

// Function to move the pion
void move(int newScore, int w, int h, int& x, int& y) {
    // Y axis
    int score = newScore;
    y = floor(score / w);

    // X axis
    if (y % 2 == 0) {
        // Jika baris genap: left to right
        x = score % w;
    } else {
        // Jika baris ganjil: right to left
        x = w - 1 - (score % w);
    }
}

void pionTriangle(int score) {
    int x = 0;
    int y = 0;

    move(score - 1, 12, 8, x, y);

    glColor3f(1.0, 0.0, 0.0);

    // Front
    glBegin(GL_POLYGON);
        glVertex3f(0.4 + x, 0.6 + y, 0.25);
        glVertex3f(0.6 + x, 0.6 + y, 0.25);
        glVertex3f(0.5 + x, 0.8 + y, 0.25);
    glEnd();

    // Back
    glBegin(GL_POLYGON);
        glVertex3f(0.4 + x, 0.6 + y, 0.40);
        glVertex3f(0.6 + x, 0.6 + y, 0.40);
        glVertex3f(0.5 + x, 0.8 + y, 0.40);
    glEnd();

    // Side 1
    glBegin(GL_POLYGON);
        glVertex3f(0.4 + x, 0.6 + y, 0.25);
        glVertex3f(0.6 + x, 0.6 + y, 0.25);
        glVertex3f(0.6 + x, 0.6 + y, 0.40);
        glVertex3f(0.4 + x, 0.6 + y, 0.40);
    glEnd();

    // Side 2
    glBegin(GL_POLYGON);
        glVertex3f(0.6 + x, 0.6 + y, 0.25);
        glVertex3f(0.5 + x, 0.8 + y, 0.25);
        glVertex3f(0.5 + x, 0.8 + y, 0.40);
        glVertex3f(0.6 + x, 0.6 + y, 0.40);
    glEnd();

    // Side 3
    glBegin(GL_POLYGON);
        glVertex3f(0.5 + x, 0.8 + y, 0.25);
        glVertex3f(0.4 + x, 0.6 + y, 0.25);
        glVertex3f(0.4 + x, 0.6 + y, 0.40);
        glVertex3f(0.5 + x, 0.8 + y, 0.40);
    glEnd();
}

void pionTile(int score) {
    int x = 0;
    int y = 0;

    move(score - 1, 12, 8, x, y);

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.2 + x, y + 0.6, 0.3);
        glVertex3f(0.4 + x, y + 0.6, 0.3);
        glVertex3f(0.4 + x, y + 0.8, 0.3);
        glVertex3f(0.2 + x, y + 0.8, 0.3);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f(0.2 + x, y + 0.6, 0.0);
        glVertex3f(0.4 + x, y + 0.6, 0.0);
        glVertex3f(0.4 + x, y + 0.8, 0.0);
        glVertex3f(0.2 + x, y + 0.8, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f(0.2 + x, y + 0.8, 0.3);
        glVertex3f(0.4 + x, y + 0.8, 0.3);
        glVertex3f(0.4 + x, y + 0.8, 0.0);
        glVertex3f(0.2 + x, y + 0.8, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f(0.4 + x, y + 0.6, 0.3);
        glVertex3f(0.4 + x, y + 0.6, 0.0);
        glVertex3f(0.4 + x, y + 0.8, 0.0);
        glVertex3f(0.4 + x, y + 0.8, 0.3);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f(0.2 + x, y + 0.6, 0.3);
        glVertex3f(0.4 + x, y + 0.6, 0.3);
        glVertex3f(0.4 + x, y + 0.6, 0.0);
        glVertex3f(0.2 + x, y + 0.6, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f(0.2 + x, y + 0.6, 0.3);
        glVertex3f(0.2 + x, y + 0.6, 0.0);
        glVertex3f(0.2 + x, y + 0.8, 0.0);
        glVertex3f(0.2 + x, y + 0.8, 0.3);
    glEnd();
}

// function to generate random number range 1-6
int randomNumber() {
    srand (time(NULL));
    int randAngka = rand() % 6 + 1;
    return randAngka;
}

// Event Handler
void onInput(unsigned char key, int x, int y) {
    key = tolower(key);

    if (key=='1'){
        if (amb == true){
            GLfloat ambient_light[] = { 0.3, 0.3, 0.3, 1.0 };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient_light);
            amb = false;
        }
        else{
            GLfloat ambient_light[] = { 0.9, 0.9, 0.9, 1.0 };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient_light);
            amb = true;
        }
    }
    if (key=='2'){
        if (diff == true){
            glDisable(GL_LIGHT0);
            diff = false;
        }
        else{
            glEnable(GL_LIGHT0);
            diff = true;
        }
    }
    if (key=='3'){
        if (spec == true){
            glDisable(GL_LIGHT1);
            spec = false;
        }
        else{
            glEnable(GL_LIGHT1);
            spec = true;
        }
    }

    switch(key) {
        case 'w':
            if(observerY >= 6.7) {
                observerY = observerY;
            } else {
                observerY += 0.1;
            }

            break;
        case 's':
            if(observerY <= 0.3) {
                observerY = observerY;
            } else {
                observerY -= 0.1;
            }

            break;
        case 'a':
            if(observerX <= 0.3) {
                observerX = observerX;
            } else {
                observerX -= 0.1;
            }

            break;
        case 'd':
            if(observerX >= 11.8) {
                observerX = observerX;
            } else {
                observerX += 0.1;
            }
            break;
    }

    glutPostRedisplay();

    if (isChangeView) {
        switch (key) {
            case 'i':
                rotationX -= 2.0;
                break;
            case 'k':
                rotationX += 2.0;
                break;
            case 'j':
                rotationY -= 2.0;
                break;
            case 'l':
                rotationY += 2.0;
                break;
            case 'u':
                rotationZ -= 2.0;
                break;
            case 'o':
                rotationZ += 2.0;
                break;
        }
    }

    if(key == 'v') {
        isRotate = !isRotate;
        if (isChangeView) {
                saveRotationX = rotationX;
                saveRotationY = rotationY;
                saveRotationZ = rotationZ;

                rotationX = 0.0;
                rotationY = 0.0;
                rotationZ = 0.0;

            } else {
                if (!currentView) {
                    rotationX = 20.0;
                    currentView = true;
                } else {
                    rotationX = saveRotationX;
                    rotationY = saveRotationY;
                    rotationZ = saveRotationZ;

                }
            }

            isChangeView = !isChangeView;
    }

    if(!gameOver) {
        // state player 1
        if(turn == 1) {
            if (scoreP1 >= 84) {
                scoreP1 = 84;
                glutPostRedisplay();
                cout << "Player 1 Win!!!" << endl;
                gameOver = true;
            } else {
                if (key == ' ') {
                    scoreP1 += randomNumber();
                    turn = 2;
                    cout << "Player 1: " << randomNumber() << endl;
                    glutPostRedisplay();

                    // Ladder Checking
                    if(scoreP1 != 1 && ladders[scoreP1]) {
                        scoreP1 = ladders[scoreP1];
                        glutPostRedisplay();
                    }

                    // Snake Checking
                    if(scoreP1 != 1 && snakes[scoreP1]) {
                        scoreP1 = snakes[scoreP1];
                        glutPostRedisplay();
                    }

                    if(scoreP1 >= 84) {
                        cout << "Player 1 Win!!!" << endl;
                        scoreP1 = 84;
                        gameOver = true;
                        exit(0);
                    }
                }
            }
        } else {
            // state player 2
            if (scoreP2 >= 84) {
                scoreP2 = 84;
                glutPostRedisplay();
                cout << "Player 2 Win!!!" << endl;
                gameOver = true;
            } else {
                if (key == ' ') {
                    scoreP2 += randomNumber();
                    turn = 1;
                    cout << "Player 2: " << randomNumber() << endl;
                    glutPostRedisplay();

                    // Ladder Checking
                    if(scoreP2 != 1 && ladders[scoreP2]) {
                        scoreP2 = ladders[scoreP2];
                        glutPostRedisplay();
                    }

                    // Snake Checking
                    if(scoreP2 != 1 && snakes[scoreP2]) {
                        scoreP2 = snakes[scoreP2];
                        glutPostRedisplay();
                    }

                    if(scoreP2 >= 84) {
                        cout << "Player 2 Win!!!" << endl;
                        scoreP2 = 84;
                        gameOver = true;
                        exit(0);
                    }
                }
            }
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        mouseButton = button;
    }
}

void observerObject() {
    glPushMatrix();
    glColor3f(0.3, 1.0, 0.0);
    glTranslatef(observerX, observerY, 0.0);
    if(isRotate) {
        glRotatef(obsX, 1, 0, 0);
        glRotatef(obsY, 0, 1, 0);
        glRotatef(obsZ, 0, 0, 1);
    }
    glutSolidCone(0.120, 0.3, 5, 7);
    glPopMatrix();
}

void drawLadder1() {
    glColor3f(1.0, 1.0, 0.0);

    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(6.5, 1.0, 0.3);
        glVertex3f(9.5, 1.0, 0.3);
        glVertex3f(9.5, 1.1, 0.3);
        glVertex3f(6.5, 1.1, 0.3);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(6.5, 1.0, 0.6);
        glVertex3f(9.5, 1.0, 0.6);
        glVertex3f(9.5, 1.1, 0.6);
        glVertex3f(6.5, 1.1, 0.6);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(9.5, 1.0, 0.6);
        glVertex3f(9.5, 1.0, 0.3);
        glVertex3f(9.5, 1.1, 0.3);
        glVertex3f(9.5, 1.1, 0.6);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(6.5, 1.0, 0.6);
        glVertex3f(6.5, 1.0, 0.3);
        glVertex3f(6.5, 1.1, 0.3);
        glVertex3f(6.5, 1.1, 0.6);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(6.5, 1.0, 0.6);
        glVertex3f(9.5, 1.0, 0.6);
        glVertex3f(9.5, 1.0, 0.3);
        glVertex3f(6.5, 1.0, 0.3);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(6.5, 1.1, 0.6);
        glVertex3f(9.5, 1.1, 0.6);
        glVertex3f(9.5, 1.1, 0.3);
        glVertex3f(6.5, 1.1, 0.3);
    glEnd();


    glBegin(GL_POLYGON);
        glVertex3f(6.5, 1.5, 0.3);
        glVertex3f(9.5, 1.5, 0.3);
        glVertex3f(9.5, 1.6, 0.3);
        glVertex3f(6.5, 1.6, 0.3);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
        glVertex3f(6.5, 1.5, 0.6);
        glVertex3f(9.5, 1.5, 0.6);
        glVertex3f(9.5, 1.6, 0.6);
        glVertex3f(6.5, 1.6, 0.6);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
        glVertex3f(9.5, 1.5, 0.6);
        glVertex3f(9.5, 1.5, 0.3);
        glVertex3f(9.5, 1.6, 0.3);
        glVertex3f(9.5, 1.6, 0.6);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
        glVertex3f(6.5, 1.5, 0.6);
        glVertex3f(6.5, 1.5, 0.3);
        glVertex3f(6.5, 1.6, 0.3);
        glVertex3f(6.5, 1.6, 0.6);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
        glVertex3f(6.5, 1.5, 0.6);
        glVertex3f(9.5, 1.5, 0.6);
        glVertex3f(9.5, 1.5, 0.3);
        glVertex3f(6.5, 1.5, 0.3);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
        glVertex3f(6.5, 1.6, 0.6);
        glVertex3f(9.5, 1.6, 0.6);
        glVertex3f(9.5, 1.6, 0.3);
        glVertex3f(6.5, 1.6, 0.3);
    glEnd();

    // Anak tangga
    glColor3f(1.0, 1.0, 0.0);

    float startX = 9.0;
    float endX = 7.0;
    float stepX = -0.5;
    float y1 = 1.0;
    float y2 = 1.6;
    float z1 = 0.6;
    float z2 = 0.3;

    for(float x = startX; x >= endX; x += stepX) {
        //  Front
        glBegin(GL_POLYGON);
        glVertex3f(x, y1, z2);
        glVertex3f(x + 0.1, y1, z2);
        glVertex3f(x + 0.1, y2, z2);
        glVertex3f(x, y2, z2);
        glEnd();

        //  back
        glBegin(GL_POLYGON);
        glVertex3f(x, y1, z1);
        glVertex3f(x + 0.1, y1, z1);
        glVertex3f(x + 0.1, y2, z1);
        glVertex3f(x, y2, z1);
        glEnd();

        // Sisi 1
        glBegin(GL_POLYGON);
        glVertex3f(x + 0.1, y1, z1);
        glVertex3f(x + 0.1, y1, z2);
        glVertex3f(x + 0.1, y2, z2);
        glVertex3f(x + 0.1, y2, z1);
        glEnd();

        // Sisi 2
        glBegin(GL_POLYGON);
        glVertex3f(x, y1, z1);
        glVertex3f(x, y1, z2);
        glVertex3f(x, y2, z2);
        glVertex3f(x, y2, z1);
        glEnd();

        // Sisi 3
        glBegin(GL_POLYGON);
        glVertex3f(x, y1, z1);
        glVertex3f(x + 0.1, y1, z1);
        glVertex3f(x + 0.1, y1, z2);
        glVertex3f(x, y1, z2);
        glEnd();

        // Sisi 4
        glBegin(GL_POLYGON);
        glVertex3f(x, y2, z1);
        glVertex3f(x + 0.1, y2, z1);
        glVertex3f(x + 0.1, y2, z2);
        glVertex3f(x, y2, z2);
        glEnd();
    }

}

void drawLadder2() {
    glColor3f(1.0, 1.0, 0.0);

    glBegin(GL_POLYGON);
        glVertex3f(5.0, 0.0, 0.6);
        glVertex3f(5.1, 0.0, 0.6);
        glVertex3f(5.1, 4, 0.6);
        glVertex3f(5.0, 4, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(5.0, 0.0, 0.3);
    glVertex3f(5.1, 0.0, 0.3);
    glVertex3f(5.1, 4, 0.3);
    glVertex3f(5.0, 4, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(5.1, 0.0, 0.3);
    glVertex3f(5.1, 0.0, 0.6);
    glVertex3f(5.1, 4, 0.6);
    glVertex3f(5.1, 4, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(5.0, 0.0, 0.3);
    glVertex3f(5.0, 0.0, 0.6);
    glVertex3f(5.0, 4, 0.6);
    glVertex3f(5.0, 4, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(5.0, 0.0, 0.3);
    glVertex3f(5.1, 0.0, 0.3);
    glVertex3f(5.1, 0.0, 0.6);
    glVertex3f(5.0, 0.0, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(5.0, 4, 0.3);
    glVertex3f(5.1, 4, 0.3);
    glVertex3f(5.1, 4, 0.6);
    glVertex3f(5.0, 4, 0.6);
    glEnd();

    //Penyangga 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(5.6, 0.0, 0.6);
    glVertex3f(5.7, 0.0, 0.6);
    glVertex3f(5.7, 4, 0.6);
    glVertex3f(5.6, 4, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(5.6, 0.0, 0.3);
    glVertex3f(5.7, 0.0, 0.3);
    glVertex3f(5.7, 4, 0.3);
    glVertex3f(5.6, 4, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(5.7, 0.0, 0.3);
    glVertex3f(5.7, 0.0, 0.6);
    glVertex3f(5.7, 4, 0.6);
    glVertex3f(5.7, 4, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(5.6, 0.0, 0.3);
    glVertex3f(5.6, 0.0, 0.6);
    glVertex3f(5.6, 4, 0.6);
    glVertex3f(5.6, 4, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(5.6, 0.0, 0.3);
    glVertex3f(5.7, 0.0, 0.3);
    glVertex3f(5.7, 0.0, 0.6);
    glVertex3f(5.6, 0.0, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(5.6, 4, 0.3);
    glVertex3f(5.7, 4, 0.3);
    glVertex3f(5.7, 4, 0.6);
    glVertex3f(5.6, 4, 0.6);
    glEnd();

    //anak tangga
    glColor3f(1.0, 1.0, 0.0);

    float startY = 3.5;
    float endY = 0.5;
    float stepY = 0.5;

    for(float y = startY; y >= endY; y -= stepY) {
        glBegin(GL_POLYGON);
        glVertex3f(5.0, y, 0.6);
        glVertex3f(5.7, y, 0.6);
        glVertex3f(5.7, y + 0.1, 0.6);
        glVertex3f(5.0, y + 0.1, 0.6);
        glEnd();

        //  back
        glBegin(GL_POLYGON);
        glVertex3f(5.0, y, 0.3);
        glVertex3f(5.7, y, 0.3);
        glVertex3f(5.7, y + 0.1, 0.3);
        glVertex3f(5.0, y + 0.1, 0.3);
        glEnd();

        // Sisi 1
        glBegin(GL_POLYGON);
        glVertex3f(5.7, y, 0.3);
        glVertex3f(5.7, y, 0.6);
        glVertex3f(5.7, y + 0.1, 0.6);
        glVertex3f(5.7, y + 0.1, 0.3);
        glEnd();

        // Sisi 2
        glBegin(GL_POLYGON);
        glVertex3f(5.0, y, 0.3);
        glVertex3f(5.0, y, 0.6);
        glVertex3f(5.0, y + 0.1, 0.6);
        glVertex3f(5.0, y + 0.1, 0.3);
        glEnd();

        // Sisi 3
        glBegin(GL_POLYGON);
        glVertex3f(5.0, y, 0.3);
        glVertex3f(5.7, y, 0.3);
        glVertex3f(5.7, y, 0.6);
        glVertex3f(5.0, y, 0.6);
        glEnd();

        // Sisi 4
        glBegin(GL_POLYGON);
        glVertex3f(5.0, y + 0.1, 0.3);
        glVertex3f(5.7, y + 0.1, 0.3);
        glVertex3f(5.7, y + 0.1, 0.6);
        glVertex3f(5.0, y + 0.1, 0.6);
        glEnd();
    }
}

void drawLadder3() {
    // Yellow
    glPushMatrix();
    glRotatef(-30.0, 0.0, 0.0, 1.0);

    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.2, 4.5, 0.6);
    glVertex3f(0.3, 4.5, 0.6);
    glVertex3f(0.3, 8.0, 0.6);
    glVertex3f(0.2, 8.0, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(0.2, 4.5, 0.3);
    glVertex3f(0.3, 4.5, 0.3);
    glVertex3f(0.3, 8.0, 0.3);
    glVertex3f(0.2, 8.0, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(0.3, 4.5, 0.3);
    glVertex3f(0.3, 4.5, 0.6);
    glVertex3f(0.3, 8.0, 0.6);
    glVertex3f(0.3, 8.0, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(0.2, 4.5, 0.3);
    glVertex3f(0.2, 4.5, 0.6);
    glVertex3f(0.2, 8.0, 0.6);
    glVertex3f(0.2, 8.0, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(0.2, 4.5, 0.3);
    glVertex3f(0.3, 4.5, 0.3);
    glVertex3f(0.3, 4.5, 0.6);
    glVertex3f(0.2, 4.5, 0.6);
    glEnd();

    // Sisi 8.0
    glBegin(GL_POLYGON);
    glVertex3f(0.2, 8.0, 0.3);
    glVertex3f(0.3, 8.0, 0.3);
    glVertex3f(0.3, 8.0, 0.6);
    glVertex3f(0.2, 8.0, 0.6);
    glEnd();

    //Penyangga 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(0.7, 4.5, 0.6);
    glVertex3f(0.8, 4.5, 0.6);
    glVertex3f(0.8, 8.0, 0.6);
    glVertex3f(0.7, 8.0, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(0.7, 4.5, 0.3);
    glVertex3f(0.8, 4.5, 0.3);
    glVertex3f(0.8, 8.0, 0.3);
    glVertex3f(0.7, 8.0, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(0.8, 4.5, 0.3);
    glVertex3f(0.8, 4.5, 0.6);
    glVertex3f(0.8, 8.0, 0.6);
    glVertex3f(0.8, 8.0, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(0.7, 4.5, 0.3);
    glVertex3f(0.7, 4.5, 0.6);
    glVertex3f(0.7, 8.0, 0.6);
    glVertex3f(0.7, 8.0, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(0.7, 4.5, 0.3);
    glVertex3f(0.8, 4.5, 0.3);
    glVertex3f(0.8, 4.5, 0.6);
    glVertex3f(0.7, 4.5, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(0.7, 8.0, 0.3);
    glVertex3f(0.8, 8.0, 0.3);
    glVertex3f(0.8, 8.0, 0.6);
    glVertex3f(0.7, 8.0, 0.6);
    glEnd();

    //anak tangga
    glColor3f(1.0, 1.0, 0.0);

    float startY = 7.5;
    float endY = 5.0;
    float stepY = 0.5;

    for(float y = startY; y >= endY; y -= stepY) {
        glBegin(GL_POLYGON);
        glVertex3f(0.2, y, 0.6);
        glVertex3f(0.8, y, 0.6);
        glVertex3f(0.8, y + 0.1, 0.6);
        glVertex3f(0.2, y + 0.1, 0.6);
        glEnd();

        //  back
        glBegin(GL_POLYGON);
        glVertex3f(0.2, y, 0.3);
        glVertex3f(0.8, y, 0.3);
        glVertex3f(0.8, y + 0.1, 0.3);
        glVertex3f(0.2, y + 0.1, 0.3);
        glEnd();

        // Sisi 1
        glBegin(GL_POLYGON);
        glVertex3f(0.8, y, 0.3);
        glVertex3f(0.8, y, 0.6);
        glVertex3f(0.8, y + 0.1, 0.6);
        glVertex3f(0.8, y + 0.1, 0.3);
        glEnd();

        // Sisi 2
        glBegin(GL_POLYGON);
        glVertex3f(0.2, y, 0.3);
        glVertex3f(0.2, y, 0.6);
        glVertex3f(0.2, y + 0.1, 0.6);
        glVertex3f(0.2, y + 0.1, 0.3);
        glEnd();

        // Sisi 3
        glBegin(GL_POLYGON);
        glVertex3f(0.2, y, 0.3);
        glVertex3f(0.8, y, 0.3);
        glVertex3f(0.8, y, 0.6);
        glVertex3f(0.2, y, 0.6);
        glEnd();

        // Sisi 4
        glBegin(GL_POLYGON);
        glVertex3f(0.2, y + 0.1, 0.3);
        glVertex3f(0.8, y + 0.1, 0.3);
        glVertex3f(0.8, y + 0.1, 0.6);
        glVertex3f(0.2, y + 0.1, 0.6);
        glEnd();
    }

    glPopMatrix();
}

void drawSnake1() {
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex3f(1.6, 1.0, 0.6);
    glVertex3f(1.8, 1.0, 0.6);
    glVertex3f(2.5, 1.3, 0.6);
    glVertex3f(2.3, 1.3, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(1.6, 1.0, 0.3);
    glVertex3f(1.8, 1.0, 0.3);
    glVertex3f(2.5, 1.3, 0.3);
    glVertex3f(2.3, 1.3, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(1.8, 1.0, 0.3);
    glVertex3f(1.8, 1.0, 0.6);
    glVertex3f(2.5, 1.3, 0.6);
    glVertex3f(2.5, 1.3, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(1.6, 1.0, 0.3);
    glVertex3f(1.6, 1.0, 0.6);
    glVertex3f(2.3, 1.3, 0.6);
    glVertex3f(2.3, 1.3, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(1.6, 1.0, 0.3);
    glVertex3f(1.8, 1.0, 0.3);
    glVertex3f(1.8, 1.0, 0.6);
    glVertex3f(1.6, 1.0, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(2.3, 1.3, 0.3);
    glVertex3f(2.5, 1.3, 0.3);
    glVertex3f(2.5, 1.3, 0.6);
    glVertex3f(2.3, 1.3, 0.6);
    glEnd();

    //Polygon 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(2.5, 1.3, 0.6);
    glVertex3f(2.3, 1.3, 0.6);
    glVertex3f(0.6, 3.6, 0.6);
    glVertex3f(0.8, 3.6, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(2.5, 1.3, 0.3);
    glVertex3f(2.3, 1.3, 0.3);
    glVertex3f(0.6, 3.6, 0.3);
    glVertex3f(0.8, 3.6, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(2.3, 1.3, 0.3);
    glVertex3f(2.3, 1.3, 0.6);
    glVertex3f(0.6, 3.6, 0.6);
    glVertex3f(0.6, 3.6, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(2.5, 1.3, 0.3);
    glVertex3f(2.5, 1.3, 0.6);
    glVertex3f(0.8, 3.6, 0.6);
    glVertex3f(0.8, 3.6, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(2.3, 1.3, 0.3);
    glVertex3f(2.5, 1.3, 0.3);
    glVertex3f(0.8, 3.6, 0.3);
    glVertex3f(0.6, 3.6, 0.3);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(2.3, 1.3, 0.6);
    glVertex3f(2.5, 1.3, 0.6);
    glVertex3f(0.8, 3.6, 0.6);
    glVertex3f(0.6, 3.6, 0.6);
    glEnd();

    //Polygon 3
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(0.6, 3.6, 0.6);
    glVertex3f(0.8, 3.6, 0.6);
    glVertex3f(1.5, 4.3, 0.6);
    glVertex3f(1.3, 4.3, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(0.6, 3.6, 0.3);
    glVertex3f(0.8, 3.6, 0.3);
    glVertex3f(1.5, 4.3, 0.3);
    glVertex3f(1.3, 4.3, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(0.8, 3.6, 0.3);
    glVertex3f(0.8, 3.6, 0.6);
    glVertex3f(1.5, 4.3, 0.6);
    glVertex3f(1.5, 4.3, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(0.6, 3.6, 0.3);
    glVertex3f(0.6, 3.6, 0.6);
    glVertex3f(1.3, 4.3, 0.6);
    glVertex3f(1.3, 4.3, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(0.6, 3.6, 0.3);
    glVertex3f(0.8, 3.6, 0.3);
    glVertex3f(0.8, 3.6, 0.6);
    glVertex3f(0.6, 3.6, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(1.3, 4.3, 0.3);
    glVertex3f(1.5, 4.3, 0.3);
    glVertex3f(1.5, 4.3, 0.6);
    glVertex3f(1.3, 4.3, 0.6);
    glEnd();
}

void drawSnake2() {
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex3f(8.3, 2.8, 0.6);
    glVertex3f(8.5, 2.8, 0.6);
    glVertex3f(7.8, 3.8, 0.6);
    glVertex3f(7.6, 3.8, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 2.8, 0.3);
    glVertex3f(8.5, 2.8, 0.3);
    glVertex3f(7.8, 3.8, 0.3);
    glVertex3f(7.6, 3.8, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(8.5, 2.8, 0.3);
    glVertex3f(8.5, 2.8, 0.6);
    glVertex3f(7.8, 3.8, 0.6);
    glVertex3f(7.8, 3.8, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 2.8, 0.3);
    glVertex3f(8.3, 2.8, 0.6);
    glVertex3f(7.6, 3.8, 0.6);
    glVertex3f(7.6, 3.8, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 2.8, 0.3);
    glVertex3f(8.5, 2.8, 0.3);
    glVertex3f(8.5, 2.8, 0.6);
    glVertex3f(8.3, 2.8, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(7.6, 3.8, 0.3);
    glVertex3f(7.8, 3.8, 0.3);
    glVertex3f(7.8, 3.8, 0.6);
    glVertex3f(7.6, 3.8, 0.6);
    glEnd();

    //Polygon 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(7.6, 3.8, 0.6);
    glVertex3f(7.8, 3.8, 0.6);
    glVertex3f(8.5, 4.3, 0.6);
    glVertex3f(8.3, 4.3, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(7.6, 3.8, 0.3);
    glVertex3f(7.8, 3.8, 0.3);
    glVertex3f(8.5, 4.3, 0.3);
    glVertex3f(8.3, 4.3, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(7.8, 3.8, 0.3);
    glVertex3f(7.8, 3.8, 0.6);
    glVertex3f(8.5, 4.3, 0.6);
    glVertex3f(8.5, 4.3, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(7.6, 3.8, 0.3);
    glVertex3f(7.6, 3.8, 0.6);
    glVertex3f(8.3, 4.3, 0.6);
    glVertex3f(8.3, 4.3, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(7.6, 3.8, 0.3);
    glVertex3f(7.8, 3.8, 0.3);
    glVertex3f(7.8, 3.8, 0.6);
    glVertex3f(7.6, 3.8, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 4.3, 0.3);
    glVertex3f(8.5, 4.3, 0.3);
    glVertex3f(8.5, 4.3, 0.6);
    glVertex3f(8.3, 4.3, 0.6);
    glEnd();

    //Polygon 3
     //  Front
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 4.3, 0.6);
    glVertex3f(8.5, 4.3, 0.6);
    glVertex3f(7.8, 5.5, 0.6);
    glVertex3f(7.6, 5.5, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 4.3, 0.3);
    glVertex3f(8.5, 4.3, 0.3);
    glVertex3f(7.8, 5.5, 0.3);
    glVertex3f(7.6, 5.5, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(8.5, 4.3, 0.3);
    glVertex3f(8.5, 4.3, 0.6);
    glVertex3f(7.8, 5.5, 0.6);
    glVertex3f(7.8, 5.5, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 4.3, 0.3);
    glVertex3f(8.3, 4.3, 0.6);
    glVertex3f(7.6, 5.5, 0.6);
    glVertex3f(7.6, 5.5, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(8.3, 4.3, 0.3);
    glVertex3f(8.5, 4.3, 0.3);
    glVertex3f(8.5, 4.3, 0.6);
    glVertex3f(8.3, 4.3, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(7.6, 5.5, 0.3);
    glVertex3f(7.8, 5.5, 0.3);
    glVertex3f(7.8, 5.5, 0.6);
    glVertex3f(7.6, 5.5, 0.6);
    glEnd();

    //Polygon 4
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(7.8, 5.5, 0.6);
    glVertex3f(7.6, 5.5, 0.6);
    glVertex3f(8.3, 6.3, 0.6);
    glVertex3f(8.5, 6.3, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(7.8, 5.5, 0.3);
    glVertex3f(7.6, 5.5, 0.3);
    glVertex3f(8.3, 6.3, 0.3);
    glVertex3f(8.5, 6.3, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(7.6, 5.5, 0.3);
    glVertex3f(7.6, 5.5, 0.6);
    glVertex3f(8.3, 6.3, 0.6);
    glVertex3f(8.3, 6.3, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(7.8, 5.5, 0.3);
    glVertex3f(7.8, 5.5, 0.6);
    glVertex3f(8.5, 6.3, 0.6);
    glVertex3f(8.5, 6.3, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(7.8, 5.5, 0.3);
    glVertex3f(7.6, 5.5, 0.3);
    glVertex3f(7.6, 5.5, 0.6);
    glVertex3f(7.8, 5.5, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(8.5, 6.3, 0.3);
    glVertex3f(8.3, 6.3, 0.3);
    glVertex3f(8.3, 6.3, 0.6);
    glVertex3f(8.5, 6.3, 0.6);
    glEnd();
}

void drawSnake3() {
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex3f(10.6, 1.0, 0.6);
    glVertex3f(10.8, 1.0, 0.6);
    glVertex3f(11.5, 1.3, 0.6);
    glVertex3f(11.3, 1.3, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(10.6, 1.0, 0.3);
    glVertex3f(10.8, 1.0, 0.3);
    glVertex3f(11.5, 1.3, 0.3);
    glVertex3f(11.3, 1.3, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(10.8, 1.0, 0.3);
    glVertex3f(10.8, 1.0, 0.6);
    glVertex3f(11.5, 1.3, 0.6);
    glVertex3f(11.5, 1.3, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(10.6, 1.0, 0.3);
    glVertex3f(10.6, 1.0, 0.6);
    glVertex3f(11.3, 1.3, 0.6);
    glVertex3f(11.3, 1.3, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(10.6, 1.0, 0.3);
    glVertex3f(10.8, 1.0, 0.3);
    glVertex3f(10.8, 1.0, 0.6);
    glVertex3f(10.6, 1.0, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(11.3, 1.3, 0.3);
    glVertex3f(11.5, 1.3, 0.3);
    glVertex3f(11.5, 1.3, 0.6);
    glVertex3f(11.3, 1.3, 0.6);
    glEnd();

    //Polygon 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(11.5, 1.3, 0.6);
    glVertex3f(11.3, 1.3, 0.6);
    glVertex3f(9.6, 3.6, 0.6);
    glVertex3f(9.8, 3.6, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(11.5, 1.3, 0.3);
    glVertex3f(11.3, 1.3, 0.3);
    glVertex3f(9.6, 3.6, 0.3);
    glVertex3f(9.8, 3.6, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(11.3, 1.3, 0.3);
    glVertex3f(11.3, 1.3, 0.6);
    glVertex3f(9.6, 3.6, 0.6);
    glVertex3f(9.6, 3.6, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(11.5, 1.3, 0.3);
    glVertex3f(11.5, 1.3, 0.6);
    glVertex3f(9.8, 3.6, 0.6);
    glVertex3f(9.8, 3.6, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(11.3, 1.3, 0.3);
    glVertex3f(11.5, 1.3, 0.3);
    glVertex3f(9.8, 3.6, 0.3);
    glVertex3f(9.6, 3.6, 0.3);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(11.3, 1.3, 0.6);
    glVertex3f(11.5, 1.3, 0.6);
    glVertex3f(9.8, 3.6, 0.6);
    glVertex3f(9.6, 3.6, 0.6);
    glEnd();

    //Polygon 3
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(9.6, 3.6, 0.6);
    glVertex3f(9.8, 3.6, 0.6);
    glVertex3f(10.5, 4.3, 0.6);
    glVertex3f(10.3, 4.3, 0.6);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(9.6, 3.6, 0.3);
    glVertex3f(9.8, 3.6, 0.3);
    glVertex3f(10.5, 4.3, 0.3);
    glVertex3f(10.3, 4.3, 0.3);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(9.8, 3.6, 0.3);
    glVertex3f(9.8, 3.6, 0.6);
    glVertex3f(10.5, 4.3, 0.6);
    glVertex3f(10.5, 4.3, 0.3);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(9.6, 3.6, 0.3);
    glVertex3f(9.6, 3.6, 0.6);
    glVertex3f(10.3, 4.3, 0.6);
    glVertex3f(10.3, 4.3, 0.3);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(9.6, 3.6, 0.3);
    glVertex3f(9.8, 3.6, 0.3);
    glVertex3f(9.8, 3.6, 0.6);
    glVertex3f(9.6, 3.6, 0.6);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(10.3, 4.3, 0.3);
    glVertex3f(10.5, 4.3, 0.3);
    glVertex3f(10.5, 4.3, 0.6);
    glVertex3f(10.3, 4.3, 0.6);
    glEnd();
}

void drawNumber9(float x, float y){
    glColor3f(0.3, 1.0, 0.0);
    // Polygon 1
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.25);
    glVertex3f(x + 0.1, y, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x, y - 0.05, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y, 0.25);
    glVertex3f(x, y, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.05, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 0.25);
    glVertex3f(x, y - 0.05, 0.25);
    glEnd();

    // Polygon 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.1, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x + 0.05, y - 0.1, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glVertex3f(x + 0.05, y - 0.1, 0.25);
    glEnd();

    // Polygon 3
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x, y - 0.1, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Polygon 4
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glEnd();

     //  back
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glVertex3f(x + 0.05, y - 0.20, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.05, y - 0.20, 0.0);
    glVertex3f(x + 0.05, y - 0.20, 0.25);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.20, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glVertex3f(x + 0.05, y - 0.20, 0.25);
    glEnd();

    // Polygon 5
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.25, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.20, 0.25);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x - 0.05, y - 0.25, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glVertex3f(x - 0.05, y - 0.20, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.20, 0.0);
    glVertex3f(x - 0.05, y - 0.20, 0.25);
    glVertex3f(x - 0.05, y - 0.25, 0.25);
    glEnd();

    // Polygon 6
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x, y, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 0.25);
    glVertex3f(x - 0.05, y, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x, y, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glEnd();
}

void drawNumber4(float x, float y){
    // Polygon 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.1, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x + 0.05, y - 0.1, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.1, 0.0);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glVertex3f(x + 0.05, y - 0.1, 0.25);
    glEnd();

    // Polygon 3
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x, y - 0.1, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Polygon 4
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glEnd();

     //  back
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glEnd();

    // Polygon 6
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x, y, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 0.25);
    glVertex3f(x - 0.05, y, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x, y, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glEnd();


}

void drawNumber5(float x, float y){
    // Polygon 1
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.25);
    glVertex3f(x + 0.1, y, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x, y - 0.05, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y, 0.25);
    glVertex3f(x, y, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.05, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 0.25);
    glVertex3f(x, y - 0.05, 0.25);
    glEnd();

    // Polygon 2
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x, y, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 0.25);
    glVertex3f(x - 0.05, y, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x, y, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.15, 0.0);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x - 0.05, y - 0.15, 0.25);
    glEnd();


    // Polygon 3
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glVertex3f(x, y - 0.1, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.1, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.1, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.15, 0.0);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x, y - 0.15, 0.25);
    glEnd();

    // Polygon 4
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glEnd();

     //  back
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.15, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.15, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.0);
    glVertex3f(x + 0.05, y - 0.15, 0.25);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glEnd();

    // Polygon 5
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.25, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.20, 0.25);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x - 0.05, y - 0.25, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glVertex3f(x - 0.05, y - 0.20, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.20, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.20, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.25, 0.0);
    glVertex3f(x - 0.05, y - 0.20, 0.0);
    glVertex3f(x - 0.05, y - 0.20, 0.25);
    glVertex3f(x - 0.05, y - 0.25, 0.25);
    glEnd();
}

void drawNumber7(float x, float y){
    // Polygon 1
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.25);
    glVertex3f(x + 0.1, y, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x, y - 0.05, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y, 0.25);
    glVertex3f(x, y, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.0);
    glVertex3f(x, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.05, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 0.25);
    glVertex3f(x, y - 0.05, 0.25);
    glEnd();

    // Polygon 2
    //  Front
       glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.25, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.05, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.05, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.1, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glVertex3f(x + 0.1, y - 0.25, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x + 0.05, y - 0.25, 0.0);
    glVertex3f(x + 0.05, y - 0.05, 0.0);
    glVertex3f(x + 0.05, y - 0.05, 0.25);
    glVertex3f(x + 0.05, y - 0.25, 0.25);
    glEnd();

    // Polygon 3
    //  Front
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x - 0.05, y - 0.1, 0.0);
    glEnd();

    //  back
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x, y, 0.25);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x - 0.05, y - 0.1, 0.25);
    glEnd();

    // Sisi 1
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 0.25);
    glVertex3f(x - 0.05, y, 0.25);
    glEnd();

    // Sisi 2
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x, y - 0.1, 0.25);
    glVertex3f(x, y, 0.25);
    glEnd();

    // Sisi 3
    glBegin(GL_POLYGON);
    glVertex3f(x, y - 0.1, 0.0);
    glVertex3f(x - 0.05, y - 0.1, 0.0);
    glVertex3f(x - 0.05, y - 0.1, 0.25);
    glVertex3f(x, y - 0.1, 0.25);
    glEnd();

    // Sisi 4
    glBegin(GL_POLYGON);
    glVertex3f(x - 0.05, y - 0.1, 0.0);
    glVertex3f(x - 0.05, y, 0.0);
    glVertex3f(x - 0.05, y, 0.25);
    glVertex3f(x - 0.05, y - 0.1, 0.25);
    glEnd();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    glTranslatef(centerX, centerY, centerZ);
        glRotatef(rotationX, 1.0, 0.0, 0.0);
        glRotatef(rotationY, 0.0, 1.0, 0.0);
        glRotatef(rotationZ, 0.0, 0.0, 1.0);
    glTranslatef(-centerX, -centerY, -centerZ);

	glPointSize(4);

    //drawTile(7, 12);
    drawBorderTile(12, 7, 0.3);
    drawTrack(12,6,1,0.1,0.3);
    pionTile(scoreP1);
    pionTriangle(scoreP2);
    observerObject();

    drawNumber9(8.80,0.85);
    drawNumber9(5.80,1.85);
    drawNumber9(4.80,2.85);
    drawNumber9(9.80,3.85);
    drawNumber9(0.80,4.85);
    drawNumber9(10.80,4.85);
    drawNumber9(3.80,5.85);
    drawNumber9(6.80,6.85);

    drawNumber4(3.80,0.85);
    drawNumber4(10.80,1.85);
    drawNumber4(0.80,1.85);
    drawNumber4(9.80,2.85);
    drawNumber4(4.80,3.85);
    drawNumber4(5.80,4.85);
    drawNumber4(8.80,5.85);
    drawNumber4(1.80,6.85);
    drawNumber4(11.70,6.85);
    drawNumber4(8.80,3.85);
    drawNumber4(7.80,3.85);
    drawNumber4(6.80,3.85);
    drawNumber4(5.80,3.85);
    drawNumber4(4.60,3.85);
    drawNumber4(3.60,3.85);
    drawNumber4(2.80,3.85);
    drawNumber4(1.60,3.85);
    drawNumber4(0.80,3.85);
    drawNumber4(0.60,4.85);

    drawNumber5(4.80,0.85);
    drawNumber5(9.80,1.85);
    drawNumber5(0.80,2.85);
    drawNumber5(10.80,2.85);
    drawNumber5(3.80,3.85);
    drawNumber5(6.80,4.85);
    drawNumber5(7.80,5.85);
    drawNumber5(2.80,6.85);
    drawNumber5(10.60,4.85);
    drawNumber5(6.60,4.85);
    drawNumber5(5.60,4.85);
    drawNumber5(7.80,4.85);
    drawNumber5(8.60,4.85);
    drawNumber5(9.80,4.85);
    drawNumber5(1.80,4.85);
    drawNumber5(2.80,4.85);
    drawNumber5(3.80,4.85);
    drawNumber5(4.80,4.85);

    drawNumber7(6.80,0.85);
    drawNumber7(7.80,1.85);
    drawNumber7(2.80,2.85);
    drawNumber7(11.80,3.85);
    drawNumber7(1.80,3.85);
    drawNumber7(8.80,4.85);
    drawNumber7(5.80,5.85);
    drawNumber7(4.80,6.85);
    drawNumber7(2.80,5.85);
    drawNumber7(1.80,5.85);
    drawNumber7(0.80,5.85);
    drawNumber7(0.80,6.85);
    drawNumber7(1.60,6.85);
    drawNumber7(2.60,6.85);
    drawNumber7(4.60,6.85);
    drawNumber7(3.80,6.85);
    drawNumber7(4.80,6.85);
    drawNumber7(5.80,6.85);
    drawNumber7(6.60,6.85);

    drawLadder1();
    drawLadder2();
    drawLadder3();

    drawSnake1();
    drawSnake2();
    drawSnake3();

    drawTile(7, 12);
    glutKeyboardFunc(onInput);
	glFlush();
	glutSwapBuffers();

}

void myIdle() {
    theta += 0.1; // Tambah nilai theta untuk rotasi
    if (theta >= 360.0) {
        theta -= 360.0;
    }

    // Perbarui sudut rotasi yang sesuai
    if (isRotate){
        if (mouseButton == GLUT_LEFT_BUTTON) {
            obsX += 0.1;
            if (obsX >= 360.0) obsX -= 360.0;
        } else if (mouseButton == GLUT_RIGHT_BUTTON) {
            obsZ += 0.1;
            if (obsZ >= 360.0) obsZ -= 360.0;
        } else if (mouseButton == GLUT_MIDDLE_BUTTON) {
            obsY += 0.1;
            if (obsY >= 360.0) obsY -= 360.0;
        }
    }

    glutPostRedisplay();
}

void myinit(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.1,12.1,-0.1,7.1,-20,30);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat light_position_diff[] = { -1.0, 1.0, 1.0, 0.0 };
    GLfloat diffuse_light[] = { 0.0, 0.0, 0.9, 1.0 };
    GLfloat light_position_spec[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat specular_light[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat ambient_light[] = { 0.9, 0.9, 0.9, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position_diff);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_spec);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient_light);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);

	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void init_snakes_ladders(){
	snakes = new int[100];
	ladders = new int[100];

    // Ladder Init
	ladders[1] = 1;
	ladders[6] = 54;
	ladders[15] = 18;
	ladders[46] = 77;

	// Snake Init
	snakes[50] = 23;
	snakes[59] = 14;
	snakes[81] = 33;
}

int main(int argc, char* argv[]){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600,350);
	glutInitWindowPosition(300,200);
	glutCreateWindow("Ular Tangga");
	cout << "Tekan Spasi Untuk Memulai Permainan" << endl;
	glutDisplayFunc(display);
	myinit();
	init_snakes_ladders();
    glutIdleFunc(myIdle);
    glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
