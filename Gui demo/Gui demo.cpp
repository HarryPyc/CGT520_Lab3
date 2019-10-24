#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp> //This header defines basic glm types (vec3, mat4, etc)

//These headers define matrix transformations
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp> //This header defines helper functions (e.g. value_ptr(...)) for passing vectors and matrices to shaders

#include "InitShader.h"    //Functions for loading shaders from text files
#include "LoadMesh.h"      //Functions for creating OpenGL buffers from mesh files
#include "LoadTexture.h"   //Functions for creating OpenGL textures from image files
#include "imgui_impl_glut.h" //Functions for using imgui UI in glut programs

static const std::string vertex_shader("gui_demo_vs.glsl");
static const std::string fragment_shader("gui_demo_fs.glsl");

GLuint shader_program = -1;
GLuint texture_id = -1; //Texture map for fish

static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name = "AmagoT.bmp";
MeshData mesh_data;

float angle = 0.0f;
float scale = 1.0f;
float lightPos[3] = { 0.0f,0.0f,1.0f };
float Ka[3] = { 0.25,0.25,0.25 };
float Kd[3] = { 0.25,0.25,0.25 };
float Ks[3] = { 1,1,1 };
float La = 1.0f, Ld = 1.0f, Ls = 1.0f;
float shiness = 32.0f;

bool useTexture = true;
//Draw the ImGui user interface
void draw_gui()
{
   ImGui_ImplGlut_NewFrame();

   ImGui::Begin("Debug menu");
   //uncomment the following line to create a sliders which changes the viewing angle and scale
   //ImGui::SliderFloat("View angle", &angle, -180.0f, +180.0f);
   //ImGui::SliderFloat("Scale", &scale, -10.0f, +10.0f);
   ImGui::End();

   ImGui::Begin("Light");
   ImGui::SliderFloat3("Point Light Pos", lightPos, -2.5f, 2.5f);
   ImGui::ColorEdit3("Ka", Ka);
   ImGui::ColorEdit3("Kd", Kd);
   ImGui::ColorEdit3("Ks", Ks);
   ImGui::SliderFloat("La", &La, 0.0f, 5.0f);
   ImGui::SliderFloat("Ld", &Ld, 0.0f, 5.0f);
   ImGui::SliderFloat("Ls", &Ls, 0.0f, 5.0f);
   ImGui::SliderFloat("Shiness", &shiness, 0.0f, 100.0f);
   ImGui::Checkbox("UseTexture", &useTexture);
   ImGui::End();

   static bool show_test = false;
   ImGui::ShowTestWindow(&show_test);

   ImGui::Render();
 }

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
   
   glm::mat4 M = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))*glm::scale(glm::vec3(scale*mesh_data.mScaleFactor));
   glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
   

   glUseProgram(shader_program);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   int PVM_loc = glGetUniformLocation(shader_program, "PVM");
   if (PVM_loc != -1)
   {
      glm::mat4 PVM = P*V*M;
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }

   int P_loc = glGetUniformLocation(shader_program, "P");
   if (P_loc != -1)
   {
      glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
   }

   int V_loc = glGetUniformLocation(shader_program, "V");
   if (V_loc != -1)
   {
      glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
   }
   int M_loc = glGetUniformLocation(shader_program, "M");
   if (M_loc != -1) {
	   glUniformMatrix4fv(M_loc, 1,false, glm::value_ptr(M));
   }

   int tex_loc = glGetUniformLocation(shader_program, "diffuse_tex");
   if (tex_loc != -1)
   {
      glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
   }
   int lightPos_loc = glGetUniformLocation(shader_program, "lightpos");
   if (lightPos_loc != -1) {
	   
	   glUniform3fv(lightPos_loc, 1, lightPos);
   }
   int Ka_loc = glGetUniformLocation(shader_program, "Ka");
   if (Ka_loc != -1) {
	   glUniform3fv(Ka_loc, 1, Ka);
   }
   int Ks_loc = glGetUniformLocation(shader_program, "Ks");
   if (Ks_loc != -1) {
	   glUniform3fv(Ks_loc, 1, Ks);
   }
   int Kd_loc = glGetUniformLocation(shader_program, "Kd");
   if (Kd_loc != -1) {
	   glUniform3fv(Kd_loc, 1, Kd);
   }

   int La_loc = glGetUniformLocation(shader_program, "La");
   if (La_loc != -1) {
	   glUniform1f(La_loc, La);
   }
   int Ld_loc = glGetUniformLocation(shader_program, "Ld");
   if (Ld_loc != -1) {
	   glUniform1f(Ld_loc, Ld);
   }
   int Ls_loc = glGetUniformLocation(shader_program, "Ls");
   if (Ls_loc != -1) {
	   glUniform1f(Ls_loc, Ls);
   }
   int shiness_loc = glGetUniformLocation(shader_program, "shiness");
   if (shiness_loc != -1) {
	   glUniform1f(shiness_loc, shiness);
   }
   int useTexture_loc = glGetUniformLocation(shader_program, "useTexture");
   if (useTexture_loc != -1) {
	   glUniform1f(useTexture_loc, useTexture);
   }
   int eye_loc = glGetUniformLocation(shader_program, "eye");
   if (eye_loc != -1) {
	   glm::vec3 eye = glm::vec3(0.0f, 1.0f, 2.0f);
	   glUniform3fv(eye_loc, 1, glm::value_ptr(eye));
   }
   glBindVertexArray(mesh_data.mVao);
   glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
   //For meshes with multiple submeshes use mesh_data.DrawMesh(); 

   draw_gui();

   glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;

   //Pass time_sec value to the shaders
   int time_loc = glGetUniformLocation(shader_program, "time");
   if (time_loc != -1)
   {
      glUniform1f(time_loc, time_sec);
   }
}

void reload_shader()
{
   GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

   if (new_shader == -1) // loading failed
   {
      glClearColor(1.0f, 0.0f, 1.0f, 0.0f); //change clear color if shader can't be compiled
   }
   else
   {
      glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

      if (shader_program != -1)
      {
         glDeleteProgram(shader_program);
      }
      shader_program = new_shader;
   }
}

void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl()
{
   glewInit();

   glEnable(GL_DEPTH_TEST);

   reload_shader();

   //mesh and texture to be rendered
   mesh_data = LoadMesh(mesh_name);
   texture_id = LoadTexture(texture_name);
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);

   switch(key)
   {
      case 'r':
      case 'R':
         reload_shader();     
      break;
   }
}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
   ImGui_ImplGlut_PassiveMouseMotionCallback(x,y);
}

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
}


int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (640, 640);
   int win = glutCreateWindow ("Yucong Pan");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutIdleFunc(idle);

   initOpenGl();
   ImGui_ImplGlut_Init(); // initialize the imgui system

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}