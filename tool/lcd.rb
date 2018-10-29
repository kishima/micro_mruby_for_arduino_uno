RGB_LCD.initialize
n=0
c=0
RGB_LCD.clear
#RGB_LCD.set_cursor(0,0)
RGB_LCD.write "Hello mruby/c!!"
while true do
  n+=1
  n = 0 if n>=100
  #  n=0
  #  c+=1
  #  c=0 if c==3
  #end
  #r=120
  #g=120
  #b=120
  #if c==0
  #  r = (n*10)%250
  #end
  #if c==1 
  #  g = (n*10)%250
  #end
  #if c==2
  #  b = (n*10)%250
  #end
  #RGB_LCD.set_rgb(r,g,b)
  RGB_LCD.set_rgb((n*10)%250,(n*10)%250,(n*10)%250)
  Arduino.delay 100
end

