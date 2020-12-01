#pragma once
class FileIO
{
public:
	FileIO();
	~FileIO();
private:
	static int IsFileOrDir(_wfinddata_t fd);
public:
	static void SearchingDir(list <wstring> * fdList, const TCHAR* _path, const TCHAR* _extention);

};

