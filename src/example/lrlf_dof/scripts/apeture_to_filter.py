#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
from PIL import Image, ImageOps
import numpy
from numpy import asarray, linalg, diag, matmul, transpose, outer

parser = argparse.ArgumentParser(description='singular value decomposition')
parser.add_argument( '-i', '--input',  help='input file path', default='input.png')
args = parser.parse_args()

image = Image.open( args.input )
gray = ImageOps.grayscale( image )

horizontal = [[ 0 for i in range( 0, image.width ) ] for i in range( 0, 5 )]
vertical = [[ 0 for i in range( 0, image.width ) ] for i in range( 0, 5 )]

horizontal[ 0 ][ image.width // 2 ] = 1.0
vertical[ 0 ][ image.width // 2 ] = 1.0

for image_size in range( 2, image.width+1, 2 ):
  border_size = ( image.width - image_size ) // 2
  resized = gray.resize(( image.width - border_size * 2, image.height - border_size * 2 ), Image.BICUBIC);
  canvas = Image.new( "L", ( image.width, image.height ), ( 0 ) );
  canvas.paste( resized, ( border_size, border_size ) );
  data = asarray( canvas )
  normalized = data / 255.0
  sum_ = normalized.sum()
  v = linalg.svd( normalized )
  u=v.U[:,:5]
  s=v.S[:5]
  vh=v.Vh[:5,:]
  us = matmul( u, diag( s ) )

  total = matmul( us[:,:1], vh[:1,:] )
  total += matmul( us[:,1:2], vh[1:2,:] )
  total += matmul( us[:,2:3], vh[2:3,:] )
  total += matmul( us[:,3:4], vh[3:4,:] )
  total += matmul( us[:,4:5], vh[4:5,:] )
  sum_ = total.sum();
  
  horizontal[0].extend( vh[0,] )
  horizontal[1].extend( vh[1,] )
  horizontal[2].extend( vh[2,] )
  horizontal[3].extend( vh[3,] )
  horizontal[4].extend( vh[4,] )
  
  vertical[0].extend( us[:,0]/sum_ )
  vertical[1].extend( us[:,1]/sum_ )
  vertical[2].extend( us[:,2]/sum_ )
  vertical[3].extend( us[:,3]/sum_ )
  vertical[4].extend( us[:,4]/sum_ )

for l in range( 0, 5 ):
  with open( 'h{}.inc'.format( l ), 'w' ) as fd:
    initial=True
    for i in range( 0, len( horizontal[ l ] ) ):
      if initial:
        initial=False
      else:
        print( ', ', file=fd, end='' )
        if i % image.width == 0:
          print( '', file=fd )
      print( '{}'.format( horizontal[ l ][ i ] ), file=fd, end='' )

for l in range( 0, 5 ):
  with open( 'v{}.inc'.format( l ), 'w' ) as fd:
    initial=True
    for i in range( 0, len( vertical[ l ] ) ):
      if initial:
        initial=False
      else:
        print( ', ', file=fd, end='' )
        if i % image.width == 0:
          print( '', file=fd )
      print( '{}'.format( vertical[ l ][ i ] ), file=fd, end='' )

