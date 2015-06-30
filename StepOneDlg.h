#if !defined(AFX_CSTM1DLG_H__1839AE3F_FC58_4B27_9A5B_250681C4AB75__INCLUDED_)
#define AFX_CSTM1DLG_H__1839AE3F_FC58_4B27_9A5B_250681C4AB75__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// CStepOneDlg dialog

class CStepOneDlg : public CAppWizStepDlg
{
// Construction
public:
	CStepOneDlg();
	virtual BOOL OnDismiss();

// Dialog Data
	//{{AFX_DATA(CStepOneDlg)
	enum { IDD = IDD_STEPONE };
	BOOL	m_b_empty;
	BOOL	m_b_wdf;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStepOneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStepOneDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM1DLG_H__1839AE3F_FC58_4B27_9A5B_250681C4AB75__INCLUDED_)
