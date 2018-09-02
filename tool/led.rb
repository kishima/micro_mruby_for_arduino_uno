puts "LED Test"
pin = 13
Arduino.pin_mode(pin,:OUTPUT)
st = :LOW
while(true) do
    if(st==:LOW)
        st=:HIGH
    else
        st=:LOW
    end
    #puts "st=#{st}"
    Arduino.digital_write(pin,st)
    Arduino.delay(500)
end

    