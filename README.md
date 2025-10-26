# emulated_rotary_encoder

This code is a working emulator for a rotary encoder. Rotary encoders convert the angular position or motion of a shaft into a digital or analog signal, but require manual control. Certain heavy duty applications require rotary encoders over other alternatives like potentiometers, making a different, automatic control system necessary. 

A rotary encoder sends two square waves as signals to a given driver. When rotated by one step, the two pins will connect and disconnect successively, resulting in a phase shift of around 90 degrees. This code emulates that using two digital pins and changing their states in a similar fashion. 
