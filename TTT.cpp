#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

using namespace std;

const int sizeOfMap = 4;
const int baseX = 0;
const int baseY = 0;

void ErrorExit(LPCSTR lpszMessage){
  cout << lpszMessage;
  ExitProcess(0);
}

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
  string lines[sizeOfMap];
  for(int i=0; i < size; i++){
    lines[i] = "";
    for(int j=0; j < size; j++){
      lines[i] += mp[i][j];
      lines[i] += "|";
    }
  }
  for(int i=0; i < size; i++){
    cout << lines[i] << endl;
    cout << "--------" << endl;
  }
}

void input(KEY_EVENT_RECORD ker, COORD &position){
  if(ker.bKeyDown){
    switch(ker.wVirtualKeyCode){
    case 37:
      position.X--;
      if(position.X<0) position.X++;
      break;
    case 38:
      position.Y--;
      if(position.Y<0) position.Y++;
      break;
    case 39:
      position.X++;
      if(position.X>sizeOfMap-1) position.X--;
      break;
    case 40:
      position.Y++;
      if(position.Y>sizeOfMap-1) position.Y--;
      break;
    default:
      break;
    }
  }
}

int main(){
  HANDLE hStdin;
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

  const short bufferSize = 128;
  INPUT_RECORD irInBuf[bufferSize];

  hStdin = GetStdHandle(STD_INPUT_HANDLE);
  if(hStdin == INVALID_HANDLE_VALUE){
    ErrorExit("GetStdHandle");
  }

  int index = 0;
  int eventsCount = 0;
  
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
    DWORD cNumRead = 0;
    BOOL peekSuccessful = PeekConsoleInput(hStdin, irInBuf, bufferSize, &cNumRead);

    if(!peekSuccessful) ErrorExit("PeekConsoleInput");
    if(!FlushConsoleInputBuffer(hStdin)) ErrorExit("FlushConsoleInputBuffer");

    eventsCount += cNumRead;

    bool kPressed = false;
    
    for(DWORD i=0; i < cNumRead; i++){
      if(irInBuf[i].EventType == KEY_EVENT){
	KEY_EVENT_RECORD ker = irInBuf[i].Event.KeyEvent;
	input(ker, position);
      }
    }
    system("CLS");
    setMap(sizeOfMap, position, mp);
    drawMap(mp, sizeOfMap, hStdout);
  }
  
  return 0;
}
