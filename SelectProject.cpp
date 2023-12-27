#include "SelectProject.h"

using namespace kursovaya;

SelectProject::SelectProject(
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
	UpdateUserInfo();
	UpdateProjects();
}

SelectProject::~SelectProject() {
	if (components) {
		delete components;
	}
}

void SelectProject::UpdateUserInfo() {
	this->idLabel->Text = msclr::interop::marshal_as<System::String^>(args->at(1));
	res->reset((*stmt)->executeQuery("SELECT userName FROM Users WHERE id = " + args->at(1)));
	while ((*res)->next())
		this->loginLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("userName").asStdString());
	res->reset((*stmt)->executeQuery("SELECT roleName FROM Roles WHERE id = (SELECT roleId FROM Users WHERE id = " + args->at(1) + ")"));
	while ((*res)->next())
		this->roleLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("roleName").asStdString());
}

void SelectProject::UpdateProjects() {
	this->projectsListBox->Items->Clear();
	(*stmt)->execute("SELECT projectName FROM Projects");
	do {
		res->reset((*stmt)->getResultSet());
		while ((*res)->next()) {
			this->projectsListBox->Items->Add(msclr::interop::marshal_as<System::String^>((*res)->getString("projectName").asStdString()));
		}
	} while ((*stmt)->getMoreResults());
}

System::Void SelectProject::tabControl_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	this->Text = this->tabControl->SelectedTab->Text;
	this->ClearMessageLabels(sender, e);
}

System::Void SelectProject::changeLoginButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (this->changeLoginTextBox->Text == "")
		return;
	res->reset((*stmt)->executeQuery(
		"SELECT id FROM Users WHERE BINARY userName = '" +
		msclr::interop::marshal_as<std::string>(this->changeLoginTextBox->Text) +
		"'"));
	while ((*res)->next()) {
		this->userMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->userMessageLabel->Text = "Ошибка: логин занят";
		return;
	}
	(*stmt)->execute("UPDATE Users SET userName = '" +
		msclr::interop::marshal_as<std::string>(this->changeLoginTextBox->Text) +
		"' WHERE id = " +
		args->at(1));
	this->changeLoginTextBox->Text = "";
	this->userMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	this->userMessageLabel->Text = "Логин успешно изменён";
	UpdateUserInfo();
}

System::Void SelectProject::changePasswordButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (this->changePasswordTextBox->Text == "")
		return;
	(*stmt)->execute("UPDATE Users SET password = '" +
		msclr::interop::marshal_as<std::string>(this->changePasswordTextBox->Text) +
		"' WHERE id = " +
		args->at(1));
	this->changePasswordTextBox->Text = "";
	this->userMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	this->userMessageLabel->Text = "Пароль успешно изменён";
}

System::Void SelectProject::changeRolesButton_Click(System::Object^ sender, System::EventArgs^ e) {
	res->reset((*stmt)->executeQuery("SELECT changeRoles FROM Roles WHERE id = (SELECT roleId FROM Users WHERE id = " +
		args->at(1) + ")"));
	while ((*res)->next())
		if ((*res)->getBoolean("changeRoles")) {
			args->at(0) = "ChangeRoles";
			this->Close();
		}
		else {
			this->userMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
			this->userMessageLabel->Text = "Ошибка: отказано в доступе";
		}
}

System::Void SelectProject::changeProjectsButton_Click(System::Object^ sender, System::EventArgs^ e) {
	res->reset((*stmt)->executeQuery("SELECT changeProjects FROM Roles WHERE id = (SELECT roleId FROM Users WHERE id = " +
		args->at(1) + ")"));
	while ((*res)->next()) {
		if ((*res)->getBoolean("changeProjects")) {
			args->at(0) = "ChangeProjects";
			this->Close();
		}
		else
			this->projectsErrorLabel->Text = "Ошибка: отказано в доступе";
	}
}

System::Void SelectProject::logOutButton_Click(System::Object^ sender, System::EventArgs^ e) {
	args->clear();
	args->push_back("Login");
	this->Close();
}

System::Void SelectProject::projectsListBox_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
	if (this->projectsListBox->SelectedItem == nullptr)
		return;
	args->at(0) = "TaskList";
	res->reset((*stmt)->executeQuery("SELECT id FROM Projects WHERE projectName = '" +
	msclr::interop::marshal_as<std::string>(this->projectsListBox->SelectedItem->ToString()) + "'"));
	while ((*res)->next()) {
		args->push_back((*res)->getString("id"));
		break;
	}
	this->Close();
}

System::Void SelectProject::ClearMessageLabels(System::Object^ sender, System::EventArgs^ e) {
	this->userMessageLabel->Text = " ";
	this->projectsErrorLabel->Text = " ";
}

System::Void SelectProject::deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
	res->reset((*stmt)->executeQuery("SELECT roleName FROM Roles" \
		" WHERE id = (SELECT roleId FROM Users WHERE id = " + args->at(1) + ")"));
	while ((*res)->next())
		if ((*res)->getString("roleName") == "Admin") {
			this->userMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
			this->userMessageLabel->Text = "Ошибка: администратора нельзя удалить";
			return;
		}
	(*stmt)->execute("DELETE FROM Users WHERE id = " + args->at(1));
	args->clear();
	args->push_back("Login");
	this->Close();
}

System::Void SelectProject::projectsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->projectsListBox->SelectedItem == nullptr)
		return;
	res->reset((*stmt)->executeQuery("SELECT Projects.id, Projects.projectName, Users.username, COUNT(Tasks.id) as tasks, " \
		"SUM(IF(Tasks.isDone, 1, 0)) AS done FROM Projects LEFT JOIN Users ON Users.id = Projects.userId " \
		"LEFT JOIN Tasks ON Tasks.projectId = Projects.id WHERE Projects.projectName = '" +
		msclr::interop::marshal_as<std::string>(this->projectsListBox->SelectedItem->ToString()) + "'"));
	while ((*res)->next()) {
		this->projectIdLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("id").asStdString());
		this->projectNameLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("projectName").asStdString());
		this->creatorLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("username").asStdString());
		this->amountOfTasksLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("tasks").asStdString());
		int progress = round(static_cast<float>((*res)->getInt("done")) / (*res)->getInt("tasks") * 100);
		this->progressLabel->Text = progress.ToString() + "%";
		this->progressBar->Value = progress;
	}
}

#pragma region Windows Form Designer generated code
void SelectProject::InitializeComponent(void) {
    this->userInfoLabel = (gcnew System::Windows::Forms::Label());
    this->idLabel = (gcnew System::Windows::Forms::Label());
    this->loginLabel = (gcnew System::Windows::Forms::Label());
    this->roleLabel = (gcnew System::Windows::Forms::Label());
    this->tabControl = (gcnew System::Windows::Forms::TabControl());
    this->selectProjectPage = (gcnew System::Windows::Forms::TabPage());
    this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
    this->progressLabel = (gcnew System::Windows::Forms::Label());
    this->amountOfTasksLabel = (gcnew System::Windows::Forms::Label());
    this->creatorLabel = (gcnew System::Windows::Forms::Label());
    this->projectNameLabel = (gcnew System::Windows::Forms::Label());
    this->projectIdLabel = (gcnew System::Windows::Forms::Label());
    this->projectsErrorLabel = (gcnew System::Windows::Forms::Label());
    this->changeProjectsButton = (gcnew System::Windows::Forms::Button());
    this->projectsListBox = (gcnew System::Windows::Forms::ListBox());
    this->projectInfoLabel = (gcnew System::Windows::Forms::Label());
    this->userSettingPage = (gcnew System::Windows::Forms::TabPage());
    this->deleteButton = (gcnew System::Windows::Forms::Button());
    this->logOutButton = (gcnew System::Windows::Forms::Button());
    this->changeRolesButton = (gcnew System::Windows::Forms::Button());
    this->userMessageLabel = (gcnew System::Windows::Forms::Label());
    this->changePasswordButton = (gcnew System::Windows::Forms::Button());
    this->changeLoginButton = (gcnew System::Windows::Forms::Button());
    this->changeLoginTextBox = (gcnew System::Windows::Forms::TextBox());
    this->changePasswordTextBox = (gcnew System::Windows::Forms::TextBox());
    this->changePasswordLabel = (gcnew System::Windows::Forms::Label());
    this->changeLoginLabel = (gcnew System::Windows::Forms::Label());
    this->tabControl->SuspendLayout();
    this->selectProjectPage->SuspendLayout();
    this->userSettingPage->SuspendLayout();
    this->SuspendLayout();
    // 
    // userInfoLabel
    // 
    this->userInfoLabel->AutoSize = true;
    this->userInfoLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->userInfoLabel->Location = System::Drawing::Point(1, 4);
    this->userInfoLabel->Name = L"userInfoLabel";
    this->userInfoLabel->Size = System::Drawing::Size(62, 72);
    this->userInfoLabel->TabIndex = 2;
    this->userInfoLabel->Text = L"id:\r\nЛогин:\r\nРоль:";
    this->userInfoLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // idLabel
    // 
    this->idLabel->AutoSize = true;
    this->idLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->idLabel->Location = System::Drawing::Point(69, 4);
    this->idLabel->Name = L"idLabel";
    this->idLabel->Size = System::Drawing::Size(14, 24);
    this->idLabel->TabIndex = 3;
    this->idLabel->Text = L".";
    this->idLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // loginLabel
    // 
    this->loginLabel->AutoSize = true;
    this->loginLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->loginLabel->Location = System::Drawing::Point(69, 28);
    this->loginLabel->Name = L"loginLabel";
    this->loginLabel->Size = System::Drawing::Size(14, 24);
    this->loginLabel->TabIndex = 4;
    this->loginLabel->Text = L".";
    this->loginLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // roleLabel
    // 
    this->roleLabel->AutoSize = true;
    this->roleLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->roleLabel->Location = System::Drawing::Point(69, 52);
    this->roleLabel->Name = L"roleLabel";
    this->roleLabel->Size = System::Drawing::Size(14, 24);
    this->roleLabel->TabIndex = 5;
    this->roleLabel->Text = L".";
    this->roleLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // tabControl
    // 
    this->tabControl->Controls->Add(this->selectProjectPage);
    this->tabControl->Controls->Add(this->userSettingPage);
    this->tabControl->Location = System::Drawing::Point(12, 15);
    this->tabControl->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->tabControl->Name = L"tabControl";
    this->tabControl->SelectedIndex = 0;
    this->tabControl->Size = System::Drawing::Size(387, 458);
    this->tabControl->TabIndex = 6;
    this->tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &SelectProject::tabControl_SelectedIndexChanged);
    this->tabControl->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // selectProjectPage
    // 
    this->selectProjectPage->Controls->Add(this->progressBar);
    this->selectProjectPage->Controls->Add(this->progressLabel);
    this->selectProjectPage->Controls->Add(this->amountOfTasksLabel);
    this->selectProjectPage->Controls->Add(this->creatorLabel);
    this->selectProjectPage->Controls->Add(this->projectNameLabel);
    this->selectProjectPage->Controls->Add(this->projectIdLabel);
    this->selectProjectPage->Controls->Add(this->projectsErrorLabel);
    this->selectProjectPage->Controls->Add(this->changeProjectsButton);
    this->selectProjectPage->Controls->Add(this->projectsListBox);
    this->selectProjectPage->Controls->Add(this->projectInfoLabel);
    this->selectProjectPage->Location = System::Drawing::Point(4, 25);
    this->selectProjectPage->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->selectProjectPage->Name = L"selectProjectPage";
    this->selectProjectPage->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->selectProjectPage->Size = System::Drawing::Size(379, 429);
    this->selectProjectPage->TabIndex = 0;
    this->selectProjectPage->Text = L"Выбрать проект";
    this->selectProjectPage->UseVisualStyleBackColor = true;
    this->selectProjectPage->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // progressBar
    // 
    this->progressBar->Location = System::Drawing::Point(182, 317);
    this->progressBar->Name = L"progressBar";
    this->progressBar->Size = System::Drawing::Size(191, 23);
    this->progressBar->TabIndex = 22;
    // 
    // progressLabel
    // 
    this->progressLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->progressLabel->Location = System::Drawing::Point(122, 317);
    this->progressLabel->Name = L"progressLabel";
    this->progressLabel->Size = System::Drawing::Size(54, 24);
    this->progressLabel->TabIndex = 21;
    this->progressLabel->Text = L" ";
    // 
    // amountOfTasksLabel
    // 
    this->amountOfTasksLabel->AutoSize = true;
    this->amountOfTasksLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->amountOfTasksLabel->Location = System::Drawing::Point(122, 293);
    this->amountOfTasksLabel->Name = L"amountOfTasksLabel";
    this->amountOfTasksLabel->Size = System::Drawing::Size(15, 24);
    this->amountOfTasksLabel->TabIndex = 20;
    this->amountOfTasksLabel->Text = L" ";
    // 
    // creatorLabel
    // 
    this->creatorLabel->AutoSize = true;
    this->creatorLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->creatorLabel->Location = System::Drawing::Point(122, 269);
    this->creatorLabel->Name = L"creatorLabel";
    this->creatorLabel->Size = System::Drawing::Size(15, 24);
    this->creatorLabel->TabIndex = 19;
    this->creatorLabel->Text = L" ";
    // 
    // projectNameLabel
    // 
    this->projectNameLabel->AutoSize = true;
    this->projectNameLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->projectNameLabel->Location = System::Drawing::Point(122, 245);
    this->projectNameLabel->Name = L"projectNameLabel";
    this->projectNameLabel->Size = System::Drawing::Size(15, 24);
    this->projectNameLabel->TabIndex = 18;
    this->projectNameLabel->Text = L" ";
    // 
    // projectIdLabel
    // 
    this->projectIdLabel->AutoSize = true;
    this->projectIdLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->projectIdLabel->Location = System::Drawing::Point(122, 221);
    this->projectIdLabel->Name = L"projectIdLabel";
    this->projectIdLabel->Size = System::Drawing::Size(15, 24);
    this->projectIdLabel->TabIndex = 17;
    this->projectIdLabel->Text = L" ";
    // 
    // projectsErrorLabel
    // 
    this->projectsErrorLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->projectsErrorLabel->ForeColor = System::Drawing::Color::DarkRed;
    this->projectsErrorLabel->Location = System::Drawing::Point(6, 395);
    this->projectsErrorLabel->Name = L"projectsErrorLabel";
    this->projectsErrorLabel->Size = System::Drawing::Size(367, 30);
    this->projectsErrorLabel->TabIndex = 15;
    this->projectsErrorLabel->Text = L" ";
    this->projectsErrorLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
    this->projectsErrorLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // changeProjectsButton
    // 
    this->changeProjectsButton->AutoSize = true;
    this->changeProjectsButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->changeProjectsButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->changeProjectsButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->changeProjectsButton->Location = System::Drawing::Point(70, 347);
    this->changeProjectsButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->changeProjectsButton->Name = L"changeProjectsButton";
    this->changeProjectsButton->Size = System::Drawing::Size(239, 44);
    this->changeProjectsButton->TabIndex = 14;
    this->changeProjectsButton->Text = L"Настроить список проектов";
    this->changeProjectsButton->UseVisualStyleBackColor = true;
    this->changeProjectsButton->Click += gcnew System::EventHandler(this, &SelectProject::changeProjectsButton_Click);
    // 
    // projectsListBox
    // 
    this->projectsListBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
    this->projectsListBox->FormattingEnabled = true;
    this->projectsListBox->ItemHeight = 16;
    this->projectsListBox->Location = System::Drawing::Point(6, 7);
    this->projectsListBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->projectsListBox->Name = L"projectsListBox";
    this->projectsListBox->Size = System::Drawing::Size(367, 210);
    this->projectsListBox->TabIndex = 0;
    this->projectsListBox->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    this->projectsListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &SelectProject::projectsListBox_SelectedIndexChanged);
    this->projectsListBox->DoubleClick += gcnew System::EventHandler(this, &SelectProject::projectsListBox_DoubleClick);
    // 
    // projectInfoLabel
    // 
    this->projectInfoLabel->AutoSize = true;
    this->projectInfoLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->projectInfoLabel->Location = System::Drawing::Point(6, 221);
    this->projectInfoLabel->Name = L"projectInfoLabel";
    this->projectInfoLabel->Size = System::Drawing::Size(110, 120);
    this->projectInfoLabel->TabIndex = 16;
    this->projectInfoLabel->Text = L"id:\r\nПроект:\r\nСоздатель:\r\nВсего задач:\r\nВыполнено:";
    // 
    // userSettingPage
    // 
    this->userSettingPage->Controls->Add(this->deleteButton);
    this->userSettingPage->Controls->Add(this->logOutButton);
    this->userSettingPage->Controls->Add(this->changeRolesButton);
    this->userSettingPage->Controls->Add(this->userMessageLabel);
    this->userSettingPage->Controls->Add(this->changePasswordButton);
    this->userSettingPage->Controls->Add(this->changeLoginButton);
    this->userSettingPage->Controls->Add(this->changeLoginTextBox);
    this->userSettingPage->Controls->Add(this->changePasswordTextBox);
    this->userSettingPage->Controls->Add(this->changePasswordLabel);
    this->userSettingPage->Controls->Add(this->changeLoginLabel);
    this->userSettingPage->Controls->Add(this->roleLabel);
    this->userSettingPage->Controls->Add(this->userInfoLabel);
    this->userSettingPage->Controls->Add(this->loginLabel);
    this->userSettingPage->Controls->Add(this->idLabel);
    this->userSettingPage->Location = System::Drawing::Point(4, 25);
    this->userSettingPage->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->userSettingPage->Name = L"userSettingPage";
    this->userSettingPage->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->userSettingPage->RightToLeft = System::Windows::Forms::RightToLeft::No;
    this->userSettingPage->Size = System::Drawing::Size(379, 429);
    this->userSettingPage->TabIndex = 1;
    this->userSettingPage->Text = L"Настройки пользователя";
    this->userSettingPage->UseVisualStyleBackColor = true;
    this->userSettingPage->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // deleteButton
    // 
    this->deleteButton->AutoSize = true;
    this->deleteButton->BackColor = System::Drawing::Color::IndianRed;
    this->deleteButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->deleteButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->deleteButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->deleteButton->Location = System::Drawing::Point(193, 265);
    this->deleteButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->deleteButton->Name = L"deleteButton";
    this->deleteButton->Size = System::Drawing::Size(180, 44);
    this->deleteButton->TabIndex = 15;
    this->deleteButton->Text = L"Удалить аккаунт";
    this->deleteButton->UseVisualStyleBackColor = false;
    this->deleteButton->Click += gcnew System::EventHandler(this, &SelectProject::deleteButton_Click);
    // 
    // logOutButton
    // 
    this->logOutButton->AutoSize = true;
    this->logOutButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->logOutButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->logOutButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->logOutButton->Location = System::Drawing::Point(6, 265);
    this->logOutButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->logOutButton->Name = L"logOutButton";
    this->logOutButton->Size = System::Drawing::Size(180, 44);
    this->logOutButton->TabIndex = 14;
    this->logOutButton->Text = L"Выйти из аккаунта";
    this->logOutButton->UseVisualStyleBackColor = true;
    this->logOutButton->Click += gcnew System::EventHandler(this, &SelectProject::logOutButton_Click);
    // 
    // changeRolesButton
    // 
    this->changeRolesButton->AutoSize = true;
    this->changeRolesButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->changeRolesButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->changeRolesButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->changeRolesButton->Location = System::Drawing::Point(225, 55);
    this->changeRolesButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->changeRolesButton->Name = L"changeRolesButton";
    this->changeRolesButton->Size = System::Drawing::Size(148, 44);
    this->changeRolesButton->TabIndex = 13;
    this->changeRolesButton->Text = L"Назначить роли";
    this->changeRolesButton->UseVisualStyleBackColor = true;
    this->changeRolesButton->Click += gcnew System::EventHandler(this, &SelectProject::changeRolesButton_Click);
    // 
    // userMessageLabel
    // 
    this->userMessageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->userMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
    this->userMessageLabel->Location = System::Drawing::Point(6, 176);
    this->userMessageLabel->Name = L"userMessageLabel";
    this->userMessageLabel->Size = System::Drawing::Size(367, 30);
    this->userMessageLabel->TabIndex = 12;
    this->userMessageLabel->Text = L" ";
    this->userMessageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
    this->userMessageLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // changePasswordButton
    // 
    this->changePasswordButton->AutoSize = true;
    this->changePasswordButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->changePasswordButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->changePasswordButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->changePasswordButton->Location = System::Drawing::Point(193, 213);
    this->changePasswordButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->changePasswordButton->Name = L"changePasswordButton";
    this->changePasswordButton->Size = System::Drawing::Size(180, 44);
    this->changePasswordButton->TabIndex = 11;
    this->changePasswordButton->Text = L"Сменить пароль";
    this->changePasswordButton->UseVisualStyleBackColor = true;
    this->changePasswordButton->Click += gcnew System::EventHandler(this, &SelectProject::changePasswordButton_Click);
    // 
    // changeLoginButton
    // 
    this->changeLoginButton->AutoSize = true;
    this->changeLoginButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->changeLoginButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->changeLoginButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->changeLoginButton->Location = System::Drawing::Point(6, 213);
    this->changeLoginButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->changeLoginButton->Name = L"changeLoginButton";
    this->changeLoginButton->Size = System::Drawing::Size(180, 44);
    this->changeLoginButton->TabIndex = 10;
    this->changeLoginButton->Text = L"Сменить логин";
    this->changeLoginButton->UseVisualStyleBackColor = true;
    this->changeLoginButton->Click += gcnew System::EventHandler(this, &SelectProject::changeLoginButton_Click);
    // 
    // changeLoginTextBox
    // 
    this->changeLoginTextBox->Location = System::Drawing::Point(153, 113);
    this->changeLoginTextBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->changeLoginTextBox->Name = L"changeLoginTextBox";
    this->changeLoginTextBox->Size = System::Drawing::Size(220, 24);
    this->changeLoginTextBox->TabIndex = 9;
    this->changeLoginTextBox->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // changePasswordTextBox
    // 
    this->changePasswordTextBox->Location = System::Drawing::Point(153, 143);
    this->changePasswordTextBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->changePasswordTextBox->Name = L"changePasswordTextBox";
    this->changePasswordTextBox->Size = System::Drawing::Size(220, 24);
    this->changePasswordTextBox->TabIndex = 8;
    this->changePasswordTextBox->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // changePasswordLabel
    // 
    this->changePasswordLabel->AutoSize = true;
    this->changePasswordLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->changePasswordLabel->Location = System::Drawing::Point(6, 138);
    this->changePasswordLabel->Name = L"changePasswordLabel";
    this->changePasswordLabel->Size = System::Drawing::Size(141, 24);
    this->changePasswordLabel->TabIndex = 7;
    this->changePasswordLabel->Text = L"Сменить пароль:";
    this->changePasswordLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // changeLoginLabel
    // 
    this->changeLoginLabel->AutoSize = true;
    this->changeLoginLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->changeLoginLabel->Location = System::Drawing::Point(6, 108);
    this->changeLoginLabel->Name = L"changeLoginLabel";
    this->changeLoginLabel->Size = System::Drawing::Size(131, 24);
    this->changeLoginLabel->TabIndex = 6;
    this->changeLoginLabel->Text = L"Сменить логин:";
    this->changeLoginLabel->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    // 
    // SelectProject
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(411, 486);
    this->Controls->Add(this->tabControl);
    this->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 10));
    this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
    this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->MaximizeBox = false;
    this->MinimizeBox = false;
    this->Name = L"SelectProject";
    this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
    this->Text = L"Выбрать проект";
    this->Click += gcnew System::EventHandler(this, &SelectProject::ClearMessageLabels);
    this->tabControl->ResumeLayout(false);
    this->selectProjectPage->ResumeLayout(false);
    this->selectProjectPage->PerformLayout();
    this->userSettingPage->ResumeLayout(false);
    this->userSettingPage->PerformLayout();
    this->ResumeLayout(false);

}
#pragma endregion