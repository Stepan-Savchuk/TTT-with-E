toPrint = false

function love.load()
  x_sprite = love.graphics.newImage("X2.png")
  o_sprite = love.graphics.newImage("O2.png")
end


function love.draw()
  love.graphics.draw(x_sprite, 0, 0)
  love.graphics.draw(o_sprite, 0, 129)
  love.graphics.line(128, 0, (128), (128*3))
end

function love.keypressed(key)
  if key == 'q' then
    love.event.quit(0)
  end
  if key == 'e' then
    toPrint = true
  end
end
