function love.load()
  x_sprite = love.graphics.newImage("X.png")
  o_sprite = love.graphics.newImage("O.png")
end
function love.draw()
  love.graphics.draw(x_sprite, 16, 16)
  love.graphics.draw(o_sprite, 32, 32)
end
