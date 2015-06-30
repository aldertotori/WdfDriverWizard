// cstm1dlg.cpp : implementation file
//

#include "stdafx.h"
#include "WdfWizard.h"
#include "StepOnedlg.h"
#include "WdfWizardaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStepOneDlg dialog

CStepOneDlg::CStepOneDlg()
	: CAppWizStepDlg(CStepOneDlg::IDD)
{
	//{{AFX_DATA_INIT(CStepOneDlg)
	m_b_empty = FALSE;
	m_b_wdf = FALSE;
	//}}AFX_DATA_INIT
}

void CStepOneDlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStepOneDlg)
	DDX_Check(pDX, IDC_CHECK_EMPTY, m_b_empty);
	DDX_Check(pDX, IDC_CHECK_WDF, m_b_wdf);
	//}}AFX_DATA_MAP
}

// This is called whenever the user presses Next, Back, or Finish with this step
//  present.  Do all validation & data exchange from the dialog in this function.
BOOL CStepOneDlg::OnDismiss()
{
	if (!UpdateData(TRUE))
		return FALSE;

	if(m_b_wdf)
	{
		WdfWizardaw.m_Dictionary.SetAt("USE_WDF","Yes");
	}
	else
	{	
		WdfWizardaw.m_Dictionary.RemoveKey("USE_WDF");
	}

	if(m_b_empty)
	{
		WdfWizardaw.m_Dictionary.SetAt("EMPTY_PROJECT","Yes");
	}
	else
	{
		WdfWizardaw.m_Dictionary.RemoveKey("EMPTY_PROJECT");
	}

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed
}

BEGIN_MESSAGE_MAP(CStepOneDlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CStepOneDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStepOneDlg message handlers

BOOL CStepOneDlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();
	
	m_b_wdf = TRUE;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
