#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

using namespace std;

const int sizeOfMap = 4;
const int baseX = 0;
const int baseY = 0;
const int actionKeys[] = {100, 119, 97, 115};

void ErrorExit(LPCSTR lpszMessage){
  cout << lpszMessage;
  ExitProcess(0);
}

bool isActionKey(int key){
  int quantityOfActionKeys = 4;
  for(int i=0; i < quantityOfActionKeys; i++){
    if(key == actionKeys[i]){
      return true;
    }
  }
  return false;
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

void movement(int key, COORD& position){
  if(key==actionKeys[0]){
    position.X++;
    cout << actionKeys[0];
  } else if(key==actionKeys[1]){
    position.Y++;
    cout << actionKeys[1];
  } else if(key==actionKeys[2]){
    position.X--;
    cout << actionKeys[2];
  } else if(key==actionKeys[3]){
    position.Y--;
    cout << actionKeys[3];
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
    cout << " iteration " << index << " total " << eventsCount << " current " << cNumRead; 
    
    bool somethingPressed = true;
    if(somethingPressed){
      int key = 0;
      movement(key, position);
      if(key == 13){
	break;
      } else if(isActionKey(key)){
	system("CLS");
	setMap(sizeOfMap, position, mp);
	drawMap(mp, sizeOfMap, hStdout);
      }
    }
  }
  
  return 0;
}
