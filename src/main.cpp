#include <iostream>
#include <mysql/mysql.h>
#include <nudb.h>

using namespace std;

MYSQL *conn;

int main() {
	NUDB::init("localhost", 0, "test", "123456", "project3-nudb");
	NUDB db;
	cout << db.login("3213", "lunch") << endl;
	cout << "getUserInfo() => " << endl;
	db.getUserInfo();
	cout << "getTranscript() => " << endl;
	db.getTranscript(false);
	cout << "getEnrolledCourses(false) => " << endl;
	db.getEnrolledCourses(false);
	cout << "getEnrolledCourses(true) => " << endl;
	db.getEnrolledCourses(true);
	/*
	conn = mysql_init(NULL);
	mysql_real_connect(
			conn,
			"localhost",
			"test",
			"123456",
			"project3-nudb",
			0,
			NULL,
			CLIENT_MULTI_RESULTS);

	MYSQL_RES *res_set;
	MYSQL_ROW row;
	mysql_query(conn, "SELECT * FROM lecture;");
	res_set = mysql_store_result(conn);
	int rows = (int) mysql_num_rows(res_set);

	for(int i = 0; i < rows; i++) {
		row = mysql_fetch_row(res_set);
		if(row != NULL) {
			cout << "ID: " << row[0] << " AGE:" << row[1] << " ADDR: " << row[2] << endl;
		}
	}
	mysql_free_result(res_set);

	mysql_close(conn);
	*/
	return 0;
}
