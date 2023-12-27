#include "TaskList.h"

using namespace kursovaya;

TaskList::TaskList(
	std::auto_ptr<sql::Statement>& stmt,
	std::auto_ptr<sql::PreparedStatement>& pstmt,
	std::auto_ptr<sql::ResultSet>& res,
	std::vector<std::string>& args) {
	InitializeComponent();
	this->stmt = &stmt;
	this->pstmt = &pstmt;
	this->res = &res;
	this->args = &args;
	this->args->at(0) = "exit";
	isByDates = 0;
	UpdateTasksListBox();
	UpdateDatesListBox();
}

TaskList::~TaskList() {
	if (components) {
		delete components;
	}
}

void TaskList::UpdateTasksListBox() {
	this->tasksListBox->Items->Clear();
	if (!isByDates)
		(*stmt)->execute("SELECT taskName, isDone FROM Tasks WHERE projectId = " +
			args->at(2));
	else
		(*stmt)->execute("SELECT taskName, isDone FROM Tasks WHERE deadline = '" +
			msclr::interop::marshal_as<std::string>(this->calendar->SelectionStart.ToString("yyyy-MM-dd")) +
			"' AND projectId = " + args->at(2));
	do {
		res->reset((*stmt)->getResultSet());
		while ((*res)->next()) {
			this->tasksListBox->Items->Add(msclr::interop::marshal_as<System::String^>((*res)->getString("taskName").asStdString()));
			if ((*res)->getBoolean("isDone"))
				this->tasksListBox->SetItemChecked(this->tasksListBox->Items->Count - 1, 1);
		}
	} while ((*stmt)->getMoreResults());
}

void TaskList::ClearInfoLabels() {
	this->taskIdLabel->Text = " ";
	this->taskNameLabel->Text = " ";
	this->deadlineLabel->Text = " ";
	this->isDoneLabel->Text = " ";
	this->projectNameLabel->Text = " ";
	this->creatorLabel->Text = " ";
	this->bugsLabel->Text = " ";
	this->toFixLabel->Text = " ";
}

void TaskList::UpdateDatesListBox() {
	this->datesListBox->Items->Clear();
	(*stmt)->execute("SELECT DISTINCT deadline FROM Tasks");
	do {
		res->reset((*stmt)->getResultSet());
		while ((*res)->next())
			this->datesListBox->Items->Add(msclr::interop::marshal_as<System::String^>((*res)->getString("deadline").asStdString()));
	} while ((*stmt)->getMoreResults());
}

System::Void TaskList::dateTasksButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabel(sender, e);
	if (isByDates) {
		this->dateTasksButton->Text = "Показать задачи по дате";
		isByDates = 0;
	}
	else {
		this->dateTasksButton->Text = "Показать все задачи";
		isByDates = 1;
	}
	ClearInfoLabels();
	UpdateTasksListBox();
}

System::Void TaskList::tasksListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->tasksListBox->SelectedItem == nullptr)
		return;
	res->reset((*stmt)->executeQuery("SELECT T.id, T.taskName, T.deadline, T.isDone, P.projectName, U.username, COUNT(B.id) AS bugs, SUM(IF(NOT B.isFixed, 1, 0)) AS toFix FROM Tasks AS T" \
		" LEFT JOIN Projects AS P ON P.id = T.projectId LEFT JOIN Users AS U ON U.id = T.userId LEFT JOIN Bugs AS B ON B.taskId = T.id WHERE T.taskName = '" +
		msclr::interop::marshal_as<std::string>(this->tasksListBox->SelectedItem->ToString()) + "'"));
	while ((*res)->next()) {
		this->taskIdLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("id").asStdString());
		this->taskNameLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("taskName").asStdString());
		this->deadlineLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("deadline").asStdString());
		this->isDoneLabel->Text = (*res)->getBoolean("isDone") ? "да" : "нет";
		this->projectNameLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("projectName").asStdString());
		this->creatorLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("username").asStdString());
		this->bugsLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("bugs").asStdString());
		this->toFixLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("toFix").asStdString());
	}
}

System::Void TaskList::tasksListBox_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e) {
	ClearMessageLabel(sender, e);
	if (this->tasksListBox->SelectedItem == nullptr)
		return;
	std::string isDone;
	if (e->CurrentValue == System::Windows::Forms::CheckState::Checked)
		isDone = "FALSE";
	else if (e->CurrentValue == System::Windows::Forms::CheckState::Unchecked)
		isDone = "TRUE";
	else {
		this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->messageLabel->Text = "Непредвиденная ошибка";
		return;
	}
	(*stmt)->execute("UPDATE Tasks SET isDone = " + isDone + " WHERE taskName = '" +
		msclr::interop::marshal_as<std::string>(this->tasksListBox->SelectedItem->ToString()) + "'");
	UpdateTasksListBox();
	ClearInfoLabels();
	this->tasksListBox->SetSelected(e->Index, 1);
	this->messageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	if (isDone == "TRUE")
		this->messageLabel->Text = "Задача помечена как выполненная";
	else
		this->messageLabel->Text = "Задача помечена как невыполненная";
}

System::Void TaskList::backButton_Click(System::Object^ sender, System::EventArgs^ e) {
	args->pop_back();
	args->at(0) = "SelectProject";
	this->Close();
}

System::Void TaskList::datesListBox_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabel(sender, e);
	if (this->datesListBox->SelectedItem == nullptr)
		return;
	isByDates = 0;
	System::DateTime buf;
	if (System::DateTime::TryParse(this->datesListBox->SelectedItem->ToString(), buf)) {
		this->calendar->SelectionStart = buf;
		dateTasksButton_Click(sender, e);
	}
}

System::Void TaskList::ClearMessageLabel(System::Object^ sender, System::EventArgs^ e) {
	this->messageLabel->Text = " ";
}

System::Void TaskList::changeTasksButton_Click(System::Object^ sender, System::EventArgs^ e) {
	res->reset((*stmt)->executeQuery("SELECT changeTasks From Roles WHERE id = (SELECT roleId FROM Users WHERE id = " + args->at(1) + ")"));
	while ((*res)->next())
		if (!(*res)->getBoolean("changeTasks")) {
			this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
			this->messageLabel->Text = "Ошибка: отказано в доступе";
			return;
		}
	args->at(0) = "ChangeTasks";
	this->Close();
}

System::Void TaskList::bugListButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (this->tasksListBox->SelectedItem == nullptr)
		return;
	res->reset((*stmt)->executeQuery("SELECT id FROM Tasks WHERE taskName = '" +
	msclr::interop::marshal_as<std::string>(this->tasksListBox->SelectedItem->ToString()) + "'"));
	args->at(0) = "BugList";
	while ((*res)->next()) {
		args->push_back((*res)->getString("id").asStdString());
		break;
	}
	this->Close();
}

#pragma region Windows Form Designer generated code
void TaskList::InitializeComponent(void) {
    this->tasksListBox = (gcnew System::Windows::Forms::CheckedListBox());
    this->calendar = (gcnew System::Windows::Forms::MonthCalendar());
    this->bugListButton = (gcnew System::Windows::Forms::Button());
    this->messageLabel = (gcnew System::Windows::Forms::Label());
    this->changeTasksButton = (gcnew System::Windows::Forms::Button());
    this->backButton = (gcnew System::Windows::Forms::Button());
    this->dateTasksButton = (gcnew System::Windows::Forms::Button());
    this->deadlineLabel = (gcnew System::Windows::Forms::Label());
    this->taskNameLabel = (gcnew System::Windows::Forms::Label());
    this->taskIdLabel = (gcnew System::Windows::Forms::Label());
    this->taskInfoLabel = (gcnew System::Windows::Forms::Label());
    this->creatorLabel = (gcnew System::Windows::Forms::Label());
    this->projectNameLabel = (gcnew System::Windows::Forms::Label());
    this->isDoneLabel = (gcnew System::Windows::Forms::Label());
    this->datesListBox = (gcnew System::Windows::Forms::ListBox());
    this->bugsLabel = (gcnew System::Windows::Forms::Label());
    this->toFixLabel = (gcnew System::Windows::Forms::Label());
    this->SuspendLayout();
    // 
    // tasksListBox
    // 
    this->tasksListBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
    this->tasksListBox->FormattingEnabled = true;
    this->tasksListBox->Location = System::Drawing::Point(12, 12);
    this->tasksListBox->Name = L"tasksListBox";
    this->tasksListBox->Size = System::Drawing::Size(291, 154);
    this->tasksListBox->TabIndex = 0;
    this->tasksListBox->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &TaskList::tasksListBox_ItemCheck);
    this->tasksListBox->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    this->tasksListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &TaskList::tasksListBox_SelectedIndexChanged);
    // 
    // calendar
    // 
    this->calendar->Location = System::Drawing::Point(309, 12);
    this->calendar->Margin = System::Windows::Forms::Padding(3);
    this->calendar->MaxSelectionCount = 1;
    this->calendar->Name = L"calendar";
    this->calendar->ShowTodayCircle = false;
    this->calendar->TabIndex = 0;
    // 
    // bugListButton
    // 
    this->bugListButton->AutoSize = true;
    this->bugListButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->bugListButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->bugListButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->bugListButton->Location = System::Drawing::Point(12, 373);
    this->bugListButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->bugListButton->Name = L"bugListButton";
    this->bugListButton->Size = System::Drawing::Size(204, 44);
    this->bugListButton->TabIndex = 15;
    this->bugListButton->Text = L"Список багов";
    this->bugListButton->UseVisualStyleBackColor = true;
    this->bugListButton->Click += gcnew System::EventHandler(this, &TaskList::bugListButton_Click);
    // 
    // messageLabel
    // 
    this->messageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
    this->messageLabel->Location = System::Drawing::Point(12, 473);
    this->messageLabel->Name = L"messageLabel";
    this->messageLabel->Size = System::Drawing::Size(461, 30);
    this->messageLabel->TabIndex = 16;
    this->messageLabel->Text = L" ";
    this->messageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
    // 
    // changeTasksButton
    // 
    this->changeTasksButton->AutoSize = true;
    this->changeTasksButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->changeTasksButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->changeTasksButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->changeTasksButton->Location = System::Drawing::Point(222, 373);
    this->changeTasksButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->changeTasksButton->Name = L"changeTasksButton";
    this->changeTasksButton->Size = System::Drawing::Size(251, 44);
    this->changeTasksButton->TabIndex = 17;
    this->changeTasksButton->Text = L"Настроить список задач";
    this->changeTasksButton->UseVisualStyleBackColor = true;
    this->changeTasksButton->Click += gcnew System::EventHandler(this, &TaskList::changeTasksButton_Click);
    // 
    // backButton
    // 
    this->backButton->AutoSize = true;
    this->backButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->backButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->backButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->backButton->Location = System::Drawing::Point(315, 425);
    this->backButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->backButton->Name = L"backButton";
    this->backButton->Size = System::Drawing::Size(158, 44);
    this->backButton->TabIndex = 18;
    this->backButton->Text = L"Назад";
    this->backButton->UseVisualStyleBackColor = true;
    this->backButton->Click += gcnew System::EventHandler(this, &TaskList::backButton_Click);
    // 
    // dateTasksButton
    // 
    this->dateTasksButton->AutoSize = true;
    this->dateTasksButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->dateTasksButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->dateTasksButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->dateTasksButton->Location = System::Drawing::Point(12, 425);
    this->dateTasksButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->dateTasksButton->Name = L"dateTasksButton";
    this->dateTasksButton->Size = System::Drawing::Size(297, 44);
    this->dateTasksButton->TabIndex = 19;
    this->dateTasksButton->Text = L"Показать задачи по дате";
    this->dateTasksButton->UseVisualStyleBackColor = true;
    this->dateTasksButton->Click += gcnew System::EventHandler(this, &TaskList::dateTasksButton_Click);
    // 
    // deadlineLabel
    // 
    this->deadlineLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->deadlineLabel->Location = System::Drawing::Point(123, 225);
    this->deadlineLabel->Name = L"deadlineLabel";
    this->deadlineLabel->Size = System::Drawing::Size(180, 24);
    this->deadlineLabel->TabIndex = 23;
    this->deadlineLabel->Text = L" ";
    this->deadlineLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // taskNameLabel
    // 
    this->taskNameLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->taskNameLabel->Location = System::Drawing::Point(123, 201);
    this->taskNameLabel->Name = L"taskNameLabel";
    this->taskNameLabel->Size = System::Drawing::Size(180, 24);
    this->taskNameLabel->TabIndex = 22;
    this->taskNameLabel->Text = L" ";
    this->taskNameLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // taskIdLabel
    // 
    this->taskIdLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->taskIdLabel->Location = System::Drawing::Point(123, 177);
    this->taskIdLabel->Name = L"taskIdLabel";
    this->taskIdLabel->Size = System::Drawing::Size(180, 24);
    this->taskIdLabel->TabIndex = 21;
    this->taskIdLabel->Text = L" ";
    this->taskIdLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // taskInfoLabel
    // 
    this->taskInfoLabel->AutoSize = true;
    this->taskInfoLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->taskInfoLabel->Location = System::Drawing::Point(12, 177);
    this->taskInfoLabel->Name = L"taskInfoLabel";
    this->taskInfoLabel->Size = System::Drawing::Size(105, 192);
    this->taskInfoLabel->TabIndex = 20;
    this->taskInfoLabel->Text = L"id:\r\nЗадача:\r\nДедлайн:\r\nВыполнена:\r\nПроект:\r\nСоздатель:\r\nВсего багов:\r\nИсправить:"
        L"";
    this->taskInfoLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // creatorLabel
    // 
    this->creatorLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->creatorLabel->Location = System::Drawing::Point(123, 297);
    this->creatorLabel->Name = L"creatorLabel";
    this->creatorLabel->Size = System::Drawing::Size(180, 24);
    this->creatorLabel->TabIndex = 26;
    this->creatorLabel->Text = L" ";
    this->creatorLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // projectNameLabel
    // 
    this->projectNameLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->projectNameLabel->Location = System::Drawing::Point(123, 273);
    this->projectNameLabel->Name = L"projectNameLabel";
    this->projectNameLabel->Size = System::Drawing::Size(180, 24);
    this->projectNameLabel->TabIndex = 25;
    this->projectNameLabel->Text = L" ";
    this->projectNameLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // isDoneLabel
    // 
    this->isDoneLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->isDoneLabel->Location = System::Drawing::Point(123, 249);
    this->isDoneLabel->Name = L"isDoneLabel";
    this->isDoneLabel->Size = System::Drawing::Size(180, 24);
    this->isDoneLabel->TabIndex = 24;
    this->isDoneLabel->Text = L" ";
    this->isDoneLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // datesListBox
    // 
    this->datesListBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
    this->datesListBox->FormattingEnabled = true;
    this->datesListBox->ItemHeight = 16;
    this->datesListBox->Location = System::Drawing::Point(309, 180);
    this->datesListBox->Name = L"datesListBox";
    this->datesListBox->Size = System::Drawing::Size(164, 178);
    this->datesListBox->TabIndex = 27;
    this->datesListBox->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    this->datesListBox->DoubleClick += gcnew System::EventHandler(this, &TaskList::datesListBox_DoubleClick);
    // 
    // bugsLabel
    // 
    this->bugsLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->bugsLabel->Location = System::Drawing::Point(123, 321);
    this->bugsLabel->Name = L"bugsLabel";
    this->bugsLabel->Size = System::Drawing::Size(180, 24);
    this->bugsLabel->TabIndex = 28;
    this->bugsLabel->Text = L" ";
    this->bugsLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // toFixLabel
    // 
    this->toFixLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->toFixLabel->Location = System::Drawing::Point(123, 345);
    this->toFixLabel->Name = L"toFixLabel";
    this->toFixLabel->Size = System::Drawing::Size(180, 24);
    this->toFixLabel->TabIndex = 29;
    this->toFixLabel->Text = L" ";
    this->toFixLabel->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    // 
    // TaskList
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(485, 512);
    this->Controls->Add(this->toFixLabel);
    this->Controls->Add(this->bugsLabel);
    this->Controls->Add(this->datesListBox);
    this->Controls->Add(this->creatorLabel);
    this->Controls->Add(this->projectNameLabel);
    this->Controls->Add(this->isDoneLabel);
    this->Controls->Add(this->deadlineLabel);
    this->Controls->Add(this->taskNameLabel);
    this->Controls->Add(this->taskIdLabel);
    this->Controls->Add(this->taskInfoLabel);
    this->Controls->Add(this->dateTasksButton);
    this->Controls->Add(this->backButton);
    this->Controls->Add(this->changeTasksButton);
    this->Controls->Add(this->messageLabel);
    this->Controls->Add(this->bugListButton);
    this->Controls->Add(this->tasksListBox);
    this->Controls->Add(this->calendar);
    this->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 10));
    this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
    this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->MaximizeBox = false;
    this->MinimizeBox = false;
    this->Name = L"TaskList";
    this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
    this->Text = L"Список задач";
    this->Click += gcnew System::EventHandler(this, &TaskList::ClearMessageLabel);
    this->ResumeLayout(false);
    this->PerformLayout();

}
#pragma endregion
