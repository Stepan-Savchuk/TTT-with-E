#include<stdio.h>
#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


class WTexture {
private:
  SDL_Texture* texture;
  int width;
  int height;
public:
  WTexture();
  ~WTexture();

  void free();

  bool loadFromString(TTF_Font *font, std::string str, SDL_Color textColor, SDL_Renderer* renderer);

  void render(int x, int y, SDL_Renderer* renderer);

  int getWidth(){
    return width;
  }
  int getHeight(){
    return height;
  }
};

WTexture::WTexture(){
  texture = NULL;
  width = 0;
  height = 0;
}

WTexture::~WTexture(){
  free();
}

void WTexture::free(){
  SDL_DestroyTexture(texture);
  texture = NULL;

  width = 0;
  height = 0;
}

bool WTexture::loadFromString(TTF_Font *font, std::string str, SDL_Color textColor, SDL_Renderer* renderer){
    free();

    SDL_Surface* surface = TTF_RenderText_Solid(font, str.c_str(), textColor);
    if(surface == NULL){
      printf("Failed to render textSurface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
      if(renderer == NULL){
	printf("Unable to use globalRenderer! SDL Error: %s\n", SDL_GetError());
      } else {
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(texture == NULL){
	  printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	} else {
	  width = surface->w;
	  height = surface->h;
	}
	SDL_FreeSurface(surface);
      }
    }

    return texture!=NULL;
}

void WTexture::render(int x, int y, SDL_Renderer* renderer){
  SDL_Rect renderRect = {x, y, height, width};

  SDL_RenderCopy(renderer, texture, NULL, &renderRect);
}

bool init(SDL_Window* window, SDL_Renderer* renderer);
bool loadFont(TTF_Font *font, WTexture texture, std::string str, SDL_Renderer* renderer);
void close(SDL_Window* window, SDL_Renderer* renderer);


bool init(SDL_Window* window, SDL_Renderer* renderer){
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
    printf("SDL couldn't initialize! SDL Error %s\n", SDL_GetError());
    return false;
  } else {
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if(window == NULL){
      printf("Window couldn't be created! SDL Error: %s\n", SDL_GetError());
      return false;
    } else {
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
      if(renderer == NULL){
	printf("Renderer couldn't be created! SDL Error: %s\n", SDL_GetError());
	return false;
      } else {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	if(TTF_Init() == -1) {
	  printf("SDL_ttf couldn't initialize! SDL Error: %s\n", TTF_GetError());
	  return false;
	}
      }
    }
  }
  return true;
}

bool loadFont(TTF_Font *font, WTexture texture, std::string str, SDL_Renderer* renderer){
  font = TTF_OpenFont("Raleway-Black.ttf", 28);

  if(font == NULL){
    printf("Falied to load font! SDL_ttf Error %s\n", TTF_GetError());
    return false;
  } else {
    SDL_Color textColor = {0xFF,0xFF,0xFF};

    if(!texture.loadFromString(font, str, textColor, renderer)){
      printf("Failed to render texture\n");
      return false;
    } 
  }
  return true;
}

void close(SDL_Window* window, SDL_Renderer* renderer){
  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  SDL_Quit();
}

int main(int argc, char *argv[]){
  SDL_Window* window = NULL;
  SDL_Renderer* globalRenderer;

  TTF_Font *font = NULL;

  std::string map = "Hello Count 0\n";

  WTexture mapTexture;
  
  if(!init(window, globalRenderer)){
    printf("Error with init\n");
  } else {
    if(!loadFont(font, mapTexture, map, globalRenderer)){
      printf("Error with font loading\n");
    } else {
      bool quit = false;
      SDL_Event event;
      while(!quit){
	while(SDL_PollEvent(&event) != 0){
	  if(event.type == SDL_QUIT){
	    quit = true;
	  }
	}
	SDL_SetRenderDrawColor(globalRenderer, 0x00, 0x00, 0x00, 0xFF);
	
	SDL_RenderClear(globalRenderer);

	mapTexture.render(0, 0, globalRenderer);

	SDL_RenderPresent(globalRenderer);
      }
    }
  }

  system("PAUSE");
  close(window, globalRenderer);
  
  return 0;
}
