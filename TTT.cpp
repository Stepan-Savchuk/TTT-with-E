#include<stdio.h>
#include<string>
#include<SDL.h>
#include<SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture{
public:
  LTexture();
  ~LTexture();

  bool loadFromFile(std::string path);

  void free();

  void render(int x, int y);

  int getWidth();
  int getHeight();
private:
  SDL_Texture* texture;

  int width;
  int height;
};


bool init();
bool loadMedia();
void close();

SDL_Surface* loadSurface(std::string path);

SDL_Texture* loadTexture(std::string path);


SDL_Texture* texture = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

LTexture fooTexture;
LTexture backgroundTexture;

SDL_Surface* screenSurface = NULL;


LTexture::LTexture(){
  texture = NULL;
  width = 0;
  height = 0;
}

LTexture::~LTexture(){
  free();
}

bool LTexture::loadFromFile(std::string path){
  free();

  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL){
    printf("Unable to load image %s! SDL_Image Error: %s\n", path.c_str(), IMG_GetError());
  } else {
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));
    
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(newTexture == NULL){
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    } else {
      width = loadedSurface->w;
      height = loadedSurface->h;
    }

    SDL_FreeSurface(loadedSurface);
  }
  
  texture = newTexture;
  return texture != NULL;
}

void LTexture::free(){
  if(texture != NULL){
    SDL_DestroyTexture(texture);
    texture = NULL;
    width = 0;
    height = 0;
  }
}

void LTexture::render(int x, int y){
  SDL_Rect renderQuad = {x, y, width, height};
  SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

int LTexture::getWidth(){
  return width;
}

int LTexture::getHeight(){
  return height;
}


bool init(){
  bool success = true;

  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("SDL couldn't initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else {
    window = SDL_CreateWindow("TTT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else {
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

      if(renderer == NULL){
	printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	success = false;
      } else {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	int imgFlags = IMG_INIT_PNG;

	if(!(IMG_Init(imgFlags) & imgFlags)){
	  printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	  success = false;
	} else {
	screenSurface = SDL_GetWindowSurface(window);
	}
      }
    }
  }
  
  return success;
}

SDL_Surface* loadSurface(std::string path){
  SDL_Surface* optimizedSurface = NULL;

  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL){
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
  } else {
    optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
    if(optimizedSurface == NULL){
      printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    SDL_FreeSurface(loadedSurface);
  }
  return optimizedSurface;
}

bool loadMedia(){
  bool success = true;

  if(!fooTexture.loadFromFile("images/billy.png")){
    printf("Failed to load Foo texture image\n");
    success = false;
  }

  if(!backgroundTexture.loadFromFile("images/van.png")){
    printf("Failed to load Background texture image\n");
    success = false;
  }
  
  return success;
}

void close(){
  fooTexture.free();
  backgroundTexture.free();
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  window = NULL;
  renderer = NULL;

  IMG_Quit();
  SDL_Quit();
}


int main(int argc, char* args[]){
  if(!init()){
    printf("Failed to initialize!\n");
  }
  else {
    if(!loadMedia()){
      printf("Failed to load media!\n");
    }
    else {
      bool quit = false;

      SDL_Event event;

      while(!quit){
	while(SDL_PollEvent(&event) != 0){
	  if(event.type == SDL_QUIT) quit = true;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	backgroundTexture.render(0,0);
	
	SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &fillRect);

	SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT /6, SCREEN_WIDTH / 2 * 3, SCREEN_HEIGHT / 2 * 3};
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(renderer, &outlineRect);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
	for(int i=0; i<SCREEN_HEIGHT; i+=4){
	  SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i);
	}

	fooTexture.render(300, 300);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	
	SDL_RenderPresent(renderer);
      }
    }
  }

  close();

  return 0;
}
