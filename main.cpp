//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h" //tiene que ser obj
#include <FreeImage.h> //*** Para Textura: Incluir librería
#define NT 2
#define NO 10

//-----------------------------------------------------------------------------



class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   cwc::glShader* shader1; //Para Textura: variable para abrir los shader de textura
   GLuint ProgramObject;
   clock_t time0,time1;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.

   GLMmodel* objmodel_ptr;	// este objeto es para la malla de la tierra
   GLMmodel* objmodel_ptr1; // este objeto es para la malla del rio
   GLMmodel* objmodel_ptr2;	// este objeto es para la malla de los cimientos de la casa
   GLMmodel* objmodel_ptr3; // este objeto es para la malla de las palmeras :)
   GLMmodel* objmodel_ptr4; // este objeto es para la malla del piso de la casa
   GLMmodel* objmodel_ptr5; // este objeto es para la malla de la fachada de la casa
   GLMmodel* objmodel_ptr6; // este objeto es para la malla del techo de la casa
   GLMmodel* objmodel_ptr7; // este objeto es para la malla del piso dentro de la casa
   GLMmodel* objmodel_ptr8; // este objeto es para la malla de las ventanas de la casa
   GLMmodel* objmodel_texture; //*** Para Textura: variable para objeto texturizado
   GLuint texid[NT]; //*** Para Textura: variable que almacena el identificador de textura
   GLMmodel* modelo[NO];


public:
	myWindow(){}

	//*** Para Textura: aqui adiciono un método que abre la textura en JPG
	void initialize_textures(int nt, char* nombre)
	{
		int w, h;
		GLubyte* data = 0;
		//data = glmReadPPM("soccer_ball_diffuse.ppm", &w, &h);
		//std::cout << "Read soccer_ball_diffuse.ppm, width = " << w << ", height = " << h << std::endl;

		//dib1 = loadImage("soccer_ball_diffuse.jpg"); //FreeImage

		glGenTextures(1, &texid[nt]);
		glBindTexture(GL_TEXTURE_2D, texid[nt]);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Loading JPG file
		FIBITMAP* bitmap = FreeImage_Load(
			FreeImage_GetFileType(nombre, 0),
			nombre);  //*** Para Textura: esta es la ruta en donde se encuentra la textura

		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);
		//
		glEnable(GL_TEXTURE_2D);
	}

	void openMesh(GLMmodel** ptr, float n, char* file) {
		if (!(*ptr))
		{
			*ptr = glmReadOBJ(file);
			if (!(*ptr))
				exit(0);
			glmUnitize(*ptr);
			glmFacetNormals(*ptr);
			glmVertexNormals(*ptr, n);
		}
	}

	void openShaderTexScaled(float xt, float yt, float zt, float xs, float ys, float zs, GLuint t, GLMmodel ** ptr) {
		glPushMatrix();
		glTranslatef(xt, yt, zt);
		glBindTexture(GL_TEXTURE_2D, t);
		glScalef(xs, ys, zs);			// reducira toda la imagen a un % con respecto a la original la escala de este objeto "esta demasiado grande". (el tamaño actual se toma como si fuera 100% = 1.0 y por ejemplo si le ponemos 10% sera un 0.1 de ese tamaño original)
		glmDraw(*ptr, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		glPopMatrix();
	}

	void openShaderNotTex(float xt, float yt, float zt, GLfloat rA, GLfloat xr, GLfloat yr, GLfloat zr, GLMmodel** ptr) {
		glPushMatrix();
		glTranslatef(xt, yt, zt);
		glRotatef(rA, xr, yr, zr);
		glmDraw(*ptr, GLM_SMOOTH | GLM_MATERIAL);
		glPopMatrix();
	}


	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      //timer010 = 0.09; //for screenshot!

	  glPushMatrix();
	  glTranslatef(0.0, -0.3, 1.8);
	  glRotatef(0, 0.0f, 0.0f, 0.0f);

      if (shader) shader->begin();
		  
	      openShaderNotTex(0.0f, -0.1500001f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, &objmodel_ptr);
		  /*glPushMatrix();
		  glTranslatef(0.0f, -0.1500001f, 0.0f);
		  glmDraw(objmodel_ptr, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();*/


		  openShaderNotTex(-0.0f, -0.118f, 0.700005f, 180, 0.0f, 0.0f, 0.0f, &objmodel_ptr1);
		  /*glPushMatrix();
		  glTranslatef(-0.0f, -0.118f, 0.700005f);
		  glRotatef(180, 0.0f, 0.0f, 0.0f);
		  glmDraw(objmodel_ptr1, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();*/

		  /*glPushMatrix();
		  glTranslatef(0.05f, -0.05f, -0.08f);
		  glScalef(0.08f, 0.08f, 0.08f);			// reducira toda la imagen a un 1% con respecto a la original la escala de este objeto "esta demasiado grande". (el tamaño actual se toma como si fuera 100% y por ejemplo si le ponemos 10% sera un 10% de ese tamaño original)
		  glmDraw(objmodel_ptr2, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();*/

		  openShaderNotTex(0.0f, 0.08f, -0.4f, 0.0f, 0.0f, 0.0f, 0.0f, &objmodel_ptr3);
		  /*glPushMatrix();
		  glTranslatef(0.0f, 0.08f, -0.4f);
		  glmDraw(objmodel_ptr3, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();*/
		  

		  /*
		  glPushMatrix();
		  glTranslatef(0.0f, 0.08f, -0.4f);
		  glScalef(0.25f, 0.25f, 0.25f);
		  glmDraw(objmodel_ptr7, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();*/


	      //glutSolidTeapot(1.0);
      if (shader) shader->end();

	  //*** Para Textura: llamado al shader para objetos texturizados
	  if (shader1) shader1->begin();
		  openShaderTexScaled(0.09f, -0.05f, 0.05f, 0.25f, 0.25f, 0.25f, texid[0], &objmodel_ptr2);

		  /*glPushMatrix();
		  glTranslatef(0.09f, -0.05f, 0.05f);
		  glBindTexture(GL_TEXTURE_2D, texid[0]);
		  glScalef(0.25f, 0.25f, 0.25f);			// reducira toda la imagen a un 1% con respecto a la original la escala de este objeto "esta demasiado grande". (el tamaño actual se toma como si fuera 100% y por ejemplo si le ponemos 10% sera un 10% de ese tamaño original)
		  glmDraw(objmodel_ptr2, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();*/

		  openShaderTexScaled(0.111f, 0.0f, 0.046f, 0.25f, 0.25f, 0.25f, texid[1], &objmodel_ptr4);

		  /*glPushMatrix();
		  glTranslatef(0.111f, 0.0f, 0.046f);
		  glBindTexture(GL_TEXTURE_2D, texid[1]);
		  glScalef(0.25f, 0.25f, 0.25f);			// reducira toda la imagen a un 1% con respecto a la original la escala de este objeto "esta demasiado grande". (el tamaño actual se toma como si fuera 100% y por ejemplo si le ponemos 10% sera un 10% de ese tamaño original)
		  glmDraw(objmodel_ptr4, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();*/


		  openShaderTexScaled(0.0308f, 0.062f, -0.084f, 0.1f, 0.1f, 0.1f, texid[2], &objmodel_ptr5);
		  /*glPushMatrix();
		  glTranslatef(0.0308f, 0.062f, -0.084f);
		  glBindTexture(GL_TEXTURE_2D, texid[2]);
		  glScalef(0.1f, 0.1f, 0.1f);			// reducira toda la imagen a un 1% con respecto a la original la escala de este objeto "esta demasiado grande". (el tamaño actual se toma como si fuera 100% y por ejemplo si le ponemos 10% sera un 10% de ese tamaño original)
		  glmDraw(objmodel_ptr5, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();*/

		  openShaderTexScaled(0.0312f, 0.1342f, -0.085f, 0.12f, 0.12f, 0.12f, texid[3], &objmodel_ptr6);
		  /*glPushMatrix();
		  glTranslatef(0.0312f, 0.1342f, -0.085f);
		  glBindTexture(GL_TEXTURE_2D, texid[3]);
		  glScalef(0.12f, 0.12f, 0.12f);			// reducira toda la imagen a un 1% con respecto a la original la escala de este objeto "esta demasiado grande". (el tamaño actual se toma como si fuera 100% y por ejemplo si le ponemos 10% sera un 10% de ese tamaño original)
		  glmDraw(objmodel_ptr6, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();*/

		  openShaderTexScaled(0.0312f, -0.0000007f, -0.080f, 0.10f, 0.10f, 0.125f, texid[4], &objmodel_ptr8);
		  /*glPushMatrix();
		  glTranslatef(0.0312f, -0.0000007f, -0.080f);
		  glBindTexture(GL_TEXTURE_2D, texid[4]);
		  glScalef(0.10f, 0.10f, 0.125f);			// reducira toda la imagen a un 1% con respecto a la original la escala de este objeto "esta demasiado grande". (el tamaño actual se toma como si fuera 100% y por ejemplo si le ponemos 10% sera un 10% de ese tamaño original)
		  glmDraw(objmodel_ptr8, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();*/



	  //glutSolidTeapot(1.0);
	  if (shader1) shader1->end();

	  
	  
      glutSwapBuffers();
      glPopMatrix();

      UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!

	virtual void OnInit()
	{
		//abrir 
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		if (shader==0) 
         std::cout << "Error Loading, compiling or linking shader\n";
      else
      {
         ProgramObject = shader->GetProgramObject();
      }

	 //*** Para Textura: abre los shaders para texturas
		shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt"); // load (and compile, link) from file
		if (shader1 == 0)
			std::cout << "Error Loading, compiling or linking shader\n";
		else
		{
			ProgramObject = shader1->GetProgramObject();
		}

      time0 = clock();
      timer010 = 0.0f;
      bUp = true;

	  //Abrir mallas
	  objmodel_ptr = NULL;
	  openMesh(&objmodel_ptr, 90.0, "./Mallas/tierra.obj");

	  objmodel_ptr1 = NULL;
	  openMesh(&objmodel_ptr1, 90.0, "./Mallas/rio.obj");

	  objmodel_ptr2 = NULL;
	  openMesh(&objmodel_ptr2, 90.0, "./Mallas/cimientosCasa.obj");

	  objmodel_ptr3 = NULL;
	  openMesh(&objmodel_ptr3, 90.0, "./Mallas/palmeras.obj");

	  objmodel_ptr4 = NULL;
	  openMesh(&objmodel_ptr4, 90.0, "./Mallas/pisoCasa.obj");

	  objmodel_ptr5 = NULL;
	  openMesh(&objmodel_ptr5, 90.0, "./Mallas/estructuraCasa.obj");

	  openMesh(&objmodel_ptr6, 90.0, "./Mallas/techo.obj");

	  objmodel_ptr6 = NULL;
	  openMesh(&objmodel_ptr6, 90.0, "./Mallas/techo.obj");

	  objmodel_ptr7 = NULL;
	  openMesh(&objmodel_ptr7, 90.0, "./Mallas/ventanas.obj");

	  objmodel_ptr8 = NULL;
	  openMesh(&objmodel_ptr8, 90.0, "./Mallas/pisoDentroCasa.obj");

	  //*** Para Textura: abrir archivo de textura
	  initialize_textures(0, "./Mallas/cimientosTextura.jpg");
	  initialize_textures(1, "./Mallas/pisoTextura.jpg");
	  initialize_textures(2, "./Mallas/fachadaCasa.jpg");
	  initialize_textures(3, "./Mallas/techoTextura.jpg");
	  initialize_textures(4, "./Mallas/pisoDentroTextura.jpg"); // textura para el piso dentro de la casa
	

      DemoLight();

	}
	
	

	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(45,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
      if (cAscii == 's')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
	}

   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     /*
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);*/
   }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

