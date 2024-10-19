#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math
import json
import argparse
import numpy as np

parser = argparse.ArgumentParser(description='lens flare matrix generator')
parser.add_argument( '-i', '--input',  help='input file path', default='input.json')
args = parser.parse_args()

def load_surface( filename ):
  surfaces = json.load( open( filename, 'r' ) )
  current_refractive_index = 1.0
  for s in surfaces:
    s[ "previous refractive index" ] = current_refractive_index;
    current_refractive_index = s[ "refractive index" ]
    if "radius" in s:
      s[ "radius" ] = s[ "radius" ] * 0.001
    s[ "thickness" ] = s[ "thickness" ] * 0.001
  return surfaces

def get_translation( di ):
  return np.matrix([
    [ 1, di ],
    [ 0, 1 ]
  ]);

def get_refraction( n1, n2, ri ):
  return np.matrix([
    [ 1, 0 ],
    [ (n1-n2)/(n2*ri), n1/n2 ]
  ]);

def get_reflection( ri ):
  return np.matrix([
    [ 1, 0 ],
    [ 2/ri, 1 ]
  ]);

def get_flat_refraction( n1, n2 ):
  return np.matrix([
    [ 1, 0 ],
    [ 0, n1/n2 ]
  ]);

def get_flat_reflection():
  return np.matrix([
    [ 1, 0 ],
    [ 0, 1 ]
  ]);


def get_light_dir( light_pos, lens_radius ):
  eye_dir = np.array([ 0.0, 0.0, 1.0 ])
  lens_edge = ( light_pos[ :2 ]/np.linalg.norm( light_pos[ :2 ] ) ) * lens_radius;
  light_pos0 = light_pos - np.array([ lens_edge[ 0 ], lens_edge[ 1 ], 0.0 ])
  light_pos1 = light_pos + np.array([ lens_edge[ 0 ], lens_edge[ 1 ], 0.0 ])
  light_dir0 = light_pos0/np.linalg.norm( light_pos0 )
  light_dir1 = light_pos0/np.linalg.norm( light_pos1 )
  theta0 = math.acos( np.dot( light_dir0, eye_dir ) )
  theta1 = math.acos( np.dot( light_dir1, eye_dir ) )
  return ( np.array([ -lens_radius, theta0 ]), np.array([ lens_radius, theta1 ]) )



light_pos = np.array([ 10.0, 10.0, 10.0 ])

edge = get_light_dir( light_pos, 0.05 )

p = [ edge[ 0 ] * ( 1.0 - x/10.0 ) + edge[ 1 ] * ( x/10.0 ) for x in range( 0, 11 ) ]

surfaces = load_surface( args.input )

matrices = []

for first_reflection in range( 0, len( surfaces ) ):
  for second_reflection in range( 0, first_reflection ):
    m = np.array( [[ 1.0, 0.0 ],[ 0.0, 1.0 ]] )
    current_surface = 0
    current_refractive_index = 1.0
    d = 0.0
    with open( "{}_{}.log".format( first_reflection, second_reflection ), 'w' ) as fd:
      print( "{} {} {} {} {} {} {} {} {} {} {} {}".format( d,
        np.linalg.matmul( m, p[ 0 ] )[ 0 ],
        np.linalg.matmul( m, p[ 1 ] )[ 0 ],
        np.linalg.matmul( m, p[ 2 ] )[ 0 ],
        np.linalg.matmul( m, p[ 3 ] )[ 0 ],
        np.linalg.matmul( m, p[ 4 ] )[ 0 ],
        np.linalg.matmul( m, p[ 5 ] )[ 0 ],
        np.linalg.matmul( m, p[ 6 ] )[ 0 ],
        np.linalg.matmul( m, p[ 7 ] )[ 0 ],
        np.linalg.matmul( m, p[ 8 ] )[ 0 ],
        np.linalg.matmul( m, p[ 9 ] )[ 0 ],
        np.linalg.matmul( m, p[ 10 ] )[ 0 ]
      ), file=fd )
      current_surface = 0
      direction = 1
      phase = 0
      while current_surface != len( surfaces ):
        surface = surfaces[ current_surface ]
        if current_surface == first_reflection and phase == 0:
          phase = 1
          direction = -1
          if "radius" in surface:
            m = np.linalg.matmul( get_reflection( surface[ "radius" ] ), m )
          else:
            m = np.linalg.matmul( get_flat_reflection(), m )
        elif current_surface == second_reflection and phase == 1:
          phase = 2
          direction = 1
          if "radius" in surface:
            m = np.linalg.matmul( np.linalg.inv( get_reflection( surface[ "radius" ] ) ), m )
          else:
            m = np.linalg.matmul( np.linalg.inv( get_flat_reflection() ), m )
        else:
          if "radius" in surface:
            if direction == 1:
              m = np.linalg.matmul( get_refraction( current_refractive_index, surface[ "refractive index" ], surface[ "radius" ] ), m )
              current_reflaction_index = surface[ "refractive index" ]
            else:
              m = np.linalg.matmul( np.linalg.inv( get_refraction( surface[ "previous refractive index" ], current_refractive_index, surface[ "radius" ] ) ), m )
              current_reflaction_index = surface[ "previous refractive index" ]
            m = np.linalg.matmul( get_translation( surface[ "thickness" ] ), m )
            d += surface[ "thickness" ] * direction
          else:
            if direction == 1:
              m = np.linalg.matmul( get_flat_refraction( current_refractive_index, surface[ "refractive index" ] ), m )
              current_reflaction_index = surface[ "refractive index" ]
            else:
              m = np.linalg.matmul( np.linalg.inv( get_flat_refraction( surface[ "previous refractive index" ], current_refractive_index ) ), m )
              current_reflaction_index = surface[ "previous refractive index" ]
            m = np.linalg.matmul( get_translation( surface[ "thickness" ] ), m )
            d += surface[ "thickness" ] * direction
          print( "{} {} {} {} {} {} {} {} {} {} {} {}".format( d,
            np.linalg.matmul( m, p [ 0 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 1 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 2 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 3 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 4 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 5 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 6 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 7 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 8 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 9 ] )[ 0,0 ],
            np.linalg.matmul( m, p[ 10 ] )[ 0,0 ]
          ), file=fd )
        current_surface += direction
      matrices.append( m )

with open( "flare.inc", "w" ) as fd:
  initial = True
  for m in matrices:
    if initial:
      initial = False
    else:
      print( ",", file=fd )
    print( "mat2( {}, {}, {}, {} )".format( m[ 0, 0 ], m[ 1, 0 ], m[ 0, 1 ], m[ 1, 1 ] ), end="", file=fd )
  print( "", file=fd )

#print( p0 * m )
#print( p1 * m )

