#pragma once

#define WINCX 1280
#define WINCY 720

#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr;}
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr;}

#ifdef _AFX
#define ERR_MSG(msg) AfxMessageBox(msg)
#else
#define ERR_MSG(msg) MessageBox(nullptr, msg, L"System_Error", MB_OK)
#endif // _AFX