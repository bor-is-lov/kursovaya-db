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

	public ref class TaskList : public System::Windows::Forms::Form
	{
	public:
		TaskList(
			std::auto_ptr<sql::Statement>& stmt,
			std::auto_ptr<sql::PreparedStatement>& pstmt,
			std::auto_ptr<sql::ResultSet>& res,
			std::vector<std::string>& args);

	protected:
		~TaskList();

	private:
		System::ComponentModel::Container ^components;

		std::auto_ptr<sql::Statement>* stmt;
		std::auto_ptr<sql::PreparedStatement>* pstmt;
		std::auto_ptr<sql::ResultSet>* res;
		std::vector<std::string>* args;
		bool isByDates;

	private: System::Windows::Forms::CheckedListBox^ tasksListBox;
	private: System::Windows::Forms::MonthCalendar^ calendar;
	private: System::Windows::Forms::Button^ bugListButton;
	private: System::Windows::Forms::Label^ messageLabel;
	private: System::Windows::Forms::Button^ changeTasksButton;
    private: System::Windows::Forms::Button^ dateTasksButton;
	private: System::Windows::Forms::Label^ deadlineLabel;
	private: System::Windows::Forms::Label^ taskNameLabel;
	private: System::Windows::Forms::Label^ taskIdLabel;
	private: System::Windows::Forms::Label^ taskInfoLabel;
	private: System::Windows::Forms::Label^ creatorLabel;

	private: System::Windows::Forms::Label^ projectNameLabel;




	private: System::Windows::Forms::Label^ isDoneLabel;
	private: System::Windows::Forms::ListBox^ datesListBox;
	private: System::Windows::Forms::Label^ bugsLabel;
	private: System::Windows::Forms::Label^ toFixLabel;


	private: System::Windows::Forms::Button^ backButton;


#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion
		void UpdateTasksListBox();
		void ClearInfoLabels();
		void UpdateDatesListBox();
	private: System::Void dateTasksButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void tasksListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
	private: System::Void tasksListBox_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e);
	private: System::Void backButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void datesListBox_DoubleClick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ClearMessageLabel(System::Object^ sender, System::EventArgs^ e);
	private: System::Void changeTasksButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void bugListButton_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
