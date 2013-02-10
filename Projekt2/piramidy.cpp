#define FREEGLUT_STATIC
#include <windows.h>
#include <GL/glew.h>
#include <GLTools.h>  
#include <GL/freeglut.h>            
#include <GLFrustum.h>
#include <GLFrame.h>
#include <StopWatch.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#pragma comment(lib, "gltools.lib")

GLuint shader;
GLuint MVMatrixLocation;
GLFrame cameraFrame;
GLFrustum viewFrustum;
CStopWatch timer;
GLMatrixStack modelView;
GLMatrixStack projection;
GLGeometryTransform geometryPipeline;
GLMatrixStack matrixStack;

void ChangeSize(int width, int height) {
    glViewport(0, 0, width, height);
    viewFrustum.SetPerspective(45.0f, width/height, 0.1f, 1000.0f);
}

void SetUpFrame(GLFrame &frame, const M3DVector3f origin, const M3DVector3f forward, const M3DVector3f up) {
        frame.SetOrigin(origin);
        frame.SetForwardVector(forward);
        M3DVector3f side,oUp;
        m3dCrossProduct3(side,forward,up);
        m3dCrossProduct3(oUp,side,forward);
        frame.SetUpVector(oUp);
        frame.Normalize();
}

void LookAt(GLFrame &frame, const M3DVector3f eye, const M3DVector3f at, const M3DVector3f up) {
    M3DVector3f forward;
    m3dSubtractVectors3(forward, at, eye);
    SetUpFrame(frame, eye, forward, up);
}

void SetupRC() {
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    shader = gltLoadShaderPairWithAttributes("pass_thru_shader.vp", "pass_thru_shader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_COLOR, "vColor");
    fprintf(stdout, "GLT_ATTRIBUTE_VERTEX : %d\nGLT_ATTRIBUTE_COLOR : %d \n", GLT_ATTRIBUTE_VERTEX, GLT_ATTRIBUTE_COLOR);
    MVMatrixLocation=glGetUniformLocation(shader,"MVMatrix");
    if(MVMatrixLocation==-1){
        fprintf(stderr,"uniform MVMatrix could not be found\n");
    }
    geometryPipeline.SetMatrixStacks(modelView, projection);
    M3DVector3f eye = {1.0f, 1.0f, 5.0f};
    M3DVector3f at = {4.0f, 4.0f, 4.0f};
    M3DVector3f up = {2.0f, 2.0f, 0.5f};
    LookAt(cameraFrame, eye, at, up);
}

void rysowaniePiramidy(){
		glBegin(GL_QUADS);
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.70f, 0.60f, 0.50f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
		
		glBegin(GL_TRIANGLES);
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.9f, 1.0f, 0.6f);
		glVertex3f(1.0f, 1.0f, 2.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();
		
		glBegin(GL_TRIANGLES);
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.75f, 1.0f, 0.15f);
		glVertex3f(1.0f, 1.0f, 3.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();
		
		glBegin(GL_TRIANGLES);
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.5f, 0.55f, 1.0f);
		glVertex3f(1.0f, 1.0f, 3.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
		
		 glBegin(GL_TRIANGLES);
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.1f, 0.0f, 0.5f);
		glVertex3f(1.0f, 1.0f, 3.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glEnd();
}

void siatka(){
    glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for(int i=-10;i<11;i++){
        for(int j=-10; j<11;j++){
            glVertex3f(i, j, 0.0f);
            glVertex3f(-i, j, 0.0f);
            glVertex3f(i, -j, 0.0f);
            glVertex3f(i, j, 0.0f);
        }
    }
    glEnd();
}
void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glFrontFace(GL_CW);
    glUseProgram(shader);
    M3DVector3f at={0.0f, 0.0f, 0.0f};
    M3DVector3f up={0.0f, 0.0f, 1.0f};
    M3DVector3f eye;
    float angle = timer.GetElapsedSeconds()*3.14159f/8;
    eye[0]= -6.8f * cos(angle);
    eye[1]= 4.0f * sin(angle);
    eye[2]= 22.0f;
    LookAt(cameraFrame,eye,at,up);

    projection.LoadMatrix(viewFrustum.GetProjectionMatrix());
    modelView.PushMatrix();
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);

    modelView.LoadMatrix(mCamera);
    modelView.PushMatrix();
	glUniformMatrix4fv(MVMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
	siatka();
		
    modelView.Translate(0.0f, 0.0f, 0.0f);
    modelView.PopMatrix();
	modelView.PushMatrix();
	glUniformMatrix4fv(MVMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
	rysowaniePiramidy();
	
    modelView.Translate(5.0f, 3.0f, 5.0f);
	modelView.Rotate(60,1,1,0);
    glUniformMatrix4fv(MVMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
	rysowaniePiramidy();
	
    modelView.PopMatrix();
    modelView.PushMatrix();
    modelView.Translate(-5.0f, -3.0f, -5.0f);
    modelView.Scale(2.5f, 2.5f, 4.0f);
    glUniformMatrix4fv(MVMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
	rysowaniePiramidy();
	
    modelView.PopMatrix();
    modelView.PopMatrix();

	glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Piramidy");
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