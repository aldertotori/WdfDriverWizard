// WdfWizardaw.cpp : implementation file
//

#include "stdafx.h"
#include "WdfWizard.h"
#include "WdfWizardaw.h"
#include "chooser.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This is called immediately after the custom AppWizard is loaded.  Initialize
//  the state of the custom AppWizard here.
void CWdfWizardAppWiz::InitCustomAppWiz()
{
	// Create a new dialog chooser; CDialogChooser's constructor initializes
	//  its internal array with pointers to the steps.
	m_pChooser = new CDialogChooser;

	// Set the maximum number of steps.
	SetNumberOfSteps(LAST_DLG);

	// TODO: Add any other custom AppWizard-wide initialization here.
}

// This is called just before the custom AppWizard is unloaded.
void CWdfWizardAppWiz::ExitCustomAppWiz()
{
	// Deallocate memory used for the dialog chooser
	ASSERT(m_pChooser != NULL);
	delete m_pChooser;
	m_pChooser = NULL;

	// TODO: Add code here to deallocate resources used by the custom AppWizard
}

// This is called when the user clicks "Create..." on the New Project dialog
//  or "Next" on one of the custom AppWizard's steps.
CAppWizStepDlg* CWdfWizardAppWiz::Next(CAppWizStepDlg* pDlg)
{
	// Delegate to the dialog chooser
	return m_pChooser->Next(pDlg);
}

// This is called when the user clicks "Back" on one of the custom
//  AppWizard's steps.
CAppWizStepDlg* CWdfWizardAppWiz::Back(CAppWizStepDlg* pDlg)
{
	// Delegate to the dialog chooser
	return m_pChooser->Back(pDlg);
}

void CWdfWizardAppWiz::CustomizeProject(IBuildProject* pProject)
{
	BOOL		bDebug = FALSE;
	CComPtr<IConfigurations> pConfigs;
	HRESULT hr=pProject->get_Configurations(&pConfigs);
	if(FAILED(hr))
	{
		AfxMessageBox("An error occurred while obtaining the IConfigurations interface pointer");
		return;
	}
	CComPtr<IConfiguration> pConfig;
	CComVariant index;
	VARIANT dummy = {0};
	CComBSTR Name;
	CString text;
	CString output;

	long Count=0;
	pConfigs->get_Count(&Count);

	// Iterate through all the configurations of the project
	for(int i=1; i <= Count; i++)
	{
		index=i;
		hr=pConfigs->Item(index, &pConfig);
		if(FAILED(hr))
		{
			AfxMessageBox("An error occurred while obtaining the IConfiguration pointer");
			return;
		}
		pConfig->get_Name(&Name);
		text = Name;
		
		if (text.Find("Debug") == -1)
		{
			bDebug = FALSE;
			output = "Release";
		}
		else
		{
			bDebug = TRUE;
			output = "Debug";
		}

		pConfig->AddToolSettings(L"mfc", L"0", dummy);

		//pConfig->AddToolSettings(L"link.exe", L"/nodefaultlib:\"MSVCRTD\"", dummy);

		// remove precompile headers
		pConfig->RemoveToolSettings(L"cl.exe",L"/Yu\"stdafx.h\"", dummy);
		pConfig->RemoveFileSettings(L"StdAfx.cpp",L"/Yc\"stdafx.h\"", dummy);
		pConfig->RemoveToolSettings(L"cl.exe",L"/GS", dummy);
		pConfig->RemoveToolSettings(L"cl.exe",L"/RTC1", dummy);
		pConfig->RemoveToolSettings(L"cl.exe",L"/RTCc", dummy);
		pConfig->RemoveToolSettings(L"cl.exe",L"/RTCs", dummy);
		pConfig->RemoveToolSettings(L"cl.exe",L"/RTCu", dummy);

		// change the preprocessor definitions
		pConfig->AddToolSettings(L"cl.exe", L"/D \"NOPCH\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/EHa", dummy);

		pConfig->AddToolSettings(L"cl.exe", L"/nologo", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_WINDOWS\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_X86_=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"i386=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"STD_CALL\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"CONDITION_HANDLING=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"NT_UP=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"NT_INST=0\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"WIN32=100\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_NT1X_=100\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"WINNT=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_WIN32_WINNT=0x0500\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_WIN32_IE=0x0400\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"WIN32_LEAN_AND_MEAN=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"DEVL=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"FPO=0\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_DLL=1\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"DRIVER\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_IDWBUILD\"", dummy);

		pConfig->AddToolSettings(L"cl.exe", L"/I$(WDF_ROOT)\\inc", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/I$(WDF_ROOT)\\crt", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/I$(DDKPATH)\\inc", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/I$(DDKPATH)\\inc\\ddk", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/I$(DDKPATH)\\inc\\ddk\\wdm", dummy);
		
		pConfig->AddToolSettings(L"cl.exe", L"/FR", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/YX", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/FD", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/Zel", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"-cbstring", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/QIfdiv-", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/QI0f", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/GF", dummy);
		pConfig->AddToolSettings(L"cl.exe",L"/Gz", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/W3", dummy);

		if(bDebug)
		{
			// /D "RDRDBG" /D "SRVDBG" /D DBG=1 /FR /YX /FD /Zel -cbstring /QIfdiv- /QI0f /GF /QIf /c
			pConfig->AddToolSettings(L"cl.exe", L"/D \"DBG=1\"", dummy);
			pConfig->AddToolSettings(L"cl.exe", L"/D \"RDRDBG\"", dummy);
			pConfig->AddToolSettings(L"cl.exe", L"/D \"SRVDBG\"", dummy);

			pConfig->AddToolSettings(L"cl.exe", L"/QIf", dummy);

			pConfig->AddToolSettings(L"cl.exe", L"/Z7", dummy);
			pConfig->AddToolSettings(L"cl.exe", L"/Oi", dummy);
			pConfig->AddToolSettings(L"cl.exe", L"/Gy", dummy);

		}
		else
		{
			pConfig->AddToolSettings(L"cl.exe", L"/D \"NDEBUG\"", dummy);
			pConfig->AddToolSettings(L"cl.exe", L"/D \"DBG=0\"", dummy);

			pConfig->AddToolSettings(L"cl.exe", L"/Oxs", dummy);
			pConfig->AddToolSettings(L"cl.exe", L"/O2", dummy);

		}

		pConfig->RemoveToolSettings(L"cl.exe",L"/GX", dummy);
		pConfig->RemoveToolSettings(L"cl.exe",L"/GZ", dummy);

		pConfig->AddToolSettings(L"cl.exe", L"/FI \"$(DDKPATH)\\inc\\warning.h\"", dummy);

		pConfig->RemoveToolSettings(L"link.exe", L"kernel32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"user32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"gdi32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"winspool.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"comdlg32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"advapi32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"shell32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"ole32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"oleaut32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"uuid.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"odbc32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"odbccp32.lib", dummy);
		
		text.Format("/out:\"%s/%s.sys\"",output,m_Dictionary["Root"]);
		pConfig->AddToolSettings(L"link.exe", text.AllocSysString(), dummy);

		// Change the libraries
		pConfig->AddToolSettings(L"link.exe", L"ntoskrnl.lib", dummy);
		pConfig->AddToolSettings(L"link.exe", L"hal.lib", dummy);

		pConfig->AddToolSettings(L"link.exe", L"/nologo", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/base:\"0x10000\"", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/version:5.0", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/subsystem:windows", dummy);

		pConfig->AddToolSettings(L"link.exe", L"/pdb:none", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/machine:I386", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/nodefaultlib", dummy);

		if(bDebug)
		{
			pConfig->AddToolSettings(L"link.exe", L"/libpath:\"$(DDKPATH)\\libchk\\i386\"", dummy);
			pConfig->AddToolSettings(L"link.exe", L"/debug", dummy);
			pConfig->AddToolSettings(L"link.exe", L"/debugtype:both", dummy);
			pConfig->AddToolSettings(L"link.exe", L"-debug:notmapped,FULL", dummy);
		}
		else
		{
			pConfig->AddToolSettings(L"link.exe", L"/libpath:\"$(DDKPATH)\\libfre\\i386\"", dummy);
			pConfig->AddToolSettings(L"link.exe", L"-debug:notmapped,minimal", dummy);
		}

		pConfig->AddToolSettings(L"link.exe", L"-MERGE:_PAGE=PAGE", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-MERGE:_TEXT=.text", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-MERGE:.rdata=.text", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-SECTION:INIT,d", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-OPT:REF", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-FORCE:MULTIPLE", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-RELEASE", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-FULLBUILD", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-IGNORE:4001,4037,4039,4049,4065,4070,4078,4087,4089,4096", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-osversion:5.00", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-optidata", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-driver", dummy);
		pConfig->AddToolSettings(L"link.exe", L"-align:0x20", dummy);	
		pConfig->AddToolSettings(L"link.exe", L"-subsystem:native,5.00", dummy);
		
		CString cstr;
		if(m_Dictionary.Lookup(TEXT("USE_WDF"),cstr))
		{
			pConfig->AddToolSettings(L"link.exe", L"WdfDriverEntry.lib", dummy);
			pConfig->AddToolSettings(L"link.exe", L"WdfDynam.lib", dummy);
			pConfig->AddToolSettings(L"link.exe", L"WdfLdr.lib", dummy);
			pConfig->AddToolSettings(L"link.exe", L"/entry:\"FxDriverEntry@8\"", dummy);
			pConfig->AddToolSettings(L"link.exe", L"/libpath:\"$(WDF_ROOT)\\lib\\i386\"", dummy);
		}
		else
		{	
			pConfig->AddToolSettings(L"link.exe", L"/entry:\"DriverEntry@8\"", dummy);
		}

		pConfig=NULL;
	}
	pConfigs=NULL;
}


// Here we define one instance of the CWdfWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global WdfWizardaw.
CWdfWizardAppWiz WdfWizardaw;

