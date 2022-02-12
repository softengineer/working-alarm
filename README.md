### working alarm
Give timeout alarm for standup / seated / rest style of working (useful for long seated working, which has been proved not healthy)

![ScreenShot](https://github.com/softengineer/working-alarm/blob/master/images/screenshot.png)

Offer different setting and animation to ease your life!

Support Mac / Linux / Windows platform, qt5 or 6 is required to be installed before compile

Have tested platform:

   Linux Ubuntu

### Install:

qmake -makefile

Or use cmake to generate make file

#point to the qt installed directory in system!

cmake . -DCMAKE_PREFIX_PATH=~/Qt/6.2.1/gcc_64 

make

./restreminder

or install qtcreator to compile 

### Config

You can adjust seated / standing / rest time in setting button or directly update setting.json under app directory

Enjoy!
