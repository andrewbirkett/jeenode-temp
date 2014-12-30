jeenode-temp
============

I use this code for my jeenode/DS1820 wireless temperature network.

jeenode.ino is the sketch I load onto the jeenodes.  The DS1820s are soldered onto a daughterboard as seen here - http://www.nobugs.org/engineer/arduino/wireless-temperature.html.  I've made some effort to run the jeenode in a fairly low power way - they run off 3 AA batteries.

create.sh initialises three rrdtool databases (I have three jeenodes).

I run temperature.rb (under GNU screen) on a raspberry PI, constantly listening for messages from the jeenodes and updating the rrdtool files and a temp.json file in the webserver directory.

I run graph.sh periodially to produce graphs of the recent 1/3/12/24h period.

Andrew Birkett
andy@nobugs.org

