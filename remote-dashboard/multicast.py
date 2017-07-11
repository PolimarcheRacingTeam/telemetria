from socket import *
import struct
s=socket(AF_INET, SOCK_DGRAM)
s.bind(('',2017))
i = 0;
while True:
  m=s.recvfrom(255)
  #packet = struct.unpack('>20H3h',m)
  print i
  print m
  i = i + 1
