#ifndef __NUDB_H__
#define __NUDB_H__

#include <string>
#include <map>
#include <vector>
#include <mysql/mysql.h>

class NUDB {
	public:
		static void init(const std::string srv, const int port, 
		                 const std::string user, const std::string passwd, 
				 const std::string db);

		// constructor
		NUDB();

		// destructor
		~NUDB();

		bool login(const std::string& id, const std::string& passwd);

		void logout();

		bool changePassword(const std::string& passwd);

		bool chanageAddress(const std::string& addr);

		std::map<std::string, std::string> getUserInfo();

		std::vector<std::map<std::string, std::string> > getTranscript(bool filterCurrent);

		std::vector<std::map<std::string, std::string> > getEnrolledCourses(bool onlyThisSemester);

		bool enrollCourse(std::string& code);
		
		bool withdrawCourse(std::string& code);

	private:
		static std::string server;
		static int port;
		static std::string user;
		static std::string password;
		static std::string database;

		bool mLogged;
		std::string mId;
		MYSQL *mConnection;

		static MYSQL* connectDB();

		std::vector<std::map<std::string, std::string> > queryResult(const std::string& query);

		bool query(const std::string& query);

		bool executeTransaction(const std::vector<std::string>& queries);

		// only for debug
		static void printResult(std::vector<std::map<std::string, std::string> >& res);

		static std::string getSemester(int mon);
		
		static void getCurrentYearAndSemester(std::string& year, std::string& semester);
};

#endif // __NUDB_H__
