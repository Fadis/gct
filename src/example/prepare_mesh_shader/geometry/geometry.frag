#version 460

layout (location = 0) out vec4 output_color;

void main() {
  // 0番目のカラーバッファを白で塗る
  output_color = vec4( 1, 1, 1, 1 );
}

