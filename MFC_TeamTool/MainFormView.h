#pragma once



// MainFormView 폼 뷰입니다.
class CMFC_TeamToolView;
class GameObject;
struct ObjectData;
class MainFormView : public CFormView
{
	DECLARE_DYNCREATE(MainFormView)

protected:
	MainFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~MainFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_texTree;
	virtual void OnInitialUpdate();
private:
	void InitTreeCtrl();
	void InitCollider();
	void InitTransform();
	void InitTexture();
	void UpdateCreatingObjectInfo();
private:
	afx_msg void OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult);

public:
	afx_msg void OnEnChangeTextureSearch();
	CComboBox m_textureComboBox;
	CEdit m_texSearchEditBox;
	CComboBox m_colliderType;
	CEdit m_positionX;
	CEdit m_positionY;
	CEdit m_positionZ;
	CEdit m_angle;
	CEdit m_scaleX;
	CEdit m_scaleY;
	CEdit m_scaleZ;


	afx_msg void OnCbnSelchangeTexture();
private:
	CMFC_TeamToolView* m_deviceView;
public:
	afx_msg void OnBnClickedButtonChangeScene();
	CComboBox m_objectComboBox;

private:
	list<ObjectData> m_objectDataList;

public:
	afx_msg void OnBnClickedCreateObject();
	afx_msg void OnEnChangeSetName();
	CEdit m_creatingObjectName;
	afx_msg void OnCbnSelchangeSelectObject();
	CEdit m_colliderOffsetX;
	CEdit m_colliderOffsetY;
	CEdit m_colliderScaleX;
	CEdit m_colliderScaleY;

private:
	CListBox m_installedObjectList;

public:
	void AddInstalledObjectData(ObjectData _data);
	afx_msg void OnEnChangeEdit1();
};


