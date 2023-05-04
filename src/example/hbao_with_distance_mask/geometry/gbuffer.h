

#define
  output_position = vec4( pos, gl_FragCoord.z );
  output_albedo = albedo;
  output_normal = vec4( normal.x, normal.y, tangent.x, tangent.y );
  output_emissive = vec4( emissive, occlusion );
  output_material = vec4( metalness, roughness, gl_InstanceID, gl_PrimitiveID );
  output_optflow = vec4( optflow, 0.0 );
  output_shadow = vec4( shadow_level0, shadow_level1, shadow_level2, shadow_level3 );
