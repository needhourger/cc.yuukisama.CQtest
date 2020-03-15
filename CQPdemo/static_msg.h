#pragma once
const char help_msg[] = {
	/*"帮助信息\n"
	"#0\t\t\t---显示本帮助\n"
	"百度文库链接\t---下载百度文库文档\n"
	"\t列如：htt[CQ:face,id=14]ps://wenku[CQ:face,id=14].baidu.com/view/604e0b7b1711cc7931b7165f.html\n"
	"\tPC端直接复制需要下载的百度文库网址粘贴到qq发送\n"
	"\t手机端按右上角的分享选择复制链接后粘贴到qq中发送\n\n"
	"#1+电影名\t\t---获得电影资源下载地址\n"
	"\t列如：#1 白雪公主\n\n"
	"#2+音乐名称\t---下载高质量音乐\n"
	"\t列如：#2 爱我中华\n\n"
	"#3+学号+密码\t---用户注册认证\n"
	"\t列如：#3 2017111111 12345678\n\n"
	""
	"\n\n本软件所转载的音视频等资料均为网络三方资源。"*/
	"[CQ:image,file=help.jpg]"
};

const char music_msg[] = {
	"欢迎使用高品质MP3下载功能,注意:\n"
	"1.不能保证所有音乐均为最高品质\n"
	"2.不能保证100%下载成功,如果下载失败请重试,或者过段时间再试(或者放弃吐舌~)\n"
	"3.下载的资源仅供交流使用,请在下载完成后24h内删除\n"
	"正在下载,请稍等..."
};

const char baidu_msg[] = {
	"欢迎使用百度文库下载功能，注意：\n\n"
	"1.本功能无法下载百度文库中格式为pdf的文件\n"
	"2.本功能所下载的百度文库文档一律为pdf格式\n"
	"3.超过50页的文件可能会下载失败\n"
	"4.生成的下载链接有效期从下载时刻起至当日24：00"
	"\n请稍等片刻，正在下载文档..."
};

const char register_msg[] = {
	"感谢您的支持参与认证注册\n"
	"1.完成认证注册之后您将可以发布失物招领\n"
	"2.认证过程中的账户密码为学校信息门户登陆账户密码\n"
	"3.默认密码为身份证后八位"
};

const char register_success_msg[] = {
	"您已经完成了注册！（如果你没有被列入黑名单用户的话）[CQ:emoji,id=128537]"
};

const char notice_success_msg[] = {
	"已成功将信息录入！感谢您的使用！\n"
	"1.失物招领信息将最多被存储15天\n"
	"2.15天之后无人问津的招领信息将被删除\n"
	"替所有失主感谢您！（。＾▽＾）"
};

const char material_notice_msg[] = {
	"如果您需要的资料但是目录中没有的请联系QQ1227027834获取，感谢您为资料完整性做出的贡献！\n"
	"[CQ:image.file=Material.jpg]"
};


const char Notice_help_msg[] = {
	"发送：*+失物招领信息的编号获取发布者联系方式\n"
	"\t列如：*1\n"
};


const char notice_confirm_msg[] = {
	"感谢您使用失物招领功能，感谢您的支持\n"
	"失物招领信息的发布与确认删除均为个人行为\n"
	"由失物招领引发的纠纷与本平台无关\n"
	"如果您发现失物招领信息中有类垃圾信息\n"
	"欢迎QQ1227027834举报"
};

const char sql_connect_error[] = {
	"数据库连接失败，请联系机器人管理员"
};

const char users_sql[] = {
	"CREATE TABLE IF NOT EXISTS Users("
	"QQ_ID INTEGER PRIMARY KEY NOT NULL,"
	"Stu_ID INTEGER,"
	"Password TEXT,"
	"Name TEXT DEFAULT NULL,"
	"Gender TEXT DEFAULT NULL,"
	"Department TEXT DEFAULT NULL,"
	"Notice_uptime TEXT DEFAULT NULL,"
	"isVIP INTEGER DEFAULT 0,"
	"isBanned INTEGER DEFAULT 0,"
	"isRegistered INTEGER DEFAULT 0"
	");"
};


const char material_sql[] = {
	"CREATE TABLE IF NOT EXISTS Material("
	"NO INTEGER PRIMARY KEY NOT NULL,"
	"path TEXT DEFAULT NULL"
	");"
};

const char material_record_sql[] = {
	"CREATE TABLE IF NOT EXISTS Material_record("
	"ID INTEGER PRIMARY KEY NOT NULL,"
	"Material TEXT DEFAULT NULL,"
	"Password TEXT DEFAULT NULL,"
	"QQ_belong INTEGER,"
	"upTime TEXT DEFAULT NULL"
	");"
};

const char notices_sql[] = {
	"CREATE TABLE IF NOT EXISTS Notices("
	"ID INTEGER PRIMARY KEY NOT NULL,"
	"Item TEXT DEFAULT NULL,"
	"Campus TEXT DEFAULT NULL,"
	"Detail TEXT DEFAULT NULL,"
	"Uptime TEXT DEFAULT NULL,"
	"QQ_ID INTEGER,"
	"FOREIGN KEY(QQ_ID) REFERENCES Users(QQ_ID)"
	");"
};

const char groups_sql[] = {
	"CREATE TABLE IF NOT EXISTS Groups("
	"ID INTEGER PRIMARY KEY NOT NULL,"
	"GroupID INTEGER NOT NULL UNIQUE"
	");"
};

const char languages_sql[] = {
	"CREATE TABLE IF NOT EXISTS Languages("
	"ID INTEGER PRIMARY KEY NOT NULL,"
	"Keyword TEXT NOT NULL,"
	"Reply TEXT DEFAULT NULL,"
	"Priority INTEGER NOT NULL DEFAULT 99,"
	"isON INTEGER DEFAULT 1,"
	"ReplyType INTEGER,"
	"FOREIGN KEY(ReplyType) REFERENCES Groups(GroupID),"
	"FOREIGN KEY(ReplyType) REFERENCES Users(QQ_ID)"
	");"
};

const char admin_sql[] = {
	"CREATE TABLE IF NOT EXISTS Admin("
	"ID INTEGER PRIMARY KEY NOT NULL,"
	"qq INTEGER NOT NULL UNIQUE"
	");"
};