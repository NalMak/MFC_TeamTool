// MainFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_TeamTool.h"
#include "MainFormView.h"

#include "FileIO.h"
#include "TextureManager.h"
#include "MFC_Utility.h"
#include "MainFrm.h"
#include "MFC_TeamToolView.h"
#include "GameObject.h"
#include "GameObjectManager.h"
// MainFormView

IMPLEMENT_DYNCREATE(MainFormView, CFormView)

MainFormView::MainFormView()
	: CFormView(IDD_MAINFORMVIEW)
{

}

MainFormView::~MainFormView()
{
}

void MainFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_texTree);
	DDX_Control(pDX, IDC_COMBO1, m_textureComboBox);
	DDX_Control(pDX, IDC_EDIT13, m_texSearchEditBox);
	DDX_Control(pDX, IDC_COMBO2, m_colliderType);
	DDX_Control(pDX, IDC_EDIT1, m_positionX);
	DDX_Control(pDX, IDC_EDIT2, m_positionY);
	DDX_Control(pDX, IDC_EDIT3, m_positionZ);
	DDX_Control(pDX, IDC_EDIT4, m_angle);
	DDX_Control(pDX, IDC_EDIT5, m_scaleX);
	DDX_Control(pDX, IDC_EDIT6, m_scaleY);
	DDX_Control(pDX, IDC_EDIT7, m_scaleZ);
	DDX_Control(pDX, IDC_COMBO3, m_objectComboBox);
}

BEGIN_MESSAGE_MAP(MainFormView, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &MainFormView::OnSelChangedTree)
	ON_EN_CHANGE(IDC_EDIT13, &MainFormView::OnEnChangeTextureSearch)
	ON_CBN_SELCHANGE(IDC_COMBO1, &MainFormView::OnCbnSelchangeTexture)
	ON_BN_CLICKED(IDC_BUTTON2, &MainFormView::OnBnClickedButtonChangeScene)
END_MESSAGE_MAP()


// MainFormView 진단입니다.

#ifdef _DEBUG
void MainFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MainFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MainFormView 메시지 처리기입니다.




void MainFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	auto frame =(CMainFrame*)(::AfxGetApp()->GetMainWnd());
	m_deviceView = (CMFC_TeamToolView*)frame->m_mainSplitter.GetPane(0, 0);

	InitTreeCtrl();
	InitTexture();
	InitCollider();
	InitTransform();
	

	for (auto texture : TextureManager::GetInstance()->GetTexList())
	{
		ObjectData data;
		data.texName = texture.first;
		data.name = texture.first;
		m_objectDataList.emplace_back(data);
		m_objectComboBox.AddString(texture.first.c_str());
	}



	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void MainFormView::InitTreeCtrl()
{
	HTREEITEM item = m_texTree.InsertItem(_T("../Texture", item));
	

	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("../Texture/*.*"));
	//  1.1.2 C드라이브의 모든 파일을 하나씩 검사하면서 폴더만 Tree의 아이템으로 삽입한다.
	while (bWorking) {
		//   1.1.2.1 "C:\\*.*"의 NextFile(C드라이브의 첫번째)부터 아래 검사를 시작한다.
		bWorking = finder.FindNextFile();
		//   1.1.2.2 현재 검사하고 있는 것이 Directory일 경우만 Tree의 아이템으로 삽입한다.
		if (finder.IsDirectory()) {
			m_texTree.InsertItem(finder.GetFileName(), item);
		}
	}

	m_texTree.EnsureVisible(item);
}

void MainFormView::InitCollider()
{
	m_colliderType.AddString(L"None");
	m_colliderType.AddString(L"Circle");
	m_colliderType.AddString(L"Rect");

	m_colliderType.SetCurSel(1);
}

void MainFormView::InitTransform()
{
	MFC_Utility::SetEditBoxFloat(&m_positionX, 0);
	MFC_Utility::SetEditBoxFloat(&m_positionY, 0);
	MFC_Utility::SetEditBoxFloat(&m_positionZ, 0);
	MFC_Utility::SetEditBoxFloat(&m_angle, 0);
	MFC_Utility::SetEditBoxFloat(&m_scaleX, 1);
	MFC_Utility::SetEditBoxFloat(&m_scaleY, 1);
	MFC_Utility::SetEditBoxFloat(&m_scaleZ, 1);


}

void MainFormView::InitTexture()
{
	OnEnChangeTextureSearch();
}

void MainFormView::OnSelChangedTree(NMHDR * pNMHDR, LRESULT * pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// 1.2 TreeView의 폴더를 마우스로 클릭하면, 하위폴더 목록을 TreeView에 보여준다.
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem; // 현재 선택한 아이템

	// 1.2.1 선택한 아이템이 선택적이 있는지 없는지 검사한다.
	// 1.2.2 선택된 적이 없는 경우, 
	//       => 해당 아이템의 GetItemData()는 0으로 초기화 되어있음.
	if (m_texTree.GetItemData(hSelected) == 0)
	{
		// 1.2.2.1 선택된 아이템의 하위 목록을 트리컨트롤에 추가한다.
		// 1.2.2.1.1 선택된 아이템의 경로를 읽어온다. 

		CString pathSelected; //최종으로 얻어올 경로 pathSelected를 선언한다.



		HTREEITEM hParentItem = hSelected; // 현재 선택된 아이템을 첫번째 시작으로 한다.
		while (hParentItem != NULL)
		{ // 최상의 부모가 없을 때 까지
			pathSelected = _T("\\") + pathSelected;
			pathSelected = m_texTree.GetItemText(hParentItem) + pathSelected;
			hParentItem = m_texTree.GetParentItem(hParentItem);
		}

		// 완성된 경로의 뒤에 "*.*"를 붙여주어서 최종으로 선택된 아이템의 경로를 완성한다.
		pathSelected = pathSelected + _T("*.*");

		// 1.2.2.1.2 CFileFind를 이용하여 읽어온 경로의 파일 또는 폴더를 찾는다.
		CFileFind finder;
		BOOL bWorking = finder.FindFile(pathSelected);
		// 1.2.2.1.3 선택한 아이템의 하위목록을 하나씩 검사하면서 폴더의 경우만 Tree 아이템으로 삽입한다. (. or ..는 예외)
		while (bWorking) {
			bWorking = finder.FindNextFile();
			if (finder.IsDots()) continue;
			if (finder.IsDirectory()) {
				m_texTree.InsertItem(finder.GetFileName(), hSelected);
			}
		}

		// 1.2.2.2 해당 아이템은 선택된 적이 있음을 SetItemData() 함수로 표시한다.
		m_texTree.SetItemData(hSelected, 1);

		// 1.2.3 선택된 적이 있는 경우, 아무 작업도 하지 않는다. (이미 하위 목록은 트리에 추가되어 있으므로)
		// 1.2.4 여태까지 삽입한 트리아이템을 TreeView에 보여지게 한다.
		m_texTree.EnsureVisible(hSelected);
	}

	*pResult = 0;
}


void MainFormView::OnEnChangeTextureSearch()
{
	m_textureComboBox.ResetContent();
	CString tok = L"";
	GetDlgItemText(IDC_EDIT13, tok);
	if (tok == L"")
	{
		for (auto& tex : TextureManager::GetInstance()->GetTexList())
		{
			CString target = tex.first.c_str();
		
			m_textureComboBox.AddString(target);
		}
	}
	else
	{
		for (auto& tex : TextureManager::GetInstance()->GetTexList())
		{
			CString target = tex.first.c_str();
			int index = target.Find(tok);

			if (index != -1)
			{
				m_textureComboBox.AddString(target);
			}
		}
	}

	

		 
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MainFormView::OnCbnSelchangeTexture()
{
	UpdateData(TRUE);

	CString texName = L"";
	m_textureComboBox.GetLBText(m_textureComboBox.GetCurSel(),texName);
	m_deviceView->GetCreatingObject()->data.texName = texName;
	Invalidate(false);
	m_deviceView->Invalidate(false);

	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MainFormView::OnBnClickedButtonChangeScene()
{
	g_obejctToolMode ^= true;
	m_deviceView->Invalidate(false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
