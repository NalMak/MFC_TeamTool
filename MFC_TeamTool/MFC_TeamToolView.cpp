
// MFC_TeamToolView.cpp : CMFC_TeamToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC_TeamToolView ����/�Ҹ�

CMFC_TeamToolView::CMFC_TeamToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC_TeamToolView::~CMFC_TeamToolView()
{
	SAFE_DELETE(m_creatingObject);
}

BOOL CMFC_TeamToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC_TeamToolView �׸���

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


	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFC_TeamToolView �μ�

BOOL CMFC_TeamToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC_TeamToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC_TeamToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFC_TeamToolView ����

#ifdef _DEBUG
void CMFC_TeamToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_TeamToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_TeamToolDoc* CMFC_TeamToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_TeamToolDoc)));
	return (CMFC_TeamToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_TeamToolView �޽��� ó����


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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

