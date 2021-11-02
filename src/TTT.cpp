#include<stdio.h>
#include<string>
#include<vector>

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>


const int MAP_SIZE = 18;
const int SCREEN_WIDTH = MAP_SIZE * 19;
const int SCREEN_HEIGHT = MAP_SIZE * 25;







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

  void setBlendMode(SDL_BlendMode blending){
    SDL_SetTextureBlendMode(texture, blending);
  }

  void setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(texture, alpha);
  }

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


bool WTexture::loadFromString(TTF_Font* font, std::string str, SDL_Color textColor, SDL_Renderer* renderer){
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
  SDL_QueryTexture(texture, NULL, NULL, &height, &width);
  SDL_Rect renderRect = {x, y, height, width};

  SDL_RenderCopy(renderer, texture, NULL, &renderRect);
}



class Position{
private:
  int x;
  int y;
public:
  int getX(){
    return x;
  }
  
  int getY(){
    return y;
  }
  
  void setX(int value);
  void setY(int value);
};


void Position::setX(int value){
  x = value;
}

void Position::setY(int value){
  y = value;
}



class Player{
private:
  std::string name;
  char symbol;
  Position position;
  bool mark;
public:
  Player(std::string name, char symbol, int x, int y, bool mark);

  int getX(){
    return position.getX();
  }

  int getY(){
    return position.getY();
  }

  std::string getName(){
    return name;
  }

  std::string getSymbol(){
    std::string s(1, symbol);
    return s;
  }

  bool getMark(){
    return mark;
  }

  void changeX(int value);
  void changeY(int value);

  void setMark(bool value);
};


Player::Player(std::string name, char symbol, int x, int y, bool mark){
  position.setX(x);
  position.setY(x);

  this->name = name;
  this->symbol = symbol;
  this->mark = mark;
}

void Player::changeX(int value){
  position.setX(position.getX() + value);
}

void Player::changeY(int value){
  position.setY(position.getY() + value);
}

void Player::setMark(bool value){
  mark = value;
}













bool init(SDL_Window** window, SDL_Renderer** renderer);
bool loadTextures(TTF_Font *font, std::vector<WTexture *> textures, std::vector<std::string> strings, SDL_Renderer* renderer);
void input(Player* player, std::string (&testMap)[MAP_SIZE], bool &mark);
void createMap(std::string strArr[MAP_SIZE], bool toFill, int size);
void render(SDL_Renderer** renderer, TTF_Font* font, std::string (&map)[MAP_SIZE], Player* currentPlayer, std::vector<WTexture *> textures, int* count);
void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);






















void createMap(std::string strArr[MAP_SIZE], int size, bool toFill){
  for(int i=0; i < size; i++){
    if(strArr[i] != ""){
      strArr[i] = "";
    }
    for(int j=0; j < size; j++){
      if(toFill){
	strArr[i] += 'O';
      } else {
	strArr[i] += ' ';
      }
    }
  }
}




void render(SDL_Renderer** renderer, TTF_Font* font, std::string map, Player* currentPlayer, std::vector<WTexture *> textures, int* count){
  SDL_RenderClear(*renderer);
  for(int i=0; i < MAP_SIZE; i++){
	  
    std::vector<std::string> strings = {map[i], (1, currentPlayer->getSymbol())};
	
    if(!loadTextures(font, textures, strings, *renderer)){
      printf("Error with font loading\n");
    } else {
      SDL_SetRenderDrawColor(*renderer, 0x00, 0x00, 0x00, 0xFF);
      if(*count%32==0){
	textures[1]->setAlpha(0);
      } else {
	textures[1]->setAlpha(100);
      }
      *count++;
      textures[0]->render(0, i*25, *renderer);//25 is const value to make multiline drawing
      textures[1]->render(currentPlayer->getX()*19, currentPlayer->getY()*25, *renderer);
    }
  }
  SDL_RenderPresent(*renderer);
}








int main(int argc, char *argv[]){
  SDL_Window* window = NULL;
  SDL_Renderer* globalRenderer;

  TTF_Font* font = NULL;
  
  if(!init(&window, &globalRenderer)){
    printf("Error with init\n");
  } else {
    std::string testMap[MAP_SIZE];

    createMap(testMap, MAP_SIZE, false);
    
    bool quit = false;
    bool toShow = true;

    WTexture mapTexture;
    WTexture playerTexture;
    
    std::vector<WTexture *> textures = {&mapTexture, &playerTexture};
      
    Player player("PlayerOne", 'X', 0, 0, true);
    Player playerTwo("PlayerTwo", 'O', 0, 0, false);
      
    SDL_Event event;

    int count = 0;

    playerTexture.setBlendMode(SDL_BLENDMODE_BLEND);
    playerTexture.setAlpha(100);
    
    while(!quit){
      while(SDL_PollEvent(&event) != 0){
	if(event.type == SDL_QUIT){
	  quit = true;
	}
	input(&player, testMap, toShow);
      }
      
      render(&globalRenderer, font, testMap, &player, textures, &count);
    }
  }
  close(window, globalRenderer, font);
  
  return 0;
}
//Render arguments: SDL_Renderer* renderer, string strings, Player currentPlayer, TTF_Font* font, std::vector<WTexture *> textures


bool init(SDL_Window** window, SDL_Renderer** renderer){
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
    printf("SDL couldn't initialize! SDL Error %s\n", SDL_GetError());
    return false;
  } else {
    /*if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, window, renderer) != 0){
      printf("Unable to create window and default renderer! SDL Error: %s\n", SDL_GetError());
      return false;
      }*/
    *window = SDL_CreateWindow("TTT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if(*window == NULL){
      printf("Unable to create window! SDL Error: %s\n", SDL_GetError());
      return false;
    } else {
      *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
      if(*renderer == NULL){
	printf("Unable to create renderer! SDL Error: %s\n", SDL_GetError());
	return false;
      } else {
	SDL_SetRenderDrawColor(*renderer, 0x00, 0x00, 0x00, 0xFF);
	if(TTF_Init() == -1) {
	  printf("SDL_ttf couldn't initialize! SDL Error: %s\n", TTF_GetError());
	  return false;
	}
      }
    }
  }
  return true;
}


bool loadTextures(TTF_Font* font, std::vector<WTexture *> textures, std::vector<std::string> strings, SDL_Renderer* renderer){
  font = TTF_OpenFont("Courier Prime.ttf", 32);
  if(font == NULL){
    printf("Falied to load font! SDL_ttf Error: %s\n", TTF_GetError());
    return false;
  } else {
    SDL_Color textColor = {0xFF,0xFF,0xFF};

    for(int i=0; i < textures.size(); i++){
      if(!textures[i]->loadFromString(font, strings[i], textColor, renderer)){
	printf("Failed to render texture\n");
	return false;
      }
    }
  }
  return true;
}


void input(Player* player, std::string (&testMap)[MAP_SIZE], bool &mark){
  const Uint8* keyState = SDL_GetKeyboardState(NULL);
  bool isPressed =  false;
  if(keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_RETURN] && !isPressed){
    if(keyState[SDL_SCANCODE_UP]){
      printf("UP\n");
      player->changeY(-1);

      if(player->getY() < 0){
	player->changeY(1);
      }
    } else if(keyState[SDL_SCANCODE_DOWN]){
      printf("DOWN\n");
      player->changeY(1);

      if(player->getY() > MAP_SIZE-1){
	player->changeY(-1);
      }
    } else if(keyState[SDL_SCANCODE_LEFT]){
      printf("LEFT\n");
      
      player->changeX(-1);

      if(player->getX() < 0){
	player->changeX(1);
      }
    } else if(keyState[SDL_SCANCODE_RIGHT]){
      printf("RIGHT\n");

      player->changeX(1);

      if(player->getX() > MAP_SIZE-1){
	player->changeX(-1);
      }
    }
    if(keyState[SDL_SCANCODE_RETURN]){
      printf("ENTER\n");
      testMap[player->getY()][player->getX()] = player->getSymbol()[0];
    }
    mark = true;
    isPressed = true;
  }
}




void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font){
  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  TTF_CloseFont(font);
  font = NULL;

  SDL_Quit();
}


