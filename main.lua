--TODO: Second-rate adapt grid to be have more than 3x3 tiles
--
--Third-rate Priority: add multipliers, like xx banana(which fills current tiles or clears them, depends on which action player takes)

function love.load()
  X_sprite = love.graphics.newImage("X2.png")
  O_sprite = love.graphics.newImage("O2.png")

  mx = 3
  my = 3

  Grid = {}

  for i = 0, mx-1, 1 do
    Grid[i] = {}
    for j = 0, mx-1, 1 do
      Grid[i][j] = "NIL"
    end
  end

  Turn = "X"
  X_Eraser = 1
  O_Eraser = 1

  Win = false
  InGame = true

  State = 0 -- 0 - menu, 1 - game, 2 - aftermath
end

function event_handler()
  if Win then
    State = 2
  end
end

function love.update()
  event_handler()
end

function love.draw()
  if State == 0 then
    love.graphics.print("Press Start to start", 64, 64, 0, 2, 2)
    love.graphics.print("Press RMB to erase opponents(or yours) figure on board", 64, 128, 0, 2, 2)
    love.graphics.print("I think you know how to play Tic-Tac-Toe, don't you?", 64, 192, 0, 2, 2)
    love.graphics.print("Press q to quit, but why would you need that", 64, 256, 0, 2,2)
  end

  if State == 1 then
  
    local testString = "Player " ..Turn
    love.graphics.print(testString, (129*3)+64, 32, 0, 4, 4)
    love.graphics.print("X Eraser left" ..X_Eraser, (129*3)+64, 64, 0, 4, 4)
    love.graphics.print("O Eraser left" ..O_Eraser, (129*3)+64, 96, 0, 4, 4)

    for i = 0, 2, 1 do
      love.graphics.line((129+(129*i)), 0, (129+(129*i)), (129*3))
      love.graphics.line(0, (129+(129*i)), (129*3), (129+(129*i)))
    end
    for i = 0, mx-1, 1 do
      for j = 0, my-1, 1 do
        if Grid[i][j] == "X" then
          love.graphics.draw(X_sprite, (i*128), (j*128))
        end
        if Grid[i][j] == "O" then
          love.graphics.draw(O_sprite, (i*128), (j*128))
        end
      end
    end
  end
  if State == 2 then
    love.graphics.clear()
    love.graphics.print("WIIIN of someone... maybe? I think so?", 64, 129*2, 0, 3, 3)
   end
end

function love.keypressed(key)
  if State == 0 then
    if key == 's' then
      State = 1
    end
  end
  if key == 'q' then
    love.event.quit(0)
  end
end

function love.mousepressed(x, y, button, istouch)
  if State == 1 then
    if x < (129*mx) and y < (129*my) then
      local tx = math.floor(x / 128)
      local ty = math.floor(y / 128)

      if Turn == "X" then
        
        if button == 1 then
          if Grid[tx][ty] == "NIL" then
            Grid[tx][ty] = Turn
            if check4win(tx, ty, mx, my) then  
              Win = true
              return
            end
            if Turn == "X" then
              ai()
              if check4win(0, 0, mx, my) then
                Win = true
                return
              end
            end
          end
        end
        if button == 2 then
          if Turn == "X" then
            if X_Eraser <= 0 then
              return
            end
            X_Eraser = X_Eraser - 1
          end
          Grid[tx][ty] = "NIL"
        end
      end
    end
  end
end

function ai()
  for i = 0, mx-1, 1 do
    for j = 0, my-1, 1 do
      if Grid[i][j] == "NIL" then
        Grid[i][j] = "O"
        return
      end
    end
  end
end

function check4win(cx, cy, maxx, maxy)
  --if ((Grid[0][0] == "X" and Grid[1][1] == "X" and Grid[2][2] == "X") or (Grid[0][0] == "X" and Grid[0][1] == "X" and Grid[0][2] == "X") or (Grid[1][0] == "X" and Grid[1][1] == "X" and Grid[1][2] == "X") or (Grid[2][0] == "X" and Grid[2][1] == "X" and Grid[2][2] == "X") or (Grid[0][0] == "X" and Grid[1][0] == "X" and Grid[2][0] == "X") or (Grid[0][1] == "X" and Grid[1][1] == "X" and Grid[2][1] == "X") or (Grid[0][2] == "X" and Grid[1][2] == "X" and Grid[2][2] == "X")) or ((Grid[0][0] == "O" and Grid[1][1] == "O" and Grid[2][2] == "O") or (Grid[0][0] == "O" and Grid[0][1] == "O" and Grid[0][2] == "O") or (Grid[1][0] == "O" and Grid[1][1] == "O" and Grid[1][2] == "O") or (Grid[2][0] == "O" and Grid[2][1] == "O" and Grid[2][2] == "O") or (Grid[0][0] == "O" and Grid[1][0] == "O" and Grid[2][0] == "O") or (Grid[0][1] == "O" and Grid[1][1] == "O" and Grid[2][1] == "O") or (Grid[0][2] == "O" and Grid[1][2] == "O" and Grid[2][2] == "O")) then
   -- return true
  --end
  --
  --new algorithm for checking win is to through the row and column on which last clicked element was clicked on
  for i = 0, maxx-1, 1 do
    if Grid[i][cy] == "X" then
    else return false
    end
  end
  return true
end
