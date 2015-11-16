#include <iostream>
#include <mysql/mysql.h>
#include <nudb.h>
#include "./mongoose.h"
#include <vector>
#include <map>
#include <sstream>

using namespace std;

#define PORT "8397"

static struct mg_serve_http_opts opts;

string json_encode(const map<string, string>& obj) {
	stringstream ss;
	ss << "{";
	bool inKV = false;
	for(auto& kv : obj) {
		if(inKV) ss << ",";
		inKV = true;
		ss << "\"" << kv.first << "\":\"" << kv.second << "\"";
	}
	ss << "}";
	return ss.str();
}

string json_list_encode(const vector<map<string, string>>& objs) {
	stringstream ss;
	ss << "[";
	bool start = false;
	for(auto& obj : objs) {
		if(start) ss << ",";
		start = true;
		ss << json_encode(obj);
	}
	ss << "]";
	return ss.str();
}

static void ev_handler(struct mg_connection* nc, int ev, void* ev_data) {
	static map<string, NUDB*> dbMap;

	struct http_message *hm = (struct http_message *) ev_data;
	struct mg_str key;

	switch(ev) {
		case MG_EV_HTTP_REQUEST:
			{
				string uri(hm->uri.p, hm->uri.len);
				cout << "|" << uri << "|"  << endl;
				if(uri == "/do_login") {
					cout << "login!!!" << endl;
				} else if (uri == "/do_logout") {
				} else if (uri == "/get_courses") {
				} else if (uri == "/update_passwd") {
				} else if (uri == "/update_addr") {
				} else if (uri == "/get_transcript") {
				} else if (uri == "/get_course_info") {
				} else if (uri == "/get_user_info") {
				} else if (uri == "/get_enroll") {
				} else if (uri == "/do_enroll") {
				} else if (uri == "/get_withdraw") {
				} else if (uri == "/do_withdraw") {
				} else {
					// serve static content
					mg_serve_http(nc, hm, opts);
				}
			}
			break;
		default:
			break;
	}
}

int main() {

	NUDB::init("localhost", 0, "test", "123456", "project3-nudb");
	/*
	NUDB db;
	cerr << db.login("3213", "lunch") << endl;
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
	cerr << "getOfferingCourses(true) => " << endl;
	cout << json_list_encode(db.getOfferingCourses());
	*/

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

	return 0;
}
