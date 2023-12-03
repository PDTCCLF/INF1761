
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
#include "variable.h"
#include "clipplane.h"
#include "luxor/luxor.h"
#include "luxor/interpolator.h"
#include "luxor/cubicinterpolator.h"
#include "luxor/linearinterpolator.h"
#include "luxor/movement.h"
#include "luxor/luxorengine.h"
#include "luxor/animation.h"


static float viewer_pos[3] = {2.0f, 3.5f, 5.0f};

static ScenePtr scene;
static ScenePtr reflector;
static Camera3DPtr camera;
static ArcballPtr arcball;
static LuxorEnginePtr engine;

static void initialize (void)
{
    

  // set background color: white 
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);  // cull back faces
  glEnable(GL_CLIP_DISTANCE0);
  camera = Camera3D::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  //camera->SetOrtho(true);
  arcball = camera->CreateArcball();
  
  /*Variables - fog*/
  

  //LightPtr light = ObjLight::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  LightPtr light = Light::Make(0.0f,0.0f,0.0f,1.0f,"camera");

  SkyBoxPtr skybox = SkyBox::Make();

  LuxorPtr luxor = Luxor::Make();
  engine = luxor->GetEngine();
  

  ClipPlanePtr clipplane = ClipPlane::Make("clipplane", 0.0f, -1.0f, 0.0f, 0.0f);
  ClipPlanePtr clipplane2 = ClipPlane::Make("clipplane", 0.0f, -1.0f, 0.0f, 0.0f);
  
  AppearancePtr white = Material::Make(1.0f,1.0f,1.0f);
  AppearancePtr blue = Material::Make(0.0f,0.0f,1.0f);
  AppearancePtr poff = PolygonOffset::Make(-1,-1);
  AppearancePtr paper = Texture::Make("decal","../images/paper.jpg");
  AppearancePtr sphereTex = Texture::Make("decal", "../images/earth.jpg");
  AppearancePtr deskText = Texture::Make("decal", "../images/wood.jpg");


  std::vector<std::string> faces = {
    "../images/skyboxImages/right.jpg",
    "../images/skyboxImages/left.jpg",
    "../images/skyboxImages/top.jpg",
    "../images/skyboxImages/bottom.jpg",
    "../images/skyboxImages/front.jpg",
    "../images/skyboxImages/back.jpg"
};
  // AppearancePtr sky_tex = TexCube::Make("sky", "../images/skybox.jpg");
  AppearancePtr sky_tex = TexCube::Make("sky",faces);
  TransformPtr trf1 = Transform::Make();
  trf1->Scale(0.6f,0.6f,0.6f);
  trf1->Translate(2.0f,2.0f,0.0f);
  TransformPtr trf2 = Transform::Make();
  trf2->Scale(0.6f,0.6f,0.6f);
  trf2->Translate(-2.0f,2.0f,0.0f);
  //trf2->Translate(0.0f, -0.5f, -1.0f);
  TransformPtr trf3 = Transform::Make();
  trf3->Translate(0.8f,0.0f,0.8f);
  trf3->Rotate(30.0f,0.0f,1.0f,0.0f);
  trf3->Rotate(90.0f,-1.0f,0.0f,0.0f);
  trf3->Scale(0.5f,0.7f,1.0f);
  TransformPtr trf4 = Transform::Make();
  trf4->Scale(0.03f,0.03f,0.03f);
  trf4->Translate(-4.0f,0.0f,0.0f);

  TransformPtr trf_floor = Transform::Make();
  trf_floor->Scale(8.0f,0.3f,3.0f);
  trf_floor->Translate(0.0f,-1.0f,0.0f);

  TransformPtr trf_desk = Transform::Make();
  trf_desk->Scale(3.0f,0.3f,3.0f);
  trf_desk->Translate(0.0f,-1.0f,0.0f);

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
  std::vector<TransformPtr> trf_spheres;


  ShaderPtr shd = Shader::Make(light,"world");
  shd->AttachVertexShader("../shaders/default/vertex.glsl");
  shd->AttachFragmentShader("../shaders/default/fragment.glsl");
  shd->Link();
  

  ShaderPtr shd_clip = Shader::Make(light,"world");
  shd_clip->AttachVertexShader("../shaders/clipplane/vertex.glsl");
  shd_clip->AttachFragmentShader("../shaders/clipplane/fragment.glsl");
  shd_clip->Link();


  ShaderPtr shd_reflect = Shader::Make(light, "world");
  shd_reflect->AttachVertexShader("../shaders/sky_reflect/vertex.glsl");
  shd_reflect->AttachFragmentShader("../shaders/sky_reflect/fragment.glsl");
  shd_reflect->Link();
  auto fcolor = Variable<glm::vec4>::Make("fcolor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  auto fdensity = Variable<float>::Make("fdensity", 0.15f);


  ShaderPtr shd_tex = Shader::Make(light, "world");
  shd_tex->AttachVertexShader("../shaders/texture/vertex.glsl");
  shd_tex->AttachFragmentShader("../shaders/texture/fragment.glsl");
  shd_tex->Link();


  ShaderPtr scene_reflect = Shader::Make(light, "world");
  scene_reflect->AttachVertexShader("../shaders/scene_reflect/vertex.glsl");
  scene_reflect->AttachFragmentShader("../shaders/scene_reflect/fragment.glsl");
  scene_reflect->Link();


  NodePtr desk = Node::Make(shd_clip, {deskText},
  {Node::Make(trf_desk,{cube}),
     Node::Make(trf_leg1,{cube}),
     Node::Make(trf_leg2,{cube}),
     Node::Make(trf_leg3,{cube}),
     Node::Make(trf_leg4,{cube}),
  });

  NodePtr root = Node::Make(
      shd,
      {fdensity,fcolor},
      {     
           
          Node::Make({sky_tex},{skybox}),
          // desk,
          Node::Make({shd_tex},trf4,{luxor->GetNode()}),
          Node::Make({shd_clip},trf1,{clipplane,white,sphereTex},{sphere}),
          Node::Make({shd_reflect},trf2,{sphere}),
  
      }
  );
 
  
  scene = Scene::Make({ root });
  reflector = Scene::Make(Node::Make({scene_reflect},trf_floor,{clipplane},{cube}));
}

static void display (GLFWwindow* win)
{ 
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
  Error::Check("before render");
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NEVER,1,0xFFFF);
  glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);
  reflector->Render(camera);
  glStencilFunc(GL_EQUAL,1,0xFFFF);
  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
  NodePtr root=scene->GetRoot();
  TransformPtr trf=Transform::Make();
  trf->Scale(1.0f,-1.0f,1.0f);
  root->SetTransform(trf);
  glFrontFace(GL_CW);
  scene->Render(camera);
  glFrontFace(GL_CCW);
  root->SetTransform(nullptr);
  glDisable(GL_STENCIL_TEST);
  scene->Render(camera);
  Error::Check("after render");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  reflector->Render(camera);
  glDisable(GL_BLEND);
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
  else if(key==GLFW_KEY_UP && action==GLFW_PRESS){
    engine->StandUp();
    engine->Update(1.0f);
  }
  else if(key==GLFW_KEY_DOWN && action==GLFW_PRESS){
    engine->StandDown();
    engine->Update(1.0f);
  }
  else if(key==GLFW_KEY_RIGHT && action==GLFW_PRESS){
      engine->StandDown();
      engine->JumpForward();
      engine->Update(2.0f);
  }
  else if(key==GLFW_KEY_LEFT && action==GLFW_PRESS){
      engine->StandDown();
      engine->JumpBackward();
      engine->Update(2.0f);
  }
  else if(key==GLFW_KEY_RIGHT && action==GLFW_REPEAT){
      if(mods&GLFW_MOD_SHIFT)
        engine->TurnHead(-5.0f);
      else{
        engine->StandDown();
        engine->JumpForward();
        engine->Update(2.0f);
      }
  }
  else if(key==GLFW_KEY_LEFT && action==GLFW_REPEAT){
      if(mods&GLFW_MOD_SHIFT)
        engine->TurnHead(5.0f);
      else{
        engine->StandDown();
        engine->JumpBackward();
        engine->Update(2.0f);
      }
  }
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
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);       // required for mac os
  //glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER,GLFW_TRUE);  // option for mac os
  
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

