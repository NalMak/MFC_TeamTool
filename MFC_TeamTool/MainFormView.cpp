// MainFormView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC_TeamTool.h"
#include "MainFormView.h"

#include "FileIO.h"
#include "TextureManager.h"
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
	DDX_Control(pDX, IDC_EDIT8, m_creatingObjectName);
	DDX_Control(pDX, IDC_EDIT9, m_colliderOffsetX);
	DDX_Control(pDX, IDC_EDIT10, m_colliderOffsetY);
	DDX_Control(pDX, IDC_EDIT11, m_colliderScaleX);
	DDX_Control(pDX, IDC_EDIT12, m_colliderScaleY);
	DDX_Control(pDX, IDC_LIST1, m_installedObjectList);
}

BEGIN_MESSAGE_MAP(MainFormView, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &MainFormView::OnSelChangedTree)
	ON_EN_CHANGE(IDC_EDIT13, &MainFormView::OnEnChangeTextureSearch)
	ON_CBN_SELCHANGE(IDC_COMBO1, &MainFormView::OnCbnSelchangeTexture)
	ON_BN_CLICKED(IDC_BUTTON2, &MainFormView::OnBnClickedButtonChangeScene)
	ON_BN_CLICKED(IDC_BUTTON1, &MainFormView::OnBnClickedCreateObject)
	ON_EN_CHANGE(IDC_EDIT8, &MainFormView::OnEnChangeSetName)
	ON_CBN_SELCHANGE(IDC_COMBO3, &MainFormView::OnCbnSelchangeSelectObject)
END_MESSAGE_MAP()


// MainFormView �����Դϴ�.

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


// MainFormView �޽��� ó�����Դϴ�.




void MainFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	auto frame =(CMainFrame*)(::AfxGetApp()->GetMainWnd());
	m_deviceView = (CMFC_TeamToolView*)frame->m_mainSplitter.GetPane(0, 0);

	InitTreeCtrl();
	InitTexture();
	InitCollider();
	InitTransform();
	

	/*for (auto texture : TextureManager::GetInstance()->GetTexList())
	{
		ObjectData data;
		data.texName = texture.first;
		data.name = texture.first;
		m_objectDataList.emplace_back(data);
		m_objectComboBox.AddString(texture.first.c_str());
	}*/



	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void MainFormView::InitTreeCtrl()
{
	HTREEITEM item = m_texTree.InsertItem(_T("../Texture", item));
	

	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("../Texture/*.*"));
	//  1.1.2 C����̺��� ��� ������ �ϳ��� �˻��ϸ鼭 ������ Tree�� ���������� �����Ѵ�.
	while (bWorking) {
		//   1.1.2.1 "C:\\*.*"�� NextFile(C����̺��� ù��°)���� �Ʒ� �˻縦 �����Ѵ�.
		bWorking = finder.FindNextFile();
		//   1.1.2.2 ���� �˻��ϰ� �ִ� ���� Directory�� ��츸 Tree�� ���������� �����Ѵ�.
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

	MFC_Utility::SetEditBoxFloat(&m_colliderOffsetX, 0);
	MFC_Utility::SetEditBoxFloat(&m_colliderOffsetY, 0);
	MFC_Utility::SetEditBoxFloat(&m_colliderScaleX, 1);
	MFC_Utility::SetEditBoxFloat(&m_colliderScaleY, 1);

}

void MainFormView::InitTexture()
{
	OnEnChangeTextureSearch();
	m_textureComboBox.SetCurSel(0);
}

void MainFormView::UpdateCreatingObjectInfo()
{
	auto obj = m_deviceView->GetCreatingObject();

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

	m_textureComboBox.SelectString(0,obj->data.texName);
	m_creatingObjectName.SetWindowTextW(obj->data.name);
	MFC_Utility::SetEditBoxFloat(&m_positionX, 0);
	MFC_Utility::SetEditBoxFloat(&m_positionY, 0);
	MFC_Utility::SetEditBoxFloat(&m_positionZ, 0);
	MFC_Utility::SetEditBoxFloat(&m_angle, obj->data.angle);
	MFC_Utility::SetEditBoxFloat(&m_scaleX, obj->data.scale.x);
	MFC_Utility::SetEditBoxFloat(&m_scaleY, obj->data.scale.y);
	MFC_Utility::SetEditBoxFloat(&m_scaleZ, obj->data.scale.z);
	MFC_Utility::SetEditBoxFloat(&m_colliderOffsetX, obj->data.colliderOffset.x);
	MFC_Utility::SetEditBoxFloat(&m_colliderOffsetY, obj->data.colliderOffset.y);
	MFC_Utility::SetEditBoxFloat(&m_colliderScaleX, obj->data.colliderScale.x);
	MFC_Utility::SetEditBoxFloat(&m_colliderScaleY, obj->data.colliderScale.y);
	//m_textureComboBox.set
	//m_textureComboBox.set
}

void MainFormView::OnSelChangedTree(NMHDR * pNMHDR, LRESULT * pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// 1.2 TreeView�� ������ ���콺�� Ŭ���ϸ�, �������� ����� TreeView�� �����ش�.
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem; // ���� ������ ������

	// 1.2.1 ������ �������� �������� �ִ��� ������ �˻��Ѵ�.
	// 1.2.2 ���õ� ���� ���� ���, 
	//       => �ش� �������� GetItemData()�� 0���� �ʱ�ȭ �Ǿ�����.
	if (m_texTree.GetItemData(hSelected) == 0)
	{
		// 1.2.2.1 ���õ� �������� ���� ����� Ʈ����Ʈ�ѿ� �߰��Ѵ�.
		// 1.2.2.1.1 ���õ� �������� ��θ� �о�´�. 

		CString pathSelected; //�������� ���� ��� pathSelected�� �����Ѵ�.



		HTREEITEM hParentItem = hSelected; // ���� ���õ� �������� ù��° �������� �Ѵ�.
		while (hParentItem != NULL)
		{ // �ֻ��� �θ� ���� �� ����
			pathSelected = _T("\\") + pathSelected;
			pathSelected = m_texTree.GetItemText(hParentItem) + pathSelected;
			hParentItem = m_texTree.GetParentItem(hParentItem);
		}

		// �ϼ��� ����� �ڿ� "*.*"�� �ٿ��־ �������� ���õ� �������� ��θ� �ϼ��Ѵ�.
		pathSelected = pathSelected + _T("*.*");

		// 1.2.2.1.2 CFileFind�� �̿��Ͽ� �о�� ����� ���� �Ǵ� ������ ã�´�.
		CFileFind finder;
		BOOL bWorking = finder.FindFile(pathSelected);
		// 1.2.2.1.3 ������ �������� ��������� �ϳ��� �˻��ϸ鼭 ������ ��츸 Tree ���������� �����Ѵ�. (. or ..�� ����)
		while (bWorking) {
			bWorking = finder.FindNextFile();
			if (finder.IsDots()) continue;
			if (finder.IsDirectory()) {
				m_texTree.InsertItem(finder.GetFileName(), hSelected);
			}
		}

		// 1.2.2.2 �ش� �������� ���õ� ���� ������ SetItemData() �Լ��� ǥ���Ѵ�.
		m_texTree.SetItemData(hSelected, 1);

		// 1.2.3 ���õ� ���� �ִ� ���, �ƹ� �۾��� ���� �ʴ´�. (�̹� ���� ����� Ʈ���� �߰��Ǿ� �����Ƿ�)
		// 1.2.4 ���±��� ������ Ʈ���������� TreeView�� �������� �Ѵ�.
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

	

		 
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void MainFormView::OnBnClickedButtonChangeScene()
{
	g_obejctToolMode ^= true;
	m_deviceView->Invalidate(false);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void MainFormView::OnBnClickedCreateObject()
{

	if (m_deviceView->GetCreatingObject()->data.name == L"")
	{
		ERR_MSG(L"�̸��� ������ �ּ���");
		return;
	}
	for (auto& data : m_objectDataList)
	{
		if (data.name == m_deviceView->GetCreatingObject()->data.name)
		{
			ERR_MSG(L"�̹� ������ �̸��� ������Ʈ�� �����մϴ�");
			return;
		}
	}

	CString value;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(value);
	m_deviceView->GetCreatingObject()->data.position.x = _tstof(value);
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(value);
	m_deviceView->GetCreatingObject()->data.position.y = _tstof(value);
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(value);
	m_deviceView->GetCreatingObject()->data.position.z = _tstof(value);
	GetDlgItem(IDC_EDIT4)->GetWindowTextW(value);
	m_deviceView->GetCreatingObject()->data.angle = _tstof(value);
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(value);
	m_deviceView->GetCreatingObject()->data.scale.x = _tstof(value);
	GetDlgItem(IDC_EDIT6)->GetWindowTextW(value);
	m_deviceView->GetCreatingObject()->data.scale.y = _tstof(value);
	GetDlgItem(IDC_EDIT7)->GetWindowTextW(value);
	m_deviceView->GetCreatingObject()->data.scale.z = _tstof(value);

	m_deviceView->Invalidate(false);

	m_objectDataList.emplace_back(m_deviceView->GetCreatingObject()->data);
	m_objectComboBox.AddString(m_deviceView->GetCreatingObject()->data.name);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void MainFormView::OnEnChangeSetName()
{
	CString	name;
	m_creatingObjectName.GetWindowTextW(name);
	m_deviceView->GetCreatingObject()->data.name = name;

}


void MainFormView::OnCbnSelchangeSelectObject()
{
	ObjectData newData;
	for (auto& data : m_objectDataList)
	{
		CString currentName = L"";
		m_objectComboBox.GetLBText(m_objectComboBox.GetCurSel(), currentName);
		if (data.name == currentName)
		{
			newData = data;
			break;
		}
		
		if (data.name ==  m_objectDataList.back().name)
			return;
	}


	m_deviceView->GetCreatingObject()->data = newData;
	UpdateCreatingObjectInfo();

	m_deviceView->Invalidate(false);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void MainFormView::AddInstalledObjectData(ObjectData _data)
{
	m_installedObjectList.AddString(_data.name);
}



