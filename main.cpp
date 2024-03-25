#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include "class/ImageLoader.h"
#include "class/Camera.h"
#include "class/Primitivas.h"

using namespace std;

const char *nomeArquivo = "./images/imagem.pgm";

int width = 350;
int height = 350;

static float ultimaPosicaoMouse = 0.0; // Última posição do mouse
static bool primeiraVezMouse = true; // Primeira vez que o mouse passa na tela

GLfloat posicaoLuzPontual[4] = {1.0, 5.0, 1.1, 0.7};

Camera camera(vetor3(0.0, 1.0, 10.0));

void luzAmbiente()
{
    GLfloat posicao[4] = {0.0, 1.0, 0.0, 0.0};
    GLfloat luzDifusa[4] = {0.1, 0.1, 0.1, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, posicao);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);

    glEnable(GL_LIGHT0);
}

void luzPontual()
{
    GLfloat difusa[4] = {0.4, 0.4, 0.4, 1.0};
    GLfloat especular[4] = {0.5, 0.5, 0.5, 1.0};
    GLfloat ambiente[4] = {0.05, 0.05, 0.05, 1.0};

    glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuzPontual);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT1, GL_SPECULAR, especular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambiente);

    glEnable(GL_LIGHT1);

    glPushMatrix();
    glTranslatef(posicaoLuzPontual[0], posicaoLuzPontual[1], posicaoLuzPontual[2]);
    glColor3f(0.8, 0.8, 0.0);
    glutSolidSphere(0.4, 50, 50);
    glPopMatrix();
}

void material()
{
    GLfloat difusa[4] = {0.8, 0.8, 0.8, 1.0};
    GLfloat ambiente[4] = {0.2, 0.8, 0.2, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difusa);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0.0);
    
    luzPontual();

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void desenhaMapa(Matriz image)
{
    int largura = image.getLargura() / 2;
    int altura = image.getAltura() / 2;

    float x1, x2, x3, x4, z1, z2, z3, z4;

    glPushAttrib(GL_LIGHTING_BIT);
    glEnable(GL_LIGHTING);
    material();

    for(int i = -altura + 1; i < altura; i++)
    {
        for(int j = -largura + 1; j < largura; j++)
        {
            // PRIMEIRO TRIÂNGULO

            // Primeiro ponto
            int indiceI = i + (altura - 1);
            int indiceJ = j + (largura - 1);
            float y1 = (float) image.getValorPosicao(indiceI, indiceJ); // Isso para poder pegar os valores a partir do início da matriz
            x1 = (float) i;
            z1 = (float) j;
            if(y1 == 0.0) y1 = -0.5;

            // Segundo ponto
            indiceI = i + (altura - 1);
            indiceJ = j + largura;
            x2 = (float) i;
            z2 = (float) j + 1;
            float y2 = image.getValorPosicao(indiceI, indiceJ);
            if(y2 == 0.0) y2 = -0.5;

            // Terceiro ponto
            indiceI = i + altura;
            indiceJ = j + (largura - 1);
            x3 = (float) i + 1;
            z3 = (float) j;
            float y3 = (float) image.getValorPosicao(indiceI, indiceJ);
            if(y3 == 0.0) y3 = -0.5;

            float normalX = (x1 + x2 + x3) / 3;
            float normalY = (y1 + y2 + y3) / 3;
            float normalZ = (z1 + z2 + z3) / 3;
            
            glBegin(GL_TRIANGLES);
                glNormal3f(normalX, normalY, normalZ);
                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
                glVertex3f(x3, y3, z3);
            glEnd();

            // SEGUNDO TRIÂNGULO

            // Quarto ponto
            indiceI = i + altura;
            indiceJ = j + largura;
            x4 = (float) i + 1;
            z4 = (float) j + 1;
            float y4 = (float) image.getValorPosicao(indiceI, indiceJ);
            if(y4 == 0.0) y4 = -0.5;

            normalX = (x1 + x2 + x3) / 3;
            normalY = (y1 + y2 + y3) / 3;
            normalZ = (z1 + z2 + z3) / 3;
            
            glBegin(GL_TRIANGLES);
                glNormal3f(normalX, normalY, normalZ);
                glVertex3f(x3, y3, z3);
                glVertex3f(x2, y2, z2);
                glVertex3f(x4, y4, z4);
            glEnd();
        }
    }

    glDisable(GL_LIGHTING);
    glPopAttrib();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    camera.ativar();

    luzAmbiente();
    // luzPontual();

    Matriz matriz = ImageLoader().load(nomeArquivo);

    desenhaMapa(matriz);
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    float aspect = (float) w / (float) h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();   
    // glOrtho(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0, 1.0, -1.0);
    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void keys(unsigned char key, int x, int y)
{
    float angulo = (2 * M_PI) / 180;

    if(key == 'w') camera.frente();
    if(key == 's') camera.tras();
    if(key == 'a') camera.esquerda();
    if(key == 'd') camera.direita();

    glutPostRedisplay();
}

void mouse(int x, int y)
{
    float direcaoX;

    if(primeiraVezMouse)
    {
        direcaoX = 0.0;
        ultimaPosicaoMouse = x;
        primeiraVezMouse = false;
    }

    direcaoX = x - ultimaPosicaoMouse;
    ultimaPosicaoMouse = x;

    camera.updateYaw(direcaoX);
    camera.update();

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Eu, Carrinho!");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    // glutSpecialFunc(keys);
    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keys);

    glutMainLoop();
}
