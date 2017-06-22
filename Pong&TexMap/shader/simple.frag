
uniform sampler2D u_texid;

varying vec2 v_texcoord;


void main()
{
  gl_FragColor= 1.0*texture2D(u_texid,v_texcoord);
}
