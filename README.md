
# MyHiStriker

This project is an electronic model of a High Striker toy for measure hit strength. The project was based on different kids of High Strikers and is an adaptation created by the author with the help of a few friends. 

>The project is still in progress...
>
><img width="70" height="70" src="https://github.com/lluchiari/MyHiStriker/raw/master/images/in_progress.png" />


## What is?

<img width="240" height="240" src="https://github.com/lluchiari/MyHiStriker/raw/master/images/img1.jpg" />

High Striker is a game to test your strength. It consists in a hammer where the contestant use to strike a kind of seesaw that in the other side has a puck that moves over a tower according to the force applied. The goal is to make the puck reach the highest point of the tower and ring the bell.

For datails, see the video below or [click here](https://youtu.be/iCO8CMlc4_0)

<a href="http://www.youtube.com/watch?feature=player_embedded&v=iCO8CMlc4_0
" target="_blank"><img src="http://img.youtube.com/vi/iCO8CMlc4_0/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="9" /></a>


## History
This toy was used in carnivals....

# How THIS High Striker works?

## Mechanical
The mechanical parts consists in two main parts:

 - Tower
 - Striker
 
### Tower
The tower is the part where the "puck" or the "lights" is going to be placed and is where the points scale will be shown to the contestant.

<img src="https://github.com/lluchiari/MyHiStriker/raw/master/images/img2.png" />

### Striker
This is where the contestant will strike the hammer. It will receive all the impact and measure the acceleration and send to the microcontroller. 

<img src="https://github.com/lluchiari/MyHiStriker/raw/master/images/img3.jpg" />

In this project was developed a different striker. It consists in a base, a piston, and a strong compression spring. 

#### Base
The base is represented in the following figure:

<img width = "50%" src="https://github.com/lluchiari/MyHiStriker/raw/master/docs/mechanical/Striker/parts/Base.png" />

It consists in an steel plate with four holes for fixation and an steel tube soldered in the center of the plate.

#### Piston
The piston is where the hammer will hit and where the accelerometer is placed.
<div class="column">
<div class="row">
	<img style="width: 40%" src="https://github.com/lluchiari/MyHiStriker/raw/master/docs/mechanical/Striker/parts/Pivot.png"/> 
</div>
<div class="row">
   <img style="width: 40%" src="https://github.com/lluchiari/MyHiStriker/raw/master/docs/mechanical/Striker/parts/Pivot2.png" />
  </div>
  </div>

It consists in a steel ***X mm**  thick plate and a tube with ***Y mm** of wall thickness. 
> The tube is soldered at the center of the plate.

Inside the tube the compress spring with ***Z mm** of diameter is introduced.

> **The spring must have the same tube inner diameter.**

> \* X, Y and Z are values that I need to measure in the real sample. But you can use any value you think is good enough. **In the future I'll introduce the real 3D models with real measures.**

The assembly of the striker can be seen in the following image:

<img style="width: 40%" src="https://github.com/lluchiari/MyHiStriker/raw/master/docs/mechanical/Striker/parts/Mountage_Perspective.png" />

The CAD files can be seen [here](https://github.com/lluchiari/MyHiStriker/tree/master/docs/mechanical/Striker).
## Electronics

Electronic section is divided in:
- Microncontroller;
- Light Driver (**LighterBoard** Project can be seen [here](https://github.com/lluchiari/LighterBoard));
- Accelerometer;

### Microncontroller
In this project was used an [Arduino Nano](https://store.arduino.cc/usa/arduino-nano), but feel free to use any other Arduino or microcontroller you desire.

### Light Driver
The Driver used is described in other project. It can be see in this [link](https://github.com/lluchiari/LighterBoard).

### Light Driver

## Assembly


# Other References

Other projects can be seen in:
- [http://www.jmillerid.com/wordpress/tag/hi-striker/](http://www.jmillerid.com/wordpress/tag/hi-striker/)

Other Ideas can be seen in:
- Blablabla
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEwOTEyNzcwNjksLTE3NTgxNTk4OTMsMT
c1Nzg0NzY5OCw2NTI0ODk1NCwtNTUwOTI3NDI2LDIwOTYyODQ1
MzYsLTE0OTc5NTY3ODIsLTM2MDEzMTMxMSw2MzYwOTUxMjcsLT
EyMTI0MDMwMzgsMTQ3NzY0MzE4MywtMTAyMDQ3Njk2NSwtMTky
OTIwNDY5Niw4OTQ1ODk2NDEsMTIzODczNDQwNywtMTYwMTE5OT
I5NV19
-->