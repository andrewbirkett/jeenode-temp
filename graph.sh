function g() {

rrdtool graph $2 --start "$1" \
  -w 800 -h 600 \
  -u 26 -l 10 -r \
  DEF:t2=t2.rrd:temp:AVERAGE \
  DEF:t3=t3.rrd:temp:AVERAGE \
  DEF:t4=t4.rrd:temp:AVERAGE \
  LINE2:t2#FF0000:T2 \
  LINE3:t3#FF0000:T2 \
  LINE4:t4#0000FF:T4 

}

g "-1h" /var/www/temp-1h.png
g "-3h" /var/www/temp-3h.png
g "-12h" /var/www/temp-12h.png
g "-24h" /var/www/temp-24h.png