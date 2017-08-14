#include <stdlib.h>
#include <time.h>
#include <list>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

using namespace std;

//----SDL global var---
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;
//---------------------
//-------other---------
SDL_Texture *ftext;

//---------------------

int main(int argc,char *argv[]){
	SDL_Init(SDL_INIT_EVERYTHING);
	//TTF_Init();
	window=SDL_CreateWindow("flappy-box",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							720,1280,
							SDL_WINDOW_SHOWN);
	renderer=SDL_CreateRenderer(window,
								-1,
								SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
							
	bool isQuit=false;
	
	TTF_Init();
	TTF_Font *font=TTF_OpenFont("./Coffeebreak.ttf",60);
	SDL_Color cor;
	cor.r=250,cor.g=10,cor.b=0,cor.a=0;
	SDL_Surface *temp=TTF_RenderUTF8_Blended(font,"cnmb!cnm!",cor);
	ftext=SDL_CreateTextureFromSurface(renderer,temp);
	TTF_CloseFont(font);
	SDL_FreeSurface(temp);
	SDL_Rect r;
	r.x=100,r.y=100;
	SDL_QueryTexture(ftext,nullptr,nullptr,&(r.w),&(r.h));
	
	while(!isQuit){
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_FINGERDOWN){
				isQuit=true;
			}
		}
		SDL_RenderCopy(renderer,ftext,nullptr,&r);
		SDL_RenderPresent(renderer);
	}
	
	SDL_Delay(1000);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
