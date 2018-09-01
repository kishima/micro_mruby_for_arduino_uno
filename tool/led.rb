puts "LED Test"
pin = 13
Arduino.pin_mode(pin,:OUTPUT)
st = 0
while(true) do
    st = !st
    puts "st=#{st}"
    Arduino.digital_write(pin,st)
    Arduino.delay(500)
end

    