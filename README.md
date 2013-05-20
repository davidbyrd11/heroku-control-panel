# Physical Heroku Control Panel

## Overview
Shohan Hasan, Taylor Brock, &amp; I built a physical control panel for our final project in NYU's Physical Computing class. It's a box with an arduino in it that has a dial for literally scaling the number of dynos, a button to restart your application, &amp; an LED that flashes red whenever there's an error.

## Installation
Start by putting your information in `config.js`. 

### Arduino Code
Push the code in `/aduino/client.ino` to your arduino so that it can start reading the values of the dial and restart button.

### Circuit
I've included a bunch of images to make it as easy to reproduce as possible:
![one](http://3.bp.blogspot.com/-eyrM7u2oa_c/UZFqPGpm4OI/AAAAAAAAANE/GRovap4XITY/s320/IMG_1736.jpg)
![two](http://4.bp.blogspot.com/-Yd9VKUUFThY/UZFqPX7lieI/AAAAAAAAANI/xRshLjkk2bo/s320/IMG_1739.jpg)
![three](http://2.bp.blogspot.com/-wbSeleg7RBo/UZFqPv-tdzI/AAAAAAAAANU/43WdcC8r5Q8/s320/IMG_1740.JPG)
![four](http://1.bp.blogspot.com/-KqTXIVSA1rA/UZFqPE7t_3I/AAAAAAAAANA/LF6saJ6Dr_M/s320/IMG_1726.jpg)
### "Server"
The Arduino should be connected to your computer via USB. On your computer you can run the Node.js Application by running `node /nodejs/app.js` in your terminal. This code takes the Arduino's readings and makes API calls to heroku