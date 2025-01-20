function love.load()
  x_sprite = love.graphics.newImage("X2.png")
  o_sprite = love.graphics.newImage("O2.png")
end
function love.draw()
  love.graphics.draw(x_sprite, 16, 16)
  love.graphics.draw(o_sprite, 256, 256)
end
