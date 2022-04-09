// CG2ex1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CG2ex1.h"
#include "CG2ex1Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCG2ex1Dlg dialog




CCG2ex1Dlg::CCG2ex1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCG2ex1Dlg::IDD, pParent)
	, mBshowYX(TRUE)
	
	, m_BSurf(TRUE)
	, m_cDegree(0)
	, m_dKnotVec(_T(""))
	, mBshowYX2(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCG2ex1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_SHOWYX, mBshowYX);
	DDX_Check(pDX, IDC_SHOWSURF, m_BSurf);
	DDX_Text(pDX, IDC_EDITDEG, m_cDegree);
	DDX_Text(pDX, IDC_EDITKNOT, m_dKnotVec);
	DDX_Check(pDX, IDC_SHOWYX2, mBshowYX2);
}

BEGIN_MESSAGE_MAP(CCG2ex1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_BTN, &CCG2ex1Dlg::OnBnClickedOpenBtn)
	ON_BN_CLICKED(IDC_EXIT, &CCG2ex1Dlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_CHKSHWCTRLPOINTS, &CCG2ex1Dlg::OnBnClickedChkshwctrlpoints)
	ON_BN_CLICKED(IDC_SHOWYX, &CCG2ex1Dlg::OnBnClickedShowyx)
	ON_BN_CLICKED(IDC_SHOWYX2, &CCG2ex1Dlg::OnBnClickedShowyx2)
	ON_BN_CLICKED(IDC_SHOWSURF, &CCG2ex1Dlg::OnBnClickedShowSurf)
	ON_BN_CLICKED(IDC_BCENTER, &CCG2ex1Dlg::OnBnClickedBcenter)
	ON_BN_CLICKED(IDC_WIREFRAME, &CCG2ex1Dlg::OnBnClickedWireframe)
	ON_BN_CLICKED(IDC_APPLY, &CCG2ex1Dlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_APPLYDEG, &CCG2ex1Dlg::OnBnClickedApplydeg)
END_MESSAGE_MAP()


// CCG2ex1Dlg message handlers

BOOL CCG2ex1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rect;

	// Get size and position of the picture control
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCG2ex1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCG2ex1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCG2ex1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCG2ex1Dlg::OnBnClickedOpenBtn()
{
	this->UpdateData();
	m_oglWindow.m_isLoading = TRUE;
	m_oglWindow.bShowAxis=FALSE;
	CStdioFile  f;
	CString str;
	CString resToken;
	int count=0;
	int curPos=0;
	int pos=0;
			
	char strFilter[] = { "TXT Files (*.txt)|*.txt|All Files (*.*)|*.*||" };
	CFileDialog FileDlg(TRUE, ".txt", NULL, 0, strFilter);
	if( FileDlg.DoModal() == IDOK )
	{
		if( f.Open(FileDlg.GetPathName(), CFile::modeRead) == FALSE )
			return;
		
		while(f.ReadString(str))
		{
			
			
			if(str[0]=='#') continue;
			switch(count)
			{
				case 0:
					m_oglWindow.pDegree=atoi(str);
					count++;
					break;
				case 1:
					m_oglWindow.pNumberOfCtrlPnts=atoi(str);
					m_oglWindow.pNumKnots=m_oglWindow.pNumberOfCtrlPnts+m_oglWindow.pDegree+1;
					m_oglWindow.pKnotVector=new float[m_oglWindow.pNumKnots];
					m_oglWindow.pCtrlPnts=new point3D[m_oglWindow.pNumberOfCtrlPnts];
					
					m_oglWindow.pBasis=new float**[m_oglWindow.pNumKnots];
					
					for(int i=0;i<m_oglWindow.pNumKnots;i++)
						m_oglWindow.pBasis[i]=new float*[m_oglWindow.pDegree+1];
					for(int i=0;i<m_oglWindow.pNumKnots;i++)
						for(int j=0;j<m_oglWindow.pDegree+1;j++)
							m_oglWindow.pBasis[i][j]=new float[LOD];
					
					m_oglWindow.pBspline=new point3D[LOD];
					
					count++;
					break;
				case 2:
					resToken= str.Tokenize(",",curPos);
					
					while (resToken != "")
					{
						m_oglWindow.pKnotVector[pos]=atof(resToken);
						resToken=str.Tokenize(",",curPos);
						pos++;
						
					}
					count++;
					curPos=0;
					pos=0;
					break;
				case 3:
					resToken=str.Tokenize(" ",curPos);
					m_oglWindow.pCtrlPnts[pos][0]=atof(resToken);
					resToken=str.Tokenize(" ",curPos);
					m_oglWindow.pCtrlPnts[pos][1]=atof(resToken);
					m_oglWindow.pCtrlPnts[pos][2]=0.0;
					curPos=0;
					pos++;
					
					if(pos==m_oglWindow.pNumberOfCtrlPnts)
					{
						count++;
						curPos=0;
						pos=0;
					}
					break;
				case 4:
					m_oglWindow.sNumberOfCtrlPnts=atoi(str);
					m_oglWindow.sNumKnots=m_oglWindow.sNumberOfCtrlPnts+4;
					m_oglWindow.sKnotVector=new float[m_oglWindow.sNumKnots];
					m_oglWindow.sCtrlPnts=new point3D[m_oglWindow.sNumberOfCtrlPnts];
					m_oglWindow.sBasis=new float**[m_oglWindow.sNumKnots];
					
					for(int i=0;i<m_oglWindow.sNumKnots;i++)
						m_oglWindow.sBasis[i]=new float*[4];
					for(int i=0;i<m_oglWindow.sNumKnots;i++)
						for(int j=0;j<4;j++)
						m_oglWindow.sBasis[i][j]=new float[LOD];
					
					m_oglWindow.sBspline=new point3D[LOD];
					
					//Derivatives init
					m_oglWindow.sDerBspline=new point3D[LOD];
					m_oglWindow.sDer2Bspline=new point3D[LOD];
					m_oglWindow.sDerNormBspline=new point3D[LOD];
					m_oglWindow.sDer2NormBspline=new point3D[LOD];
					m_oglWindow.sBcoord=new point3D[LOD];
					m_oglWindow.sNcoord=new point3D[LOD];

					m_oglWindow.sDerCtrlPnts=new point3D[m_oglWindow.sNumberOfCtrlPnts-1];
					m_oglWindow.sDer2CtrlPnts=new point3D[m_oglWindow.sNumberOfCtrlPnts-2];

					count++;
					break;
				case 5:
					resToken= str.Tokenize(",",curPos);
					
					while (resToken != "")
					{
						m_oglWindow.sKnotVector[pos]=atof(resToken);
						resToken=str.Tokenize(",",curPos);
						pos++;
						
					}
					count++;
					curPos=0;
					pos=0;
					break;
				case 6:
					resToken=str.Tokenize(" ",curPos);
					m_oglWindow.sCtrlPnts[pos][0]=atof(resToken);
					resToken=str.Tokenize(" ",curPos);
					m_oglWindow.sCtrlPnts[pos][1]=atof(resToken);
					resToken=str.Tokenize(" ",curPos);
					m_oglWindow.sCtrlPnts[pos][2]=atof(resToken);
					curPos=0;
					pos++;
					
					if(pos==m_oglWindow.sNumberOfCtrlPnts)
					{
						count++;
						curPos=0;
						pos=0;
					}
					break;
				default:
					continue;
			}
		}
	}
	else
		return;

	f.Close();
	m_cDegree=m_oglWindow.pDegree;

	m_dKnotVec=FloatToText(m_oglWindow.pKnotVector[0],3);
	for(int i=1;i<m_oglWindow.pNumKnots;i++)
		m_dKnotVec=m_dKnotVec + ", " + FloatToText(m_oglWindow.pKnotVector[i],2);
	
	m_oglWindow.bShowAxis=FALSE;

	DataInit();
	
	
}

void CCG2ex1Dlg::DataInit()
{
	m_oglWindow.pIndexOfKnotsInLODs=new int[m_oglWindow.pNumKnots];
	m_oglWindow.sIndexOfKnotsInLODs=new int[m_oglWindow.sNumKnots];
	m_oglWindow.findMaxMin();
	
	//m_oglWindow.normalizeKnots();
	m_oglWindow.find_sBasis(0);
	m_oglWindow.find_pBasis(0);
	m_oglWindow.calc_sBSpline();
	m_oglWindow.calc_pBSpline();

	m_oglWindow.calcDerivCtrlPoints();
	m_oglWindow.calc_sDerBSpline();
	m_oglWindow.calc_localCoord();

	m_oglWindow.surfCtrlPnts=new point3D*[LOD];
	m_oglWindow.surface=new point3D*[LOD];
	m_oglWindow.surfaceNorm=new point3D*[LOD];
	for(int i=0;i<LOD;i++)
	{
		m_oglWindow.surfCtrlPnts[i]=new point3D[m_oglWindow.pNumberOfCtrlPnts];
		m_oglWindow.surface[i]=new point3D[LOD];
		m_oglWindow.surfaceNorm[i]=new point3D[LOD];
	}

	m_oglWindow.calc_surface();
	m_oglWindow.calc_surfaceNormals();
	
	if(m_oglWindow.bShowAxis==FALSE)
	{
		m_oglWindow.m_fPosX = 0.0f;    // X position of model in camera view
		m_oglWindow.m_fPosY = 0.0f;    // Y position of model in camera view
		m_oglWindow.m_fRotX = 30.0f;    // X position of model in camera view
		m_oglWindow.m_fRotY = -45.0f;    // Y position of model in camera view
		m_oglWindow.m_fZoom = 1.1*m_oglWindow.maxLength;
	}
		
	m_oglWindow.bShowAxis=TRUE;
	this->UpdateData(FALSE);
}

void CCG2ex1Dlg::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	

	OnOK();
}

void CCG2ex1Dlg::OnBnClickedChkshwctrlpoints()

{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowCtrlPoints==TRUE) m_oglWindow.m_bShowCtrlPoints=FALSE;
	else m_oglWindow.m_bShowCtrlPoints=TRUE;
}

void CCG2ex1Dlg::OnBnClickedShowyx()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowYX==TRUE) m_oglWindow.m_bShowYX=FALSE;
	else m_oglWindow.m_bShowYX=TRUE;
}

void CCG2ex1Dlg::OnBnClickedShowyx2()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowYX2==TRUE) m_oglWindow.m_bShowYX2=FALSE;
	else m_oglWindow.m_bShowYX2=TRUE;
}

void CCG2ex1Dlg::OnBnClickedBcenter()
{
	// TODO: Add your control notification handler code here
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_oglWindow.m_fPosX = 0.0f;    // X position of model in camera view
	m_oglWindow.m_fPosY = 0.0f;    // Y position of model in camera view
	m_oglWindow.m_fRotX = 30.0f;    // X position of model in camera view
	m_oglWindow.m_fRotY = -45.0f;    // Y position of model in camera view
	m_oglWindow.m_fZoom = 1.1*m_oglWindow.maxLength;
	this->UpdateData(FALSE);
}



void CCG2ex1Dlg::OnBnClickedWireframe()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bWireframe==TRUE) m_oglWindow.m_bWireframe=FALSE;
	else m_oglWindow.m_bWireframe=TRUE;
}

void CCG2ex1Dlg::OnBnClickedShowSurf()
{
	if(m_oglWindow.m_bShowSurf==TRUE) m_oglWindow.m_bShowSurf=FALSE;
	else m_oglWindow.m_bShowSurf=TRUE;
}

//Apply knot changes
void CCG2ex1Dlg::OnBnClickedApply()
{
	CString resToken;
	vector<float> tmp;
	int curPos=0;
	int pos=0;
	
	//Save previous knot vector and control points
	m_oglWindow.prevNumKnots=m_oglWindow.pNumKnots;
	m_oglWindow.prevKnotVector=new float[m_oglWindow.prevNumKnots];
	for(int k=0;k<m_oglWindow.prevNumKnots;k++)
		m_oglWindow.prevKnotVector[k]=m_oglWindow.pKnotVector[k];

	m_oglWindow.prevNumberOfCtrlPnts=m_oglWindow.pNumberOfCtrlPnts;
	m_oglWindow.prevCtrlPnts=new point3D[m_oglWindow.prevNumberOfCtrlPnts];
	for(int k=0;k<m_oglWindow.prevNumberOfCtrlPnts;k++)
		for(int i=0;i<3;i++)
			m_oglWindow.prevCtrlPnts[k][i]=m_oglWindow.pCtrlPnts[k][i];

	this->UpdateData(TRUE);   //Get the input from the user
	
	resToken= m_dKnotVec.Tokenize(",",curPos);
	
	while (resToken != "")
	{
		tmp.push_back(atof(resToken));
		resToken=m_dKnotVec.Tokenize(",",curPos);
		pos++;
	}

	m_oglWindow.pNumKnots=pos;  //New number of knots
	delete[] m_oglWindow.pKnotVector;
	
	//Save new knot vector
	m_oglWindow.pKnotVector=new float[pos];

	pos=0;
	for(vector<float>::iterator it=tmp.begin();it!=tmp.end();it++)
	{
		m_oglWindow.pKnotVector[pos]=*it;
		pos++;
	}

	delete[] m_oglWindow.pIndexOfKnotsInLODs;
	delete[] m_oglWindow.pBasis;
	delete[] m_oglWindow.pBspline;
	
	//Assign space for new Basis
	m_oglWindow.pBasis=new float**[m_oglWindow.pNumKnots];
	
	for(int i=0;i<m_oglWindow.pNumKnots;i++)
		m_oglWindow.pBasis[i]=new float*[m_oglWindow.pDegree+1];
	for(int i=0;i<m_oglWindow.pNumKnots;i++)
		for(int j=0;j<m_oglWindow.pDegree+1;j++)
			m_oglWindow.pBasis[i][j]=new float[LOD];
	
	//Assign space for new BSpline
	m_oglWindow.pBspline=new point3D[LOD];
	
	//Calculate alpha only if the number of knots has changed
	if(m_oglWindow.prevNumKnots!=m_oglWindow.pNumKnots)
	{
		delete[] m_oglWindow.pCtrlPnts;
		m_oglWindow.pNumberOfCtrlPnts=pos-m_oglWindow.pDegree-1;
		m_oglWindow.pCtrlPnts=new point3D[m_oglWindow.pNumberOfCtrlPnts];

		m_oglWindow.alpha=new float**[m_oglWindow.prevNumKnots];

		for(int i=0;i<m_oglWindow.prevNumKnots;i++)
			m_oglWindow.alpha[i]=new float*[m_oglWindow.pDegree+1];
		for(int i=0;i<m_oglWindow.prevNumKnots;i++)
			for(int k=0;k<m_oglWindow.pDegree+1;k++)
				m_oglWindow.alpha[i][k]=new float[m_oglWindow.pNumKnots];

		m_oglWindow.calc_alpha(0);
		
		for(int j=0;j<m_oglWindow.pNumberOfCtrlPnts;j++)
			for(int k=0;k<3;k++)
				m_oglWindow.pCtrlPnts[j][k]=0.0;
		
		for(int j=0;j<m_oglWindow.pNumberOfCtrlPnts;j++)
			for(int i=0;i<m_oglWindow.prevNumberOfCtrlPnts;i++)
				for(int k=0;k<3;k++)
					m_oglWindow.pCtrlPnts[j][k]+=m_oglWindow.alpha[i][m_oglWindow.pDegree][j]*m_oglWindow.prevCtrlPnts[i][k];

	}
	
	//Recalculate basis and bspline (knots has changed and/or their number)
	m_oglWindow.pIndexOfKnotsInLODs=new int[m_oglWindow.pNumKnots];
	m_oglWindow.find_pBasis(0);
	m_oglWindow.calc_pBSpline();

	m_oglWindow.surfCtrlPnts=new point3D*[LOD];
	m_oglWindow.surface=new point3D*[LOD];
	m_oglWindow.surfaceNorm=new point3D*[LOD];
	for(int i=0;i<LOD;i++)
	{
		m_oglWindow.surfCtrlPnts[i]=new point3D[m_oglWindow.pNumberOfCtrlPnts];
		m_oglWindow.surface[i]=new point3D[LOD];
		m_oglWindow.surfaceNorm[i]=new point3D[LOD];
	}

	m_oglWindow.calc_surface();
	m_oglWindow.calc_surfaceNormals();
	
	this->UpdateData(FALSE);

}

//Apply degree changes
void CCG2ex1Dlg::OnBnClickedApplydeg()
{
	this->UpdateData(TRUE);
	
	if(	m_oglWindow.pDegree>m_cDegree)
	{
		MessageBox("Only degree elevation is allowed","Error",MB_ICONERROR);
		m_cDegree=m_oglWindow.pDegree;
		this->UpdateData(FALSE);
		return;
	}
	
	if(	m_oglWindow.pDegree<m_cDegree)
	{
		int raise,pos;
		vector<float> tmp;
		delete[] m_oglWindow.pBspline;
		
		raise=m_cDegree-m_oglWindow.pDegree; //How much was raised

		for(int r=0;r<raise;r++)
		{
			//Save previous knot vector and control points
			m_oglWindow.prevNumKnots=m_oglWindow.pNumKnots;
			m_oglWindow.prevKnotVector=new float[m_oglWindow.prevNumKnots];
			for(int k=0;k<m_oglWindow.prevNumKnots;k++)
				m_oglWindow.prevKnotVector[k]=m_oglWindow.pKnotVector[k];

			m_oglWindow.prevNumberOfCtrlPnts=m_oglWindow.pNumberOfCtrlPnts;
			m_oglWindow.prevCtrlPnts=new point3D[m_oglWindow.prevNumberOfCtrlPnts];
			for(int k=0;k<m_oglWindow.prevNumberOfCtrlPnts;k++)
				for(int i=0;i<3;i++)
					m_oglWindow.prevCtrlPnts[k][i]=m_oglWindow.pCtrlPnts[k][i];
			
			
			m_oglWindow.pDegree++;  //Increase degree by one
			
			delete[] m_oglWindow.pIndexOfKnotsInLODs;
			delete[] m_oglWindow.pKnotVector;
			delete[] m_oglWindow.pBasis;
			delete[] m_oglWindow.pCtrlPnts;

			tmp.push_back(m_oglWindow.prevKnotVector[0]);
			for(int k=1;k<m_oglWindow.prevNumKnots;k++)
			{
				if(m_oglWindow.prevKnotVector[k]!=m_oglWindow.prevKnotVector[k-1])
					tmp.push_back(m_oglWindow.prevKnotVector[k-1]);
				tmp.push_back(m_oglWindow.prevKnotVector[k]);
			}
			tmp.push_back(m_oglWindow.prevKnotVector[m_oglWindow.prevNumKnots-1]);

			//Save new knot vector
			m_oglWindow.pKnotVector=new float[tmp.size()];

			pos=0;
			for(vector<float>::iterator it=tmp.begin();it!=tmp.end();it++)
			{
				m_oglWindow.pKnotVector[pos]=*it;
				pos++;
			}

			m_oglWindow.pNumKnots=pos;
			m_oglWindow.pNumberOfCtrlPnts=m_oglWindow.pNumKnots-m_oglWindow.pDegree-1;
			m_oglWindow.pCtrlPnts=new point3D[m_oglWindow.pNumberOfCtrlPnts];

			m_oglWindow.pBasis=new float**[m_oglWindow.pNumKnots];
			for(int i=0;i<m_oglWindow.pNumKnots;i++)
				m_oglWindow.pBasis[i]=new float*[m_oglWindow.pDegree+1];
			for(int i=0;i<m_oglWindow.pNumKnots;i++)
				for(int j=0;j<m_oglWindow.pDegree+1;j++)
					m_oglWindow.pBasis[i][j]=new float[LOD];
			
			m_oglWindow.pIndexOfKnotsInLODs=new int[m_oglWindow.pNumKnots];
			m_oglWindow.find_pBasis(0);

			m_oglWindow.alpha=new float**[m_oglWindow.prevNumKnots];

			for(int i=0;i<m_oglWindow.prevNumKnots;i++)
				m_oglWindow.alpha[i]=new float*[m_oglWindow.pDegree+1];
			for(int i=0;i<m_oglWindow.prevNumKnots;i++)
				for(int k=0;k<m_oglWindow.pDegree+1;k++)
					m_oglWindow.alpha[i][k]=new float[m_oglWindow.pNumKnots];

			m_oglWindow.alfa=new float**[m_oglWindow.prevNumKnots];

			for(int i=0;i<m_oglWindow.prevNumKnots;i++)
				m_oglWindow.alfa[i]=new float*[m_oglWindow.pDegree];
			for(int i=0;i<m_oglWindow.prevNumKnots;i++)
				for(int k=0;k<m_oglWindow.pDegree;k++)
					m_oglWindow.alfa[i][k]=new float[m_oglWindow.pNumKnots];

			m_oglWindow.calc_alpha(0);
			m_oglWindow.calc_alfa(0);
			
			for(int j=0;j<m_oglWindow.pNumberOfCtrlPnts;j++)
				for(int k=0;k<3;k++)
					m_oglWindow.pCtrlPnts[j][k]=0.0;
			

			for(int j=0;j<m_oglWindow.pNumberOfCtrlPnts;j++)
				for(int i=0;i<m_oglWindow.prevNumberOfCtrlPnts;i++)
					for(int k=0;k<3;k++)
						m_oglWindow.pCtrlPnts[j][k]+=m_oglWindow.alfa[i][m_oglWindow.pDegree-1][j]*m_oglWindow.prevCtrlPnts[i][k];
			for(int j=0;j<m_oglWindow.pNumberOfCtrlPnts;j++)
				for(int k=0;k<3;k++)
					m_oglWindow.pCtrlPnts[j][k]/=m_oglWindow.pDegree;

			tmp.erase(tmp.begin(),tmp.end());
		
		}
		
		m_oglWindow.pBspline=new point3D[LOD];
		m_oglWindow.calc_pBSpline();
		m_oglWindow.surfCtrlPnts=new point3D*[LOD];
		m_oglWindow.surface=new point3D*[LOD];
		m_oglWindow.surfaceNorm=new point3D*[LOD];
		for(int i=0;i<LOD;i++)
		{
			m_oglWindow.surfCtrlPnts[i]=new point3D[m_oglWindow.pNumberOfCtrlPnts];
			m_oglWindow.surface[i]=new point3D[LOD];
			m_oglWindow.surfaceNorm[i]=new point3D[LOD];
		}

		m_oglWindow.calc_surface();
		m_oglWindow.calc_surfaceNormals();

		this->UpdateData(FALSE);
		return;
	}
}


// Converts a floating point number to ascii (without the appended 0's)
// Rounds the value if nNumberOfDecimalPlaces >= 0
CString CCG2ex1Dlg::FloatToText(float n, int nNumberOfDecimalPlaces)
{
    CString str;

    if(nNumberOfDecimalPlaces >= 0)
    {
        int decimal, sign;
        char *buffer = _fcvt((double)n, nNumberOfDecimalPlaces, &decimal, &sign);

        CString temp(buffer);
        
        // Sign for +ve or -ve
        if(sign != 0)
            str = "-";

        // Copy digits up to decimal point
        if(decimal <= 0)
        {
            str += "0.";
            for(; decimal < 0; decimal++)
                str += "0";
            str += temp;
        } else {
            str += temp.Left(decimal);
            str += ".";
            str += temp.Right(temp.GetLength() - decimal);
        }
    } else {
        str.Format("%-g", n);
    }

    // Remove appended zero's. "123.45000" become "123.45"
    int nFind = str.Find(".");
    if(nFind >= 0)
    {
        int nFinde = str.Find("e");    // 1.0e-010 Don't strip the ending zero
        if(nFinde < 0)
        {
            while(str.GetLength() > 1 && str.Right(1) == "0")
                str = str.Left(str.GetLength() - 1);
        }
    }

    // Remove decimal point if nothing after it. "1234." becomes "1234"
    if(str.Right(1) == ".")
        str = str.Left(str.GetLength() - 1);
    
    return str;
}

