#include <GLUT/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <math.h>

GLdouble view_x = -2.0;
GLdouble view_y = -2.0;
GLdouble view_w = 4.0;
int buttonState = 0;
GLdouble new_x = 0.0;
GLdouble new_y = 0.0;
GLdouble new_x2 = 0.0;
GLdouble new_y2 = 0.0;
int w = 700;
int h = 700;
GLuint shaderProgram;

void checkCompile(GLuint shader)
{
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint logSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        char* log = new char[logSize];
        glGetShaderInfoLog(shader, logSize, &logSize, log);
        printf("Compile error: %s\n", log);
        delete []log;
    }
}

std::string loadSource(const char* name)
{
  std::ifstream i(name);
  std::stringstream buffer;
  buffer << i.rdbuf() << '\0';
  return buffer.str();
}

void init()
{
  int logLen = 1000;
  GLchar log[1000];

  printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glClearColor(1.0, 0.5, 1.0, 1.0);

  std::string vertSrcString = loadSource("shader.vert");
  std::string fragSrcString = loadSource("shader.frag");
  const char* vertSrc = vertSrcString.c_str();
  const char* fragSrc = fragSrcString.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertSrc, NULL);
  glCompileShader(vertexShader);
  checkCompile(vertexShader);

  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragSrc, NULL);
  glCompileShader(fragShader);
  checkCompile(fragShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
  if(logLen > 0) {
      // Show any errors as appropriate
      glGetProgramInfoLog(shaderProgram, logLen, &logLen, log);
      fprintf(stderr, "ShaderProgram Info Log: %s\n", log);
  }
  glAttachShader(shaderProgram, fragShader);
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
  if(logLen > 0) {
      // Show any errors as appropriate
      glGetProgramInfoLog(shaderProgram, logLen, &logLen, log);
      fprintf(stderr, "ShaderProgram Info Log: %s\n", log);
  }
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
  if(logLen > 0) {
      // Show any errors as appropriate
      glGetProgramInfoLog(shaderProgram, logLen, &logLen, log);
      fprintf(stderr, "ShaderProgram Info Log: %s\n", log);
  }
  glValidateProgram(shaderProgram);


  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
  if(logLen > 0) {
      // Show any errors as appropriate
      glGetProgramInfoLog(shaderProgram, logLen, &logLen, log);
      fprintf(stderr, "ShaderProgram Info Log: %s\n", log);
  }
  glUseProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
  if(logLen > 0) {
      // Show any errors as appropriate
      glGetProgramInfoLog(shaderProgram, logLen, &logLen, log);
      fprintf(stderr, "ShaderProgram Info Log: %s\n", log);
  }


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(view_x, view_x+view_w, view_y, view_y+view_w);
  glMatrixMode(GL_MODELVIEW);
}

void display()
{
    //glColor3f(1.0,0.5,0.5);
  glBegin(GL_QUADS);
  glVertex2d(view_x, view_y);
  glVertex2d(view_x+view_w, view_y);
  glVertex2d(view_x+view_w, view_y+view_w);
  glVertex2d(view_x, view_y+view_w);
  glEnd();

  if(buttonState == 1) {
      glUseProgram(0);
      glBegin(GL_LINE_LOOP);      
      glColor3f(1.0,0.0,0.0);
      glVertex2d(new_x, new_y);
      glVertex2d(new_x2, new_y);
      glVertex2d(new_x2, new_y2);
      glVertex2d(new_x, new_y2);
      glEnd();
      glUseProgram(shaderProgram);
  }

  glFlush();
}

void mouse(int button, int state, int x, int y) 
{
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            new_x = (GLdouble)x/(GLdouble)w*view_w + view_x;
            new_y = (GLdouble)(h-y)/(GLdouble)h*view_w + view_y;
            new_x2 = new_x;
            new_y2 = new_y;
            buttonState = 1;
            printf("Button Down! (%f,%f)\n", new_x, new_y);
        }
        if(state == GLUT_UP) {

            GLdouble second_x = (GLdouble)x/(GLdouble)w*view_w + view_x;
            GLdouble second_y = (GLdouble)(h-y)/(GLdouble)h*view_w + view_y;
            view_x = second_x < new_x ? second_x : new_x;
            view_y = second_y < new_y ? second_y : new_y;
            view_w = (GLdouble)fabs(second_x - new_x);
            buttonState = 0;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(view_x, view_x+view_w, view_y, view_y+view_w);
            glMatrixMode(GL_MODELVIEW);
            glutPostRedisplay();
            printf("New view! (%f,%f,%f,%f)\n", view_x, view_y, view_w, view_w);
        }
    }
}

void motion(int x, int y)
{
    if(buttonState == 1) {
        new_x2 = (GLdouble)x/(GLdouble)w*view_w + view_x;
        new_y2 = (GLdouble)(h-y)/(GLdouble)h*view_w + view_y;
        glutPostRedisplay();
    }
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(w,h);
  glutCreateWindow("Mandelbrot");
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;
}
