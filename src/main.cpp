#include <iostream>
#include <mysql/mysql.h>
#include <nudb.h>
#include <mongoose.h>
#include <vector>
#include <map>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <set>
#include <pthread.h>


using namespace std;

#define PORT "8397"
#define BUF_SIZE (1024)

static const int RAND_MASK = 0x76543210;

static struct mg_serve_http_opts opts;

static set<struct mg_connection*> wsSet;

static pthread_mutex_t mutex;

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

static bool is_websocket(struct mg_connection* nc) {
	return nc->flags & MG_F_IS_WEBSOCKET;
}

static void broadcast_warning(const string& msg) {
	pthread_mutex_lock(&mutex);
	for(struct mg_connection* conn : wsSet) {
		cout << "##################### websocket in" << endl;
		if(is_websocket(conn)) {
			mg_send_websocket_frame(conn, WEBSOCKET_OP_TEXT, msg.c_str(), msg.size());
			cout << "send broadcast" << endl;
		}
		cout << "##################### websocket out" << endl;
	}
	pthread_mutex_unlock(&mutex);
}

static void send_json_response(struct mg_connection* nc, const string& data) {
	mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Type: text/json\r\n\r\n%s", data.c_str());
}

static void ev_handler(struct mg_connection* nc, int ev, void* ev_data) {
	static map<int, NUDB*> dbMap;

	struct http_message* hm = (struct http_message*) ev_data;
	struct websocket_message* wm = (struct websocket_message*) ev_data;
	struct mg_str key;

	switch(ev) {
		case MG_EV_HTTP_REQUEST:
			{
				string uri(hm->uri.p, hm->uri.len),
				       query(hm->query_string.p, hm->query_string.len),
				       method(hm->method.p, hm->method.len);
				transform(method.begin(), method.end(), method.begin(), ::tolower);
				char cookieBuf[BUF_SIZE];
				memset(cookieBuf, 0, BUF_SIZE);
				mg_get_http_var(&hm->body, "session", cookieBuf, BUF_SIZE);
				string curCookie(cookieBuf);

				cout << "|" << uri << "|"  << endl;
				//cout << hm->uri.p << endl;
				if(method == "post") {
					if(uri == "/do_login" && method == "post") {
						NUDB* db = new NUDB;
						char user[BUF_SIZE], password[BUF_SIZE];
						memset(user, 0, BUF_SIZE);
						memset(password, 0, BUF_SIZE);
						
						mg_get_http_var(&hm->body, "user", user, BUF_SIZE);
						mg_get_http_var(&hm->body, "password", password, BUF_SIZE);

						string userStr(user), passwordStr(password);
						if(db->login(userStr, passwordStr)) {
							int cookie = rand() % RAND_MASK;
							while(dbMap.find(cookie) != dbMap.end())
								cookie = rand() % RAND_MASK;
							string msg = json_encode({
										{"success", "true"},
										{"cookie", to_string(cookie)},
									});
							dbMap[cookie] = db;
							send_json_response(nc, msg);
							cout << "login success! cookie: " << cookie << endl;
						} else {
							string msg = json_encode({{"success","false"}});
							send_json_response(nc, msg);
							cout << "login failed!" << endl;
							db->logout();
							delete db;
						}
						cout << "login!!!=>" <<  "user: " << user << " pwd: " << password<< endl;
					} else if (uri == "/do_logout") {
						cout << "log cookie: " << curCookie << endl;
						int cookieInt;
						if(curCookie.size() > 0) {
							cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								delete dbMap[cookieInt];
								dbMap.erase(cookieInt);
								cout << "deleted!" << endl;
							}
						}
						send_json_response(nc, json_encode({{"success", "true"}}));
					} else if (uri == "/get_courses") {
						string resp = json_encode({{"success","false"}});
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								resp = json_list_encode(cur->getEnrolledCourses(true, NUDB::GradeOption::All));
							}
						}
						send_json_response(nc, resp);
						cout << "############ get_courses" << endl;
					} else if (uri == "/update_profile") {
						string resp = json_encode({{"success","false"}});

						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "address", cookieBuf, BUF_SIZE);
						string new_addr(cookieBuf);

						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "password", cookieBuf, BUF_SIZE);

						string new_password(cookieBuf);

						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								if(new_addr.size())
									cur->chanageAddress(new_addr);
								if(new_password.size())
									cur->changePassword(new_password);
								resp = json_encode({{"success","true"}});
							}
						}
						send_json_response(nc, resp);
						cout << "############ update_user_info" << endl;
					} else if (uri == "/get_transcript") {
						string resp = json_encode({{"success","false"}});
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								resp = json_list_encode(cur->getTranscript(false));
							}
						}
						send_json_response(nc, resp);
						cout << "############ get_transcript" << endl;
					} else if (uri == "/get_course_info") {
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "uoscode", cookieBuf, BUF_SIZE);
						string uoscode(cookieBuf);
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "semester", cookieBuf, BUF_SIZE);
						string semester(cookieBuf);
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "year", cookieBuf, BUF_SIZE);
						string year(cookieBuf);
						string resp = json_encode({{"success","false"}});
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								resp = json_encode(cur->getCourseInfo(uoscode, semester, stoi(year)));
							}
						}
						send_json_response(nc, resp);
						cout << "############ get_course_info" << endl;

					} else if (uri == "/get_user_info") {
						string resp = json_encode({{"success","false"}});
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								resp = json_encode(cur->getUserInfo());
							}
						}
						send_json_response(nc, resp);
						cout << "############ get_user_info" << endl;
					} else if (uri == "/get_enroll") {
						string resp = json_encode({{"success","false"}});
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								resp = json_list_encode(cur->getOfferingCourses());
							}
						}
						send_json_response(nc, resp);
						cout << "############ get_enroll" << endl;
					} else if (uri == "/do_enroll") {
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "uoscode", cookieBuf, BUF_SIZE);
						string uoscode(cookieBuf);
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "semester", cookieBuf, BUF_SIZE);
						string semester(cookieBuf);
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "year", cookieBuf, BUF_SIZE);
						string year(cookieBuf);
						string resp = json_encode({{"success","false"}});
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								if(cur->enrollCourse(uoscode, semester, stoi(year)))
									resp = json_encode({{"success","true"}});
							}
						}
						send_json_response(nc, resp);
						cout << "############ do_enroll" << endl;
					} else if (uri == "/get_withdraw") {
						string resp = json_encode({{"success","false"}});
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								resp = json_list_encode(cur->getEnrolledCourses(false, NUDB::GradeOption::NoGrade));
							}
						}
						send_json_response(nc, resp);
						cout << "############ get_withdraw" << endl;
					} else if (uri == "/do_withdraw") {
						cout << "hehe begin do_withdraw" << endl;
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "uoscode", cookieBuf, BUF_SIZE);
						string uoscode(cookieBuf);
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "semester", cookieBuf, BUF_SIZE);
						string semester(cookieBuf);
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "year", cookieBuf, BUF_SIZE);
						string year(cookieBuf);
						string resp = json_encode({{"success","false"}});
						cout << "hehe in do_withdraw: " << uoscode << "-" << semester << "-" << year << endl;
						if(curCookie.size() > 0) {
							int cookieInt = stoi(curCookie);
							if(dbMap.find(cookieInt) != dbMap.end()) {
								NUDB* cur = dbMap[cookieInt];
								if(cur->withdrawCourse(uoscode, semester, stoi(year)))
									resp = json_encode({{"success","true"}});
							}
						}
						send_json_response(nc, resp);
						cout << "############ do_withdraw" << endl;
					} else if (uri == "/trigger_warning") {
						// TODO: to broadcast the trigger warning
						memset(cookieBuf, 0, BUF_SIZE);
						mg_get_http_var(&hm->body, "course", cookieBuf, BUF_SIZE);
						string course(cookieBuf);
						send_json_response(nc, json_encode({{"success","true"}}));
						broadcast_warning(course);
						cout << "trigger_warning: " << course << endl;
					} else {
						mg_serve_http(nc, hm, opts);
					}
				} else if (uri == "/caesar.html") {
					mg_serve_http(nc, hm, opts);
				} else {
					// serve static content
					mg_serve_http(nc, hm, opts);
				}
				nc->flags |= MG_F_SEND_AND_CLOSE;
			}
			break;
		case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
			{
				cout << "websocket come!*********************************************" << endl;
				pthread_mutex_lock(&mutex);
				wsSet.insert(nc);
				pthread_mutex_unlock(&mutex);
			}
			break;
		/*
		case MG_EV_WEBSOCKET_FRAME:
			{
			}
			break;
		*/
		case MG_EV_CLOSE:
			{
				if(is_websocket(nc)) {
					pthread_mutex_lock(&mutex);
					wsSet.erase(nc);
					pthread_mutex_unlock(&mutex);
				}
			}
		default:
			break;
	}
}

int main() {

	NUDB::init("localhost", 0, "root", "123456", "project3-nudb");

	pthread_mutex_init(&mutex, nullptr);


	srand(time(nullptr));

	struct mg_mgr mgr;
	struct mg_connection* nc;
	mg_mgr_init(&mgr, nullptr);
	nc = mg_bind(&mgr, PORT, ev_handler);
	mg_set_protocol_http_websocket(nc);
	mg_enable_multithreading(nc);
	opts.document_root = "./public";

	while(1) {
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);
	pthread_mutex_destroy(&mutex);

	return 0;
}
