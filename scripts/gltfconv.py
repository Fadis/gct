#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import re
import json
import argparse
import subprocess
import shutil

possible_input_format=[
  'asis',
  'dds:bc1',
  'dds:bc2',
  'dds:bc3',
  'dds:bc4',
  'dds:bc5',
  'dds:bc6h',
  'dds:bc7',
  'ktx1:bc1',
  'ktx1:bc2',
  'ktx1:bc3',
  'ktx1:bc4',
  'ktx1:bc5',
  'ktx1:bc6h',
  'ktx1:bc7',
  'ktx1:etc2',
  'ktx2'
]

compressonator = shutil.which( 'compressonatorcli-bin' )

if compressonator is None:
  print( "compressonatorcli-bin is not installed." )
  sys.exit( 1 )

ktx = shutil.which( 'ktx' )

if ktx is None:
  print( "ktx is not installed." )
  sys.exit( 1 )

def main() -> int:
  parser = argparse.ArgumentParser(
    prog=sys.argv[ 0 ],
    description='Convert glTF'
  )
  parser.add_argument('input')
  parser.add_argument('output')
  parser.add_argument('--image_format',choices=possible_input_format)
  args = parser.parse_args()
  if args.input == args.output:
    print( "input file and output file must be different." )
    sys.exit( 1 )
  input_dir = os.path.dirname( args.input );
  output_dir = os.path.dirname( args.output );
  parsed = json.load( open( args.input, 'r' ) );
  sampled_image_ids = set()
  if "textures" in parsed:
    for t in parsed[ "textures" ]:
      if "source" in t:
        sampled_image_ids.add( t[ "source" ] )

  ktx2_image_id = set()
  if "images" in parsed:
    id = 0
    for i in parsed[ "images" ]:
      if "uri" in i:
        input_filepath = os.path.join( input_dir, i[ "uri" ] )
        image_format = args.image_format.split( ':' )
        if not id in sampled_image_ids:
          output_filepath = os.path.join( output_dir, i[ "uri" ] )
          if input_filepath != output_filepath:
            shutil.copyfile( input_filepath, output_filepath )
        elif image_format[ 0 ] == 'dds':
          output_filename = os.path.splitext( i[ "uri" ] )[ 0 ]+'.dds'
          output_filepath = os.path.join( output_dir, output_filename )
          subprocess.run( [ 'compressonatorcli-bin', '-fd', image_format[ 1 ], '-miplevels', '20', input_filepath, output_filepath ], check=True )
          i[ "uri" ] = output_filename
          i[ "mimeType" ] = "image/vnd.ms-dds"
        elif image_format[ 0 ] == 'ktx1':
          output_filename = os.path.splitext( i[ "uri" ] )[ 0 ]+'.ktx'
          output_filepath = os.path.join( output_dir, output_filename )
          subprocess.run( [ 'compressonatorcli-bin', '-fd', image_format[ 1 ], '-miplevels', '20', input_filepath, output_filepath ], check=True )
          i[ "uri" ] = output_filename
          i[ "mimeType" ] = "image/ktx"
        elif image_format[ 0 ] == 'ktx2':
          output_filename = os.path.splitext( i[ "uri" ] )[ 0 ]+'.ktx2'
          output_filepath = os.path.join( output_dir, output_filename )
          subprocess.run( [ 'ktx', 'create', '--format', 'R8G8B8A8_SRGB', '--assign-tf=srgb', '--encode=basis-lz', '--generate-mipmap', input_filepath, output_filepath ], check=True )
          i[ "uri" ] = output_filename
          i[ "mimeType" ] = "image/ktx2"
          ktx2_image_id.add( id )
        else:
          output_filepath = os.path.join( output_dir, i[ "uri" ] )
          if input_filepath != output_filepath:
            shutil.copyfile( input_filepath, output_filepath )
      id += 1
  
  if "textures" in parsed:
    for t in parsed[ "textures" ]:
      if "source" in t:
        if t[ "source" ] in ktx2_image_id:
          t[ "extensions" ] = {
            "KHR_texture_basisu" : {
              "source" : t[ "source" ]
            }
          }
          del t[ "source" ]

  if "buffers" in parsed:
    for b in parsed[ "buffers" ]:
      if "uri" in b:
        input_filename = os.path.join( input_dir, b[ "uri" ] )
        output_filename = os.path.join( output_dir, b[ "uri" ] )
        if input_filename != output_filename:
          shutil.copyfile( input_filename, output_filename )

  json.dump( parsed, open( args.output, 'w' ), indent=2 )

if __name__ == '__main__':
  sys.exit( main() )
