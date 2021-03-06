#ifndef __NUDB_H__
#define __NUDB_H__

#include <string>
#include <map>
#include <vector>
#include <mysql/mysql.h>

class NUDB {
	public:
		enum GradeOption { All, HasGrade, NoGrade };
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

		std::map<std::string, std::string> getCourseInfo(const std::string& c, const std::string& s, int y);

		std::vector<std::map<std::string, std::string> > getTranscript(bool filterCurrent);

		std::vector<std::map<std::string, std::string> > getEnrolledCourses(bool onlyThisSemester, GradeOption option);

		std::vector<std::map<std::string, std::string> > getOfferingCourses();

		bool enrollCourse(const std::string& code, const std::string& sm, int y);
		
		bool withdrawCourse(const std::string& code, const std::string& sm, int y);

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
		static void initProcedures(MYSQL* conn);
		static std::vector<std::vector<std::string>> procedureList;

		static std::vector<std::map<std::string, std::string> > queryResult(MYSQL* conn, const std::string& query);

		static bool query(MYSQL* conn, const std::string& query);

		static bool executeSequence(MYSQL* conn, const std::vector<std::string>& queries, bool isTransaction);

		// only for debug
		static void printResult(std::vector<std::map<std::string, std::string> >& res);

		static std::string getSemester(int mon);
		
		static void getCurrentYearAndSemester(std::string& year, std::string& semester);

		static void getNextSemester(const std::string& y, const std::string& s,
				std::string& ny, std::string& ns);
};

#endif // __NUDB_H__
