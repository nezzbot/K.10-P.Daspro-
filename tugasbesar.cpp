#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <stdlib.h>


#define JUMLAH_HANTU 3

typedef struct Hantu {
  int x;
  int y;
  int arahx;
  int arahy;
  int nyawa;
} hantu;

void gambar_hantu(SDL_Renderer* r, SDL_Texture* tex, hantu *hnt) {
	if (hnt->nyawa <= 0) {
		return;
	}
	SDL_Rect rect_hantu;
	rect_hantu.x = 0;
	rect_hantu.y = 0;
    rect_hantu.h = 100;
	rect_hantu.w = 100;

	rect_hantu.x = hnt->x;
	rect_hantu.y = hnt->y;
	SDL_RenderCopy(r, tex, NULL, &rect_hantu);

}

void gerakin_hantu(hantu *hnt) {
	int speed = 25;
	int gerakan_x = rand() % (2*speed) - speed;
	int gerakan_y = rand() % (2*speed) - speed;

	hnt->x += gerakan_x;
	hnt->y += gerakan_y;

	if (hnt->y < 0) {
		hnt->y = 0;
	}
	if (hnt->x < 0) {
		hnt->x = 0;
	}
	if (hnt->x >999) {
		hnt->x = 999;
	}
	if (hnt->y >999) {
		hnt->y = 999;
	}
}

int main(int argc, char *argv[])
{
	hantu hantus[JUMLAH_HANTU];
	srand(time(NULL));   // Initialization, should only be called once.


	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("Open Fire", // creates a window
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									1000, 1000, 0);

	
	// triggers the program that controls
	// your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	// creates a renderer to render our images
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	// creates a surface to load an image into the main memory
	SDL_Surface* img_target;
	SDL_Surface* img_kuburan;
	SDL_Surface* img_hantu;

	// please provide a path for your image
	IMG_Init( IMG_INIT_PNG );
	img_target = IMG_Load("target.png"); 
	img_kuburan = IMG_Load("background.jpg");
	img_hantu = IMG_Load("ghost.png");


	// loads image to our graphics hardware memory.
	SDL_Texture* tex_target = SDL_CreateTextureFromSurface(rend, img_target);
	SDL_Texture* tex_kuburan = SDL_CreateTextureFromSurface(rend, img_kuburan);
	SDL_Texture* tex_hantu = SDL_CreateTextureFromSurface(rend, img_hantu);

	// clears main-memory
	SDL_FreeSurface(img_target);
	SDL_FreeSurface(img_kuburan);
	SDL_FreeSurface(img_hantu);

	// let us control our image position
	// so that we can move it with our keyboard.
	SDL_Rect rect_target;
	SDL_Rect zero;

	// connects our texture with dest to control position
	SDL_QueryTexture(tex_target, NULL, NULL, &rect_target.w, &rect_target.h);

	// adjust height and width of our image box.
	zero.x = 0;
	zero.y = 0;
    zero.h = 1000;
	zero.w = 1000;


	for (int i=0; i< JUMLAH_HANTU; i++) {
		hantus[i].x = 0;
		hantus[i].y = 0;
		hantus[i].nyawa = 1;

	}

    rect_target.w = 100;
	rect_target.h = 100;    
	// sets initial x-position of object
	rect_target.x = (1000 - rect_target.w) / 2;
	rect_target.y = (1000 - rect_target.h) / 2;



	// controls animation loop
	int close = 0;

	// speed of box
	int speed = 200;

	// animation loop
	while (!close) {
		SDL_Event event;

		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

			case SDL_QUIT:
				// handling of close button
				close = 1;
				break;

			case SDL_KEYDOWN:
				// keyboard API for key pressed
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					rect_target.y -= speed / 30;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					rect_target.x -= speed / 30;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					rect_target.y += speed / 30;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					rect_target.x += speed / 30;
					break;
				default:
					break;
				}
			}
		}

		// right boundary
		if (rect_target.x + rect_target.w > 1000)
			rect_target.x = 1000 - rect_target.w;

		// left boundary
		if (rect_target.x < 0)
			rect_target.x = 0;

		// bottom boundary
		if (rect_target.y + rect_target.h > 1000)
			rect_target.y = 1000 - rect_target.h;

		// upper boundary
		if (rect_target.y < 0)
			rect_target.y = 0;



		// clears the screen
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, tex_kuburan, NULL, &zero);

		for (int i = 0; i < JUMLAH_HANTU; i++) {
			gerakin_hantu(&hantus[i]);
			gambar_hantu(rend,tex_hantu,&hantus[i]);

		}
		SDL_RenderCopy(rend, tex_target, NULL, &rect_target);

		// triggers the double buffers
		// for multiple rendering
		SDL_RenderPresent(rend);

		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}

	// destroy texture
	SDL_DestroyTexture(tex_target);
	SDL_DestroyTexture(tex_hantu);
	SDL_DestroyTexture(tex_kuburan);

	// destroy renderer
	SDL_DestroyRenderer(rend);

	// destroy window
	SDL_DestroyWindow(win);
	
	// close SDL
	SDL_Quit();

	return 0;
