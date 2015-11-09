#include <iostream>
#include <mysql/mysql.h>
#include <nudb.h>
#include <vector>
#include <map>

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

map<string, string> NUDB::getUserInfo() {
	map<string, string> m;
	auto res = queryResult(
			"SELECT * FROM student WHERE id = " + mId + ";");
	if(res.size() > 0) {
		m = move(res[0]);
	}
	return m;
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
