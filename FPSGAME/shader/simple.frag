uniform vec3 u_light_vector_vc;

uniform vec4 u_light_ambient;
uniform vec4 u_light_diffuse;
uniform vec4 u_light_specular;

uniform vec4  u_material_ambient;
uniform vec4  u_material_diffuse;
uniform vec4  u_material_specular;
uniform float u_material_shininess;

varying vec3 v_vertex_vc;
varying vec3 v_normal_vc;


uniform sampler2D u_texid;
varying vec2 v_texcoord;

vec4 directional_light()
{
  vec4 color = vec4(0);
  vec3 normal_vc = normalize(v_normal_vc);

  vec3 light_vector_vc = normalize(u_light_vector_vc);
  vec3 light_incident_vector_vc = - light_vector_vc;
  vec3 reflect_vector_vc = reflect(light_incident_vector_vc, normal_vc);

  vec3 view_vector_vc = -v_vertex_vc;
  view_vector_vc = normalize(view_vector_vc);

  color += (u_light_ambient * u_material_ambient);

  float ndotl = max(0.0, dot(normal_vc, light_vector_vc));
  color += (ndotl * u_light_diffuse * u_material_diffuse);

  float rdotv = max(0.0, dot(reflect_vector_vc, view_vector_vc) );
  color += (pow(rdotv, u_material_shininess) * u_light_specular * u_material_specular);

  return color;
}

void main()
{
  gl_FragColor= 0.5*directional_light()+0.5*texture2D(u_texid,v_texcoord);
}
