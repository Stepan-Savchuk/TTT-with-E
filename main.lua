--TODO: check for win/draw

function love.load()
X_sprite = love.graphics.newImage("X2.png")
  O_sprite = love.graphics.newImage("O2.png")

  Grid = {}

  for i = 0, 2, 1 do
    Grid[i] = {}
    for j = 0, 2, 1 do
      Grid[i][j] = "NIL"
    end
  end

  Turn = "X"
  X_Eraser = 1
  O_Eraser = 1
end


function love.draw()
  local testString = "Player " ..Turn
  love.graphics.print(testString, (129*3)+64, 32, 0, 4, 4)
  love.graphics.print("X Eraser left" ..X_Eraser, (129*3)+64, 64, 0, 4, 4)
  love.graphics.print("O Eraser left" ..O_Eraser, (129*3)+64, 96, 0, 4, 4)

  for i = 0, 2, 1 do
    love.graphics.line((129+(129*i)), 0, (129+(129*i)), (129*3))
    love.graphics.line(0, (129+(129*i)), (129*3), (129+(129*i)))
  end
  for i = 0, 2, 1 do
    for j = 0, 2, 1 do
      if Grid[i][j] == "X" then
        love.graphics.draw(X_sprite, (i*128), (j*128))
      end
      if Grid[i][j] == "O" then
        love.graphics.draw(O_sprite, (i*128), (j*128))
      end
    end
  end
end

function love.keypressed(key)
  if key == 'q' then
    love.event.quit(0)
  end
end

function love.mousepressed(x, y, button, istouch)
  if x < (129*3) and y < (129*3) then
    local tx = math.floor(x / 128)
    local ty = math.floor(y / 128)

    if button == 1 then
      if Grid[tx][ty] == "NIL" then
        Grid[tx][ty] = Turn
        if Turn == "X" then
          Turn = "O"
        else
          Turn = "X"
        end
      end
    end
    if button == 2 then
      if Turn == "X" then
        if X_Eraser <= 0 then
          return
        end
        X_Eraser = X_Eraser - 1
      else
        if O_Eraser <= 0 then
          return
        end
        O_Eraser = O_Eraser - 1
      end
      Grid[tx][ty] = "NIL"
    end
  end
end
