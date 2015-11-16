#include <iostream>
#include <mysql/mysql.h>
#include <nudb.h>
#include "./mongoose.h"

using namespace std;

#define PORT "8397"

static struct mg_serve_http_opts opts;

static void ev_handler(struct mg_connection* nc, int ev, void* ev_data) {
	struct http_message *hm = (struct http_message *) ev_data;
	struct mg_str key;

	switch(ev) {
		case MG_EV_HTTP_REQUEST:
			if(false) {
			} else {
				// serve static content
				mg_serve_http(nc, hm, opts);
			}
			break;
		default:
			break;
	}
}

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
	for(int i = 0; i < 3; i++) {
		cout << "idx: " << i << " => " << db.withdrawCourse("COMP3615", "Q2", 2016) << endl;
		//cout << "idx: " << i << " => " << db.withdrawCourse("COMP2129", "Q1", 2015) << endl;
	}
	cout << "enroll!!!" << endl;
	for(int i = 0; i < 3; i++) {
		cout << "idx: " << i << " => " << db.enrollCourse("COMP5348", "Q1", 2015) << endl;
		//cout << "idx: " << i << " => " << db.withdrawCourse("COMP2129", "Q1", 2015) << endl;
	}
	/*
	struct mg_mgr mgr;
	struct mg_connection* nc;
	mg_mgr_init(&mgr, nullptr);
	nc = mg_bind(&mgr, PORT, ev_handler);
	mg_set_protocol_http_websocket(nc);
	opts.document_root = "./public";

	while(1) {
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);
	*/

	return 0;
}
