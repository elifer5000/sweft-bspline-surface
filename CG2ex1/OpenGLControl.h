#pragma once
#include "afxwin.h"
#include <GL/glut.h>



typedef float point2D[2];
typedef float point3D[3];

template <class T> std::string toString( T value );


class COpenGLControl :
	public CWnd
{
public:
	COpenGLControl(void);
	~COpenGLControl(void);
	// Timer
    UINT_PTR m_unpTimer;
	void oglCreate(CRect rect, CWnd *parent);
	void oglInitialize(void);
	void oglDrawScene(void);
	void findMaxMin(void);
	//void normalizeKnots(void);
	void find_pBasis(int d);
	void find_sBasis(int d);
	void calc_sBSpline(void);
	void calc_pBSpline(void);
	void calcDerivCtrlPoints(void);
	void calc_sDerBSpline(void);
	void calc_localCoord(void);
	void cross(point3D& v1, point3D& v2, point3D& result);
	void calc_surface(void);
	void calc_surfaceNormals(void);
	void calc_alpha(int d);
	void calc_alfa(int d);
	void doCircle(double x, double y, double radius);

private:
	CWnd    *hWnd;
    HDC     hdc;
    HGLRC   hrc;
    int     m_nPixelFormat;
    CRect   m_rect;
    CRect   m_oldWindow;
    CRect   m_originalRect;
	BOOL	m_bIsMaximized;
	

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDraw(CDC *pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	float	m_fPosX;
	float	m_fPosY;
	float	m_fZoom;
	float	m_fRotX;
	float	m_fRotY;
	float	m_fLastX;
	float	m_fLastY;
	int		pDegree;
	int		pNumberOfCtrlPnts;
	int		sNumberOfCtrlPnts;
	float*	pKnotVector;
	float*	sKnotVector;
	int		pNumKnots;
	int		sNumKnots;
	point3D* pCtrlPnts;
	point3D* sCtrlPnts;

	point3D maxCoords;
	point3D minCoords;
	double lengthX;
	double lengthY;
	double lengthZ;
	double maxLength;
	BOOL	 bShowAxis;
	BOOL	 m_bShowCtrlPoints;
	BOOL	 m_bShowSurf;
	BOOL	 m_bShowYX;
	BOOL	 m_bShowYX2;
	BOOL	 m_isLoading;
	BOOL	 m_bWireframe;
	float*** pBasis;
	float*** sBasis;
	float*** alpha;
	float*** alfa;

	int* pIndexOfKnotsInLODs;
	int* sIndexOfKnotsInLODs;
	/*float* pBsplineX;
	float* pBsplineY;
	float* sBsplineX;
	float* sBsplineY;
	float* sBsplineZ;*/
	point3D* pBspline;
	point3D* sBspline;

	point3D* sDerBspline;
	point3D* sDerNormBspline;
	point3D* sDer2Bspline;
	point3D* sDer2NormBspline;
	point3D* sDerCtrlPnts;
	point3D* sDer2CtrlPnts;
	point3D* sBcoord;
	point3D* sNcoord;
	point3D** surfCtrlPnts;
	point3D** surface;
	point3D** surfaceNorm;

	float*	prevKnotVector;
	int		prevNumKnots;
	int		prevNumberOfCtrlPnts;
	point3D* prevCtrlPnts;

		

};


