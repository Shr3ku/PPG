// piramida.cpp
// Our first OpenGL program that will just draw a triangle on the screen.
#include "stdafx.h"
#include <GLTools.h>            // OpenGL toolkit
#include <GLFrustum.h>
#include <GLFrame.h>

#ifdef __APPLE__
#include <glut/glut.h>          // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>            // Windows FreeGlut equivalent
#endif


GLuint shader;
GLFrame frame;
GLint MVPMatrixLocation;
GLFrame cameraFrame;
GLFrustum viewFrustum;
CStopWatch timer;

//GLfloat matrix[16];
M3DMatrix44f MatrixCamera;
M3DMatrix44f mViewProjectionMatrix;

///////////////////////////////////////////////////////////////////////////////
//uklad wspólrzednych

				
void SetUpFrame(GLFrame &frame,const M3DVector3f origin,const M3DVector3f forward,const M3DVector3f up) {
						frame.SetOrigin(origin);
						frame.SetForwardVector(forward);
						M3DVector3f side,oUp;
						m3dCrossProduct3(side,forward,up);
						m3dCrossProduct3(oUp,side,forward);
						frame.SetUpVector(oUp);
						frame.Normalize();
						
};

void LookAt(GLFrame &frame, const M3DVector3f eye,const M3DVector3f at,const M3DVector3f up) {
    M3DVector3f forward;
    m3dSubtractVectors3(forward, at, eye);
    SetUpFrame(frame, eye, forward, up);
}

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(50.0f, w/h, 0.5f, 500.0f);
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.

void SetupRC() {
    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    shader = gltLoadShaderPairWithAttributes("shader.vp", "shader.fp",
            2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_COLOR, "vColor");
    fprintf(stdout, "GLT_ATTRIBUTE_VERTEX : %d\nGLT_ATTRIBUTE_COLOR : %d \n",
            GLT_ATTRIBUTE_VERTEX, GLT_ATTRIBUTE_COLOR);

	MVPMatrixLocation=glGetUniformLocation(shader,"MVMatrix");
	if(MVPMatrixLocation==-1)
	{
		fprintf(stderr,"uniform MVPMatrix could not be found\n");
	}
	  M3DVector3f	eye = {0.0f, -0.5f, 2.0f};		//Ustawienia kamery
	  M3DVector3f	at = {0.0f, 0.2f, 0.5f};
	  M3DVector3f	up = {2.0f, 0.0f, 0.0f};
	LookAt(frame,eye,at,up);

}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene

void RenderScene(void) {
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	M3DMatrix44f MatrixCamera;
    M3DMatrix44f ViewProjectionMatrix;

   
	glUseProgram(shader);

	cameraFrame.GetCameraMatrix(MatrixCamera);
	frame.GetMatrix(mViewProjectionMatrix);
	m3dMatrixMultiply44(ViewProjectionMatrix,mViewProjectionMatrix,MatrixCamera);
	glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,ViewProjectionMatrix);

   // podstawa
	glBegin(GL_QUADS);
	glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.73f, 0.73f, 0.73f);
		glVertex3f(-1.0f, -1.0f, 0.0f); //wierzcholek lewa dol
		glVertex3f(1.0f, -1.0f, 0.0f); //wierzcholek prawa dol
		glVertex3f(1.0f, 1.0f, 0.0f); //wierzcholek prawa gora
		glVertex3f(-1.0f, 1.0f, 0.0f); //wierzcholek lewa gora
	glEnd();


	//niebieska
	glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f); 
		glVertex3f(-1.0f, 1.0f, 0.0f); 
	glEnd();
	
	//zólta
	glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f); 
		glVertex3f(1.0f, 1.0f, 0.0f); 
	glEnd();

	//zielona
	glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(1.0f, -1.0f, 0.0f); 
		glVertex3f(-1.0f, -1.0f, 0.0f); 
	glEnd();
	
	//czerwona	
	glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(1.0f, 1.0f, 0.0f); 
		glVertex3f(1.0f, -1.0f, 0.0f); 
	glEnd();

	//animacja
	M3DVector3f at={0,0,0};
	M3DVector3f up={0,0,1};
	M3DVector3f eye;
	float angle =timer.GetElapsedSeconds()*3.15f;

	eye[0]=6.8f*cos(angle);
	eye[1]=6.0f*sin(angle);
	eye[2]=7.0f; 
	LookAt(cameraFrame,eye,at,up);


    // Perform the buffer swap to display back buffer
    glutSwapBuffers();
	glutPostRedisplay();
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs

int main(int argc, char* argv[]) {
  

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Piramida");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    SetupRC();

    glutMainLoop();
    return 0;
}
