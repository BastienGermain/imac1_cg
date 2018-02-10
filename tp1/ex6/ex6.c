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

void drawPoints(PointList list){
    while (list != NULL){

        glColor3ub(list->r, list->g, list->b);
        glVertex2f(-1 + 2. * list->x / WINDOW_WIDTH, -(-1 + 2. * list->y / WINDOW_HEIGHT));

        list = list->next;
    }
}

void deletePoints(PointList *list){
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
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }    
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Le titre est changé ! :)", NULL);

    glClear(GL_COLOR_BUFFER_BIT); // Nettoie la fenêtre
    
    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        
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
            
            int forme = 0;
            
            switch(e.type) {

                /* Click souris */
                case SDL_MOUSEBUTTONUP:                    
                    glColor3ub(255,0,0);

                    glBegin(GL_POINTS);
                        glVertex2f(-1 + 2. * e.button.x / WINDOW_WIDTH, -(-1 + 2. * e.button.y / WINDOW_HEIGHT));
                    glEnd();
                    
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                    if (e.key.keysym.sym == 113) {
						loop = 0;
					}

                    if (e.key.keysym.sym == 112) {
                        forme = 0;
                    }

                    if (e.key.keysym.sym == 108) {
                        forme = 1;
                    }

                    if (e.key.keysym.sym == 116) {
                        forme = 2;
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
    

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}
