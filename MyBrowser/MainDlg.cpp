// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
	
#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}

void CMainDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN) {
		OnBtnWebkitGo();
	} else {
		SetMsgHandled(FALSE);
	}
}

void CMainDlg::OnChromeTabNew(EventArgs *pEvt)
{
	// 页面逻辑
	EventChromeTabNew *pEvtTabNew = (EventChromeTabNew *)pEvt;
	UINT uCurTab = pEvtTabNew->iNewTab;
	/*SStringT strTitle = SStringT().Format(_T("标签页 %d"), uCurTab + 1);*/
	SStringT strTitle = L"标签页";
	pEvtTabNew->pNewTab->SetWindowText(strTitle);
	pEvtTabNew->pNewTab->SetAttribute(L"tip", S_CT2W(strTitle));
	// 新增wke对象
	SWkeWebkit *pWebKit = FindChildByName2<SWkeWebkit>(L"wke_test");
	assert(pWebKit);
	pWebKit->AddTab(uCurTab);
	// 显示默认网址，并且写入默认网址文本
	SEdit *pURLEdit = FindChildByName2<SEdit>(L"edit_url");
	assert(pURLEdit);
	pURLEdit->SetWindowText(L"http://www.baidu.com");
	pWebKit->SetAttribute(L"url", L"http://www.baidu.com", FALSE);
	pWebKit->RequestRelayout();
}

void CMainDlg::OnChromeTabSel(EventArgs *pEvt)
{
	EventChromeTabSelChanged *pEvtTabSel = (EventChromeTabSelChanged *)pEvt;

	UINT uCurTab = pEvtTabSel->iNewSel;
	SWkeWebkit *pWebKit = FindChildByName2<SWkeWebkit>(L"wke_test");
	assert(pWebKit);
	pWebKit->SetCurTab(uCurTab);
	pWebKit->RequestRelayout();
}

void CMainDlg::OnChromeTabClose(EventArgs *pEvt)
{
	EventChromeTabClose *pEvtTabClose = (EventChromeTabClose *)pEvt;

	UINT uCurTab = pEvtTabClose->iCloseTab;
	SWkeWebkit *pWebKit = FindChildByName2<SWkeWebkit>(L"wke_test");
	assert(pWebKit);
	pWebKit->SetAttribute(L"url", L"http://www.baidu.com", FALSE);
	// 这里不能直接删除对象
	pWebKit->DeleteCurTab(uCurTab);
}

HRESULT CMainDlg::OnBtnWebkitGo()
{
	SWkeWebkit *pWebKit = FindChildByName2<SWkeWebkit>(L"wke_test");
	assert(pWebKit);
	SEdit *pEdit = FindChildByName2<SEdit>(L"edit_url");
	SStringT strUrl = pEdit->GetWindowText();
	pWebKit->SetAttribute(L"url", S_CT2W(strUrl), FALSE);
	pWebKit->RequestRelayout();
	return S_OK;
}

HRESULT CMainDlg::OnBtnWebkitRefresh()
{
	SWkeWebkit *pWebkit = FindChildByName2<SWkeWebkit>(L"wke_test");
	assert(pWebkit);
	pWebkit->GetWebView()->reload();

	return S_OK;
}
HRESULT CMainDlg::OnBtnWebkitBackward()
{
	SWkeWebkit *pWebkit = FindChildByName2<SWkeWebkit>(L"wke_test");
	assert(pWebkit);
	pWebkit->GetWebView()->goBack();

	return S_OK;
}
HRESULT CMainDlg::OnBtnWebkitForward()
{
	SWkeWebkit *pWebkit = FindChildByName2<SWkeWebkit>(L"wke_test");
	assert(pWebkit);
	pWebkit->GetWebView()->goForward();

	return S_OK;
}


//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

