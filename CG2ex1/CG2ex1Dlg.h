// CG2ex1Dlg.h : header file
//
#include "OpenGLControl.h"

#pragma once

// CCG2ex1Dlg dialog
class CCG2ex1Dlg : public CDialog
{
// Construction
public:
	CCG2ex1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CG2EX1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	COpenGLControl m_oglWindow;



public:
	afx_msg void OnBnClickedOpenBtn();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedChkshwctrlpoints();

	afx_msg void OnBnClickedShowyx();
	afx_msg void OnBnClickedShowyx2();
	afx_msg void OnBnClickedShowSurf();
	
	
		
	BOOL mBshowYX;
	BOOL mBshowCoord;
	
	afx_msg void OnBnClickedBcenter();
	CString FloatToText(float n, int nNumberOfDecimalPlaces);
	
	void DataInit();
	afx_msg void OnBnClickedWireframe();
	BOOL m_BSurf;
	int m_cDegree;
	afx_msg void OnBnClickedApply();
	CString m_dKnotVec;
	afx_msg void OnBnClickedApplydeg();
	BOOL mBshowYX2;
};
