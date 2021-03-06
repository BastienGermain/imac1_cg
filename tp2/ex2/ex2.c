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
        glColor3ub(255, 255, 255);
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
    gluOrtho2D(-4., 4., -4., 4.);    
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Le titre est changé ! :)", NULL);
    
    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT); // Nettoie la fenêtre

        /* Ici le dessin */

        drawLandmark();
        drawCircle(0);

        
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
