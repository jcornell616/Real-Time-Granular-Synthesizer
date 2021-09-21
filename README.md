# Real-Time Granular Synthesizer

## Summary

[Wikipedia article on Granular Synthesis](https://en.wikipedia.org/wiki/Granular_synthesis)

The project consists of using granular synthesis techniques in real-time. Grains are 1 to
1000 ms snippets of audio, which will be dynamically created and destroyed. Grains
can be stretched, squeezed, repeated, reversed, rearranged, and overlayed. Grains
will have  parameters such as grain size, grain density, feedback, random grain generation,
reverse grain playback, and freeze buffer can be adjusted by the user using potentiometers
interfaced with an ADC.

## Specs

* TI C2000 Dual-Core Processor
* McBSP interface with TLV320AIC23
* ADC interface with potentiometers
* SPI interface with 256K SRAM
* 32 kHz sampling, 16 bit data
* Psuedo-random number generation
* Dynamic creation and destruction of grains with linked list
* Grain scheduling
* Parameters:

     Volume
     
     Wet/Dry
     
     Grain Size
     
     Grain Texture
     
     Feedback
     
     Random, Reverse, Freeze states
