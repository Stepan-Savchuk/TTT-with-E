#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

using namespace std;

const int sizeOfMap = 4;
const int baseX = 0;
const int baseY = 0;

void setMap(int size, COORD position, char mp[sizeOfMap][sizeOfMap]){
  for(int i=0; i < size; i++){
    for(int j=0; j < size; j++){
      if(position.X==i and position.Y==j){
	mp[i][j] = 'X';
      } else {
	mp[i][j]  = ' ';
      }
    }
  }
}

void drawMap(char mp[sizeOfMap][sizeOfMap], int size, HANDLE stream){
  COORD c = {baseX, baseY};
  for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){
      cout << mp[i][j] << "|";
      c.Y+=2;
      SetConsoleCursorPosition(stream, c);
    }
    c.X+=2;
    c.Y=0;
    SetConsoleCursorPosition(stream, c);
  }
  c.X=baseX;
  c.Y=baseY;
  SetConsoleCursorPosition(stream, c);
}

void input(int key, COORD& position){
  if(key==100){
    position.X++;
  } else if(key==119){
    position.Y++;
  } else if(key==97){
    position.X--;
  } else if(key==115){
    position.Y--;
  }
}

int main(){
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD size;
  size.X = 80;
  size.Y = 40;

  BOOL resizeSuccessful = SetConsoleScreenBufferSize(hStdout, size);
  if(!resizeSuccessful){
    DWORD error = GetLastError();
    cout << "Fail " << error;
    return 1;
  }

  HWND console = GetConsoleWindow();
  BOOL moveSuccessful = MoveWindow(console, 0, 0, 1920, 1080, TRUE);

  COORD position = {baseX, baseY};

  char mp[sizeOfMap][sizeOfMap];

  setMap(sizeOfMap, position, mp);
  drawMap(mp, sizeOfMap, hStdout);
  
  while(true){
    bool somethingPressed = _kbhit();
    if(somethingPressed){
      system("CLS");
      int key = _getch();
      input(key, position);
      if(key == 13){
	break;
      }
      setMap(sizeOfMap, position, mp);
      drawMap(mp, sizeOfMap, hStdout);
    }
  }
  
  return 0;
}
