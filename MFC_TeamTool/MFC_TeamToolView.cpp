
// MFC_TeamToolView.cpp : CMFC_TeamToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_TeamTool.h"
#endif

#include "MFC_TeamToolDoc.h"
#include "MFC_TeamToolView.h"
#include "MainFrm.h"

#include "TextureManager.h"
#include "DeviceManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
bool g_obejctToolMode = true;
// CMFC_TeamToolView

IMPLEMENT_DYNCREATE(CMFC_TeamToolView, CView)

BEGIN_MESSAGE_MAP(CMFC_TeamToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC_TeamToolView 생성/소멸

CMFC_TeamToolView::CMFC_TeamToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC_TeamToolView::~CMFC_TeamToolView()
{
	SAFE_DELETE(m_creatingObject);
}

BOOL CMFC_TeamToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC_TeamToolView 그리기

void CMFC_TeamToolView::OnDraw(CDC* /*pDC*/)
{
	
	

	CMFC_TeamToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DeviceManager::GetInstance()->RenderBegin();

	if (!g_obejctToolMode)
		GameObjectManager::GetInstance()->Render();
	else
		m_creatingObject->Render();

	DeviceManager::GetInstance()->RenderEnd();


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFC_TeamToolView 인쇄

BOOL CMFC_TeamToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC_TeamToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC_TeamToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC_TeamToolView 진단

#ifdef _DEBUG
void CMFC_TeamToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_TeamToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_TeamToolDoc* CMFC_TeamToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_TeamToolDoc)));
	return (CMFC_TeamToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_TeamToolView 메시지 처리기


void CMFC_TeamToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	
	CMainFrame* mainFrame = (CMainFrame*)::AfxGetApp()->GetMainWnd();
	RECT frameRect = {};
	RECT thisRect = {};
	mainFrame->GetWindowRect(&frameRect);
	GetClientRect(&thisRect);

	mainFrame->SetWindowPos(nullptr,
		0, 0,
		WINCX + frameRect.right - frameRect.left - thisRect.right,
		WINCY + frameRect.bottom - frameRect.top - thisRect.bottom,
		SWP_NOSIZE | SWP_NOMOVE);



	g_hWnd = m_hWnd;

	DeviceManager::GetInstance()->Initialize();
	TextureManager::GetInstance()->LoadTexture(L"jpg");
	TextureManager::GetInstance()->LoadTexture(L"png");



	m_creatingObject = new GameObject;
	m_creatingObject->data.position = D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

