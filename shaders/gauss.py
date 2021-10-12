#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import math

def gauss( x, y, s ):
  return math.exp( -(x*x+y*y)/(2*s*s) )/(2*math.pi*s*s)

def gauss_array():
  #for x in range( -3, 3 ):
    for y in range( -3, 4 ):
      print( gauss( 0, y, 7 ) );

gauss_array()
