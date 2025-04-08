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

for image_size in range( 1, image.width+1, 2 ):
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
  
  total /= sum_
  Image.fromarray( ( numpy.clip( total, 0, 100 )*255 ).astype(numpy.uint8) ).save( 'out_{}.png'.format( resized.width ) )

