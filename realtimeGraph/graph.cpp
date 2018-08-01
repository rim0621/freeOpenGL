#include"graph.h"

/* Main function */
int main(int argc, char **argv){

	pthread_t thread_t;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1000,300);
	glutCreateWindow("Graph plotter");

	/* Register GLUT callbacks. */

	glutDisplayFunc(redraw);
	glutKeyboardFunc(key);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	pthread_create(&thread_t,NULL,getData,NULL);
	/* Init the GL state */
	glLineWidth(1.0);

	/* Main loop */
	glutMainLoop();
	return 0;
}
