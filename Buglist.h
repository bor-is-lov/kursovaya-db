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

	public ref class Buglist : public System::Windows::Forms::Form
	{
	public:
		Buglist(
			std::auto_ptr<sql::Statement>& stmt,
			std::auto_ptr<sql::PreparedStatement>& pstmt,
			std::auto_ptr<sql::ResultSet>& res,
			std::vector<std::string>& args);

	protected:
		~Buglist();

	private:
		System::ComponentModel::Container ^components;

		std::auto_ptr<sql::Statement>* stmt;
		std::auto_ptr<sql::PreparedStatement>* pstmt;
		std::auto_ptr<sql::ResultSet>* res;
		std::vector<std::string>* args;

	private: System::Windows::Forms::Label^ descriptionLabel;
	private: System::Windows::Forms::Label^ creatorLabel;
	private: System::Windows::Forms::Label^ projectNameLabel;
	private: System::Windows::Forms::Label^ isFixedLabel;
	private: System::Windows::Forms::Label^ taskNameLabel;
	private: System::Windows::Forms::Label^ bugIdLabel;
	private: System::Windows::Forms::Label^ bugInfoLabel;
	private: System::Windows::Forms::Button^ backButton;
	private: System::Windows::Forms::Button^ changeBugsButton;
	private: System::Windows::Forms::Label^ messageLabel;
	private: System::Windows::Forms::CheckedListBox^ bugsListBox;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion
		void UpdateBugsListBox();
		void ClearInfoLabels();
	private: System::Void backButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ClearMessageLabel(System::Object^ sender, System::EventArgs^ e);
	private: System::Void bugsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
	private: System::Void bugsListBox_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e);
	private: System::Void changeBugsButton_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
