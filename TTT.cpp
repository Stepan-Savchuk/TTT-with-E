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

  void setColor(Uint8 red, Uint8 green, Uint8 blue);

  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);

  void render(int x, int y, SDL_Rect* clip = NULL);

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

SDL_Rect spriteClips[4];
LTexture spriteSheetTexture;

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

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
  SDL_SetTextureColorMod(texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
  SDL_SetTextureBlendMode(texture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
  SDL_SetTextureAlphaMod(texture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip){
  SDL_Rect renderQuad = {x, y, width, height};

  if(clip != NULL){
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  
  SDL_RenderCopy(renderer, texture, clip, &renderQuad);
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

  if(!spriteSheetTexture.loadFromFile("images/sprites.png")){
    printf("Failed to load sprite sheet texture!\n");
    success = false;
  } else {
    spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 100;
    spriteClips[0].h = 100;

    spriteClips[1].x = 100;
    spriteClips[1].y = 0;
    spriteClips[1].w = 100;
    spriteClips[1].h = 100;

    spriteClips[2].x = 0;
    spriteClips[2].y = 100;
    spriteClips[2].w = 100;
    spriteClips[2].h = 100;

    spriteClips[3].x = 100;
    spriteClips[3].y = 100;
    spriteClips[3].w = 100;
    spriteClips[3].h = 100;
  }
  
  if(!fooTexture.loadFromFile("images/billy.png")){
    printf("Failed to load Foo texture image\n");
    success = false;
  } else {
    fooTexture.setBlendMode(SDL_BLENDMODE_BLEND);
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

      Uint8 red = 255;
      Uint8 green = 255;
      Uint8 blue = 255;
      Uint8 alpha = 255;

      while(!quit){
	while(SDL_PollEvent(&event) != 0){
	  if(event.type == SDL_QUIT) {
	    quit = true;
	  } else if(event.type == SDL_KEYDOWN){
	    switch(event.key.keysym.sym){
	    case SDLK_q:
	      red +=32;
	      break;
	    case SDLK_w:
	      green +=32;
	      break;
	    case SDLK_e:
	      blue +=32;
	      break;
	    case SDLK_a:
	      red -=32;
	      break;
	    case SDLK_s:
	      green -=32;
	      break;
	    case SDLK_d:
	      blue -=32;
	      break;
	    case SDLK_z:
	      if(alpha+10 > 255){
		alpha=255;
	      } else {
		alpha+=10;
	      }
	      break;
	    case SDLK_x:
	      if(alpha-10 < 0){
		alpha=0;
	      } else {
		alpha-=10;
	      }
	      break;
	    }
	  }
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	backgroundTexture.setColor(red, green, blue);
	backgroundTexture.render(150,0);
	
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

	fooTexture.setAlpha(alpha);
	fooTexture.render(300, 300);

	spriteSheetTexture.render(0, 0, &spriteClips[0]);
	spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[1].w, 0, &spriteClips[1]);
	spriteSheetTexture.render(0, SCREEN_HEIGHT-spriteClips[2].h, &spriteClips[2]);
	spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[3].w, SCREEN_HEIGHT-spriteClips[3].h, &spriteClips[3]);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	
	SDL_RenderPresent(renderer);
      }
    }
  }

  close();

  return 0;
}
