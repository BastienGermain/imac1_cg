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

typedef struct Primitive{
    GLenum primitiveType;
    PointList points;
    struct Primitive *next;
} Primitive, *PrimitiveList;

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

Primitive* allocPrimitive(GLenum primitiveType) {
    Primitive *new = malloc(sizeof(Primitive));

    if (new == NULL) {
        return NULL;
    }

    new->primitiveType = primitiveType;
    new->points = NULL;
    new->next = NULL;

    return new;
}

void addPrimitive(Primitive *primitive, PrimitiveList *list) {
    primitive->next = *list;
    *list = primitive;
}

void drawPrimitives(PrimitiveList list) {
    while (list != NULL) {
        glBegin(list->primitiveType);
        drawPoints(list->points);
        glEnd();

        list = list->next;
    }
}

void deletePrimitives(PrimitiveList *list) {
    while (*list !=  NULL){
        PrimitiveList next = (*list)->next;
        deletePoints(&(*list)->points);
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

void drawPalette() {
    int nbColors = 4;

    int colors[4][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 255}
    };

    glBegin(GL_QUADS);
        int i;
        for (i = 0; i < nbColors; i++) {
            glColor3ub(colors[i][0], colors[i][1], colors[i][2]);

            glVertex2f(-1 + i * (2.f / nbColors), -1);
            glVertex2f(-1 + (i + 1) * (2.f / nbColors), -1);
            glVertex2f(-1 + (i + 1) * (2.f / nbColors), 1);
            glVertex2f(-1 + i * (2.f / nbColors), 1);
        }
    glEnd();
}

void setColor(int r, int g, int b, int color[3]) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
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
    int mode = 0; /* 1 = palette */

    int color[3] = {255, 255, 255}; /* Blanc */

    PrimitiveList primitives = NULL;
    addPrimitive(allocPrimitive(GL_POINTS), &primitives);    
    
    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT); // Nettoie la fenêtre

        /* Ici le dessin */
        if (mode == 1) {
            drawPalette();
        } else {
            drawPrimitives(primitives);
        }
        
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

                    if (e.key.keysym.sym == 32) {
                        mode = 1;
                    }

                    if (e.key.keysym.sym == 112) {
                        addPrimitive(allocPrimitive(GL_POINTS), &primitives);
                    }

                    if (e.key.keysym.sym == 108) {
                        addPrimitive(allocPrimitive(GL_LINES), &primitives);
                    }

                    if (e.key.keysym.sym == 116) {
                        addPrimitive(allocPrimitive(GL_TRIANGLES), &primitives);
                    }

                    break;

                case SDL_KEYUP:

                    if (e.key.keysym.sym == 32) {
                        mode = 0;
                    }

                    break;

                case SDL_MOUSEBUTTONUP:

                    if (mode == 1) {

                        if (-1 + 2. * e.button.x / WINDOW_WIDTH < -0.5) {
                            setColor(255, 0, 0, color);
                        } else if (-1 + 2. * e.button.x / WINDOW_WIDTH < 0) {
                            setColor(0, 255, 0, color);                            
                        } else if (-1 + 2. * e.button.x / WINDOW_WIDTH < 0.5) {
                            setColor(0, 0, 255, color);
                        } else {
                            setColor(255, 255, 255, color);                       
                        }

                    } else {
                        addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, -(-1 + 2. * e.button.y / WINDOW_HEIGHT), color[0], color[1], color[2]), &primitives->points);
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
    deletePrimitives(&primitives);

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}
