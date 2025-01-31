function love.load()
  x_sprite = love.graphics.newImage("X2.png")
  o_sprite = love.graphics.newImage("O2.png")
end


function love.draw()
  for i = 0, 2, 1 do
    love.graphics.line((129+(129*i)), 0, (129+(129*i)), (129*3))
    love.graphics.line(0, (129+(129*i)), (129*3), (129+(129*i)))
  end
end

function love.keypressed(key)
  if key == 'q' then
    love.event.quit(0)
  end
end
