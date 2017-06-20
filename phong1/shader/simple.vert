uniform mat4 u_pvm_matrix;
uniform mat4 u_vm_matrix;
uniform mat3 u_normal_matrix;

attribute vec3 a_vertex;
attribute vec3 a_normal;

varying vec3 v_vertex_vc;
varying vec3 v_normal_vc;

void main()
{
  v_normal_vc = normalize(u_normal_matrix * a_normal);
  v_vertex_vc = (u_vm_matrix * vec4(a_vertex, 1)).xyz;

  gl_Position = u_pvm_matrix * vec4(a_vertex, 1);
}
