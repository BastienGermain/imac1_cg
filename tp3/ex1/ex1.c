#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

static float PI = 3.14;
static int NB_SEGMENT = 50;


void drawSquare(int full) {
    if (full == 1) {
        glBegin(GL_QUADS);
            glColor3ub(255, 100, 100);
            glVertex2f(-0.5, -0.5);
            glVertex2f(-0.5, 0.5);
            glVertex2f(0.5, 0.5);
            glVertex2f(0.5, -0.5);
        glEnd();
    } else {
        glBegin(GL_LINE_LOOP);
            glColor3ub(255, 255, 255);
            glVertex2f(-0.5, -0.5);
            glVertex2f(-0.5, 0.5);
            glVertex2f(0.5, 0.5);
            glVertex2f(0.5, -0.5);
        glEnd();
    }
    
}

void drawLandmark() {
    glBegin(GL_LINES);
        glColor3ub(0, 255, 0);
        glVertex2f(0, -0.5);
        glVertex2f(0, 0.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3ub(255, 0, 0);
        glVertex2f(-0.5, 0);
        glVertex2f(0.5, 0);
    glEnd();
}

void drawCircle(int full){
    int i;
    
    GLfloat twicePi = 2.0f * PI;

    if (full == 1) {
        glBegin(GL_TRIANGLE_FAN);
            glColor3ub(255, 255, 255);
            glVertex2f(0, 0); // center of circle
            for(i = 0; i <= NB_SEGMENT; i++) { 
                glVertex2f(
                    0.5 * cos(i * twicePi / NB_SEGMENT), 
                    0.5 * sin(i * twicePi / NB_SEGMENT)
                );
            }
        glEnd();
    } else {
        glBegin(GL_LINE_LOOP);
            glColor3ub(255, 255, 255);
            for(i = 0; i <= NB_SEGMENT; i++) { 
                glVertex2f(
                    0.5 * cos(i * twicePi / NB_SEGMENT), 
                    0.5 * sin(i * twicePi / NB_SEGMENT)
                );
            }
        glEnd();
    }
    
}

void drawRoundedSquare() {
    glPushMatrix();
        glScalef(0.5, 0.5, 1);
        glPushMatrix();
            glTranslatef(-0.08, 0.08, 0);
            drawCircle(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.08, 0.08, 0);
            drawCircle(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0.08, -0.08, 0);
            drawCircle(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.08, -0.08, 0);
            drawCircle(1);
        glPopMatrix();
    glPopMatrix();
}

void drawFirstArm() {
    glPushMatrix();
        glScalef(2, 2, 1);
        drawCircle(1);
    glPopMatrix();
    glBegin(GL_LINE_LOOP);
        glColor3ub(255, 255, 255);
        glVertex2f(-3, -0.5);
        glVertex2f(-3, 0.5);
        glVertex2f(0, 1);
        glVertex2f(0, -1);
    glEnd();
    glPushMatrix();
        glTranslatef(-3, 0, 0);
        drawCircle(1);
    glPopMatrix();    
}

void drawSecondArm() {
    drawRoundedSquare();
    glPushMatrix();
        glTranslatef(2.25, 0, 0);
        drawRoundedSquare();
    glPopMatrix();
    glPushMatrix();
        glScalef(2.5, 0.4, 1);
        glTranslatef(0.45, 0, 0);
        drawSquare(1);
    glPopMatrix();
}

void drawThirdArm() {
    glPushMatrix();
        glScalef(0.5, 0.5, 1);
        drawCircle(1);
    glPopMatrix();
    glPushMatrix();
        glScalef(2.2, 0.3, 1);
        glTranslatef(0.45, 0, 0);
        drawSquare(0);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(2, 0, 0);
        glScalef(0.45, 0.45, 0);
        drawSquare(1);
    glPopMatrix();
}

int main(int argc, char** argv) {
    
    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-6., 6., -6., 6.);    
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Le titre est changé ! :)", NULL);

    /* Valeur angles */
    float alpha = 0.0;
    float beta = 0.0;
    float gamma = 0.0;
    
    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT); // Nettoie la fenêtre

        alpha += 1.0;
        beta += 2.0;
        gamma += 4.0;

        /* Ici le dessin */
            
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //drawLandmark();

        glPushMatrix();
            glRotatef(alpha, 0, 0, 1.0);
            drawFirstArm();
            glTranslatef(-3, 0, 0);
            glRotatef(180, 0, 0, 1.0);
            glRotatef(beta, 0, 0, 1.0);            
            drawSecondArm();
            glTranslatef(2.25, 0, 0);
            glRotatef(gamma, 0, 0, 1.0);
            drawThirdArm();
        glPopMatrix();        
        
        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();
        
        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            /* L'utilisateur ferme la fenêtre : */
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
            
            switch(e.type) {

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == 113) {
                        loop = 0;
                    }
                    break;

                default:
                    break;
            }
        }

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}
