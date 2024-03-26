#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include "class/ImageLoader.h"
#include "class/Camera.h"
#include "class/Primitivas.h"
#include "class/objLoader.h"


using namespace std;

static unsigned blenderModelId;

const char *nomeArquivo = "./images/imagem.pgm";
const char *nomeArquivoCarro = "./images/carro.obj";

int width = 800;
int height = 600;

static float ultimaPosicaoMouse = 0.0;
static bool primeiraVezMouse = true;

Camera camera(vetor3(0.0, -0.45, 1.35));

void luzAmbiente()
{
    GLfloat posicao[4] = {0.0, 0.0, 1.0, 0.0};
    GLfloat luzDifusa[4] = {0.1, 0.1, 0.1, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, posicao);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);

    glEnable(GL_LIGHT0);
}

void luzPontual()
{
    GLfloat posicaoLuzPontual[4] = {1.0, 1.0, 15.0, 0.7};

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
    glutSolidSphere(1.0, 50, 50);
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

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    ObjLoader::loadOBJ(blenderModelId, nomeArquivoCarro);
}

void desenhaMapa(Matriz image)
{
    int largura = image.getLargura() / 2;
    int altura = image.getAltura() / 2;

    float x1, x2, x3, x4, y1, y2, y3, y4;

    vector<GLfloat> vertices;
    vector<GLfloat> cores;
    vector<GLfloat> normais;

    float normalX, normalY, normalZ = 0.0;

    for(int i = -altura + 1; i < altura; i++)
    {
        for(int j = -largura + 1; j < largura; j++)
        {
            // Primeiro ponto
            int indiceI = i + (altura - 1);
            int indiceJ = j + (largura - 1);
            float z1 = (float) image.getValorPosicao(indiceI, indiceJ);
            x1 = (float) i;
            y1 = (float) j;

            // Segundo ponto
            indiceI = i + (altura - 1);
            indiceJ = j + largura;
            x2 = (float) i;
            y2 = (float) j + 1;
            float z2 = image.getValorPosicao(indiceI, indiceJ);

            // Terceiro ponto
            indiceI = i + altura;
            indiceJ = j + (largura - 1);
            x3 = (float) i + 1;
            y3 = (float) j;
            float z3 = (float) image.getValorPosicao(indiceI, indiceJ);

            // Quarto ponto
            indiceI = i + altura;
            indiceJ = j + largura;
            x4 = (float) i + 1;
            y4 = (float) j + 1;
            float z4 = (float) image.getValorPosicao(indiceI, indiceJ);

            vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(z1);
            vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(z2);
            vertices.push_back(x3); vertices.push_back(y3); vertices.push_back(z3);

            // Cor verde para o primeiro triângulo
            for(int k = 0; k < 3; k++)
            {
                cores.push_back(0.0); // R
                cores.push_back(1.0); // G
                cores.push_back(0.0); // B
            }

            float lado1x = x3 - x1;
            float lado1y = y3 - y1;
            float lado1z = z3 - z1;
            float lado2x = x2 - x1;
            float lado2y = y2 - y1;
            float lado2z = z2 - z1;

            float nx1 = lado1y * lado2z - lado1z * lado2y;
            float ny1 = lado1z * lado2x - lado1x * lado2z;
            float nz1 = lado1x * lado2y - lado1y * lado2x;

            float comprimento1 = sqrt(nx1 * nx1 + ny1 * ny1 + nz1 * nz1);
            nx1 /= comprimento1;
            ny1 /= comprimento1;
            nz1 /= comprimento1;

            for(int k = 0; k < 3; k++)
            {
                normais.push_back(nx1);
                normais.push_back(ny1);
                normais.push_back(nz1);
            }

            vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(z2);
            vertices.push_back(x3); vertices.push_back(y3); vertices.push_back(z3);
            vertices.push_back(x4); vertices.push_back(y4); vertices.push_back(z4);
            
            for(int k = 0; k < 3; k++)
            {
                cores.push_back(0.0); // R
                cores.push_back(1.0); // G
                cores.push_back(0.0); // B
            }

            float lado3x = x2 - x4;
            float lado3y = y2 - y4;
            float lado3z = z2 - z4;

            float lado4x = x3 - x4;
            float lado4y = y3 - y4;
            float lado4z = z3 - z4;

            float nx2 = lado3y * lado4z - lado3z * lado4y;
            float ny2 = lado3z * lado4x - lado3x * lado4z;
            float nz2 = lado3x * lado4y - lado3y * lado4x;

            float norma2 = sqrt(nx2 * nx2 + ny2 * ny2 + nz2 * nz2);

            nx2 /= norma2;
            ny2 /= norma2;
            nz2 /= norma2;
            
            for(int k = 0; k < 3; k++)
            {
                normais.push_back(nx2);
                normais.push_back(ny2);
                normais.push_back(nz2);
            }
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glColorPointer(3, GL_FLOAT, 0, &cores[0]);
    glNormalPointer(GL_FLOAT, 0, &normais[0]);

    glPushAttrib(GL_LIGHTING_BIT);
    glEnable(GL_LIGHTING);
    material();

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

    glDisable(GL_LIGHTING);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopAttrib();
}

void desenhaCarro(Camera& camera)
{
    glEnable(GL_LIGHTING);
    
    GLfloat ambient[] = {0.2125f, 0.1275f, 0.054f};
    GLfloat diffuse[] = {1.0f, 0.0f, 0.0f};
    GLfloat shininess = 128.0f * 0.2f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // glPushMatrix();
    vetor3 posicaoCamera = camera.getPosicao();

    // glTranslatef(posicaoCamera.x, posicaoCamera.y, posicaoCamera.z);
    float carroYaw = camera.getCarroYaw();
    // glRotatef(carroYaw, 0.0f, 0.0f, 1.0f);
    
    // glPopMatrix();
    glPushMatrix();
        glTranslated(posicaoCamera.x, posicaoCamera.y, posicaoCamera.z);
        carroYaw = camera.getCarroYaw();
        glRotatef(carroYaw, 0.0f, 0.0f, 1.0f);
        glTranslated(-posicaoCamera.x, -posicaoCamera.y, -posicaoCamera.z);
        GLfloat olharx = posicaoCamera.x;
        GLfloat olhary = posicaoCamera.y + 2.45f ;
        GLfloat olharz = posicaoCamera.z - 0.9f;
        glTranslatef(olharx, olhary, olharz);
        
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); 
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glRotatef( camera.getInclinacaoCarro(), 0.0f, 1.0f, 0.0f);         
        glScalef(0.3f, 0.3f, 0.3f);
        glCallList(blenderModelId);

    glPopMatrix();

    glDisable(GL_LIGHTING);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    camera.ativar();

    luzAmbiente();
    luzPontual();

    Matriz matriz = ImageLoader().load(nomeArquivo);

    desenhaMapa(matriz);
    desenhaCarro(camera);
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    float aspect = (float) w / (float) h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void keys(unsigned char key, int x, int y)
{
    if(key == 'w' || key == 'W') camera.frente();
    if(key == 's' || key == 'S') camera.tras();
    if(key == 'a' || key == 'A') camera.esquerda();
    if(key == 'd' || key == 'D') camera.direita();

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
    glutPassiveMotionFunc(mouse);
    // glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keys);

    glutMainLoop();
}
