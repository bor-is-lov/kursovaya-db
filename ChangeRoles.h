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

	public ref class ChangeRoles : public System::Windows::Forms::Form
	{
	public:
		ChangeRoles(
			std::auto_ptr<sql::Statement>& stmt,
			std::auto_ptr<sql::PreparedStatement>& pstmt,
			std::auto_ptr<sql::ResultSet>& res,
			std::vector<std::string>& args);

	protected:
		~ChangeRoles();

	private:
		System::ComponentModel::Container ^components;

		std::auto_ptr<sql::Statement>* stmt;
		std::auto_ptr<sql::PreparedStatement>* pstmt;
		std::auto_ptr<sql::ResultSet>* res;
		std::vector<std::string>* args;

	private: System::Windows::Forms::Label^ userInfoLabel;
	private: System::Windows::Forms::Label^ idLabel;
	private: System::Windows::Forms::TextBox^ loginTextBox;

	private: System::Windows::Forms::Label^ roleLabel;
	private: System::Windows::Forms::Label^ messageLabel;
	private: System::Windows::Forms::RadioButton^ moderRadio;
	private: System::Windows::Forms::RadioButton^ userRadio;
	private: System::Windows::Forms::RadioButton^ testerRadio;
	private: System::Windows::Forms::Button^ changeRoleButton;
	private: System::Windows::Forms::Button^ deleteButton;
	private: System::Windows::Forms::Button^ BackButton;


#pragma region Windows Form Designer generated code
		   void InitializeComponent(void);
#pragma endregion
	private: System::Void loginTextBox_TextChanged(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ClearMessageLabel(System::Object^ sender, System::EventArgs^ e);
	private: System::Void changeRoleButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void deleteButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void BackButton_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
