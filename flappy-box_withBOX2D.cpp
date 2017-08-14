#include <cstdlib>
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"

#define SCALE_K 32.0f
#define PI 3.1415926535897f
//----------------------------------
//convert meters to pixels
inline int mtp(float m){
	return (int)(m*SCALE_K);
}
//convert pixels to meters
inline float ptm(int p){
	return (float)(p)/SCALE_K;
}
//convert radians to degrees
inline float rtd(float r){
	return r/PI*180.0f;
}
//convert degrees to radians
inline float dtr(float d){
	return d*PI/180.0f;
}
//--------------------------------
class Box{
	private:
		SDL_Renderer *renderer;
		SDL_Texture *texture;
		
		b2World *world;
		b2PolygonShape *boxShape;
		b2Fixture *fixture;
		b2Body *body;
		
		SDL_Rect _r;
	protected:
	public:
		Box(SDL_Renderer*,b2World*,int,int);
		~Box();
		
		void render();
		bool onEvent(SDL_Event*);
};
Box::Box(SDL_Renderer *renderer,b2World *world,int width,int height){
	//init common
	this->renderer=renderer;
	this->world=world;
	
	this->_r.x=0;
	this->_r.y=0;
	this->_r.w=width;
	this->_r.h=height;
	
	//init shape
	this->texture=SDL_CreateTexture(this->renderer,
									SDL_PIXELFORMAT_RGBA8888,
									SDL_TEXTUREACCESS_TARGET,
									width,height);
	
	SDL_SetRenderTarget(renderer,texture);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer,nullptr);
	//init physics
	b2BodyDef bodyDef;
	bodyDef.type=b2_dynamicBody;
	bodyDef.position.Set(0.0f,0.0f);//attention
	this->body=this->world->CreateBody(&bodyDef);
	
	this->boxShape=new b2PolygonShape;
	boxShape->SetAsBox(ptm(width),ptm(height));//attention
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape=this->boxShape;
	fixtureDef.density=1.0f;
	fixtureDef.friction=0.2f;
	
	this->fixture=this->body->CreateFixture(&fixtureDef);
	
}
Box::~Box(){
	SDL_DestroyTexture(this->texture);
	this->body->DestroyFixture(this->fixture);
	this->world->DestroyBody(this->body);
}
void Box::render(){
	b2Vec2 pos=this->body->GetPosition();
	float ang=rtd(this->body->GetAngle());
	this->_r.x=mtp(pos.x);
	this->_r.y=mtp(pos.y);
	SDL_RenderCopyEx(this->renderer,
					 this->texture,
					 nullptr,
					 &(this->_r),
					 (double)ang,
					 nullptr,
					 SDL_FLIP_NONE);
}
bool Box::onEvent(SDL_Event *e){
	if(e->type==SDL_FINGERDOWN){
		b2Vec2 v(2.0f,-10.0f);
		this->body->SetLinearVelocity(v);
		return true;
	}
}
//-----------------------------
class Spt{
	private:
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	
	b2World *world;
	protected:
	public:
};

bool isQuit=false;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

b2World *world;
b2Body *ground;
const int vi=8,pi=3;
const float timeStep=1.0f/60.0f;
//----------------------------------------------------------------------------------------------
int main(int argc,char *argv[]){
	//init
	b2Vec2 tmp_gravity(0.0f,10.0f);
	world=new b2World(tmp_gravity);
	
	b2BodyDef groundDef;
	groundDef.position.Set(0.0f,47.8f);
	ground=world->CreateBody(&groundDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(50.0f,10.0f);
	ground->CreateFixture(&groundShape,0.0f);
	
	SDL_Init(SDL_INIT_EVERYTHING);
	window=SDL_CreateWindow("flappy-box",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							720,1280,
							SDL_WINDOW_SHOWN);
	renderer=SDL_CreateRenderer(window,
								-1,
								SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	
	Box box(renderer,world,50,50);
	//main loop
	while(!isQuit){
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		
		world->Step(timeStep,vi,pi);
		
		while(SDL_PollEvent(&e)){
			box.onEvent(&e);
		}
		
		box.render();
		
		SDL_RenderPresent(renderer);
	}
	//ending
	world->DestroyBody(ground);
	delete world;
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}
