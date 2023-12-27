#pragma once
#include <vector>
#include <msclr/marshal_cppstd.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

namespace kursovaya {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class SelectProject : public System::Windows::Forms::Form
	{
	public:
		SelectProject(
			std::auto_ptr<sql::Statement>& stmt,
			std::auto_ptr<sql::PreparedStatement>& pstmt,
			std::auto_ptr<sql::ResultSet>& res,
			std::vector<std::string>& args);

	protected:
		~SelectProject();

	private:
		System::ComponentModel::Container ^components;

		std::auto_ptr<sql::Statement>* stmt;
		std::auto_ptr<sql::PreparedStatement>* pstmt;
		std::auto_ptr<sql::ResultSet>* res;
		std::vector<std::string>* args;

	private: System::Windows::Forms::Label^ userInfoLabel;
	private: System::Windows::Forms::Label^ idLabel;
	private: System::Windows::Forms::Label^ loginLabel;
	private: System::Windows::Forms::Label^ roleLabel;
	private: System::Windows::Forms::TabControl^ tabControl;
	private: System::Windows::Forms::TabPage^ selectProjectPage;
	private: System::Windows::Forms::TextBox^ changeLoginTextBox;
	private: System::Windows::Forms::TextBox^ changePasswordTextBox;
	private: System::Windows::Forms::Label^ changePasswordLabel;
	private: System::Windows::Forms::Label^ changeLoginLabel;
	private: System::Windows::Forms::Button^ changePasswordButton;
	private: System::Windows::Forms::Button^ changeLoginButton;
	private: System::Windows::Forms::Label^ userMessageLabel;
	private: System::Windows::Forms::Button^ changeRolesButton;
	private: System::Windows::Forms::Button^ changeProjectsButton;
	private: System::Windows::Forms::ListBox^ projectsListBox;
	private: System::Windows::Forms::Label^ projectsErrorLabel;
	private: System::Windows::Forms::Button^ logOutButton;
	private: System::Windows::Forms::Button^ deleteButton;
	private: System::Windows::Forms::Label^ creatorLabel;

	private: System::Windows::Forms::Label^ projectNameLabel;


	private: System::Windows::Forms::Label^ projectIdLabel;

	private: System::Windows::Forms::Label^ projectInfoLabel;
	private: System::Windows::Forms::Label^ progressLabel;


	private: System::Windows::Forms::Label^ amountOfTasksLabel;
	private: System::Windows::Forms::ProgressBar^ progressBar;


	private: System::Windows::Forms::TabPage^ userSettingPage;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion
		void UpdateUserInfo();
		void UpdateProjects();
	private: System::Void tabControl_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
	private: System::Void changeLoginButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void changePasswordButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void changeRolesButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void changeProjectsButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void logOutButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void projectsListBox_DoubleClick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ClearMessageLabels(System::Object^ sender, System::EventArgs^ e);
	private: System::Void deleteButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void projectsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
	};
}
