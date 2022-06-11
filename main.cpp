// 33_ModelLoading
//     Mouse: left mouse dragging - arcball rotation
//            wheel - zooming
//     Keyboards:  r - reset camera and object position
//                 a - toggle camera/object rotations for arcball
//                 arrow left, right, up, down: panning object position

#define GLM_ENABLE_EXPERIMENTAL
#include <stb_image.h>
// Std. Includes
#include <string>
#include <iostream>
#include <ctime>
#include <cmath>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include <shader.h>
#include <text.h>
#include <plane.h>
#include <cube.h>
using namespace std;
#include <arcball.h>
#include <Model.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <keyframe.h>
#include "link.h"


// Globals
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWwindow *mainWindow = NULL;
glm::mat4 projection;
Shader *textShader = NULL;
Shader *lightingShader = NULL;
Text *text = NULL;
Shader *shader;
Shader *globalShader = NULL;
Shader *lightingShader1 = NULL;

// For model
Model *ourModel;
Model *ddong;
Plane *vRect;
Cube *cube;
int inter = 0;
// for arcball
float arcballSpeed = 0.2f;
static Arcball camArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true );
static Arcball modelArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
bool arcballCamRot = true;
float one;
float two;
float three;
float four;
float five;
float six;
float seven;
float eight;
float nine;
float ten;
float oone;
float ttwo;
float tthree;
float ffour;
float ffive;
float ssix;
float sseven;
float eeight;
float nnine;
float tten;
double get1;
double get2;
double get3;
bool dead = false;
bool a = false;
bool b = false;
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

// for camera
glm::vec3 cameraOrigPos(0.0f, 0.0f, 9.0f);
glm::vec3 cameraPos(0.0f, 0.0f, 15.0f);
glm::vec3 modelPan(0.0f, 0.0f, 0.0f);
glm::vec3 rright(0.0f, 1.0f, 0.0f);
glm::vec3 lleft(0.0f, -1.0f, 0.0f);
glm::vec3 dir = rright;
// Function prototypes

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double x, double y);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
GLFWwindow *glAllInit();
void render();
void initKeyframes();
void updateAnimData();
void initLinks();
void drawLinks(Link *root, float t, glm::mat4 cmodel, Shader *shader);

unsigned int loadTexture(string);
Link *root;

enum RenderMode { INIT, ANIM, STOP };
RenderMode renderMode;                  // current rendering mode
float beginT;                           // animation beginning time (in sec)
float timeT;                            // current time (in sec)
float animEndTime = 5.0f;               // ending time of animation (in sec)
float xTrans, yTrans, zTrans;           // current translation factors
float xAngle, yAngle, zAngle;           // current rotation factors
KeyFraming xTKF(4), yTKF(4), zTKF(4);   // translation keyframes
KeyFraming xRKF(4), yRKF(4), zRKF(4);   // rotation keyframes


static unsigned int texture1, texture2; // Array of texture ids.
glm::vec3 spotlightPos(0.0f, -4.4f, 1.0f);
glm::vec3 spotlightDirection = -spotlightPos;

int main( )
{
	srand((size_t)time(NULL));

	one = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	two = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	three = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	four = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	five = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	six = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	seven = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	eight = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	nine = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	ten = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	oone = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	ttwo = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	tthree = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	ffour = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	ffive = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	ssix = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	sseven = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	eeight = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	nnine = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	tten = (double)rand() / (RAND_MAX + 1)*(8) - 4;
	cout << one<< endl;
    mainWindow = glAllInit();
	
	vRect = new Plane();
	textShader = new Shader("text.vs", "text.frag");
	textShader->use();
	text = new Text((char*)"fonts/ENBLBK.ttf", textShader, SCR_WIDTH, SCR_HEIGHT);
    // Create shader program object
    shader = new Shader( "res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag" );
	globalShader = new Shader("global.vs", "global.fs");
	//globalShader1 = new Shader("global.vs", "lobal.fs");
	lightingShader = new Shader("lighting_maps.vs", "lighting_maps.fs");
	lightingShader1 = new Shader("spotlight.vs", "spotlight.fs");
    // Load models
    ourModel = new Model((GLchar *)"res/models/cat/12221_Cat_v1_l3.obj" );

    ddong = new Model((GLchar *)"res/models/rock/rock.obj" );
    //ourModel = new Model((GLchar *)"res/models/nanosuit/nanosuit.obj" );
    //ourModel = new Model((GLchar *)"res/models/planet/planet.obj" );
   
    // Projection initialization
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	lightingShader1->use();
	lightingShader1->setMat4("projection", projection);
	lightingShader1->setInt("material.diffuse", 0);
	lightingShader1->setInt("material.specular", 1);
	lightingShader1->setFloat("material.shininess", 32);
	lightingShader1->setVec3("viewPos", cameraPos);
	lightingShader1->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	lightingShader1->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	lightingShader1->setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
	lightingShader1->setVec3("dirLight.specular", 0.8f, 0.8f, 0.8f);
	lightingShader1->setVec3("spotLight.position", spotlightPos);
	lightingShader1->setVec3("spotLight.direction", spotlightDirection);
	lightingShader1->setVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
	lightingShader1->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	lightingShader1->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	lightingShader1->setFloat("spotLight.constant", 1.0f);
	lightingShader1->setFloat("spotLight.linear", 0.09);
	lightingShader1->setFloat("spotLight.quadratic", 0.032);
	lightingShader1->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader1->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

	lightingShader->use();
	lightingShader->setMat4("projection", projection);
	texture1 = loadTexture("container2.bmp");
	texture2 = loadTexture("feature.jpg");
	cube = new Cube();

	//globalShader1->use();
	//globalShader1->setMat4("projection", projection);
	globalShader->use();
	globalShader->setMat4("projection", projection);
    shader->use();
    shader->setMat4("projection", projection);
    cameraPos = cameraOrigPos;
    
	initKeyframes();
	timeT = 0.0f;
	updateAnimData();
	renderMode = INIT;

	initLinks();
	timeT = 0.0f;
	renderMode = INIT;

    // Game loop
    while( !glfwWindowShouldClose( mainWindow ) )
    {
        glfwPollEvents( );
        
        render();
        
        glfwSwapBuffers( mainWindow );
    }
    
    glfwTerminate( );
    return 0;
}

void initKeyframes() {

	// x-translation keyframes
	xTKF.setKey(0, 0, -4.0);
	xTKF.setKey(1, 1.5, -2.0);
	xTKF.setKey(2, 3.0, 2.0);
	xTKF.setKey(3, animEndTime, 4.0);

	// y-translation keyframes
	yTKF.setKey(0, 0, 3.0);
	yTKF.setKey(1, 1.5, -2.0);
	yTKF.setKey(2, 3.0, 2.0);
	yTKF.setKey(3, animEndTime, -3.0);

	// z-translation keyframes
	zTKF.setKey(0, 0, 1.0);
	zTKF.setKey(1, 1.5, 2.0);
	zTKF.setKey(2, 3.0, 2.0);
	zTKF.setKey(3, animEndTime, 2.0);

	// x-rotation keyframes
	xRKF.setKey(0, 0, 0.0);
	xRKF.setKey(1, 1.5, 20.0);
	xRKF.setKey(2, 3.0, 80.0);
	xRKF.setKey(3, animEndTime, 0.0);

	// y-rotation keyframes
	yRKF.setKey(0, 0, 0.0);
	yRKF.setKey(1, 1.5, -30.0);
	yRKF.setKey(2, 3.0, 50.0);
	yRKF.setKey(3, animEndTime, 0.0);

	// z-rotation keyframes
	zRKF.setKey(0, 0, 0.0);
	zRKF.setKey(1, 1.5, 90.0);
	zRKF.setKey(2, 3.0, 180.0);
	zRKF.setKey(3, animEndTime, 200.0);
}

void updateAnimData() {
	if (timeT > animEndTime) {
		renderMode = STOP;
		timeT = animEndTime;
	}
	xTrans = xTKF.getValLinear(timeT);
	yTrans = yTKF.getValLinear(timeT);
	zTrans = zTKF.getValLinear(timeT);
	xAngle = xRKF.getValLinear(timeT);
	yAngle = yRKF.getValLinear(timeT);
	zAngle = zRKF.getValLinear(timeT);
}
void initLinks()
{
	//Link(string name, glm::vec3 color, bool isRoot, int nChild,
	//     glm::vec3 size,
	//     glm::vec3 offset,
	//     glm::vec3 trans1,
	//     glm::vec3 trans2,
	//     glm::vec3 rot1,       //angles in degree
	//     glm::vec3 rot2)       //angles in degree

	// root link: yellow
	root = new Link("ROOT", glm::vec3(1.0, 1.0, 0.0), true, 2,
		glm::vec3(1.0, 3.0, 1.0),   // size
		glm::vec3(0.0, 0.0, 0.0),   // offset
		glm::vec3(0.0, 0.0, 0.0),   // trans1 w.r.t. origin (because root)
		glm::vec3(0.0, 0.0, 0.0),   // trans2 w.r.t. origin (because root)
		glm::vec3(0.0, 0.0, 0.0),   // no rotation
		glm::vec3(0.0, 0.0, 0.0));  // no rotation

// left upper arm: red
	root->child[0] = new Link("LEFT_ARM_UPPER", glm::vec3(1.0, 0.0, 0.0), false, 1,
		glm::vec3(1.0, 1.0, 1.0),  // size
		glm::vec3(1.0, 0.0, 0.0),  // offset
		glm::vec3(0.0, 0, 0.0),  // trans1
		glm::vec3(0.0,0, 0.0),  // trans2
		glm::vec3(0.0, 0.0, 0.0),  // rotation about parent
		glm::vec3(0.0, 0.0, 60.0));

	// left low arm: orange
	root->child[0]->child[0] = new Link("LEFT_ARM_LOWER", glm::vec3(1.0, 0.5, 0.0), false, 0,
		glm::vec3(1.0, 1.0, 1.0),  // size
		glm::vec3(1.0, 0.0, 0.0),  // offset
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, -30.0, -50.0));

	// right upper arm: green
	root->child[1] = new Link("RIGHT_ARM_UPPER", glm::vec3(0.0, 1.0, 0.0), false, 1,
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 0.0, 0.0),
		glm::vec3(0.0, 0, 0.0),  // trans1
		glm::vec3(0.0, 0, 0.0),  // trans2
		glm::vec3(0.0, 0.0, 0.0),  // rotation about parent
		glm::vec3(0.0, 0.0, -60.0));

	// right lower arm: cyan
	root->child[1]->child[0] = new Link("RIGHT_ARM_LOWER", glm::vec3(0.0, 0.5, 1.0), false, 0,
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(-1.0, 0.0, 0.0),  // offset
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 30.0, 50.0));
}

void drawLinks(Link *clink, float t, glm::mat4 cmodel, Shader *shader)
{

	if (t > 1.0) t = 1.0f;

	glm::mat4 thisMat = glm::mat4(1.0f);

	// accumulate the parent's transformation
	thisMat = thisMat * cmodel;

	// if root, interpolates the translation
	glm::vec3 ctrans = glm::mix(clink->trans1, clink->trans2, t);
	thisMat = glm::translate(thisMat, ctrans);

	// interpolates the rotation
	//glm::quat q = glm::slerp(clink->q1, clink->q2, t);
	glm::vec3 euler = glm::mix(clink->rot1, clink->rot2, t);
	glm::quat q = glm::quat(euler);

	glm::mat4 crot = q.operator glm::mat4x4();

	thisMat = thisMat * crot;

	// render the link
	shader->use();
	shader->setMat4("model", thisMat);
	clink->draw(shader);

	// recursively call the drawLinks for the children
	for (int i = 0; i < clink->nChild; i++) {
		drawLinks(clink->child[i], t, thisMat, shader);
	}

}


void render()
{
	float current = glfwGetTime();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	shader->use();

	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = view * camArcBall.createRotationMatrix();
	shader->setMat4("view", view);
	globalShader->use();
	globalShader->setMat4("view", view);

	if (renderMode == ANIM) {
		float cTime = (float)glfwGetTime(); // current time
		timeT = cTime - beginT;
		updateAnimData();
	}

	if (a == true || b == true) {
		glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDisable(GL_DEPTH_TEST);

			glm::mat4 modelr(1.0);
			modelr = glm::scale(modelr, glm::vec3(8.0f, 2.0f, 1.0f));
			globalShader->setMat4("model", modelr);
			vRect->draw(globalShader);

	}
	
	if (a == true || b == true) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}
	

	if (dead == true) {
		a = true;
		textShader->use();
		text->RenderText("You Lose", 160.0f, 270.0f, 3.0f, glm::vec3(0.5, 0.8f, 0.2f));
		if (current > 20) {
			glfwTerminate();
			return;
		}
	}

	if (current > 20 && dead == false) {
		b = true;
		textShader->use();
		text->RenderText("You Win", 100.0f, 270.0f, 3.0f, glm::vec3(0.5, 0.8f, 0.2f));
	}
	lightingShader->use();
	lightingShader->setMat4("view", view);
	lightingShader->setVec3("light.position", lightPos);
	lightingShader->setVec3("viewPos", cameraPos);

	lightingShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader->setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
	lightingShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	// material properties
	lightingShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	lightingShader->setFloat("material.shininess", 64.0f);
	glm::mat4 modelc(1.0f);
	modelc = glm::translate(modelc, glm::vec3(0.0f, 3.0f, 0.0f));
	modelc = glm::scale(modelc, glm::vec3(10.0f, 1.0f, 5.0f));
	lightingShader->setMat4("model", modelc);

	glBindTexture(GL_TEXTURE_2D, texture1);
	cube->draw(lightingShader);
	
	lightingShader->use();
	glm::mat4 modela(1.0f);
	modela = glm::translate(modela, glm::vec3(xTrans, yTrans, zTrans));
	glm::vec3 eulerAngles(glm::radians(xAngle), glm::radians(yAngle), glm::radians(zAngle));
	glm::quat q(eulerAngles);
	glm::mat4 rotMatrix = q.operator glm::mat4x4();
	modela = modela * rotMatrix;

	lightingShader->setMat4("model", modela);
	cube->draw(lightingShader);

	glm::mat4 modelL(1.0f);
	drawLinks(root, timeT / animEndTime, modelL, lightingShader);


	lightingShader1->use();
	lightingShader1->setMat4("view", view);

	glm::mat4 modelc1(1.0f);
	modelc1 = glm::translate(modelc1, glm::vec3(0.0f, -3.5f, 0.0f));
	modelc1 = glm::scale(modelc1, glm::vec3(20.0f, 1.0f, 5.0f));
	lightingShader1->setMat4("model", modelc1);

	glBindTexture(GL_TEXTURE_2D, texture1);
	cube->draw(lightingShader1);

	lightingShader->use();
	glm::mat4 modelc2(1.0f);
	modelc2 = glm::translate(modelc2, glm::vec3(-4.7f, 0.0f, 0.0f));
	modelc2 = glm::rotate(modelc2, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelc2 = glm::scale(modelc2, glm::vec3(10.0f, 1.0f, 3.0f));
	lightingShader->setMat4("model", modelc2);

	glBindTexture(GL_TEXTURE_2D, texture1);
	cube->draw(lightingShader);

	glm::mat4 modelc3(1.0f);
	modelc3 = glm::translate(modelc3, glm::vec3(4.7f, 0.0f, 0.0f));
	modelc3 = glm::rotate(modelc3, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelc3 = glm::scale(modelc3, glm::vec3(10.0f, 1.0f, 3.0f));
	lightingShader->setMat4("model", modelc3);

	glBindTexture(GL_TEXTURE_2D, texture1);
	cube->draw(lightingShader);

	globalShader->use();

	glm::mat4 modelfeature(1.0f);
	//modelfeature = glm::translate(modelfeature, glm::vec3(0.0f, 0.0f, -3.0f));
	modelfeature = glm::scale(modelfeature, glm::vec3(10.0f, 7.0f, 1.0f));
	globalShader->setMat4("model", modelfeature);
	glBindTexture(GL_TEXTURE_2D, texture2);

	vRect->draw(globalShader);

    // Draw the loaded model
	shader->use();
    glm::mat4 model(1.0);
    
    // Rotate model by arcball and panning
    model = glm::translate( model, modelPan);
    model = model * modelArcBall.createRotationMatrix();
    
    // It's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), dir);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f,0.0f,0.0f));
    model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );
    
    shader->setMat4("model", model);
    
    ourModel->Draw( shader );
	if (current > 0) {

		glm::mat4 model1(1.0f);
		model1 = glm::translate(model1, glm::vec3(one, 3.0f - (current*2.1), 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model1);
		ddong->Draw(shader);
		if ( 3.0 - (current*2.1) > -2.8 && 3.0 -(current*2.1) < -2.3) {
			if ((one - 1) < (modelPan[0] + 0.3) && (one + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//2
		glm::mat4 model2(1.0f);
		model2 = glm::translate(model2, glm::vec3(two, 3.0f - (current*1.5), 0.0f));
		model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model2);
		ddong->Draw(shader);
		if (3.0 - (current*1.5) > -2.8 && 3.0 - (current*1.5) < -2.3) {
			if ((two - 1) < (modelPan[0] + 0.3) && (two + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//3
		glm::mat4 model3(1.0f);
		model3 = glm::translate(model3, glm::vec3(three, 3.0f - (current*1.8), 0.0f));
		model3 = glm::scale(model3, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model3);
		ddong->Draw(shader);
		if (3.0 - (current*1.8) > -2.8 && 3.0 - (current*1.8) < -2.3) {
			if ((three - 1) < (modelPan[0] + 0.3) && (three + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
	}
	//4
	if (current > 3) {
		glm::mat4 model4(1.0f);
		model4 = glm::translate(model4, glm::vec3(four, 3.0f - ((current-3)*1.4), 0.0f));
		model4 = glm::scale(model4, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model4);
		ddong->Draw(shader);
		if (3.0 - ((current - 3)*1.4) > -2.8 && 3.0 - ((current - 3)*1.4) < -2.3) {
			if ((four - 1) < (modelPan[0] + 0.3) && (four + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//5
		glm::mat4 model5(1.0f);
		model5 = glm::translate(model5, glm::vec3(five, 3.0f - ((current - 3)*1.7), 0.0f));
		model5 = glm::scale(model5, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model5);
		ddong->Draw(shader);
		if (3.0 - ((current - 3)*1.7) > -2.8 && 3.0 - ((current - 3)*1.7) < -2.3) {
			if ((five - 1) < (modelPan[0] + 0.3) && (five + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//6
		glm::mat4 model6(1.0f);
		model6 = glm::translate(model6, glm::vec3(six, 3.0f - ((current - 3)*2.0), 0.0f));
		model6 = glm::scale(model6, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model6);
		ddong->Draw(shader);
		if (3.0 - ((current - 3)*2.0) > -2.8 && 3.0 - ((current - 3)*2.0) < -2.3) {
			if ((six - 1) < (modelPan[0] + 0.3) && (six + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
	}

	if (current > 6) {
		glm::mat4 model7(1.0f);
		model7 = glm::translate(model7, glm::vec3(seven, 3.0f - ((current - 6)*1.4), 0.0f));
		model7 = glm::scale(model7, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model7);
		ddong->Draw(shader);
		if (3.0f - ((current - 6)*1.4) > -2.8 && 3.0 - ((current - 6)*1.4) < -2.3) {
			if ((seven - 1) < (modelPan[0] + 0.3) && (seven + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//8
		glm::mat4 model8(1.0f);
		model8 = glm::translate(model8, glm::vec3(eight, 3.0f - ((current - 6)*1.4), 0.0f));
		model8 = glm::scale(model8, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model8);
		ddong->Draw(shader);
		if (3.0f - ((current - 6)*1.4) > -2.8 && 3.0 - ((current - 6)*1.4) < -2.3) {
			if ((eight - 1) < (modelPan[0] + 0.3) && (eight + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//9
		glm::mat4 model9(1.0f);
		model9 = glm::translate(model9, glm::vec3(nine, 3.0f - ((current - 6)*2.6), 0.0f));
		model9 = glm::scale(model9, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model9);
		ddong->Draw(shader);
		if (3.0f - ((current - 6)*2.6) > -2.8 && 3.0 - ((current - 6)*2.6) < -2.3) {
			if ((nine - 1) < (modelPan[0] + 0.3) && (nine + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
	}
	if (current > 9) {
		//10
		glm::mat4 model10(1.0f);
		model10 = glm::translate(model10, glm::vec3(ten, 3.0f - ((current - 9)*3.0), 0.0f));
		model10 = glm::scale(model10, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model10);
		ddong->Draw(shader);
		if (3.0f - ((current - 9)*3.0) > -2.8 && 3.0 - ((current - 9)*3.0) < -2.3) {
			if ((ten - 1) < (modelPan[0] + 0.3) && (ten + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//11
		glm::mat4 model11(1.0f);
		model11 = glm::translate(model11, glm::vec3(oone, 3.0f - ((current - 9)*3.4), 0.0f));
		model11 = glm::scale(model11, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model11);
		ddong->Draw(shader);
		if (3.0f - ((current - 9)*3.4) > -2.8 && 3.0 - ((current - 9)*3.4) < -2.3) {
			if ((oone - 1) < (modelPan[0] + 0.3) && (oone + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//12
		glm::mat4 model12(1.0f);
		model12 = glm::translate(model12, glm::vec3(ttwo, 3.0f - ((current - 9)*1.5), 0.0f));
		model12 = glm::scale(model12, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model12);
		ddong->Draw(shader);
		if (3.0f - ((current - 9)*1.5) > -2.8 && 3.0 - ((current - 9)*1.5) < -2.3) {
			if ((ttwo - 1) < (modelPan[0] + 0.3) && (ttwo + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
	}
	if (current > 12) {
		//13
		glm::mat4 model13(1.0f);
		model13 = glm::translate(model13, glm::vec3(tthree, 3.0f - ((current - 12)*4.0), 0.0f));
		model13 = glm::scale(model13, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model13);
		ddong->Draw(shader);
		if (3.0f - ((current - 12)*4.0) > -2.8 && 3.0 - ((current - 12)*4.0) < -2.3) {
			if ((tthree - 1) < (modelPan[0] + 0.3) && (tthree + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//14
		glm::mat4 model14(1.0f);
		model14 = glm::translate(model14, glm::vec3(ffour, 3.0f - ((current - 12)*3.6), 0.0f));
		model14 = glm::scale(model14, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model14);
		ddong->Draw(shader);
		if (3.0f - ((current - 12)*3.6) > -2.8 && 3.0 - ((current - 12)*3.6) < -2.3) {
			if ((ffour - 1) < (modelPan[0] + 0.3) && (ffour + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//15
		glm::mat4 model10(1.0f);
		model10 = glm::translate(model10, glm::vec3(ffive, 3.0f - ((current - 12)*3.0), 0.0f));
		model10 = glm::scale(model10, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model10);
		ddong->Draw(shader);
		if (3.0f - ((current - 12)*3.0) > -2.8 && 3.0 - ((current - 12)*3.0) < -2.3) {
			if ((ffive - 1) < (modelPan[0] + 0.3) && (ffive + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
	}
	if (current > 15) {
		//16
		glm::mat4 model16(1.0f);
		model16 = glm::translate(model16, glm::vec3(ssix, 3.0f - ((current - 15)*2.3), 0.0f));
		model16 = glm::scale(model16, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model16);
		ddong->Draw(shader);
		if (3.0f - ((current - 15)*2.3) > -2.8 && 3.0 - ((current - 15)*2.3) < -2.3) {
			if ((ssix - 1) < (modelPan[0] + 0.3) && (ssix + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//17
		glm::mat4 model17(1.0f);
		model17 = glm::translate(model17, glm::vec3(sseven, 3.0f - ((current - 15)*4.6), 0.0f));
		model17 = glm::scale(model17, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model17);
		ddong->Draw(shader);
		if (3.0f - ((current - 15)*4.6) > -2.8 && 3.0 - ((current - 15)*4.6) < -2.3) {
			if ((sseven - 1) < (modelPan[0] + 0.3) && (sseven + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//18
		glm::mat4 model18(1.0f);
		model18 = glm::translate(model18, glm::vec3(eight, 3.0f - ((current - 15)*4.0), 0.0f));
		model18 = glm::scale(model18, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model18);
		ddong->Draw(shader);
		if (3.0f - ((current - 15)*4.0) > -2.8 && 3.0 - ((current - 15)*4.0) < -2.3) {
			if ((eight - 1) < (modelPan[0] + 0.3) && (eight + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//19
		glm::mat4 model19(1.0f);
		model19 = glm::translate(model19, glm::vec3(nnine, 3.0f - ((current - 15)*4.3), 0.0f));
		model19 = glm::scale(model19, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model19);
		ddong->Draw(shader);
		if (3.0f - ((current - 15)*4.3) > -2.8 && 3.0 - ((current - 15)*4.3) < -2.3) {
			if ((nnine - 1) < (modelPan[0] + 0.3) && (nnine + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
		//20
		glm::mat4 model20(1.0f);
		model20 = glm::translate(model20, glm::vec3(tten, 3.0f - ((current - 15)*7.0), 0.0f));
		model20 = glm::scale(model20, glm::vec3(0.5f, 0.5f, 0.5f));
		shader->setMat4("model", model20);
		ddong->Draw(shader);
		if (3.0f - ((current - 15)*5.0) > -2.8 && 3.0 - ((current - 15)*5.0) < -2.3) {
			if ((tten - 1) < (modelPan[0] + 0.3) && (tten + 1) > (modelPan[0] - 0.3)) {
				dead = true;
			}
		}
	}

}

GLFWwindow *glAllInit()
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "Model Loading", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        exit(-1);
    }
    
    glfwMakeContextCurrent( window );
    
    // Set the required callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCR_WIDTH, SCR_HEIGHT );
    
    // OpenGL options
    glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
    glEnable( GL_DEPTH_TEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->use();
    shader->setMat4("projection", projection); 
}
unsigned int loadTexture(string texFileName) {
	unsigned int texture;

	// Create texture ids.
	glGenTextures(1, &texture);

	// All upcomming GL_TEXTURE_2D operations now on "texture" object
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture parameters for wrapping.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);   // vertical flip the texture
	unsigned char *image = stbi_load(texFileName.c_str(), &width, &height, &nrChannels, 0);
	if (!image) {
		cout << "texture " << texFileName << " loading error" << endl;
	}
	else cout << "texture " << texFileName << " loaded" << endl;

	GLenum format;
	if (nrChannels == 1) format = GL_RED;
	else if (nrChannels == 3) format = GL_RGB;
	else if (nrChannels == 4) format = GL_RGBA;
	else {
		cout << "loadTexture in main.cpp error ... illegal nrChannels: " << nrChannels << endl;
		exit(-1);
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        cameraPos = cameraOrigPos;
        modelPan[0] = modelPan[1] = modelPan[2] = 0.0f;
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        arcballCamRot = !arcballCamRot;
        if (arcballCamRot) {
            cout << "ARCBALL: Camera rotation mode" << endl;
        }
        else {
            cout << "ARCBALL: Model  rotation mode" << endl;
        }
    }
    else if (key == GLFW_KEY_LEFT) {
		if (modelPan[0] > -4) {
			modelPan[0] -= 0.3;
			dir = lleft;
		}
    }
    else if (key == GLFW_KEY_RIGHT) {
		if (modelPan[0] < 4) {
			modelPan[0] += 0.3;
			dir = rright;
		}
    }
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (renderMode == INIT) {
			renderMode = ANIM;
			beginT = glfwGetTime();
		}
		else if (renderMode == STOP) {
			if (timeT == animEndTime) renderMode = INIT;
			else {
				renderMode = ANIM;
			}
		}
		else if (renderMode == ANIM) renderMode = STOP;

		if (renderMode == INIT) {
			timeT = 0.0;
			updateAnimData();
		}
	}
	else if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		if (renderMode == INIT) {
			renderMode = ANIM;
			beginT = glfwGetTime();
		}
		else {  // renderMode == ANIM
			renderMode = INIT;
			timeT = 0.0f;
		}
	}
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (arcballCamRot)
        camArcBall.mouseButtonCallback( window, button, action, mods );
    else
        modelArcBall.mouseButtonCallback( window, button, action, mods );
}

void cursor_position_callback(GLFWwindow *window, double x, double y) {
    if (arcballCamRot)
        camArcBall.cursorCallback( window, x, y );
    else
        modelArcBall.cursorCallback( window, x, y );
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    cameraPos[2] -= (yoffset * 0.5);
}
