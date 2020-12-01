#include "stdafx.h"
#include "MFC_Utility.h"

void MFC_Utility::SetEditBoxFloat(CEdit * _edit, float _value)
{
	CString str;
	str.Format(L"%f", _value);
	_edit->SetWindowTextW(str);
}

float MFC_Utility::GetEditBoxFloat(int _id)
{
	/*GetDlgItem(_id)->
	float fValue = _tstof(value);

	return ;*/
	return 0;
}
