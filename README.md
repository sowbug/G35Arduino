Introduction
============

![G-35 Lights, Green](G35Arduino/blob/master/images/green_lights.jpg?raw=true "G-35 Lights") Read [the blog post](http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/)
that started it all.

This library makes it easy to replace the controller in GE Color Effects Christmas lights with an Arduino, and to run much better light programs. If you can program C or Processing, it's pretty easy to make new programs, too!

See a [demo](http://www.youtube.com/watch?v=QyKt7tHV-EM). There is also a [not-awful professional demo](http://www.geholidaylighting.com/holiday-lighting/led/color-effects) of the original, unmodified hardware.

Features
========

- With your Arduino, Teensy, or pile of laundry lint that happens to
  contain an Atmel AVR ATmega, G35Arduino becomes a
  splice-in replacement for the stock G35 controller. Contains versions
  of the same programs that ship with the lights. Your spouse won't know
  the difference!

- Also contains custom programs: Inchworm, Orbit, Cylon, Meteorite,
  just to name a few! Your spouse will know the difference!

- Easy to add your own programs. If you can make it through the Arduino
  Blink tutorial, you can probably write your own light program!

- Synchronizes multiple strings. Have them each run individual instances
  of the same light program. Have them each run different programs, but
  with synchronized program changes. Or combine as many strings as you
  want into a single mondo gigantic virtual string! (Number of physical
  strings is limited by the number of output pins on your
  microcontroller, as well as the memory requirements of the running
  light programs.)

- Express your individualism! G35Arduino operates completely
  independently of [requests from Twitter](http://www.cheerlights.com/), Facebook, SMS, XBee, neighbors,
  and drive-thru spectators. You bought 'em, you should get to control
  what they do!

Requirements
============

- [Arduino 1.0 SDK](http://arduino.cc/en/Main/Software). This code is not compatible with pre-1.0 Arduino.

- Any of the following: an Arduino or Arduino-compatible, a [Teensy](http://www.pjrc.com/) with Teensyduino or else flashed with an Arduino Leonardo bootloader. Also a plain Atmel ATmega of some sort, if you know what you're doing.

- At least one working strand of GE G-35 Color Effects Christmas lights that you are willing to slice up and solder back together again.

Installation
============

1. Find your Arduino IDE's sketches directory.

1. If there isn't already one, create a `libraries` directory inside the sketches directory.

1. Unzip the project archive into `libraries`. It should end up looking like this: `[Your Arduino sketches directory]/libraries/G35Arduino/G35.h (etc.)`

1. Restart your Arduino IDE and you should see G35Arduino among the libraries.

1. Upload the BasicExample sketch to your Arduino. You should see the pin-13 LED flicker as the sketch tries to send data to your lights.

1. Chop off the stock controller on your lights, being careful to note the function of the three leads from the first light (from left to right looking at the first light pointing upward, they're V+, DATA, and GND).

1.  Put V+ on the Arduino 5V pin, GND on the GND pin, and DATA on pin 13, and your lights should come back to life!

I've been happily running two strands on my house directly off the
Arduino (i.e., not using the stock power transformer) with an old USB
power supply rated 5V/1A from a Kodak Zi8. Your mileage may vary.

Contributing
============

Source-code contributions are welcome. If you think you might contribute
improvements back to the project, git clone into libraries/ instead of
unzipping, and to avoid going insane, check the "external editor" option
in the Arduino IDE and use a text editor directly on the library source.

We try to follow Google's C++ coding standards: 2 spaces, no tabs, 80
columns, and follow the existing naming/capitalization conventions in the
code.

Credits
=======

- [Paul Martis](http://www.digitalmisery.com)

- [Robert Quattlebaum](http://www.deepdarc.com/2010/11/27/hacking-christmas-lights)

- [Scott Harris](http://scottrharris.blogspot.com/2010/12/controlling-ge-color-effects-lights.html)

- [Mike Tsao](http://github.com/sowbug)