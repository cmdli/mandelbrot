varying vec4 pos;
void main() {
    float num_colors = 100.0;
    int max_iterations = 10000;
    float a = 0.0;
    float atmp = 0.0;
    float b = 0.0;
    int i = 0;
    while(a * a + b * b < 4.0 && i < max_iterations) {
      atmp = 2.0 * a * b + pos.y;
      b = b * b - a * a + pos.x;
	    a = atmp;
	    i = i + 1;
    }
    float fi = float(i);
    if(i >= max_iterations) {
      gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
      gl_FragColor = vec4(0.0, (num_colors - mod(fi,num_colors))/num_colors, mod(fi,num_colors)/num_colors, 1.0);
    }
    //gl_FragColor = vec4(pos.x, pos.y, 0.0, 1.0);
    //gl_FragColor = vec4(abs(pos.x)/2.0, 0.0, 0.0, 1.0);
}