#include "stdafx.h"
#include "FileIO.h"
#include <atlconv.h>
#include <io.h>
FileIO::FileIO()
{
}


FileIO::~FileIO()
{
}

int FileIO::IsFileOrDir(_wfinddata_t fd)
{
	if (fd.attrib & _A_SUBDIR)
		return 0; // 디렉토리면 0 반환
	else
		return 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환
}

void FileIO::SearchingDir(list<wstring>* fdList, const TCHAR* _path, const TCHAR* _extention)
{
	wstring defaultPath = _path;
	wstring path = defaultPath + L"/*.*";
	wstring extention = _extention;
	int checkDirFile = 0;
	struct _wfinddata_t fd;
	intptr_t handle;


	//fd 구조체 초기화.
	if ((handle = _wfindfirst(path.c_str(), &fd)) == -1L)
	{
		//파일이나 디렉토리가 없을 경우.
		cout << "No file in directory!" << endl;
		return;
	}
	do //폴더 탐색 반복 시작
	{
		//현재 객체 종류 확인(파일 or 디렉토리)
		checkDirFile = IsFileOrDir(fd);
		if (checkDirFile == 0 && fd.name[0] != '.') {
			//디렉토리일 때의 로직
			//cout << "Dir  : " << _path << "/" << fd.name << endl;
			SearchingDir(fdList, (defaultPath + L"/" + fd.name).c_str(), extention.c_str());//재귀적 호출

		}
		else if (checkDirFile == 1 && fd.name[0] != '.') {
			//파일일 때의 로직
			//cout << "File : " << path << "/" << fd.name << endl;
			wstring fileName = fd.name;
			wstring fileFormat = fileName.substr(fileName.find_last_of(L".") + 1);

			if (fileFormat == _extention)
			{
				wstring temp = defaultPath + L"/" + fileName;

				fdList->emplace_back(temp.c_str());
			}
		}
	} while (_wfindnext(handle, &fd) == 0);
	_findclose(handle);
}
