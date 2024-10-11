#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Para usar strings

#ifdef WIN32
#include <windows.h> // Apenas para Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>   // Funções da OpenGL
#include <GL/glu.h>  // Funções da GLU
#include <GL/glut.h> // Funções da FreeGLUT
#endif

#include "SOIL.h"

typedef struct
{
    unsigned char r, g, b;
} Pixel;

typedef struct
{
    int width, height;
    Pixel *img;
} Img;

// Protótipos
void load(char *name, Img *pic);
void processa();
void init();
void draw();
void keyboard(unsigned char key, int x, int y);

int width, height;
int fator;
GLuint tex[2];
Img pic[2];
int sel;

void load(char *name, Img *pic)
{
    int chan;
    pic->img = (Pixel *)SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if (!pic->img)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char **argv)
{
    if (argc < 1)
    {
        printf("seeing [im. entrada]\n");
        exit(1);
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    load(argv[1], &pic[0]);

    fator = 5;
    if (argc == 3)
        fator = atoi(argv[2]);

    width = pic[0].width;
    height = pic[0].height;

    pic[1].width = pic[0].width;
    pic[1].height = pic[0].height;
    pic[1].img = calloc(pic[1].width * pic[1].height, 3);

    glutInitWindowSize(width, height);
    glutCreateWindow("Analise Forense de Imagens");
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);

    printf("Entrada  : %s %d x %d\n", argv[1], pic[0].width, pic[0].height);
    sel = 0;

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, width, height, 0.0);
    glMatrixMode(GL_MODELVIEW);

    tex[0] = SOIL_create_OGL_texture((unsigned char *)pic[0].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
    tex[1] = SOIL_create_OGL_texture((unsigned char *)pic[1].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

    processa();

    glutMainLoop();
}

int calcMediana(int valores[], int tamanho){
    int mediana;
    int temp;

    for (int i = 0; i < tamanho - 1; i++){
        for (int j = i + 1; j < tamanho; j++){
            if (valores[i] > valores[j]){
                temp = valores[i];
                valores[i] = valores[j];
                valores[j] = temp;
            }
        }
    }

    //nao eh necessario verificar mediana para num pares
    mediana = valores[tamanho / 2];
    return mediana;
}

// Retorna o valor de luminância do pixel
int calcLuminancia(Pixel pixel){
    return (int)(0.59 * pixel.g + 0.3 * pixel.r + 0.11 * pixel.b);
}


int limiteIntervalo(int val, int min, int max){
    if (val < min)return min;
    if (val > max) return max;
    return val;
}

void processa(){
    Pixel(*in)[width] = (Pixel(*)[width])pic[0].img;
    Pixel(*out)[width] = (Pixel(*)[width])pic[1].img;

    int regsize = 3;
    int tam = regsize * regsize;

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int valR[tam], valG[tam], valB[tam];
            int index = 0;

            for (int i = -1; i <= 1; i++){
                for (int j = -1; j <= 1; j++){
                    if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height){
                        valR[index] = in[y + j][x + i].r;
                        valG[index] = in[y + j][x + i].g;
                        valB[index] = in[y + j][x + i].b;
                        index++;
                    }
                }
            }

            //calcula a med dos valores RGB
            int medR= calcMediana(valR, tam);
            int medG = calcMediana(valG, tam);
            int medB= calcMediana(valB, tam);

            //pega val original de RGB do pixel
            int ogR = in[y][x].r;
            int ogG = in[y][x].g;
            int ogB = in[y][x].b;

            //calcula o novo val de RGB
            int newR = (ogR - medR) * fator;
            int newG = (ogG- medG) * fator;
            int newB = (ogB - medB) * fator;

            //limita o novo valor de RGB entre 0 e 255
            newR = limiteIntervalo(newR, 0, 255);
            newG = limiteIntervalo(newG, 0, 255);
            newB = limiteIntervalo(newB, 0, 255);

            //atualiza a img de saida com o new val
            out[y][x] = (Pixel){(unsigned char)newR, (unsigned char)newG, (unsigned char)newB};

        }
    }

    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, out);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        free(pic[0].img);
        free(pic[1].img);
        exit(1);
    }
    if (key >= '1' && key <= '2')
        sel = key - '1';

    if (key == '=')
    {
        fator += 5;
        processa();
    }
    if (key == '-')
    {
        fator -= 5;
        processa();
    }
    glutPostRedisplay();
}

void draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3ub(255, 255, 255);

    glBindTexture(GL_TEXTURE_2D, tex[sel]);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(0, 0);

    glTexCoord2f(1, 0);
    glVertex2f(pic[sel].width, 0);

    glTexCoord2f(1, 1);
    glVertex2f(pic[sel].width, pic[sel].height);

    glTexCoord2f(0, 1);
    glVertex2f(0, pic[sel].height);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}
