#pragma once

class CEdit;
class MFC_Utility
{
public:
	static void SetEditBoxFloat(CEdit* _edit, float _value);
	static float GetEditBoxFloat(int _id);
};

