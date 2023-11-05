
#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif
#include <GLFW/glfw3.h>

#include "arcball.h"
#include "scene.h"
#include "state.h"
#include "camera3d.h"
#include "material.h"
#include "texture.h"
#include "transform.h"
#include "cube.h"
#include "quad.h"
#include "sphere.h"
#include "error.h"
#include "shader.h"
#include "light.h"
#include "light.h"
#include "polyoffset.h"
#include "skybox.h"
#include "texcube.h"

#include <iostream>

static float viewer_pos[3] = {2.0f, 3.5f, 5.0f};

static ScenePtr scene;
static Camera3DPtr camera;
static ArcballPtr arcball;

static void initialize (void)
{
  // set background color: white 
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);  // cull back faces

  // create objects
  camera = Camera3D::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  //camera->SetOrtho(true);
  arcball = camera->CreateArcball();

  //LightPtr light = ObjLight::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  LightPtr light = Light::Make(0.0f,0.0f,0.0f,1.0f,"camera");

  SkyBoxPtr skybox = SkyBox::Make();

  AppearancePtr white = Material::Make(1.0f,1.0f,1.0f);
  AppearancePtr red = Material::Make(1.0f,0.5f,0.5f);
  AppearancePtr poff = PolygonOffset::Make(-1,-1);
  AppearancePtr paper = Texture::Make("decal","../../images/paper.jpg");

  TransformPtr trf1 = Transform::Make();
  trf1->Scale(3.0f,0.3f,3.0f);
  trf1->Translate(0.0f,-1.0f,0.0f);
  TransformPtr trf2 = Transform::Make();
  trf2->Scale(0.5f,0.5f,0.5f);
  trf2->Translate(0.0f,1.0f,0.0f);
  TransformPtr trf3 = Transform::Make();
  trf3->Translate(0.8f,0.0f,0.8f);
  trf3->Rotate(30.0f,0.0f,1.0f,0.0f);
  trf3->Rotate(90.0f,-1.0f,0.0f,0.0f);
  trf3->Scale(0.5f,0.7f,1.0f);

  TransformPtr trf_leg1 = Transform::Make();
  trf_leg1->Scale(0.2f, 2.0f, 0.2f);
  trf_leg1->Translate(-7.0f, -1.0f, -7.0f);

  TransformPtr trf_leg2 = Transform::Make();
  trf_leg2->Scale(0.2f, 2.0f, 0.2f);
  trf_leg2->Translate(-7.0f, -1.0f, 7.0f);

  TransformPtr trf_leg3 = Transform::Make();
  trf_leg3->Scale(0.2f, 2.0f, 0.2f);
  trf_leg3->Translate(7.0f, -1.0f, -7.0f);

  TransformPtr trf_leg4 = Transform::Make();
  trf_leg4->Scale(0.2f, 2.0f, 0.2f);
  trf_leg4->Translate(7.0f, -1.0f, 7.0f);

  Error::Check("before shps");
  ShapePtr cube = Cube::Make();
  Error::Check("before quad");
  ShapePtr quad = Quad::Make();
  Error::Check("before sphere");
  ShapePtr sphere = Sphere::Make();
  Error::Check("after shps");
  // ShapePtr legShape1 = Cube::Make();
  // Error::Check("before quad");
  // ShapePtr legShape2 = Cube::Make();
  // Error::Check("before quad");
  // ShapePtr legShape3 = Cube::Make();
  // Error::Check("before quad");
  // ShapePtr legShape4 = Cube::Make();
  // Error::Check("before quad");
  AppearancePtr sky_tex = TexCube::Make("sky","../../images/skybox2.png");

  // create shader
  //ShaderPtr shader = Shader::Make();
  //shader->AttachVertexShader("shader/ilum_vert/vertex.glsl");
  //shader->AttachFragmentShader("shader/ilum_vert/fragment.glsl");
  //shader->Link();
  ShaderPtr shader = Shader::Make(light,"world");
  shader->AttachVertexShader("../../shaders/sky_reflect/vertex.glsl");
  shader->AttachFragmentShader("../../shaders/sky_reflect/fragment.glsl");
  shader->Link();

  ShaderPtr shd_tex = Shader::Make(light,"world");
  shd_tex->AttachVertexShader("../../shaders/texture/vertex.glsl");
  shd_tex->AttachFragmentShader("../../shaders/texture/fragment.glsl");
  shd_tex->Link();

  ShaderPtr shd_sky = Shader::Make();
  shd_sky->AttachVertexShader("../../shaders/skybox/vertex.glsl");
  shd_sky->AttachFragmentShader("../../shaders/skybox/fragment.glsl");
  shd_sky->Link();

  NodePtr desk = Node::Make(shader,
  {Node::Make(trf1,{cube}),
     Node::Make(trf_leg1,{cube}),
     Node::Make(trf_leg2,{cube}),
     Node::Make(trf_leg3,{cube}),
    Node::Make(trf_leg4,{cube}),
  }
  );

  // build scene
  NodePtr root = Node::Make(shader,
    // {Node::Make(shd_tex,trf1,{desk},{cube}),
    {Node::Make(shd_sky,{sky_tex},{skybox}),
     desk,
     Node::Make(shader,trf3,{white,poff,paper},{quad}),
     Node::Make(shader,trf2,{sphere}),
    }
  );
  scene = Scene::Make(root);
}

static void display (GLFWwindow* win)
{ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
  Error::Check("before render");
  scene->Render(camera);
  Error::Check("after render");
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
}

static void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

static void cursorpos (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->AccumulateMouseMotion(x,y);
}
static void cursorinit (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->InitMouseMotion(x,y);
  glfwSetCursorPosCallback(win, cursorpos);     // cursor position callback
}
static void mousebutton (GLFWwindow* win, int button, int action, int mods)
{
  if (action == GLFW_PRESS) {
    glfwSetCursorPosCallback(win, cursorinit);     // cursor position callback
  }
  else // GLFW_RELEASE 
    glfwSetCursorPosCallback(win, nullptr);      // callback disabled
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(600,400,"Window title",nullptr,nullptr);
  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback
  glfwSetMouseButtonCallback(win, mousebutton); // mouse button callback
  
  glfwMakeContextCurrent(win);

#ifdef __glad_h_
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD OpenGL context\n");
    exit(1);
   }
#endif
#ifdef __glew_h__
  glewInit(); 
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW OpenGL context\n");
    exit(1);
  }
#endif

  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  while(!glfwWindowShouldClose(win)) {
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

