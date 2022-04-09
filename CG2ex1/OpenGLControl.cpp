#include "StdAfx.h"
#include "OpenGLControl.h"

COpenGLControl::COpenGLControl(void)
{
	bShowAxis = FALSE;
	m_bShowCtrlPoints = FALSE;
	m_bShowYX=TRUE;
	m_bShowYX2=TRUE;
	m_isLoading = FALSE;
	m_bWireframe = TRUE;
	m_bShowSurf = TRUE;
	pNumberOfCtrlPnts = 0;
	pKnotVector = NULL;
	pCtrlPnts = NULL;
	sNumberOfCtrlPnts = 0;
	sKnotVector = NULL;
	sCtrlPnts = NULL;
	m_fPosX = 0.0f;    // X position of model in camera view
    m_fPosY = 0.0f;    // Y position of model in camera view
    m_fZoom = 10.0f;   // Zoom on model in camera view
    m_fRotX = 30.0f;    // Rotation on model in camera view
    m_fRotY = -45.0f;    // Rotation on model in camera view

	maxCoords[0]=0.0;
	maxCoords[1]=0.0;
	maxCoords[2]=0.0;
	minCoords[0]=0.0;
	minCoords[1]=0.0;
	minCoords[2]=0.0;

}

COpenGLControl::~COpenGLControl(void)
{
}

void COpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
   CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL,(HBRUSH)GetStockObject(BLACK_BRUSH), NULL);
   CreateEx(0, className, "OpenGL", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

   // Set initial variables' values
   m_oldWindow    = rect;
   m_originalRect = rect;

   hWnd = parent;
}


BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	oglInitialize();
	return 0;
}

void COpenGLControl::oglInitialize(void)
{
   // Initial Setup:
   //
   static PIXELFORMATDESCRIPTOR pfd =
   {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,    // bit depth
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      16,    // z-buffer depth
      0, 0, 0, 0, 0, 0, 0,
   };

   // Get device context only once.
   hdc = GetDC()->m_hDC;

   // Pixel format.
   m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
   SetPixelFormat(hdc, m_nPixelFormat, &pfd);

   // Create the OpenGL Rendering Context.
   hrc = wglCreateContext(hdc);
   wglMakeCurrent(hdc, hrc);

   // Basic Setup:
   //
   // Set color to use when clearing the background.
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClearDepth(1.0f);

   glEnable(GL_POINT_SMOOTH);
   //glEnable(GL_LINE_SMOOTH);
   glHint(GL_NICEST,GL_POINT_SMOOTH_HINT);
   //glHint(GL_NICEST,GL_LINE_SMOOTH_HINT);
   glShadeModel(GL_SMOOTH);

    
   // Turn on backface culling
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);

   // Turn on depth testing
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   // Send draw request
   OnDraw(NULL);
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);

}

void COpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	

	switch (nIDEvent)
   {
      case 1:
      {
         // Clear color and depth buffer bits
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		 // Draw OpenGL scene
         oglDrawScene();

         // Swap buffers
         SwapBuffers(hdc);

         break;
      }

      default:
         break;
   }

   CWnd::OnTimer(nIDEvent);}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

   // Map the OpenGL coordinates.
   glViewport(0, 0, cx, cy);

   // Projection view
   glMatrixMode(GL_PROJECTION);

   glLoadIdentity();

   // Set our current view perspective
   gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

   // Model view
   glMatrixMode(GL_MODELVIEW);

	
}

void COpenGLControl::oglDrawScene(void)
{
	GLfloat pos[] = {200,200,200};
	GLfloat diffuse[] = {0,0,0};
	GLfloat specular[] = {0.4,0.8,0.95};
	GLfloat ambient[] = {0.9,0.9,0.9};

	//Brass-ish
	GLfloat mat_ambient2[] = {0.329412, 0.223529, 0.027451, 1.0};
	GLfloat mat_diffuse2[] = { 0.780392, 0.568627, 0.113725, 1.0};
	GLfloat mat_diffuse2t[] = { 0.780392, 0.568627, 0.113725, 0.15 };
	GLfloat mat_specular2[] = { 0.992157, 0.941176, 0.807843, 1.0};
	GLfloat mat_shininess2[] = { 27.8974};

	//Steel-ish
	GLfloat mat_ambient0[] = {0.23125, 0.23125, 0.23125, 1.0};
	GLfloat mat_diffuse0[] = {0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_specular0[] = {0.87, 0.871, 0.87, 1.0};
	GLfloat mat_shininess0[] = { 100.6 };

	glEnable(GL_LIGHTING);                     //enables lighting

	glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
		
	GLfloat global_ambient[4]={0.9, 0.9, 0.9, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightfv(GL_LIGHT0,GL_POSITION,pos);       //updates the light's position
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);    //updates the light's diffuse colour
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);  //updates the light's specular colour
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);    //updates the light's ambient colour+
	glEnable(GL_LIGHT0);                       //enables a light

	GLfloat position1[4]={0.0, 0.0, 100, 1.0};
	GLfloat ambient1[4]={0.6, 0.6, 0.6, 1.0};
	GLfloat diffuse1[4]={0.5, 0.5, 0.8, 1.0};
	GLfloat specular1[4]={0.30, 0.30, 0.30, 1.0};
	GLfloat spotdir1[4]={0.0, 0.0, 0.0, 1.0};
		
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT1 ,GL_SPOT_DIRECTION, spotdir1);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 120.0);
	glEnable(GL_LIGHT1);

	GLfloat position2[4]={20.0, 20.0, -120, 1.0};
	GLfloat ambient2[4]={0.2, 0.2, 0.2, 1.0};
	GLfloat diffuse2[4]={0.3, 0.5, 0.3, 1.0};
	GLfloat specular2[4]={0.35, 0.35, 0.35, 1.0};
	GLfloat spotdir2[4]={0.0, 0.0, 0.0, 1.0};
		
	glLightfv(GL_LIGHT2, GL_POSITION, position2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT2 ,GL_SPOT_DIRECTION, spotdir2);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 150.0);
	glEnable(GL_LIGHT2);
	

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess0);
	//Draw axis
	if(bShowAxis)
	{
		m_isLoading = FALSE;
		glBegin(GL_LINES);
			glColor3f(1.0,1.0,1.0);
			glVertex3f(-1000,0,0);
			glVertex3f(1000,0,0);
		
			glVertex3f(0,-1000,0);
			glVertex3f(0,1000,0);
			glVertex3f(0,0,-1000);
			glVertex3f(0,0,1000);
		glEnd();
	}
	
	if(sNumberOfCtrlPnts!=0 && m_bShowYX2)   //Draw sweeping line
	{
		glBegin(GL_LINE_STRIP);
		for(int i=sIndexOfKnotsInLODs[3];i<sIndexOfKnotsInLODs[sNumKnots-4];i++)
		{
			
				glColor3f(1.0,0.1,0.05);
				glVertex3fv(sBspline[i]);
		}
		glEnd();

		/*glBegin(GL_LINE_STRIP);
		for(int i=sIndexOfKnotsInLODs[3];i<sIndexOfKnotsInLODs[sNumKnots-4];i++)
		{
			
				glColor3f(1.0,0.1,0.8);
				glVertex3fv(sDerNormBspline[i]);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for(int i=sIndexOfKnotsInLODs[3];i<sIndexOfKnotsInLODs[sNumKnots-4];i++)
		{
			
				glColor3f(0.0,0.9,0.5);
				glVertex3fv(sDer2NormBspline[i]);
		}
		glEnd();*/
		
		//Draws local coordinate system
		for(int i=sIndexOfKnotsInLODs[3];i<sIndexOfKnotsInLODs[sNumKnots-4];i++)
		{
				point3D tmp;   
				for(int k=0;k<3;k++)
					tmp[k]=sBspline[i][k]+sBcoord[i][k];
				glBegin(GL_LINES);	
				glColor3f(0.9,0.9,0.5);
				glVertex3fv(sBspline[i]);
				glVertex3fv(tmp);
				glEnd();
				for(int k=0;k<3;k++)
					tmp[k]=sBspline[i][k]+sNcoord[i][k];
				glBegin(GL_LINES);	
				glColor3f(0.9,0.4,0.65);
				glVertex3fv(sBspline[i]);
				glVertex3fv(tmp);
				glEnd();


		}
	}
	if(sNumberOfCtrlPnts!=0 && m_bShowSurf)  //Draw surface
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
		if(m_bWireframe)
		{
			for(int u=sIndexOfKnotsInLODs[3];u<sIndexOfKnotsInLODs[sNumKnots-4];u++)
			{
				
		
				glBegin(GL_LINE_STRIP);
				for(int v=pIndexOfKnotsInLODs[pDegree];v<pIndexOfKnotsInLODs[pNumKnots-pDegree-1];v++)
				{
					//glColor3f(0.0,0.9,0.35);
					glVertex3fv(surface[u][v]);
				}
				glEnd();
			}
			for(int v=pIndexOfKnotsInLODs[pDegree];v<pIndexOfKnotsInLODs[pNumKnots-pDegree-1];v++)
			{
				glBegin(GL_LINE_STRIP);
				for(int u=sIndexOfKnotsInLODs[3];u<sIndexOfKnotsInLODs[sNumKnots-4];u++)
				{
					//glColor3f(0.0,0.9,0.35);
					glVertex3fv(surface[u][v]);
				}
				glEnd();
			}
		}
		else   //Solid
		{
			for(int u=sIndexOfKnotsInLODs[3];u<sIndexOfKnotsInLODs[sNumKnots-4]-1;u++)
			{
				for(int v=pIndexOfKnotsInLODs[pDegree];v<pIndexOfKnotsInLODs[pNumKnots-pDegree-1]-1;v++)
				{
					glBegin(GL_QUADS);
					glColor3f(0.0,0.9,0.35);
					glNormal3fv(surfaceNorm[u][v]);
					glVertex3fv(surface[u][v]);
					glVertex3fv(surface[u][v+1]);
					glVertex3fv(surface[u+1][v+1]);
					glVertex3fv(surface[u+1][v]);
					glEnd();

					/*point3D tmp;   //Show surface normals
					for(int k=0;k<3;k++)
						tmp[k]=surface[u][v][k]+surfaceNorm[u][v][k];
					glBegin(GL_LINES);	
					glColor3f(0.9,0.9,0.5);
					glVertex3fv(surface[u][v]);
					glVertex3fv(tmp);
					glEnd();*/

				}
				
			}


		}

	}

	glDisable(GL_LIGHTING);
	if(pNumberOfCtrlPnts!=0 && m_bShowYX)
	{
		glBegin(GL_LINE_STRIP);
		for(int i=pIndexOfKnotsInLODs[pDegree];i<pIndexOfKnotsInLODs[pNumKnots-pDegree-1];i++)
		{
			
				glColor3f(1.0,0.5,0.65);
				glVertex3fv(pBspline[i]);
		}
		glEnd();
	}

	

	//Show control points
	if(m_bShowCtrlPoints)
	{
		if(m_bShowYX)
		{
			for(int i=0;i<pNumberOfCtrlPnts;i++)
			{
				glPointSize(3.0);
				glBegin(GL_POINTS);
					glColor3f(1.0,0.0,0.0);
					glVertex3fv(pCtrlPnts[i]);
				glEnd();
			}
			
			for(int i=0; i<pNumberOfCtrlPnts-1;i++)
			{
				glBegin(GL_LINES);
					glColor3f(0.0,0.30,0.8);
					glVertex3fv(pCtrlPnts[i]);
					glVertex3fv(pCtrlPnts[i+1]);
				glEnd();
			}
		}
		
		if(m_bShowYX2)
		{
			for(int i=0;i<sNumberOfCtrlPnts;i++)
			{
				glPointSize(3.0);
				glBegin(GL_POINTS);
					glColor3f(0.0,1.0,0.0);
					glVertex3fv(sCtrlPnts[i]);
				glEnd();
			}
			
			for(int i=0; i<sNumberOfCtrlPnts-1;i++)
			{
				glBegin(GL_LINES);
					glColor3f(0.8,0.40,0.8);
					glVertex3fv(sCtrlPnts[i]);
					glVertex3fv(sCtrlPnts[i+1]);
				glEnd();
			}
		}
		
	}
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
   if(m_isLoading == FALSE)
   {
	   int diffX = (int)(point.x - m_fLastX);
	   int diffY = (int)(point.y - m_fLastY);
	   m_fLastX  = (float)point.x;
	   m_fLastY  = (float)point.y;

	   // Left mouse button
	   if (nFlags & MK_LBUTTON)
	   {
		  m_fRotX += (float)0.5f * diffY;

		  if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		  {
			 m_fRotX = 0.0f;
		  }

		  m_fRotY += (float)0.5f * diffX;

		  if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		  {
			 m_fRotY = 0.0f;
		  }
	   }

	   // Right mouse button
	   else if (nFlags & MK_RBUTTON)
	   {
		  m_fZoom -= (float)0.1f * diffY;
	   }

	   // Middle mouse button
	   else if (nFlags & MK_MBUTTON)
	   {
		  m_fPosX += (float)0.05f * diffX;
		  m_fPosY -= (float)0.05f * diffY;
	   }

	   OnDraw(NULL);

	   CWnd::OnMouseMove(nFlags, point);
   }
}

/*void COpenGLControl::normalizeKnots()
{
	for(int i=0;i<numKnots;i++)
		knotVector[i]/=knotVector[numberOfCtrlPnts+3];
}*/

void COpenGLControl::doCircle(double x, double y, double radius)
	{
		double y1=y+radius;
		double x1=x;
		glBegin(GL_LINE_STRIP);
		for(double angle=0.0f;angle<=(2.0f*3.14159);angle+=0.01f)
		{
			double x2=x+(radius*(float)sin((double)angle));
			double y2=y+(radius*(float)cos((double)angle));
			glVertex2d(x1,y1);
			y1=y2;
			x1=x2;
		}
		glEnd();
	}

void COpenGLControl::find_pBasis(int d)
{
	float num1=0;
	float num2=0;
	float den1=0;
	float den2=0;
	//float normed_u=0;
	float u=0;
	float u1=0;
	BOOL iszero1=FALSE;
	BOOL iszero2=FALSE;

	switch(d)
	{
	case 0:
		for(int k=0;k<pNumKnots;k++)
			for(int i=0;i!=LOD;i++)
			{
				u=(pKnotVector[pNumKnots-1]-pKnotVector[0])*i/(float)(LOD-1);
				u1=(pKnotVector[pNumKnots-1]-pKnotVector[0])*(i+1)/(float)(LOD-1);
				
				if(i==LOD-1) u-=0.001f;
				if(i+1==LOD-1) u1-=0.001f;
				if(pKnotVector[k]>=u && pKnotVector[k]<u1)
					pIndexOfKnotsInLODs[k]=i;
				//normed_u=u/((float)(LOD));
				if((pKnotVector[k]<=u && u<=pKnotVector[k+1]))
					pBasis[k][0][i]=1.0;
				else pBasis[k][0][i]=0.0;
			}
		find_pBasis(1);
		break;
	default:
		if(d>pDegree) break;
		for(int k=0;k<pNumKnots-d;k++)
		{
			
			den1=pKnotVector[k+d]-pKnotVector[k];
			den2=pKnotVector[k+d+1]-pKnotVector[k+1];
			
			if(fabs(den1)<EPSILON) { den1=1.0; iszero1=TRUE; }
				else iszero1=FALSE;
			if(fabs(den2)<EPSILON) { den2=1.0; iszero2=TRUE; }
				else iszero2=FALSE;
			
			for(int i=0;i<LOD;i++)
			{
				u=(pKnotVector[pNumKnots-1]-pKnotVector[0])*i/(float)(LOD-1);
				if(i==LOD-1) u-=0.001f;
				if(iszero1==FALSE)
					num1=u-pKnotVector[k];
				else num1=0;
				if(iszero2==FALSE)
					num2=pKnotVector[k+d+1]-u;
				else num2=0;
				pBasis[k][d][i]=(num1/den1)*pBasis[k][d-1][i]+(num2/den2)*pBasis[k+1][d-1][i];
			}
		}
		find_pBasis(++d);
	}
							
}

void COpenGLControl::find_sBasis(int d)
{
	float num1=0;
	float num2=0;
	float den1=0;
	float den2=0;
	//float normed_u=0;
	float u=0;
	float u1=0;
	BOOL iszero1=FALSE;
	BOOL iszero2=FALSE;

	switch(d)
	{
	case 0:
		for(int k=0;k<sNumKnots;k++)
			for(int i=0;i!=LOD;i++)
			{
				u=(sKnotVector[sNumKnots-1]-sKnotVector[0])*i/(float)(LOD-1);
				u1=(sKnotVector[sNumKnots-1]-sKnotVector[0])*(i+1)/(float)(LOD-1);
				
				if(i==LOD-1) u-=0.001f;
				if(i+1==LOD-1) u1-=0.001f;
				if(sKnotVector[k]>=u && sKnotVector[k]<u1)
					sIndexOfKnotsInLODs[k]=i;
				//normed_u=u/((float)(LOD));
				if((sKnotVector[k]<=u && u<=sKnotVector[k+1]))
					sBasis[k][0][i]=1.0;
				else sBasis[k][0][i]=0.0;
			}
		find_sBasis(1);
		break;
	default:
		if(d>3) break;
		for(int k=0;k<sNumKnots-d;k++)
		{
			
			den1=sKnotVector[k+d]-sKnotVector[k];
			den2=sKnotVector[k+d+1]-sKnotVector[k+1];
			
			if(fabs(den1)<EPSILON) { den1=1.0; iszero1=TRUE; }
				else iszero1=FALSE;
			if(fabs(den2)<EPSILON) { den2=1.0; iszero2=TRUE; }
				else iszero2=FALSE;
			
			for(int i=0;i<LOD;i++)
			{
				u=(sKnotVector[sNumKnots-1]-sKnotVector[0])*i/(float)(LOD-1);
				if(i==LOD-1) u-=0.001f;
				if(iszero1==FALSE)
					num1=u-sKnotVector[k];
				else num1=0;
				if(iszero2==FALSE)
					num2=sKnotVector[k+d+1]-u;
				else num2=0;
				sBasis[k][d][i]=(num1/den1)*sBasis[k][d-1][i]+(num2/den2)*sBasis[k+1][d-1][i];
			}
		}
		find_sBasis(++d);
	}
				
}

void COpenGLControl::calc_sBSpline()
{
	for(int i=0;i<LOD;i++)
	{
		sBspline[i][0]=0.0;
		sBspline[i][1]=0.0;
		sBspline[i][2]=0.0;
		for(int k=0;k<sNumberOfCtrlPnts;k++)
		{
			sBspline[i][0]+=sBasis[k][3][i]*sCtrlPnts[k][0];
			sBspline[i][1]+=sBasis[k][3][i]*sCtrlPnts[k][1];
			sBspline[i][2]+=sBasis[k][3][i]*sCtrlPnts[k][2];
		}
	}
}
void COpenGLControl::calc_pBSpline()
{
	for(int i=0;i<LOD;i++)
	{
		pBspline[i][0]=0.0;
		pBspline[i][1]=0.0;
		pBspline[i][2]=0.0;
		for(int k=0;k<pNumberOfCtrlPnts;k++)
		{
			pBspline[i][0]+=pBasis[k][pDegree][i]*pCtrlPnts[k][0];
			pBspline[i][1]+=pBasis[k][pDegree][i]*pCtrlPnts[k][1];
		}
	}
	//Move to center
	//for(int i=0;i<LOD;i++)
		//for(int k=0;k<3;k++)
			//pBspline[i][k]-=pBspline[pIndexOfKnotsInLODs[pDegree]][k];

}

void COpenGLControl::calcDerivCtrlPoints()
{
	for(int i=0;i<sNumberOfCtrlPnts-1;i++)
		for(int k=0;k<3;k++)
			sDerCtrlPnts[i][k]=3.0*(sCtrlPnts[i+1][k]-sCtrlPnts[i][k])/(sKnotVector[i+4]-sKnotVector[i+1]);
	for(int i=0;i<sNumberOfCtrlPnts-2;i++)
		for(int k=0;k<3;k++)
			sDer2CtrlPnts[i][k]=2.0*(sDerCtrlPnts[i+1][k]-sDerCtrlPnts[i][k])/(sKnotVector[i+4]-sKnotVector[i+2]);
}

void COpenGLControl::calc_sDerBSpline()
{
	for(int i=0;i<LOD;i++)
	{
		sDerBspline[i][0]=0.0;
		sDerBspline[i][1]=0.0;
		sDerBspline[i][2]=0.0;
		for(int k=0;k<sNumberOfCtrlPnts-1;k++)
		{
			sDerBspline[i][0]+=sBasis[k+1][2][i]*sDerCtrlPnts[k][0];
			sDerBspline[i][1]+=sBasis[k+1][2][i]*sDerCtrlPnts[k][1];
			sDerBspline[i][2]+=sBasis[k+1][2][i]*sDerCtrlPnts[k][2];
		}
		float len=sqrt(sDerBspline[i][0]*sDerBspline[i][0]+sDerBspline[i][1]*sDerBspline[i][1]+sDerBspline[i][2]*sDerBspline[i][2]);
		if(len<EPSILON) len=1;
		for(int k=0;k<3;k++)
			sDerNormBspline[i][k]=sDerBspline[i][k]/len;
	}

	for(int i=0;i<LOD;i++)
	{
		sDer2Bspline[i][0]=0.0;
		sDer2Bspline[i][1]=0.0;
		sDer2Bspline[i][2]=0.0;
		for(int k=0;k<sNumberOfCtrlPnts-2;k++)
		{
			sDer2Bspline[i][0]+=sBasis[k+2][1][i]*sDer2CtrlPnts[k][0];
			sDer2Bspline[i][1]+=sBasis[k+2][1][i]*sDer2CtrlPnts[k][1];
			sDer2Bspline[i][2]+=sBasis[k+2][1][i]*sDer2CtrlPnts[k][2];
		}
		float len=sqrt(sDer2Bspline[i][0]*sDer2Bspline[i][0]+sDer2Bspline[i][1]*sDer2Bspline[i][1]+sDer2Bspline[i][2]*sDer2Bspline[i][2]);
		if(len<EPSILON) len=1;
		for(int k=0;k<3;k++)
			sDer2NormBspline[i][k]=sDer2Bspline[i][k]/len;
	}


}


template <class T> std::string toString( T value )
{
   std::ostringstream oss;

   oss << value;

   return oss.str();
}




void COpenGLControl::findMaxMin(void)
{
	maxCoords[0]=sCtrlPnts[0][0];
	maxCoords[1]=sCtrlPnts[0][1];
	maxCoords[2]=sCtrlPnts[0][2];
	minCoords[0]=sCtrlPnts[0][0];
	minCoords[1]=sCtrlPnts[0][1];
	minCoords[2]=sCtrlPnts[0][2];
	for(int i=1;i<sNumberOfCtrlPnts;i++)
	{
		if(sCtrlPnts[i][0]>maxCoords[0])
			maxCoords[0]=sCtrlPnts[i][0];
		if(sCtrlPnts[i][1]>maxCoords[1])
			maxCoords[1]=sCtrlPnts[i][1];
		if(sCtrlPnts[i][2]>maxCoords[2])
			maxCoords[2]=sCtrlPnts[i][2];
		if(sCtrlPnts[i][0]<minCoords[0])
			minCoords[0]=sCtrlPnts[i][0];
		if(sCtrlPnts[i][1]<minCoords[1])
			minCoords[1]=sCtrlPnts[i][1];
		if(sCtrlPnts[i][2]<minCoords[2])
			minCoords[2]=sCtrlPnts[i][2];
	}
	lengthX=maxCoords[0];//-minCoords[0];
	lengthY=maxCoords[1];//-minCoords[1];
	lengthZ=maxCoords[2];//-minCoords[2];
	if(lengthX>lengthY) maxLength=lengthX;
	else maxLength=lengthY;
	if(lengthZ>maxLength) maxLength=lengthZ;

}

void COpenGLControl::calc_localCoord()
{
	point3D k;
	k[0]=0.0;
	k[1]=1.0;
	k[2]=0.0;

	/*for(int i=0;i<LOD;i++)    //twist problem
	{
		cross(sDerNormBspline[i],sDer2NormBspline[i],sBcoord[i]);
		float len=sqrt(sBcoord[i][0]*sBcoord[i][0]+sBcoord[i][1]*sBcoord[i][1]+sBcoord[i][2]*sBcoord[i][2]);
		if(len<EPSILON) len=1;
		for(int k=0;k<3;k++)
			sBcoord[i][k]=sBcoord[i][k]/len;

		cross(sBcoord[i],sDerNormBspline[i],sNcoord[i]);
	}*/

	for(int i=0;i<LOD;i++)
	{
		cross(sDerNormBspline[i],k,sBcoord[i]);
		float len=sqrt(sBcoord[i][0]*sBcoord[i][0]+sBcoord[i][1]*sBcoord[i][1]+sBcoord[i][2]*sBcoord[i][2]);
		if(len<EPSILON) len=1;
		for(int k=0;k<3;k++)
			sBcoord[i][k]=sBcoord[i][k]/len;

		cross(sBcoord[i],sDerNormBspline[i],sNcoord[i]);
		len=sqrt(sNcoord[i][0]*sNcoord[i][0]+sNcoord[i][1]*sNcoord[i][1]+sNcoord[i][2]*sNcoord[i][2]);
		if(len<EPSILON) len=1;
		for(int k=0;k<3;k++)
			sNcoord[i][k]=sNcoord[i][k]/len;
	}


}

void COpenGLControl::calc_surface()
{
	int i,u,v;
	for(u=0;u<LOD;u++)
		for(v=0;v<LOD;v++)
			for(i=0;i<3;i++)
				surface[u][v][i]=0.0;
	for(u=sIndexOfKnotsInLODs[3];u<sIndexOfKnotsInLODs[sNumKnots-4];u++)
		for(int v=pIndexOfKnotsInLODs[pDegree];v<pIndexOfKnotsInLODs[pNumKnots-pDegree-1];v++)
			for(i=0;i<3;i++)
				surface[u][v][i]=sBspline[u][i]+(pBspline[v][0]-pBspline[pIndexOfKnotsInLODs[pDegree]][0])*sBcoord[u][i]+(pBspline[v][1]-pBspline[pIndexOfKnotsInLODs[pDegree]][1])*sNcoord[u][i];


}

void COpenGLControl::calc_surfaceNormals()
{
	point3D v1,v2,res;
	float len;
	for(int u=sIndexOfKnotsInLODs[3];u<sIndexOfKnotsInLODs[sNumKnots-4]-1;u++)
		{
			for(int v=pIndexOfKnotsInLODs[pDegree];v<pIndexOfKnotsInLODs[pNumKnots-pDegree-1]-1;v++)
			{
				for(int i=0;i<3;i++)
				{
					v1[i]=surface[u][v+1][i]-surface[u][v][i];
					v2[i]=surface[u+1][v][i]-surface[u][v][i];
				}
				cross(v1,v2,res);
				len=sqrt(res[0]*res[0]+res[1]*res[1]+res[2]*res[2]);
				if(len<EPSILON) len=1;
				for(int k=0;k<3;k++)
					res[k]=res[k]/len;

				for(int i=0;i<3;i++)
					surfaceNorm[u][v][i]=res[i];
			}
	}
}

void COpenGLControl::calc_alpha(int d)
{
	float num1=0;
	float num2=0;
	float den1=0;
	float den2=0;
	//float normed_u=0;
	float u=0;
	float u1=0;
	BOOL iszero1=FALSE;
	BOOL iszero2=FALSE;

	switch(d)
	{
	case 0:
		for(int j=0;j<pNumKnots;j++)
		{
			for(int i=0;i<prevNumKnots-1;i++)
			{
				u=prevKnotVector[i];
				u1=prevKnotVector[i+1];
				if(pKnotVector[j]>=u && pKnotVector[j]<u1)
					alpha[i][0][j]=1.0;
				else alpha[i][0][j]=0.0;

			}
			alpha[prevNumKnots-1][0][j]=0.0;
		}
		calc_alpha(1);
		break;
	default:
		if(d>pDegree) break;
		for(int j=0;j<pNumKnots-d;j++)
		{
			for(int i=0;i<prevNumKnots-d-1;i++)
			{
				
				den1=prevKnotVector[i+d]-prevKnotVector[i];
				den2=prevKnotVector[i+d+1]-prevKnotVector[i+1];
				
				if(fabs(den1)<EPSILON) { den1=1.0; iszero1=TRUE; }
					else iszero1=FALSE;
				if(fabs(den2)<EPSILON) { den2=1.0; iszero2=TRUE; }
					else iszero2=FALSE;
				
				if(iszero1==FALSE)
					num1=pKnotVector[j+d]-prevKnotVector[i];
				else num1=0;
				if(iszero2==FALSE)
					num2=prevKnotVector[i+d+1]-pKnotVector[j+d];
				else num2=0;
				
				alpha[i][d][j]=(num1/den1)*alpha[i][d-1][j]+(num2/den2)*alpha[i+1][d-1][j];
			}
			for(int i=prevNumKnots-d-1;i<prevNumKnots;i++)
				alpha[i][d][j]=0.0;


		}
		calc_alpha(++d);
	}
		

}

void COpenGLControl::calc_alfa(int d)
{
	float num1=0;
	float num2=0;
	float den1=0;
	float den2=0;
	//float normed_u=0;
	float u=0;
	float u1=0;
	BOOL iszero1=FALSE;
	BOOL iszero2=FALSE;

	switch(d)
	{
	case 0:
		for(int j=0;j<pNumKnots;j++)
		{
			for(int i=0;i<prevNumKnots-1;i++)
				alfa[i][0][j]=alpha[i][0][j];
			alfa[prevNumKnots-1][0][j]=0.0;
		}
		calc_alfa(1);
		break;
	default:
		if(d>pDegree-1) break;
		for(int j=0;j<pNumKnots-d;j++)
		{
			for(int i=0;i<prevNumKnots-d-1;i++)
			{
				
				den1=prevKnotVector[i+d]-prevKnotVector[i];
				den2=prevKnotVector[i+d+1]-prevKnotVector[i+1];
				
				if(fabs(den1)<EPSILON) { den1=1.0; iszero1=TRUE; }
					else iszero1=FALSE;
				if(fabs(den2)<EPSILON) { den2=1.0; iszero2=TRUE; }
					else iszero2=FALSE;
				
				if(iszero1==FALSE)
					num1=pKnotVector[j+d+1]-prevKnotVector[i];
				else num1=0;
				if(iszero2==FALSE)
					num2=prevKnotVector[i+d+1]-pKnotVector[j+d+1];
				else num2=0;
				
				alfa[i][d][j]=(num1/den1)*alfa[i][d-1][j]+(num2/den2)*alfa[i+1][d-1][j]+alpha[i][d][j];
			}
			for(int i=prevNumKnots-d-1;i<prevNumKnots;i++)
				alpha[i][d][j]=0.0;
		}
		calc_alfa(++d);
	}
		

}
				

void COpenGLControl::cross(point3D& v1, point3D& v2, point3D& result)
{
	result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
	result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
	result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}
