#!/usr/bin/env ruby
require 'rubygems'
require 'json'
f = open("/dev/ttyUSB0")

def log(type,msg) 
  now = Time.new.strftime("%Y-%m-%d %H:%M:%S") 
  puts "#{now} #{type} #{msg}"
  STDOUT.flush
end

$temps = { "2" => "U", "3" => "U", "4" => "U" }

while true
  l = f.readline.chomp
  log("RX", l)

  node,msg,value = l.split(",")

  if msg != "0"
    log("EE", "Unknown message #{msg}")
    next
  end

  temp = value.to_f / 100.0

  if temp < 5 && temp > 30
    log("EE", "Temperature #{temp} out of range")
    next
  end

  cmd = "rrdtool update t#{node}.rrd N:#{temp}"
  log("CM", cmd)
  `#{cmd}`
  $temps[node] = temp
  File.open("/var/www/temp.json","w") { |out| out.puts "var temps = " + $temps.to_json + ";" }
end
