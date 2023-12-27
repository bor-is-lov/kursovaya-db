#include "Login.h"
#include "SignUp.h"
#include "SelectProject.h"
#include "ChangeProjects.h"
#include "ChangeRoles.h"
#include "TaskList.h"
#include "ChangeTasks.h"
#include "BugList.h"
#include "ChangeBugs.h"

using namespace std;
using namespace System;
using namespace System::Windows::Forms;

int main() {
	sql::Driver* driver;
	auto_ptr<sql::Connection> con;
	auto_ptr<sql::Statement> stmt;
	auto_ptr<sql::PreparedStatement> pstmt;
	auto_ptr<sql::ResultSet> res;
		
	try {
		driver = get_driver_instance();
		con.reset(driver->connect("localhost", "root", "123qweasdzxcWSX"));
	}
	catch (sql::SQLException e)	{
		cout << "Could not connect to server. Error message: " << e.what() << endl;
		exit(1);
	}

	con->setSchema("kursovaya");

	stmt.reset(con->createStatement());
	stmt->execute("CREATE TABLE IF NOT EXISTS Roles" \
		" (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," \
		" roleName VARCHAR(50) NOT NULL UNIQUE," \
		" changeRoles BOOL NOT NULL," \
		" changeProjects BOOL NOT NULL," \
		" changeTasks BOOL NOT NULL," \
		" changeBugs BOOL NOT NULL)");
	stmt->execute("CREATE TABLE IF NOT EXISTS Users" \
		" (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," \
		" userName VARCHAR(50) NOT NULL UNIQUE," \
		" password VARCHAR(50) NOT NULL," \
		" roleId INT NOT NULL," \
		" FOREIGN KEY (roleId) REFERENCES Roles (id))");
	stmt->execute("CREATE TABLE IF NOT EXISTS Projects" \
		" (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," \
		" projectName VARCHAR(50) NOT NULL UNIQUE," \
		" userId INT, " \
		" FOREIGN KEY (userId) REFERENCES Users (id) ON DELETE SET NULL)");
	stmt->execute("CREATE TABLE IF NOT EXISTS Tasks" \
		" (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," \
		" taskName VARCHAR(50) NOT NULL UNIQUE," \
		" deadline DATE NOT NULL," \
		" isDone BOOL NOT NULL DEFAULT FALSE," \
		" projectId INT," \
		" userId INT," \
		" FOREIGN KEY (projectId) REFERENCES Projects (id) ON DELETE SET NULL," \
		" FOREIGN KEY (userId) REFERENCES Users (id) ON DELETE SET NULL)");
	stmt->execute("CREATE TABLE IF NOT EXISTS Bugs" \
		" (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," \
		" description VARCHAR(255) NOT NULL UNIQUE," \
		" isFixed BOOL NOT NULL DEFAULT FALSE," \
		" taskId INT," \
		" userId INT," \
		" FOREIGN KEY (taskId) REFERENCES Tasks (id) ON DELETE SET NULL," \
		" FOREIGN KEY (userId) REFERENCES Users (id) ON DELETE SET NULL)");
	pstmt.reset(con->prepareStatement("INSERT IGNORE INTO Roles" \
		" (roleName, changeRoles, changeProjects, changeTasks, changeBugs)" \
		" VALUES (?, ?, ?, ?, ?)"));
	pstmt->setString(1, "Admin");
	pstmt->setBoolean(2, 1);
	pstmt->setBoolean(3, 1);
	pstmt->setBoolean(4, 1);
	pstmt->setBoolean(5, 1);
	pstmt->execute();
	pstmt->setString(1, "Moder");
	pstmt->setBoolean(2, 0);
	pstmt->setBoolean(3, 1);
	pstmt->setBoolean(4, 1);
	pstmt->setBoolean(5, 1);
	pstmt->execute();
	pstmt->setString(1, "Tester");
	pstmt->setBoolean(2, 0);
	pstmt->setBoolean(3, 0);
	pstmt->setBoolean(4, 0);
	pstmt->setBoolean(5, 1);
	pstmt->execute();
	pstmt->setString(1, "User");
	pstmt->setBoolean(2, 0);
	pstmt->setBoolean(3, 0);
	pstmt->setBoolean(4, 0);
	pstmt->setBoolean(5, 0);
	pstmt->execute();
	stmt->execute("INSERT IGNORE INTO Users" \
		" (id, userName, password, roleId)" \
		" VALUES (1, 'boss', 'qwerty', (SELECT id FROM Roles WHERE roleName = 'Admin'))");

	Application::SetCompatibleTextRenderingDefault(false);
	Application::EnableVisualStyles();
	// аргументы, передаваемые окнами перед закрытием.
	// [0] - открываемое окно
	// [1] - id пользователя (если нужно)
	// [2] - id проекта (если нужно)
	// [3] - id задачи (если нужно)
	vector<string> args;
	kursovaya::Login loginForm(stmt, pstmt, res, args);
	Application::Run(% loginForm);
	while (!args.empty()) {
		for (int i = 0; i < args.size(); ++i)
			cout << '#' << i << ": " << args[i] << '\t';
		cout << endl;
		if (args[0] == "exit")
			break;
		else if (args[0] == "SignUp") {
			kursovaya::SignUp signUpForm(stmt, pstmt, res, args);
			Application::Run(% signUpForm);
		}
		else if (args[0] == "SelectProject") {
			kursovaya::SelectProject selectProjectForm(stmt, pstmt, res, args);
			Application::Run(% selectProjectForm);
		}
		else if (args[0] == "ChangeRoles") {
			kursovaya::ChangeRoles changeRolesForm(stmt, pstmt, res, args);
			Application::Run(% changeRolesForm);
		}
		else if (args[0] == "ChangeProjects") {
			kursovaya::ChangeProjects changeProjectsForm(stmt, pstmt, res, args);
			Application::Run(% changeProjectsForm);
		}
		else if(args[0] == "TaskList") {
			kursovaya::TaskList taskListForm(stmt, pstmt, res, args);
			Application::Run(% taskListForm);
		}
		else if (args[0] == "ChangeTasks") {
			kursovaya::ChangeTasks changeTasksForm(stmt, pstmt, res, args);
			Application::Run(% changeTasksForm);
		}
		else if (args[0] == "BugList") {
			kursovaya::Buglist bugListForm(stmt, pstmt, res, args);
			Application::Run(% bugListForm);
		}
		else if (args[0] == "ChangeBugs") {
			kursovaya::ChangeBugs changeBugsForm(stmt, pstmt, res, args);
			Application::Run(% changeBugsForm);
		}
		else{
			kursovaya::Login loginForm(stmt, pstmt, res, args);
			Application::Run(% loginForm);
		};
	}

	return 0;
}
