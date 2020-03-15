/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include "libs.h"
#include "qrencode.h"
#include "sql.h"
#include "static_msg.h"

#include <shellapi.h>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include <map>
#pragma comment(lib,"qrencode.lib")


using namespace std;
using namespace sql;

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;
bool flag = false;
std::string SAVE_PATH;
std::string url_base;
std::string suffix;

map<std::string, std::string> filmlist;
clock_t preMsg = clock();
clock_t nowMsg;
int64_t preQQ;
short msgCount;



/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {
    ifstream csv(".\\film.csv");
    if (csv.is_open()) {
        csv.close();
        return 0;
    }

    wfstream file(".\\film.csv",ios::out);
    if (file.is_open()) {
        file << "电影名称" << "," << "网盘链接" << endl;
        file.close();
    }
    else {
        MessageBox(GetDesktopWindow(), _T("未能成功创建csv文件"), _T("警告"),MB_OK);
        file.close();
    }

    return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {
	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
    filmlist.clear();
	
	sql::User_table_init();
	sql::Notice_table_init();
	sql::Groups_init();
	sql::language_init();
	sql::admin_init();
	sql::Material_table_init();
	sql::Materual_record_table_init();

    fstream csv(".\\film.csv", ios::in);
    if (!csv.is_open()) {
        MessageBox(NULL, _T("未能找到film.csv，请检查文件完整性"), _T("WARNING"), MB_OK);
        return 0;
    }

    std::string line;
    std::string film, link;
    getline(csv, line);
    while (getline(csv, line)) {
        std::stringstream ss(line);
        getline(ss, film, ',');
        getline(ss, link, ',');
        filmlist.insert(pair<std::string, std::string>(film, link));
    }
    csv.close();

    fstream setting(".\\setting",ios::in);
    if (!setting.is_open()) {
        MessageBox(GetDesktopWindow(), _T("严重错误:无法打开设置文件"), _T("警告"), MB_OK);
        setting.close();
        return 0;
    }
    getline(setting, SAVE_PATH);
    getline(setting, url_base);
    getline(setting, suffix);
    //setting >> SAVE_PATH;
    //setting >> url_base;
    //setting >> suffix;
    setting.close();
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}

/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

    std::string temp;
    std::string filename;
	std::string pass;
  
    QRcode *code=nullptr;
    long pos;
	wfstream file;
	vector<std::string> strs;

	if (!sql::ban_check(fromQQ)) {
		if (preQQ == fromQQ) {
			nowMsg = clock();
			if (difftime(nowMsg, preMsg) < 1500) {
				preMsg = nowMsg;
				return EVENT_BLOCK;
			}
			else preMsg = nowMsg;
		}
		else preQQ = fromQQ;

		if (msg[0] == '#') {
			switch (msg[1]) {
			case '0':
				Sleep(500);
				CQ_sendPrivateMsg(ac, fromQQ, help_msg);
				return EVENT_BLOCK;

			case '1':
				Sleep(500);
				if (strlen(msg) <= 3) break;
				temp = std::string(msg);
				temp = temp.substr(3,std::string::npos);
				
				for (map<std::string, std::string>::iterator iter = filmlist.begin(); iter != filmlist.end(); iter++) {
					if (iter->first.find(temp) != std::string::npos) {
						CQ_sendPrivateMsg(ac, fromQQ, iter->second.c_str());
						return EVENT_BLOCK;
					}
				}

				temp = temp + " " + SAVE_PATH + "//" + qq2word(fromQQ);
				CQtestlibExec(temp,"movie.exe");

				temp = url_base + qq2word(fromQQ) + "/BDPurl.html";
				QRTextConvate(temp, "../data/image/" + qq2word(fromQQ));
				temp = "[CQ:image,file=" + qq2word(fromQQ) + "/QRcode.png]";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());

				return EVENT_BLOCK;
			case '2':
				if (strlen(msg) <= 3) break;
				CQ_sendPrivateMsg(ac, fromQQ, music_msg);

				temp = std::string(msg);
				strs = split(temp);
				temp.clear();
				while (strs.size() > 1) {
					temp.append(strs.back());
					strs.pop_back();
				}
				strs.clear();
				temp = temp + " " + SAVE_PATH + "/" + qq2word(fromQQ) + " \"" + suffix + "\"";
				
				
				if (!CQtestlibExec(temp, "music.exe")) {
					temp = to_string(GetLastError()) + "下载失败，请检查url重试或者联系机器人管理员";
					CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
					return EVENT_BLOCK;
				}
				CQ_sendPrivateMsg(ac, fromQQ, "下载完成，正在为您生成链接");


				temp = SAVE_PATH + "//" + qq2word(fromQQ) + " Welcome";
				if (!CQtestlibExec(temp, "htmls.exe")) {
					CQ_sendPrivateMsg(ac, fromQQ, "链接生成失败,请重试");
					return EVENT_BLOCK;
				}
				else {
					temp = url_base + qq2word(fromQQ);
					QRTextConvate(temp, "../data/image/" + qq2word(fromQQ));
					temp = "[CQ:image,file=" + qq2word(fromQQ) + "/QRcode.png]";
					CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				}
				return EVENT_BLOCK;

			case '3':
				if (strlen(msg) < 3) break;
				CQ_sendPrivateMsg(ac, fromQQ, register_msg);
				if (sql::register_check(fromQQ)) {
					CQ_sendPrivateMsg(ac, fromQQ, "亲，你已经注册过了哟");
					return EVENT_BLOCK;
				}

				temp = std::string(msg);
				temp = temp.substr(3, temp.npos);
				temp = "\"" + temp + "\" ";
				temp.append(to_string(fromQQ));
				
				if (!CQtestlibExec(temp, "register.exe")) {
					CQ_sendPrivateMsg(ac, fromQQ, "注册失败，请重试或者联系机器人管理员");
					return EVENT_BLOCK;
				}
				else {
					CQ_sendPrivateMsg(ac, fromQQ, register_success_msg);
				}

				return EVENT_BLOCK;

			case '4':
				if (strlen(msg) < 3) break;
				if (!sql::register_check(fromQQ)) {
					CQ_sendPrivateMsg(ac, fromQQ, "您尚未完成认证注册，完成认证注册之后才可以发布失物招领信息哦[CQ:emoji,id=128521]");
					return EVENT_BLOCK;
				}
				if (!sql::uptime_check(fromQQ)) {
					CQ_sendPrivateMsg(ac, fromQQ, "抱歉您今天已经发布过失物招领信息了\n每个普通用户每天只能发布一条失物招领信息哦\n请明天再来哟(＾Ｕ＾)ノ~ＹＯ");
					return EVENT_BLOCK;
				}

				temp = string(msg);
				strs = split(temp, " ");
				if (strs.size() < 4) {
					CQ_sendPrivateMsg(ac, fromQQ, "您输入的参数格式不正确，请查看#0帮助示例哟~");
					strs.clear();
					return EVENT_BLOCK;
				}
				pos = 3;
				temp.clear();
				while (pos < strs.size()) temp.append(strs[pos++]);
				if (sql::Notice_upload(strs[1], strs[2],temp, fromQQ)) CQ_sendPrivateMsg(ac, fromQQ, notice_success_msg);
				else CQ_sendPrivateMsg(ac, fromQQ, "信息录入失败，请联系机器人管理员");
				strs.clear();
				return EVENT_BLOCK;

			case '5':
				if (strlen(msg) < 3) break;

				/*if (!sql::register_check(fromQQ)) {
					CQ_sendPrivateMsg(ac, fromQQ, "您尚未完成认证注册，完成认证注册之后才可以获取失物招领信息哦[CQ:emoji,id=128521]");
					return EVENT_BLOCK;
				}*/

				temp = std::string(msg);
				strs = split(temp);

				if (strs.size() < 2) {
					CQ_sendPrivateMsg(ac, fromQQ, "您输入的命令格式不正确，请输入#0查看帮助示例");
					strs.clear();
					return EVENT_BLOCK;
				}

				temp = sql::Get_Notice_items(strs[1]);
				strs.clear();
				if (temp != "") {
					CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
					CQ_sendPrivateMsg(ac, fromQQ, Notice_help_msg);
				}
				else {
					CQ_sendPrivateMsg(ac, fromQQ, "未查找到您输入的相关信息");
					CQ_sendPrivateMsg(ac, fromQQ, "目前已录入的丢失物品关键词:");
					temp = sql::Get_Notice_keywords();
					CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				}
				return EVENT_BLOCK;

			case '6':
				if (strlen(msg) < 3) break;
				file.open("./6.txt", ios::app);
				if (file.is_open()) {
					file << msg << endl;
					CQ_sendPrivateMsg(ac, fromQQ, "收到~(￣▽￣)~*");
				}
				else CQ_sendPrivateMsg(ac, fromQQ, "录入失败惹，请重试啦~");
				file.close();
				
				return EVENT_BLOCK;
			default:
				temp = std::string(msg);
				strs = split(temp, "#");
				//debug_win(strs[0]);
				if (strs[1] == "资料") {
					if (strs.size() == 3) {
						pass = generatePass("wanerCollect");
						//debug_win(pass);
						filename = sql::Material_Path_Query(strs[2]);
						if (filename.empty()) {
							CQ_sendPrivateMsg(ac, fromQQ, "未能找到您需要的资料");
							Sleep(500);
							CQ_sendPrivateMsg(ac, fromQQ, material_notice_msg);
							return EVENT_BLOCK;
						}
						CQ_sendPrivateMsg(ac, fromQQ, "正在为您打包资料文件，请稍等...");
						temp = "a " + SAVE_PATH + "\\" + qq2word(fromQQ) + "\\"+split(filename,"\\\\").back() + ".zip " + filename + " -p" + pass;
						if (!CQtestlibExec(temp, "7zr.exe")) {
							temp=to_string(GetLastError())+"未能成功打包资料，请重试或联系管理员";
							CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
							return EVENT_BLOCK;
						}

						if (!sql::upload_Material_record(split(filename, "\\\\").back(), pass, fromQQ)) {
							CQ_sendPrivateMsg(ac, fromQQ, "未能成功记录压缩包密码，请重试或者联系管理员");
							return EVENT_BLOCK;
						}
						temp = "已为您成功打包资料，压缩包提取密码请到群490726290发送“#密码”获取:";
						CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
						Sleep(200);

						CQ_sendPrivateMsg(ac, fromQQ, "正在为您生成链接,如果超过5秒未收到链接请发送help002");
						temp = SAVE_PATH + "//" + qq2word(fromQQ) + " Welcome";
						if (!CQtestlibExec(temp, "htmls.exe")) {
							CQ_sendPrivateMsg(ac, fromQQ, "链接生成失败,请重试");
							return EVENT_BLOCK;
						}

						temp = url_base + qq2word(fromQQ);
						QRTextConvate(temp, "../data/image/" + qq2word(fromQQ));
						temp = "[CQ:image,file=" + qq2word(fromQQ) + "\\QRcode.png]";
						CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
						return EVENT_BLOCK;
					}
					else {
						CQ_sendPrivateMsg(ac, fromQQ, material_notice_msg);
						return EVENT_BLOCK;
					}
				}
				else if (strs[1] == "课表") {
					if (strs.size() == 4) {
						temp = strs[2] + " " + strs[3] + " ../data/image/" + qq2word(fromQQ);
						CQ_sendPrivateMsg(ac, fromQQ, "正在获取课程表...");
						if (!CQtestlibExec(temp, "school_timetable.exe")) {
							CQ_sendPrivateMsg(ac, fromQQ, "获取课表失败，请重试或者联系管理员");
							return EVENT_BLOCK;
						}
						temp = "[CQ:image,file=" + qq2word(fromQQ) + "/school_timetable.jpg]";
						CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
					}
				}
				else CQ_sendPrivateMsg(ac, fromQQ, "未能识别您的指令，请回复#0查看帮助信息(+_+)?");
				break;
			}
		}
		else if (msg[0] == '*') {
			if (!sql::register_check(fromQQ)) {
				CQ_sendPrivateMsg(ac, fromQQ, "您尚未完成认证注册，完成认证注册之后才可以使用失物招领哦[CQ:emoji,id=128521]");
				return EVENT_BLOCK;
			}

			pos = std::strtol(msg + 1, NULL, 10);
			if (pos == 0) return EVENT_IGNORE;

			temp = "该失物招领发布者的QQ: ";
			temp.append(sql::confirm_notice(pos));
			CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
			CQ_sendPrivateMsg(ac, fromQQ, notice_confirm_msg);
			return EVENT_BLOCK;
		}
		else if (msg[0] =='-' && sql::isadmin(fromQQ)) {
			temp = std::string(msg);
			strs = split(temp);
			if (strs[0] == "--send") {
				temp = strs[1];
				for (int i = 2; i < strs.size(); i++) {
					pos = atol(strs[i].c_str());
					CQ_sendGroupMsg(ac, pos, temp.c_str());
				}
			}
			else if (strs[0] == "--repeat") {
				CQ_sendPrivateMsg(ac, fromQQ, "flag true");
				flag = true;
				return EVENT_BLOCK;
			}
			else return EVENT_IGNORE;
		}
		else if (
			std::string(msg).find("https://wenku.baidu.com/view/") != std::string::npos ||
			std::string(msg).find("https://wk.baidu.com/view/") != std::string::npos || 
			std::string(msg).find("https://m.baidu.com/sf_edu_wenku/view/") != std::string::npos
		) {
			temp = std::string(msg);
			CQ_sendPrivateMsg(ac, fromQQ, baidu_msg);
			pos = temp.find("https://");
			temp = temp.substr(pos,temp.npos) + " " + SAVE_PATH + "//" + qq2word(fromQQ) + " 40";

			if (!CQtestlibExec(temp, "BDWKdownload.exe")) {
				temp = to_string(GetLastError()) + "下载失败，请检查url重试或者联系机器人管理员";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				return EVENT_BLOCK;
			}


			CQ_sendPrivateMsg(ac, fromQQ, "下载完成，正在为您生成链接，如果超过5秒未收到链接请发送help002");

			temp = SAVE_PATH + "//" + qq2word(fromQQ) + " Welcome";
			if (!CQtestlibExec(temp, "htmls.exe")) {
				CQ_sendPrivateMsg(ac, fromQQ, "链接生成失败,请重试");
				return EVENT_BLOCK;
			}
			else {
				temp = url_base + qq2word(fromQQ);
				QRTextConvate(temp, "../data/image/" + qq2word(fromQQ));
				temp = "[CQ:image,file=" + qq2word(fromQQ) + "/QRcode.png]";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				Sleep(1000);
				CQ_sendPrivateMsg(ac, fromQQ, "如果您需要将pdf转换成其他格式可以访问如下网站");
				temp = "[CQ:image,file=PDFconvery.jpg]";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
			}
			return EVENT_BLOCK;
		}
		else {
			temp = std::string(msg);
			if (flag && isadmin(fromQQ)) {
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				flag = false;
				return EVENT_BLOCK;
			}
			if (temp == "锦鲤" && subType == 11) {
				file.open("./抽奖.txt", ios::app);
				if (file.is_open()) file << fromQQ << endl;
				file.close();
				return EVENT_BLOCK;
			}
			temp=sql::Private_msg_reply(temp,fromQQ);
			if (!temp.empty()) {
				temp.append(randSuffix());
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				return EVENT_BLOCK;
			}
			else {
				CQ_sendPrivateMsg(ac, fromQQ, "我不是很懂你的意思，请发送#0查看帮助!（英文井号）");
				return EVENT_IGNORE;
			}
		}
	}
	//如果要回复消息，请调酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	return EVENT_IGNORE;

}



/*
* Type=2 群消息
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	std::string temp;
	std::string filename;

	QRcode *code = nullptr;
	int pos;
	vector<std::string> strs;

	if (sql::Groups_check(fromGroup) && !sql::ban_check(fromQQ)) {
		if (msg[0] == '#') {
			switch (msg[1]) {
			case '0':
				Sleep(500);
				CQ_sendGroupMsg(ac, fromGroup, help_msg);
				return EVENT_BLOCK;

			case '1':
				Sleep(500);
				if (strlen(msg) <= 3) break;
				temp = std::string(msg);
				temp = temp.substr(3);
				CQ_sendGroupMsg(ac, fromGroup, "链接二维码将通过私聊发送给您o(*￣▽￣*)o");

				for (map<std::string, std::string>::iterator iter = filmlist.begin(); iter != filmlist.end(); iter++) {
					if (iter->first.find(temp) != std::string::npos) {
						CQ_sendPrivateMsg(ac, fromQQ, iter->second.c_str());
						return EVENT_BLOCK;
					}
				}
				
				temp = temp + " " + SAVE_PATH + "//" + qq2word(fromQQ);
				CQtestlibExec(temp, "movie.exe");
				
				temp = url_base + qq2word(fromQQ) + "/BDPurl.html";
				QRTextConvate(temp, "../data/image/" + qq2word(fromQQ));
				temp = "[CQ:image,file=" + qq2word(fromQQ) + "/QRcode.png]";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());

				return EVENT_BLOCK;
			case '2':
				if (strlen(msg) <= 3) break;
				CQ_sendGroupMsg(ac, fromGroup, "已私聊o((>ω< ))o");
				CQ_sendPrivateMsg(ac, fromQQ, music_msg);

				temp = std::string(msg);
				strs = split(temp);
				temp.clear();
				while (strs.size() > 1) {
					temp.append(strs.back());
					strs.pop_back();
				}
				strs.clear();
				temp = temp + " " + SAVE_PATH + "/" + qq2word(fromQQ) + " \"" + suffix + "\"";

				if (!CQtestlibExec(temp,"music.exe")) {
					temp = to_string(GetLastError()) + "下载失败，请检查url重试或者联系机器人管理员";
					CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
					return EVENT_BLOCK;
				}
				CQ_sendPrivateMsg(ac, fromQQ, "下载完成，正在为您生成链接,链接二维码将通过私聊发送");



				temp = SAVE_PATH + "//" + qq2word(fromQQ) + " Welcome";
				if (!CQtestlibExec(temp,"htmls.exe")) {
					CQ_sendPrivateMsg(ac, fromQQ, "链接生成失败,请重试");
					return EVENT_BLOCK;
				}
				else {
					temp = url_base + qq2word(fromQQ);
					QRTextConvate(temp, "../data/image/" + qq2word(fromQQ));
					temp = "[CQ:image,file=" + qq2word(fromQQ) + "/QRcode.png]";
					CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				}
				return EVENT_BLOCK;
			case '3':
				CQ_sendGroupMsg(ac, fromGroup, "认证注册仅限私聊哦[CQ:emoji,id=128522]");
				return EVENT_BLOCK;
			case '4':
				CQ_sendGroupMsg(ac, fromGroup, "失物招领发布仅限私聊哦[CQ:emoji,id=128522]");
				return EVENT_BLOCK;

			case '5':
				if (strlen(msg) < 3) break;

				temp = std::string(msg);
				strs = split(temp);

				if (strs.size() < 2) {
					CQ_sendGroupMsg(ac, fromGroup, "您输入的命令格式不正确，请输入#0查看帮助示例");
					strs.clear();
					return EVENT_BLOCK;
				}

				temp = sql::Get_Notice_items(strs[1]);
				strs.clear();
				if (temp != "") {
					CQ_sendGroupMsg(ac, fromGroup, temp.c_str());
					CQ_sendGroupMsg(ac, fromGroup, Notice_help_msg);
				}
				else {
					CQ_sendGroupMsg(ac, fromGroup, "未查找到您输入的相关信息");
					CQ_sendGroupMsg(ac, fromGroup, "目前已录入的丢失物品关键词:");
					temp = sql::Get_Notice_keywords();
					CQ_sendGroupMsg(ac, fromGroup, temp.c_str());
				}
				return EVENT_BLOCK;
			default:
				temp = std::string(msg);
				strs = split(temp, "#");
				//debug_win(strs[0]);
				if (strs[1] == "密码" && fromGroup == 490726290) {
					temp = sql::get_Material_record(fromQQ);
					if (temp.empty()) {
						temp = AT(fromQQ) + "未查询到您获取资料的记录";
						CQ_sendGroupMsg(ac, fromGroup, temp.c_str());
						return EVENT_BLOCK;
					}
					else {
						temp = AT(fromQQ) + "\n" + temp;
						CQ_sendGroupMsg(ac, fromGroup, temp.c_str());
						return EVENT_BLOCK;
					}
				}
				CQ_sendGroupMsg(ac, fromGroup, "未能识别您的指令，请回复#0查看帮助信息(+_+)?");
				break;
			}
		}
		else if (
				std::string(msg).find("https://wenku.baidu.com/view/") != std::string::npos || 
				std::string(msg).find("https://wk.baidu.com/view/") != std::string::npos || 
				std::string(msg).find("https://m.baidu.com/sf_edu_wenku/view/") != std::string::npos
			) {
			temp = std::string(msg);
			CQ_sendGroupMsg(ac, fromGroup, "私聊啦d=====(￣▽￣*)b");
			CQ_sendPrivateMsg(ac, fromQQ, baidu_msg);
			pos = temp.find("https://");
			temp = temp.substr(pos) + " " + SAVE_PATH + "//" + qq2word(fromQQ) + " 40";

			if (!CQtestlibExec(temp,"BDWKdownload.exe")) {
				temp = to_string(GetLastError()) + "下载失败，请检查url重试或者联系机器人管理员";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				return EVENT_BLOCK;
			}

			CQ_sendPrivateMsg(ac, fromQQ, "下载完成，正在为您生成链接,链接二维码将通过私聊二维码发送");

			temp = SAVE_PATH + "//" + qq2word(fromQQ) + " Welcome";
			if (!CQtestlibExec(temp,"htmls.exe")) {
				CQ_sendPrivateMsg(ac, fromQQ, "链接生成失败,请重试");
				return EVENT_BLOCK;
			}
			else {
				temp = url_base + qq2word(fromQQ);
				QRTextConvate(temp, "../data/image/" + qq2word(fromQQ));
				temp = "[CQ:image,file=" + qq2word(fromQQ) + "/QRcode.png]";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
				Sleep(1000);
				CQ_sendPrivateMsg(ac, fromQQ, "如果您需要将pdf转换成其他格式可以访问如下网站");
				temp = "[CQ:image,file=PDFconvery.jpg]";
				CQ_sendPrivateMsg(ac, fromQQ, temp.c_str());
			}
			return EVENT_BLOCK;
		}
		else {
			temp = std::string(msg);
			temp = sql::Group_msg_reply(temp,fromGroup,fromQQ);
			if (!temp.empty()) {
				temp.append(randSuffix());
				CQ_sendGroupMsg(ac, fromGroup, temp.c_str());
				return EVENT_BLOCK;
			}
		}
	}
	return EVENT_IGNORE;
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, savePath_setting, 0)() {
	//MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
    MessageBox(GetDesktopWindow(), Ascii2WideByte(SAVE_PATH).c_str(), _T("文件保存目录"), MB_OK);
    return 0;
}

CQEVENT(int32_t, URL_setting, 0)() {
    MessageBox(GetDesktopWindow(), Ascii2WideByte(url_base).c_str(), _T("WEB URL"), MB_OK);
	return 0;
}

CQEVENT(int32_t, suffix_setting, 0)() {
    MessageBox(GetDesktopWindow(), Ascii2WideByte(suffix).c_str(), _T("后缀"), MB_OK);
    return 0;
}


CQEVENT(int32_t, about_setting, 0)() {
    MessageBoxA(
        GetDesktopWindow(),
        "修改如上设置请到酷q目录下的setting中\n"
        "code by cc 2019.4.22",
        "关于", 0);
    return 0;
}

