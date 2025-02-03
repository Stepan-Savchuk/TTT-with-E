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
end


function love.draw()
  for i = 0, 2, 1 do
    love.graphics.line((129+(129*i)), 0, (129+(129*i)), (129*3))
    love.graphics.line(0, (129+(129*i)), (129*3), (129+(129*i)))
  end
  for i = 0, 2, 1 do
    for j = 0, 2, 1 do
      if Grid[i][j] == "X" then
        love.graphics.draw(X_sprite, (i*128), (j*128))
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
  if button == 1 then
    local tx = math.floor(x / 128)
    print(tx)
    local ty = math.floor(y / 128)
    print(ty)
    Grid[tx][ty] = "X"
  end
end
