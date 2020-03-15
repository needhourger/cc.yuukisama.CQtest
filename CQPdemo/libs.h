#pragma once
//#include<Windows.h>
#include<string>
#include<shellapi.h>
#include<tchar.h>
#include<vector>
#include<cstdlib>
#include<ctime>
#pragma comment(lib,"7zra.lib")
using namespace std;
#define W 651

#define charset "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_#*%"


string generatePass(const string key) {
	srand((unsigned int)time(0));
	string ret = key;
	unsigned short len = rand() % 5 + 10;
	for (int i = 0; i <= len; i++) {
		ret.append(1, charset[rand() % sizeof(charset)]);
	}
	return ret;
}


string UTF8ToGBK(const char* str)
{
    string result;
    WCHAR *strSrc;
    LPSTR szRes;

    //获得临时变量的大小
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    //获得临时变量的大小
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[]strSrc;
    delete[]szRes;

    return result;
}

/*将传入文本转换为二维码存储带对应文件中*/
void QRTextConvate(string words,string filename) {
    SHELLEXECUTEINFO shellinfo;

    string temp = words + " " + filename+" None";
    size_t size = temp.length();
    wchar_t* buffer = new wchar_t[size + 1];
    MultiByteToWideChar(CP_ACP, 0, temp.c_str(), size, buffer, size * sizeof(wchar_t));
    buffer[size] = 0;
    //MessageBox(GetDesktopWindow(), buffer, _T("url"), MB_OK);

    //ZeroMemory(&shellinfo, sizeof(shellinfo));
	std::memset(&shellinfo, 0, sizeof(shellinfo));
	shellinfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shellinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellinfo.hwnd = GetDesktopWindow();
    shellinfo.lpVerb = _T("open");
    shellinfo.lpFile = _T("QR.exe");
    shellinfo.lpParameters = buffer;
    shellinfo.lpDirectory = _T(".\\CQtestlib");
    shellinfo.nShow = SW_HIDE;
    shellinfo.hInstApp = NULL;

    BOOL ret = ShellExecuteEx(&shellinfo);
    WaitForSingleObject(shellinfo.hProcess, INFINITE);
    delete buffer;
    return;
}

string AT(int64_t qq) {
	return "[CQ:at,qq=" + to_string(qq) + "]";
}

string qq2word(int64_t qq) {
	/*string temp = to_string(qq);
	for (int i = 0; i < temp.length(); i++) 
		temp[i] = temp[i] * 2 + 1;*/
	return to_string(qq);
}


/*分割字符串*/
vector<std::string> split(std::string str,std::string dim=" ") {
	vector<std::string> ret;
	if (str == "") return ret;
	std::string strs = str + dim;
	size_t pos = strs.find(dim);
	while (pos != std::string::npos) {
		string temp = strs.substr(0, pos);
		ret.push_back(temp);
		strs = strs.substr(pos + dim.length(), strs.size());
		pos = strs.find(dim);
	}
	return ret;
}





//UTF-8转Unicode 
std::wstring Utf82Unicode(const std::string& utf8string) {
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}
	return std::wstring(&resultstring[0]);
}


//unicode 转为 ascii 
std::string WideByte2Acsi(std::wstring& wstrcode) {
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
	if (convresult != asciisize)
	{
		throw std::exception("La falla!");
	}
	return std::string(&resultstring[0]);
}



//utf-8 转 ascii 
std::string UTF_82ASCII(std::string& strUtf8Code) {
	using namespace std;
	string strRet = "";
	//先把 utf8 转为 unicode 
	wstring wstr = Utf82Unicode(strUtf8Code);
	//最后把 unicode 转为 ascii 
	strRet = WideByte2Acsi(wstr);
	return strRet;
}



//ascii 转 Unicode 
std::wstring Ascii2WideByte(std::string& strascii) {
	using namespace std;
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}
	return std::wstring(&resultstring[0]);
}


//Unicode 转 Utf8 
std::string Unicode2Utf8(const std::wstring& widestring) {
	using namespace std;
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(utf8size);
	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);
	if (convresult != utf8size)
	{
		throw std::exception("La falla!");
	}
	return std::string(&resultstring[0]);
}


//ascii 转 Utf8 
std::string ASCII2UTF_8(std::string& strAsciiCode) {
	using namespace std;
	string strRet("");
	//先把 ascii 转为 unicode 
	wstring wstr = Ascii2WideByte(strAsciiCode);
	//最后把 unicode 转为 utf8 
	strRet = Unicode2Utf8(wstr);
	return strRet;
}


bool CQtestlibExec(std::string tArgs, std::string pName) {
	bool ret;
	SHELLEXECUTEINFO shellinfo;
	wstring args = Ascii2WideByte(tArgs);
	wstring program = Ascii2WideByte(pName);
	//ZeroMemory(&shellinfo, sizeof(shellinfo));
	std::memset(&shellinfo, 0, sizeof(shellinfo));
	shellinfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shellinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shellinfo.hwnd = GetDesktopWindow();
	shellinfo.lpVerb = _T("open");
	shellinfo.lpFile = program.c_str();
	shellinfo.lpParameters = args.c_str();
	shellinfo.lpDirectory = _T(".\\CQtestlib");
	shellinfo.nShow = SW_HIDE;
	shellinfo.hInstApp = NULL;

	ret = ShellExecuteEx(&shellinfo);
	WaitForSingleObject(shellinfo.hProcess, INFINITE);
	return ret;
}

void debug_win(std::string temp) {
	/*int size;
	wchar_t* buffer = NULL;
	size = temp.length();
	buffer = new wchar_t[size + 1];
	MultiByteToWideChar(CP_ACP, 0, temp.c_str(), size, buffer, size * sizeof(wchar_t));
	buffer[size] = '\0';*/
	MessageBox(GetDesktopWindow(), Ascii2WideByte(temp).c_str(), _T("debug"), MB_OK);
	/*delete buffer;*/
	return;
}

std::string randSuffix() {
	srand(time(NULL));
	std::string ret = "";

	for (int i = 0; i < 32; i++) {
		switch (rand()%3)
		{
		case 1:
			ret.append(1, (char)('A' + rand() % 26));
			break;
		case 2:
			ret.append(1, (char)('a' + rand() % 26));
			break;
		default:
			ret.append(1, (char)('0' + rand() % 10));
			break;
		}
	}
	return ret;
}