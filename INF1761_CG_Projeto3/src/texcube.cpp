#include "texcube.h"
#include "image.h"
#include "state.h"

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cstring>

TexCubePtr TexCube::Make (const std::string& varname, const std::string& filename)
{
  return TexCubePtr(new TexCube(varname,filename));
}

TexCubePtr TexCube::Make(const std::string& varname, std::vector<std::string> faces)
{
    return TexCubePtr(new TexCube(varname,faces));
}

TexCube::TexCube (const std::string& varname, const std::string& filename)
{
  ImagePtr img = Image::Make(filename,true);

  glGenTextures(1,&m_tex);
  glBindTexture(GL_TEXTURE_CUBE_MAP,m_tex);

  // subimages' dimension
  int w = img->GetWidth() / 4;
  int h = img->GetHeight() / 3;
  int x[] = {2*w,  0,  w,  w,  w,3*w};
  int y[] = {  h,  h,2*h,  0,  h,  h};
  GLenum face[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,  // right
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,  // left
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,  // top
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,  // bottom
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,  // front
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,  // back
  };
  unsigned char* subimg = new unsigned char[w*h*img->GetNChannels()];
  for (int i=0; i<6; ++i) {
    img->ExtractSubimage(x[i],y[i],w,h,subimg);
    glTexImage2D(face[i],0,GL_RGB,w,h,0,
                 img->GetNChannels()==3?GL_RGB:GL_RGBA,
                 GL_UNSIGNED_BYTE,subimg);
  }
  delete [] subimg;
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);	
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);	
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);	
}
TexCube::TexCube(const std::string& varname, std::vector<std::string> faces)
{
   

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        ImagePtr img = Image::Make(faces[i], false);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img->GetWidth(), img->GetHeight(), 0, img->GetNChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, img->GetData());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    
}


TexCube::~TexCube ()
{
}

unsigned int TexCube::GetTexId () const
{
  return m_tex;
}

void TexCube::Load (StatePtr st)
{
  ShaderPtr shd = st->GetShader();
  shd->ActiveTexture(m_varname.c_str());
  glBindTexture(GL_TEXTURE_CUBE_MAP,m_tex);
}

void TexCube::Unload (StatePtr st)
{
  ShaderPtr shd = st->GetShader();
  shd->DeactiveTexture();
}