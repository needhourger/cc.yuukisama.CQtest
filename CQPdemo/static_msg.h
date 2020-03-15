#pragma once
const char help_msg[] = {
	/*"������Ϣ\n"
	"#0\t\t\t---��ʾ������\n"
	"�ٶ��Ŀ�����\t---���ذٶ��Ŀ��ĵ�\n"
	"\t���磺htt[CQ:face,id=14]ps://wenku[CQ:face,id=14].baidu.com/view/604e0b7b1711cc7931b7165f.html\n"
	"\tPC��ֱ�Ӹ�����Ҫ���صİٶ��Ŀ���ַճ����qq����\n"
	"\t�ֻ��˰����Ͻǵķ���ѡ�������Ӻ�ճ����qq�з���\n\n"
	"#1+��Ӱ��\t\t---��õ�Ӱ��Դ���ص�ַ\n"
	"\t���磺#1 ��ѩ����\n\n"
	"#2+��������\t---���ظ���������\n"
	"\t���磺#2 �����л�\n\n"
	"#3+ѧ��+����\t---�û�ע����֤\n"
	"\t���磺#3 2017111111 12345678\n\n"
	""
	"\n\n�������ת�ص�����Ƶ�����Ͼ�Ϊ����������Դ��"*/
	"[CQ:image,file=help.jpg]"
};

const char music_msg[] = {
	"��ӭʹ�ø�Ʒ��MP3���ع���,ע��:\n"
	"1.���ܱ�֤�������־�Ϊ���Ʒ��\n"
	"2.���ܱ�֤100%���سɹ�,�������ʧ��������,���߹���ʱ������(���߷�������~)\n"
	"3.���ص���Դ��������ʹ��,����������ɺ�24h��ɾ��\n"
	"��������,���Ե�..."
};

const char baidu_msg[] = {
	"��ӭʹ�ðٶ��Ŀ����ع��ܣ�ע�⣺\n\n"
	"1.�������޷����ذٶ��Ŀ��и�ʽΪpdf���ļ�\n"
	"2.�����������صİٶ��Ŀ��ĵ�һ��Ϊpdf��ʽ\n"
	"3.����50ҳ���ļ����ܻ�����ʧ��\n"
	"4.���ɵ�����������Ч�ڴ�����ʱ����������24��00"
	"\n���Ե�Ƭ�̣����������ĵ�..."
};

const char register_msg[] = {
	"��л����֧�ֲ�����֤ע��\n"
	"1.�����֤ע��֮���������Է���ʧ������\n"
	"2.��֤�����е��˻�����ΪѧУ��Ϣ�Ż���½�˻�����\n"
	"3.Ĭ������Ϊ���֤���λ"
};

const char register_success_msg[] = {
	"���Ѿ������ע�ᣡ�������û�б�����������û��Ļ���[CQ:emoji,id=128537]"
};

const char notice_success_msg[] = {
	"�ѳɹ�����Ϣ¼�룡��л����ʹ�ã�\n"
	"1.ʧ��������Ϣ����౻�洢15��\n"
	"2.15��֮�������ʽ��������Ϣ����ɾ��\n"
	"������ʧ����л���������ި��ޣ�"
};

const char material_notice_msg[] = {
	"�������Ҫ�����ϵ���Ŀ¼��û�е�����ϵQQ1227027834��ȡ����л��Ϊ���������������Ĺ��ף�\n"
	"[CQ:image.file=Material.jpg]"
};


const char Notice_help_msg[] = {
	"���ͣ�*+ʧ��������Ϣ�ı�Ż�ȡ��������ϵ��ʽ\n"
	"\t���磺*1\n"
};


const char notice_confirm_msg[] = {
	"��л��ʹ��ʧ�����칦�ܣ���л����֧��\n"
	"ʧ��������Ϣ�ķ�����ȷ��ɾ����Ϊ������Ϊ\n"
	"��ʧ�����������ľ����뱾ƽ̨�޹�\n"
	"���������ʧ��������Ϣ������������Ϣ\n"
	"��ӭQQ1227027834�ٱ�"
};

const char sql_connect_error[] = {
	"���ݿ�����ʧ�ܣ�����ϵ�����˹���Ա"
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