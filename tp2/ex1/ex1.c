#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

typedef struct Point{
    float x, y; // Position 2D du point
    unsigned char r, g, b; // Couleur du point
    struct Point *next; // Point suivant à dessiner
} Point, *PointList;

PointList allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
    PointList new = malloc(sizeof(Point));

    if (new == NULL) {
        return NULL;
    }

    new->x = x;
    new->y= y;
    new->r = r;
    new->g = g;
    new->b = b;
    new->next = NULL;

    return new;
}

void addPointToList(Point *point, PointList *list) {
    if (*list == NULL) {
        *list = point;
    } else {
        addPointToList(point, &(*list)->next);
    }
}

void drawPoints(PointList list) {
    while (list != NULL){

        glColor3ub(list->r, list->g, list->b);
        glVertex2f(list->x, list->y);

        list = list->next;
    }
}

void deletePoints(PointList *list) {
    while (*list !=  NULL){
        PointList next = (*list)->next;
        free(*list);
        *list = next;
    }
}

void resizeGL() {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
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
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Le titre est changé ! :)", NULL);

    /* Initialisation variables */

    PointList points = NULL;   
    
    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT); // Nettoie la fenêtre

        int nbForme = 0;
        int i;

        /* Ici le dessin */
        for (i = 0; i < nbForme; i++) {
            glBegin(GL_LINE_LOOP);
            drawPoints(points);
            glEnd();
        }

        glBegin(GL_LINE_STRIP);
        drawPoints(points);
        glEnd();
        
        
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

                case SDL_MOUSEBUTTONUP:

                    addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, -(-1 + 2. * e.button.y / WINDOW_HEIGHT), 255, 255, 255), &points);

                    if (e.button.button == SDL_BUTTON_RIGHT) {
                        printf("ok\n");
                        nbForme+=1;
                    }                               
                    break;
                    
                case SDL_VIDEORESIZE:					
					WINDOW_WIDTH = e.resize.w;
					WINDOW_HEIGHT = e.resize.h;
					resizeGL();
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
    
    /* Libère la mémoire */
    deletePoints(&points);

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}
