for x in 2 3 4; do 
  rrdtool create t${x}.rrd --start $(date +%s) --step 60 \
    DS:temp:GAUGE:600:U:U \
    RRA:AVERAGE:0.5:1:10080 \
    RRA:AVERAGE:0.5:10:4320 \
    RRA:AVERAGE:0.5:60:43800
done
