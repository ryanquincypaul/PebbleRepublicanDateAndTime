# PebbleRepublicanDateAndTime
[French Republican Calendar](https://en.wikipedia.org/wiki/French_Republican_Calendar) including the date and time for the revolutionary heart.

## Getting Started
Just copy these files into a project on [CloudPebble](https://cloudpebble.net).

### Prerequisities
I use this on an original pebble. It shows up on the other watches but might look funny, especially on the round. See if you can draw the hour and second notches yourself on a rounded screen!

I am about to get a Pebble 2 so if work is needed to get this watchface looking right on that I will probably make changes here. If there is interest for other nerds out there, I will be happy to publish this on the pebble store.

### Installing
Once you have all the files together and compiling on your project in CloudPebble, fiddle with the compilation settings to deploy to your phone (and in turn to the pebble device you have connected to it).

## Acknowledgments
The time portion of this watchface could not be done without vuzzbox's [modern_decimal](https://github.com/vuzzbox/pebble/tree/master/modern_decimal) watchface from which I borrowed the decimal time image and the angle logic. I had to make many changes to account for the differences in Pebble SDK2 (which modern_decimal was developed against) and 4.

I also looked to Stephen Morse's [French Calendar](http://stevemorse.org/jcal/french.html) web application to get me started on how to make the date conversion in C. His method is still there but since it wasn't easy to calculate the difference in dates between today and the beginning of the [French First Republic](https://en.wikipedia.org/wiki/French_First_Republic), I added a stop in between. I convert the epoch time to days and then add the days between the beginning of the unix epoch (January 1, 1970) and the French First Republic.

Also, thanks to Mike Duncan for his [Revolution's Podcast](http://www.revolutionspodcast.com/) for making the French Revolution fun and interesting. I would not have written an esoteric watchface without his passion to make others passionate about history.

