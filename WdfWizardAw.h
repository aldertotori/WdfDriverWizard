#if !defined(AFX_WDFWIZARDAW_H__7E18938D_EF11_4ACD_AF85_7E93884830E8__INCLUDED_)
#define AFX_WDFWIZARDAW_H__7E18938D_EF11_4ACD_AF85_7E93884830E8__INCLUDED_

// WdfWizardaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see WdfWizard.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CWdfWizardAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
	virtual CAppWizStepDlg* Back(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);

protected:
	CDialogChooser* m_pChooser;
};

// This declares the one instance of the CWdfWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global WdfWizardaw.  (Its definition is in WdfWizardaw.cpp.)
extern CWdfWizardAppWiz WdfWizardaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDFWIZARDAW_H__7E18938D_EF11_4ACD_AF85_7E93884830E8__INCLUDED_)
