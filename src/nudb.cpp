#include <iostream>
#include <mysql/mysql.h>
#include <nudb.h>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

string NUDB::server;
int NUDB::port = 0;
string NUDB::user, NUDB::password, NUDB::database;

void NUDB::init(const string srv, const int port,
		const string user, const string passwd,
		const string db) {
	NUDB::server = srv;
	NUDB::port = port;
	NUDB::user = user;
	NUDB::password = passwd;
	NUDB::database = db;
}

MYSQL* NUDB::connectDB() {
	MYSQL *conn = mysql_init(nullptr);
	mysql_real_connect(
			conn,
			NUDB::server.c_str(),
			NUDB::user.c_str(),
			NUDB::password.c_str(),
			NUDB::database.c_str(),
			NUDB::port,
			nullptr,
			CLIENT_MULTI_RESULTS);
	return conn;
}

NUDB::NUDB() : mLogged(false), mConnection(nullptr) {}
NUDB::~NUDB() { logout(); }

bool NUDB::login(const std::string& user, const std::string& passwd) {
	mConnection = connectDB();
	if(!mConnection) return false;

	vector<map<string, string> > res = queryResult(
			"SELECT * FROM student WHERE id = "
			+ user + " AND password = '" + passwd + "';");

	if(res.size() <= 0) {
		logout();
		return false;
	}

	mLogged = true;
	mId = user;
	return true;
}

void NUDB::logout() {
	if(mLogged && mConnection) {
		mysql_close(mConnection);
		mConnection = nullptr;
	}
}

bool NUDB::changePassword(const string& passwd) {
	vector<string> queries = {
		"UPDATE student SET password = '" + passwd + "' WHERE id = " + mId + ";"
	};
	return executeTransaction(queries);
}

bool NUDB::chanageAddress(const string& addr) {
	vector<string> queries = {
		"UPDATE student SET address = '" + addr + "' WHERE id = " + mId + ";"
	};
	return executeTransaction(queries);
}

map<string, string> NUDB::getUserInfo() {
	map<string, string> m;
	auto res = queryResult(
			"SELECT * FROM student WHERE id = " + mId + ";");
	if(res.size() > 0) {
		m = move(res[0]);
	}
	return m;
}

vector<map<string, string> > NUDB::getTranscript(bool filterCurrent) {
	vector<map<string, string> > res;
	res = queryResult(
			"SELECT A.uoscode, B.uosname, A.grade "
			"FROM transcript A, unitofstudy B "
			"WHERE A.studid = " + mId +
			" AND A.uoscode = B.uoscode " +
			(filterCurrent ? "AND A.grade IS NOT NULL" : "") + ";");
	printResult(res);
	return res;
}

vector<map<string, string> > NUDB::getEnrolledCourses(bool onlyThisSemester) {
	vector<map<string, string> > res;
	string year, semester;
	if(onlyThisSemester)
		getCurrentYearAndSemester(year, semester);
	res = queryResult(
			"SELECT A.uoscode, B.uosname "
			"FROM transcript A, unitofstudy B "
			"WHERE A.studid = " + mId +
			" AND A.uoscode = B.uoscode " +
			(onlyThisSemester ? 
			 "AND A.year = " + year + " AND A.semester = '"  + semester + "'"
			 : "") + ";");
	printResult(res);
	return res;
}


vector<map<string, string> > NUDB::queryResult(const string& query) {
	vector<map<string, string> > res;
	MYSQL_RES* res_set;
	MYSQL_ROW row;
	MYSQL_FIELD* field;
	mysql_query(mConnection, query.c_str());
	res_set = mysql_store_result(mConnection);
	int num_row = mysql_num_rows(res_set);
	int num_field = mysql_num_fields(res_set);
	vector<string> fieldNames;
	for(int i = 0; i < num_field; i++) {
		field = mysql_fetch_field(res_set);
		fieldNames.push_back(string(field->name));
	}
	for(int i = 0; i < num_row; i++) {
		map<string, string> m;
		row = mysql_fetch_row(res_set);
		unsigned long* lengths;
		lengths = mysql_fetch_lengths(res_set);
		for(int j = 0; j < num_field; j++) {
			if(!row[j]) continue;
			m[fieldNames[j]] = string(row[j], lengths[j]);
		}
		res.push_back(move(m));
	}
	mysql_free_result(res_set);

	return res;
}

bool NUDB::query(const string& query) {
	mysql_query(mConnection, query.c_str());
	return true;
}

bool NUDB::executeTransaction(const vector<string>& queries) {

	// TODO: start transaction
	for(auto q : queries) {
		if(!query(q)) goto failure;
	}
	// TODO: end transaction

	return true;

failure:
	// TODO: rollback
	return false;
}

void NUDB::printResult(vector<map<string, string> >& res) {
	for(auto m : res) {
		cout << "ROW: ";
		for(auto p : m) {
			cout << p.first << "=>" << p.second << ", ";
		}
		cout << endl;
	}
}

string NUDB::getSemester(int mon) {
	string res = "Q0";
	switch(mon) {
		case 1: case 2: case 3:
			res = "Q2";
			break;
		case 4: case 5: case 6:
			res = "Q3";
			break;
		case 7: case 8:
			res = "Q4";
			break;
		case 9: case 10: case 11: case 12:
			res = "Q1";
			break;
	}
	return res;
}

void NUDB::getCurrentYearAndSemester(string& year, string& semester) {
	time_t tv = time(nullptr);
	struct tm currentTime;
	localtime_r(&tv, &currentTime);
	year = to_string(currentTime.tm_year + 1900);
	semester = getSemester(currentTime.tm_mon + 1);
}

