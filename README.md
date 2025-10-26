# Emulated Rotary Encoder:

This code is a working emulator for a rotary encoder. Rotary encoders convert the angular position or motion of a shaft into a digital or analog signal, but require manual control. Certain heavy duty applications require rotary encoders over other alternatives like potentiometers, making a different, automatic control system necessary. 

A rotary encoder sends two square waves as signals to a given driver. When rotated by one step, the two pins will connect and disconnect successively, resulting in a phase shift of around 90 degrees. The following image depicts what this signal looks like. 
(credit: 
<a href="https://www.youtube.com/watch?v=v4BbSzJ-hz4" target="_blank">How To Mechatronics</a>
)

<img width="1946" height="810" alt="image" src="https://github.com/user-attachments/assets/5db21d06-2691-4106-ab4b-0baaaa074d87" />



This code emulates that using two digital pins and changing their states in a similar fashion. Below is the output signal as represented in the Arduino Serial Plotter:
<img width="1534" height="707" alt="image" src="https://github.com/user-attachments/assets/5d6c89ba-1935-4ae8-8c3b-e68e4a12732f" />

