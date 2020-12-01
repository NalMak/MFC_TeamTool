
// MFC_TeamToolView.h : CMFC_TeamToolView Ŭ������ �������̽�
//

#pragma once

class CMFC_TeamToolDoc;
class GameObject;
class CMFC_TeamToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC_TeamToolView();
	DECLARE_DYNCREATE(CMFC_TeamToolView)

// Ư���Դϴ�.
public:
	CMFC_TeamToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFC_TeamToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

private:
	GameObject* m_creatingObject;
public:
	GameObject* GetCreatingObject() { return m_creatingObject; }
};

#ifndef _DEBUG  // MFC_TeamToolView.cpp�� ����� ����
inline CMFC_TeamToolDoc* CMFC_TeamToolView::GetDocument() const
   { return reinterpret_cast<CMFC_TeamToolDoc*>(m_pDocument); }
#endif

