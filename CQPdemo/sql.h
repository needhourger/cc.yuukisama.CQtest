#pragma once

#include<iostream>
#include<ctime>
#include"sqlite/sqlite3.h"

#include "static_msg.h"
#include "libs.h"

; using namespace std;

const char DATABASE[] = "./CQtestlib/CQtest.db";
const short SQLLEN = 512;

namespace sql {
	/*�ж��Ƿ�ΪVIP�û����Ƿ���True���񷵻�False*/
	bool isadmin(int64_t qq) {
		sqlite3 *db = NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;
		bool ret = false;
		
		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		sprintf_s(sql, SQLLEN, "select id from admin where qq=%lld", qq);
		if (sqlite3_prepare_v2(db, sql, SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				if (sqlite3_column_count(stmt)) ret = true;
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}

	/*ʧ�����췢��ʱ���飬�����ϴ�����True���������ϴ�False*/
	bool uptime_check(int64_t qq) {
		sqlite3 *db=NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;
		bool ret = false;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		sprintf_s(sql, SQLLEN, "select QQ_ID from users where Notice_uptime<date() or Notice_uptime is NULL or isVIP=1 and QQ_ID=%lld;", qq);
		if (sqlite3_prepare_v2(db, sql, SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				if (qq == sqlite3_column_int64(stmt, 0)) ret = true;
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}



	/*��������飬�Ǻ�������Ա����True�����Ƿ���false*/
	bool ban_check(int64_t qq) {
		bool ret = false;

		sqlite3 *db=NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;
		
		rc = sqlite3_open(DATABASE,&db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		sprintf_s(sql, SQLLEN, "select isBanned from Users where QQ_ID=%lld limit 1;", qq);
		if (sqlite3_prepare_v2(db, sql, SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				ret = sqlite3_column_int(stmt, 0);
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}

	/*�Ƿ������֤ע�ᣬ��ɷ���True��δ���False*/
	bool register_check(int64_t qq) {
		sqlite3 *db=NULL;
		sqlite3_stmt* stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = {0};
		int rc;
		bool ret = false;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}
		
		sprintf_s(sql, SQLLEN, "select isRegistered from users where QQ_ID=%lld limit 1;", qq);
		if (sqlite3_prepare_v2(db, sql, SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				ret = sqlite3_column_int(stmt, 0);
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}

	/*ȷ���Ƿ�ΪӦ��Ⱥ,�Ƿ���True�����Ƿ���False*/
	bool Groups_check(int64_t Group) {
		bool ret = false;
		int64_t temp;

		sqlite3 *db=NULL;
		sqlite3_stmt *stmt=NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		sprintf_s(sql, SQLLEN, "select GroupID from Groups where GroupID=%lld limit 1;",Group);
		if (sqlite3_prepare_v2(db, sql, SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				temp = sqlite3_column_int64(stmt, 0);
				if (temp == Group) ret = true;
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}

	/*ȷ��ʧ��������Ϣ���飬��ȡʧ�����췢������Ϣ*/
	std::string confirm_notice(long id) {
		std::string ret;
		std::string temp;

		sqlite3 *db=NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return sql_connect_error;
		}

		sprintf_s(sql, SQLLEN, "select QQ_ID from Notices where ID=%ld;",id);
		if (sqlite3_prepare_v2(db, sql, SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			ret.clear();
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				temp = std::to_string(sqlite3_column_int64(stmt, 0));
				ret.append(temp);
			}
		}
		sqlite3_finalize(stmt);

		//sprintf_s(sql, SQLLEN, "delete from Notices where ID = %ld;", id);
		//sqlite3_exec(db,sql,NULL,NULL,NULL);
		sqlite3_close(db);
		return ret;
	}

	/*���ݹؼ��ʻ�ȡ���ݿ���ʧ��������Ϣ*/
	std::string Get_Notice_items(std::string words) {
		std::string ret;
		std::string temp;

		sqlite3 *db=NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return sql_connect_error;
		}

		sprintf_s(sql, SQLLEN, "select ID,Item,Campus,Detail from Notices where item like '%%%s%%' order by ID;", words.c_str());
		temp = ASCII2UTF_8(std::string(sql));
		if (sqlite3_prepare_v2(db, temp.c_str(), SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			ret.clear();
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				ret.append("[CQ:emoji,id=127801] ");
				ret.append(to_string(sqlite3_column_int(stmt, 0)));
				ret.push_back(' ');
				temp =std::string((const char *)sqlite3_column_text(stmt, 1));
				ret.append(UTF_82ASCII(temp));
				ret.push_back(' ');
				temp = std::string((const char *)sqlite3_column_text(stmt, 2));
				ret.append(UTF_82ASCII(temp));
				ret.push_back('\n');
				temp = std::string((const char *)sqlite3_column_text(stmt, 3));
				ret.append(UTF_82ASCII(temp));
				ret.push_back('\n');
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}


	/*��ȡ���ݿ�������ʧ������ؼ���*/
	std::string Get_Notice_keywords() {
		std::string ret;
		std::string temp;
		unsigned int i=0;

		sqlite3 *db=NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = "select distinct item from Notices order by item;";
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return sql_connect_error;
		}

		if (sqlite3_prepare_v2(db, sql, SQLLEN, &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				temp = std::string((const char *)sqlite3_column_text(stmt, 0));
				ret.append("[CQ:emoji,id=10084] ");
				ret.append(UTF_82ASCII(temp));
				i++;
				if (i % 2 == 0) ret.push_back('\n');
				else ret.append(2, '\t');
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}


	/*Ⱥ�������Կ����*/
	std::string Group_msg_reply(std::string sentence,int64_t Group,int64_t QQ) {
		std::string ret;
		ret.clear();
		std::string temp;

		sqlite3 *db = NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = {0};
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		sprintf_s(sql, SQLLEN, "select Keyword,Reply from Languages where ReplyType=0 or ReplyType=2 or ReplyType=%lld or ReplyType=%lld and isON=1 order by Priority desc;", Group,QQ);
		if (sqlite3_prepare_v2(db, sql, sizeof(sql), &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt)==SQLITE_ROW){
				temp = std::string((const char *)sqlite3_column_text(stmt, 0));
				temp = UTF_82ASCII(temp);
				if (sentence.find(temp) != std::string::npos) {
					temp = std::string((const char *)sqlite3_column_text(stmt, 1));
					sqlite3_finalize(stmt);
					sqlite3_close(db);
					return UTF_82ASCII(temp);
				}
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}


	/*�����������Կ����*/
	std::string Private_msg_reply(std::string sentence,int64_t QQ) {
		std::string ret;
		ret.clear();
		std::string temp;

		sqlite3 *db=NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = {0};
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		sprintf_s(sql, SQLLEN, "select Keyword,Reply from Languages where ReplyType=0 or ReplyType=1 or ReplyType=%lld and isON=1 order by Priority desc;", QQ);
		if (sqlite3_prepare_v2(db, sql, sizeof(sql), &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				temp = std::string((const char *)sqlite3_column_text(stmt, 0));
				temp = UTF_82ASCII(temp);
				if (sentence.find(temp) != std::string::npos) {
					temp = std::string((const char *)sqlite3_column_text(stmt, 1));
					sqlite3_finalize(stmt);
					sqlite3_close(db);
					return UTF_82ASCII(temp);
				}
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}

	/*�������ϱ�Ż�ȡ����·��*/
	std::string Material_Path_Query(const std::string NO) {
		std::string ret;
		ret.clear();

		sqlite3 *db = NULL;
		sqlite3_stmt *stmt = NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		long no = atoi(NO.c_str());
		sprintf_s(sql, SQLLEN, "select path from material where NO=%ld", no);
		if (sqlite3_prepare_v2(db, sql, sizeof(sql), &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				ret = std::string((const char *)sqlite3_column_text(stmt, 0));
				ret = UTF_82ASCII(ret);
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return ret;
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}


	std::string get_Material_record(int64_t qq) {
		std::string ret;
		ret.clear();

		sqlite3 *db = NULL;
		sqlite3_stmt *stmt= NULL;
		const char *zTail = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return ret;
		}

		sprintf_s(sql, SQLLEN, "select Material,password from Material_record where QQ_belong=%lld", qq);
		if (sqlite3_prepare_v2(db, sql, sizeof(sql), &stmt, &zTail) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				ret.append("ѹ������:" + UTF_82ASCII(std::string((const char *)sqlite3_column_text(stmt, 0))));
				ret.append("  ����:" + UTF_82ASCII(std::string((const char *)sqlite3_column_text(stmt, 1))));
				ret.append(1, '\n');
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}


	/*����ѹ������ȡ��¼��Ϣ*/
	bool upload_Material_record(std::string materialName, std::string password, int64_t qq) {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return false;
		}

		sprintf_s(
			sql,
			SQLLEN,
			"insert into Material_record (Material,password,QQ_belong,upTime) values ('%s','%s','%lld',date());",
			ASCII2UTF_8(materialName).c_str(), password.c_str(), qq
		);

		rc = sqlite3_exec(db, sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			return false;
		}
		sqlite3_close(db);
		return true;
	}

	/*
	* ¼��ʧ��������Ϣ
	* ��������Ʒ���ƣ�У�������飬��ԴQQ
	* ����ֵ���ɹ�True��ʧ��False
	*/
	bool Notice_upload(std::string item_name ,std::string Campus,std:: string details,int64_t fromQQ) {
		sqlite3 *db=NULL;
		char *ErrMsg = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE,&db);
		if (rc) {
			sqlite3_close(db);
			return false;
		}
		
		sprintf_s(
			sql,
			SQLLEN, 
			"insert into Notices (Item,Campus,Detail,Uptime,QQ_ID) values ('%s','%s','%s',date(),%lld);"
			"update Users set Notice_uptime=date() where QQ_ID=%lld;",
			ASCII2UTF_8(item_name).c_str(),ASCII2UTF_8(Campus).c_str(),ASCII2UTF_8(details).c_str(),fromQQ,fromQQ
		);
		
		rc = sqlite3_exec(db, sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			return false;
		}
		sqlite3_close(db);
		return true;
	}
	
	/*�����û�*/
	void ban_user(int64_t QQ) {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		char sql[SQLLEN] = { 0 };
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			return;
		}

		sprintf_s(sql, SQLLEN, "insert or replace into Users (QQ_ID,isVIP,isBanned,isRegistered) values (%lld,0,1,0);", QQ);
		rc = sqlite3_exec(db, sql, NULL, NULL, &ErrMsg);
		sqlite3_close(db);
		return;
	}

	/*���ݿ����ϱ��ʼ��*/
	void Material_table_init() {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷��������ݿ�"), _T("����"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, material_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷���ʼ��Matriel��"), _T("����"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}

	/*ѹ�������¼���ʼ��*/
	void Materual_record_table_init() {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷��������ݿ�"), _T("����"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, material_record_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷���ʼ��Material_record��"), _T("����"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}

	/*���ݿ�ʧ��������Ϣ���ʼ��*/
	void Notice_table_init() {
		sqlite3 *db=NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE,&db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷��������ݿ⣡"), _T("����"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, notices_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷�����Notices��"), _T("����"), MB_OK);
			return;
		}
		//else MessageBox(GetDesktopWindow(), _T("���Notices�����ɹ�"), _T("��ʾ"), MB_OK);
		sqlite3_close(db);
		return;
	}


	/*���ݿ��û����ʼ��*/
	void User_table_init() {
		sqlite3 *db=NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷��������ݿ⣡"), _T("����"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, users_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷�����Users��"), _T("����"), MB_OK);
			return;
		}
		//else MessageBox(GetDesktopWindow(), _T("���Users�����ɹ�"), _T("��ʾ"), MB_OK);
		sqlite3_close(db);
		return;
	}

	/*���ݿ�Ӧ��Ⱥ���ʼ��*/
	void Groups_init() {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷��������ݿ⣡"), _T("����"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, groups_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷�����Groups��"), _T("����"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}

	/*���ݿ����Կ���ʼ��*/
	void language_init() {
		sqlite3 *db=NULL;
		char *ErrMsg = NULL;
		int rc;
		
		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷��������ݿ�"), _T("����"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, languages_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷�����Languages��"), _T("����"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}

	void admin_init() {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷��������ݿ�"), _T("����"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, admin_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("�޷�����admin��"), _T("����"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}


};
