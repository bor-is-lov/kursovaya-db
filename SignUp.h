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

	public ref class SignUp : public System::Windows::Forms::Form
	{
	public:
		SignUp(
			std::auto_ptr<sql::Statement>& stmt,
			std::auto_ptr<sql::PreparedStatement>& pstmt,
			std::auto_ptr<sql::ResultSet>& res,
			std::vector<std::string>& args);

	protected:
		~SignUp();

	private:
		System::ComponentModel::Container ^components;

		std::auto_ptr<sql::Statement>* stmt;
		std::auto_ptr<sql::PreparedStatement>* pstmt;
		std::auto_ptr<sql::ResultSet>* res;
		std::vector<std::string>* args;

	private: System::Windows::Forms::Label^ title;
	private: System::Windows::Forms::Label^ loginLabel;
	private: System::Windows::Forms::Label^ passwordLabel;

	private: System::Windows::Forms::TextBox^ passwordTextBox;
	private: System::Windows::Forms::Button^ signUpButton;
	private: System::Windows::Forms::Label^ orLabel;
	private: System::Windows::Forms::Button^ enterButton;
    private: System::Windows::Forms::TextBox^ loginTextBox;
    private: System::Windows::Forms::Label^ errorLabel;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion

	private: System::Void enterButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void signUpButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ClearErrorLabel(System::Object^ sender, System::EventArgs^ e);
    };
}
