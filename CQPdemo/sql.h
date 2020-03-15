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
	/*判断是否为VIP用户，是返回True，否返回False*/
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

	/*失物招领发布时间检查，可再上传返回True，不可再上传False*/
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



	/*黑名单检查，是黑名单成员返回True，不是返回false*/
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

	/*是否完成认证注册，完成返回True，未完成False*/
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

	/*确认是否为应答群,是返回True，不是返回False*/
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

	/*确认失物招领信息详情，获取失物招领发布者信息*/
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

	/*依据关键词获取数据库内失物招领信息*/
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


	/*获取数据库内现有失物招领关键词*/
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


	/*群聊天语言库检索*/
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


	/*个人聊天语言库检索*/
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

	/*依据资料编号获取资料路径*/
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
				ret.append("压缩包名:" + UTF_82ASCII(std::string((const char *)sqlite3_column_text(stmt, 0))));
				ret.append("  密码:" + UTF_82ASCII(std::string((const char *)sqlite3_column_text(stmt, 1))));
				ret.append(1, '\n');
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ret;
	}


	/*插入压缩包获取记录信息*/
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
	* 录入失物招领信息
	* 参数：物品名称，校区，详情，来源QQ
	* 返回值：成功True，失败False
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
	
	/*拉黑用户*/
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

	/*数据库资料表初始化*/
	void Material_table_init() {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法连接数据库"), _T("警告"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, material_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法初始化Matriel表"), _T("警告"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}

	/*压缩密码记录表初始化*/
	void Materual_record_table_init() {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法连接数据库"), _T("警告"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, material_record_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法初始化Material_record表"), _T("警告"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}

	/*数据库失物招领信息表初始化*/
	void Notice_table_init() {
		sqlite3 *db=NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE,&db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法连接数据库！"), _T("警告"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, notices_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法创建Notices表"), _T("警告"), MB_OK);
			return;
		}
		//else MessageBox(GetDesktopWindow(), _T("表格Notices创建成功"), _T("提示"), MB_OK);
		sqlite3_close(db);
		return;
	}


	/*数据库用户表初始化*/
	void User_table_init() {
		sqlite3 *db=NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法连接数据库！"), _T("警告"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, users_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法创建Users表"), _T("警告"), MB_OK);
			return;
		}
		//else MessageBox(GetDesktopWindow(), _T("表格Users创建成功"), _T("提示"), MB_OK);
		sqlite3_close(db);
		return;
	}

	/*数据库应答群表初始化*/
	void Groups_init() {
		sqlite3 *db = NULL;
		char *ErrMsg = NULL;
		int rc;

		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法连接数据库！"), _T("警告"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, groups_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法创建Groups表"), _T("警告"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}

	/*数据库语言库表初始化*/
	void language_init() {
		sqlite3 *db=NULL;
		char *ErrMsg = NULL;
		int rc;
		
		rc = sqlite3_open(DATABASE, &db);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法链接数据库"), _T("警告"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, languages_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法创建Languages表"), _T("警告"), MB_OK);
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
			MessageBox(GetDesktopWindow(), _T("无法链接数据库"), _T("警告"), MB_OK);
			return;
		}

		rc = sqlite3_exec(db, admin_sql, NULL, NULL, &ErrMsg);
		if (rc) {
			sqlite3_close(db);
			MessageBox(GetDesktopWindow(), _T("无法创建admin表"), _T("警告"), MB_OK);
			return;
		}
		sqlite3_close(db);
		return;
	}


};
