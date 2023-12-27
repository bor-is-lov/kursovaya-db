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

	public ref class ChangeProjects : public System::Windows::Forms::Form
	{
	public:
		ChangeProjects(
			std::auto_ptr<sql::Statement>& stmt,
			std::auto_ptr<sql::PreparedStatement>& pstmt,
			std::auto_ptr<sql::ResultSet>& res,
			std::vector<std::string>& args);

	protected:
		~ChangeProjects();

	private:
		System::ComponentModel::Container ^components;
		
		std::auto_ptr<sql::Statement>* stmt;
		std::auto_ptr<sql::PreparedStatement>* pstmt;
		std::auto_ptr<sql::ResultSet>* res;
		std::vector<std::string>* args;

	private: System::Windows::Forms::TabControl^ tabControl;
	private: System::Windows::Forms::TabPage^ addPage;
	private: System::Windows::Forms::TabPage^ changePage;
	private: System::Windows::Forms::TabPage^ deletePage;
	private: System::Windows::Forms::Label^ addLabel;
	private: System::Windows::Forms::TextBox^ addTextBox;
	private: System::Windows::Forms::Button^ addButton;
	private: System::Windows::Forms::TextBox^ newTextBox;
	private: System::Windows::Forms::Label^ newLabel;
	private: System::Windows::Forms::Button^ changeButton;
	private: System::Windows::Forms::TextBox^ oldTextBox;
	private: System::Windows::Forms::Label^ oldLabel;
	private: System::Windows::Forms::Label^ addMessageLabel;
	private: System::Windows::Forms::Label^ changeMessageLabel;
	private: System::Windows::Forms::Button^ deleteButton;
	private: System::Windows::Forms::TextBox^ deleteTextBox;
	private: System::Windows::Forms::Label^ deleteMessageLabel;
	private: System::Windows::Forms::Button^ backButton;
	private: System::Windows::Forms::Label^ deleteLabel;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion
	private: System::Void ClearMessageLabels(System::Object^ sender, System::EventArgs^ e);
	private: System::Void tabControl_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
	private: System::Void backButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void addButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void changeButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void deleteButton_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
