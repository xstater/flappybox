#include <stdlib.h>
#include <time.h>
#include <list>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

using namespace std;

int score=0;

string its(int x){
	int tmp=x;
	string s("");
	while(tmp!=0){
		s+=(char)((tmp%10)+'0');
		tmp/=10;
	}
	reverse(s.begin(),s.end());
	return s;
}


//---------box----------
class Box{
	private:
		//SDL_Texture *txtr;
		SDL_Renderer *renderer;
		SDL_Rect rect;
		
		int vy=2;
		int countframe;
	protected:
	public:
		Box(SDL_Renderer *);
		~Box();
		
		int getx();
		int gety();
		int getw();
		int geth();
		
		//void draw();
		void render();
		
		bool onEvent(SDL_Event*);
		void move();
};
Box::Box(SDL_Renderer *r){
	this->renderer=r;
	this->rect.x=260;
	this->rect.y=100;
	this->rect.w=50;
	this->rect.h=50;
	
	this->countframe=0;
}
Box::~Box(){
	//SDL_Destroy();
}
		
int Box::getx(){
	return this->rect.x;
}
int Box::gety(){
	return this->rect.y;
}
int Box::getw(){
	return this->rect.w;
}
int Box::geth(){
	return this->rect.h;
}

void Box::render(){
	SDL_SetRenderDrawColor(this->renderer,
							255,255,255,255);
	SDL_RenderFillRect(this->renderer,&rect);	
}
bool Box::onEvent(SDL_Event *e){
	if(e->type==SDL_FINGERDOWN){
		this->vy=-25;
	}
	return false;
}
void Box::move(){
	if(this->rect.y>=1200)this->rect.y=1200;
	if(this->rect.y<=0)this->rect.y=0;
	if(this->countframe==0/*true*/){
		this->rect.y+=vy;
		this->vy+=1;
	}
	this->countframe++;
	this->countframe%=1;
}
//--------end box------

//--------spt----------
class Spt{
	private:
		SDL_Rect rect1,rect2;
		SDL_Renderer *renderer;
		
		static const int vx=8;
		int countframe;
	protected:
	public:
		Spt(SDL_Renderer*);
		~Spt();
		
		int getx();
		
		void render();
		bool onEvent(SDL_Event *);
		
		bool hitTest(int,int);
		
		void move();
};
Spt::Spt(SDL_Renderer *r){
	this->renderer=r;
	this->rect1.x=720+200;
	this->rect1.y=0;
	this->rect1.w=100;
	this->rect1.h=rand()%700+50;
	
	this->rect2.x=720+200;
	this->rect2.y=this->rect1.h+400;
	this->rect2.w=100;
	this->rect2.h=1280-rect2.y;
	
	this->countframe=0;
}
Spt::~Spt(){
	score++;
}

int Spt::getx(){
	return this->rect1.x;
}
		
void Spt::render(){
	SDL_SetRenderDrawColor(this->renderer,
							128,128,0,255);
	SDL_RenderFillRect(this->renderer,&rect1);
	SDL_RenderFillRect(this->renderer,&rect2);
}
bool Spt::onEvent(SDL_Event *e){
	return false;
}

bool Spt::hitTest(int x,int y){
	return (x>=this->rect1.x)&&(x<=(this->rect1.x+this->rect1.w))&&(((y>=this->rect1.y)&&(y<=(this->rect1.y+this->rect1.h)))||((y>=this->rect2.y)&&(y<=1280)));
}

void Spt::move(){
	if(/*this->countframe==0*/true){
		this->rect1.x-=Spt::vx;
		this->rect2.x-=Spt::vx;
	}
	//countframe++;
	//countframe%=2;
}
//--------end spt------
//--------txt----------
class Text{
	private:
		TTF_Font *font;
		SDL_Color cor;
		string s;
		SDL_Texture *txtre;
		SDL_Renderer *renderer;
		SDL_Rect rc;
	protected:
	public:
		Text(SDL_Renderer *r,const char *path,int size);
		~Text();
		
		void setText(const char *_str){s=string(_str);}
		void render();
};
Text::Text(SDL_Renderer *r,const char *path,int size){
	this->renderer=r;
	this->font=TTF_OpenFont(path,size);
	this->cor.r=255;
	this->cor.g=255;
	this->cor.b=255;
	this->cor.a=255;
	this->rc.x=720>>1;
	this->rc.y=100;
}
Text::~Text(){
	TTF_CloseFont(this->font);
}
void Text::render(){
	SDL_Surface *tmp=TTF_RenderUTF8_Blended(font,s.c_str(),cor);
	this->txtre=SDL_CreateTextureFromSurface(this->renderer,tmp);
	SDL_FreeSurface(tmp);
	SDL_QueryTexture(this->txtre,nullptr,nullptr,&(this->rc.w),&(this->rc.h));
	SDL_RenderCopy(this->renderer,this->txtre,nullptr,&(this->rc));
}
//---------------------

//----SDL global var---
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;
//---------------------
//-------other---------
//int score=0;
//---------------------

int main(int argc,char *argv[]){
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	window=SDL_CreateWindow("flappy-box",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							720,1280,
							SDL_WINDOW_SHOWN);
	renderer=SDL_CreateRenderer(window,
								-1,
								SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
								
	Box box(renderer);
	list<Spt*> l;
	int fcount=0;
	
	Text txt(renderer,"./Coffeebreak.ttf",60);
	
	bool isQuit=false;
	while(!isQuit){
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		if(fcount==0)
			l.push_back(new Spt(renderer));
		fcount++;
		fcount%=100;
		if(((*(l.begin()))->getx())<=-500){
			delete (*(l.begin()));
			l.pop_front();
		}
		txt.setText(its(score).c_str());
		for(auto &itr:l){
			itr->render();
		}
		while(SDL_PollEvent(&e)){
			box.onEvent(&e);
		}
		for(auto &itr:l){
			if(itr->hitTest(box.getx(),box.gety())|itr->hitTest(box.getx()+box.getw(),box.gety())|itr->hitTest(box.getx(),box.gety()+box.geth())|itr->hitTest(box.getx()+box.getw(),box.gety()+box.geth())){
				isQuit=true;
			}
			itr->move();
		}
		box.render();
		txt.render();
		box.move();
		SDL_RenderPresent(renderer);
	}
	
	SDL_Delay(1000);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
