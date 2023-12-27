#include "ChangeTasks.h"

using namespace kursovaya;

ChangeTasks::ChangeTasks(
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
}

ChangeTasks::~ChangeTasks() {
	if (components) {
		delete components;
	}
}

System::Void ChangeTasks::ClearMessageLabels(System::Object^ sender, System::EventArgs^ e) {
	this->addMessageLabel->Text = " ";
	this->changeMessageLabel->Text = " ";
	this->deleteMessageLabel->Text = " ";
}

System::Void kursovaya::ChangeTasks::tabControl_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	this->Text = this->tabControl->SelectedTab->Text;
	ClearMessageLabels(sender, e);
}

System::Void kursovaya::ChangeTasks::backButton_Click(System::Object^ sender, System::EventArgs^ e) {
	args->at(0) = "TaskList";
	this->Close();
}

System::Void kursovaya::ChangeTasks::addButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabels(sender, e);
	if (this->addNameTextBox->Text == "")
		return;
	System::DateTime buf;
	if (!System::DateTime::TryParse(this->addDateTextBox->Text, buf)) {
		this->addMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->addMessageLabel->Text = "Сначала введите дату в формате дд.мм.гггг";
		return;
	}
	res->reset((*stmt)->executeQuery("SELECT id FROM Tasks WHERE BINARY taskName = '" +
		msclr::interop::marshal_as<std::string>(this->addNameTextBox->Text) + "'"));
	while ((*res)->next()) {
		this->addMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->addMessageLabel->Text = "Ошибка: задача с таким именем уже существует";
		return;
	}
	(*stmt)->execute("INSERT INTO Tasks(taskName, deadline, projectId, userId) VALUES ('" +
		msclr::interop::marshal_as<std::string>(this->addNameTextBox->Text) + "', '" +
		msclr::interop::marshal_as<std::string>(buf.ToString("yyyy-MM-dd")) + "', " +
		args->at(2) + ", " + args->at(1) + ")");
	this->addNameTextBox->Text = "";
	this->addDateTextBox->Text = "  .  .";
	this->addMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	this->addMessageLabel->Text = "Задача успешно добавлена";
}

System::Void kursovaya::ChangeTasks::changeNameButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabels(sender, e);
	if (this->oldNameTextBox->Text == "" || this->newNameTextBox->Text == "")
		return;
	res->reset((*stmt)->executeQuery("SELECT id FROM Tasks WHERE BINARY taskName = '" +
		msclr::interop::marshal_as<std::string>(this->newNameTextBox->Text) + "'"));
	while ((*res)->next()) {
		this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->changeMessageLabel->Text = "Ошибка: задача с таким именем уже существует";
		return;
	}
	(*stmt)->execute("UPDATE Tasks SET taskName = '" +
		msclr::interop::marshal_as<std::string>(this->newNameTextBox->Text) +
		"' WHERE taskName = '" +
		msclr::interop::marshal_as<std::string>(this->oldNameTextBox->Text) + "'");
	this->newNameTextBox->Text = "";
	this->oldNameTextBox->Text = "";
	this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	this->changeMessageLabel->Text = "Задача успешно переименована";
}

System::Void kursovaya::ChangeTasks::changeDateButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabels(sender, e);
	if (this->oldNameTextBox->Text == "") {
		this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->changeMessageLabel->Text = "Введите имя проекта в поле \"Старое имя\"";
		return;
	}
	System::DateTime buf;
	if (!System::DateTime::TryParse(this->changeDateTextBox->Text, buf)) {
		this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->changeMessageLabel->Text = "Сначала введите дату в формате дд.мм.гггг";
		return;
	}
	res->reset((*stmt)->executeQuery("SELECT id FROM Tasks WHERE BINARY taskName = '" +
		msclr::interop::marshal_as<std::string>(this->oldNameTextBox->Text) + "'"));
	while ((*res)->next()) {
		(*stmt)->execute("UPDATE Tasks SET deadline = '" +
			msclr::interop::marshal_as<std::string>(buf.ToString("yyyy-MM-dd")) +
			"' WHERE taskName = '" +
			msclr::interop::marshal_as<std::string>(this->oldNameTextBox->Text) + "'");
		this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
		this->changeMessageLabel->Text = "Дедлайн задачи успешно изменён";
		return;
	}
	this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->changeMessageLabel->Text = "Ошибка: задача с таким именем не найдена";
}

System::Void kursovaya::ChangeTasks::deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabels(sender, e);
	if (this->deleteTextBox->Text == "")
		return;
	res->reset((*stmt)->executeQuery("SELECT id FROM Tasks WHERE BINARY taskName = '" +
		msclr::interop::marshal_as<std::string>(this->deleteTextBox->Text) + "'"));
	while ((*res)->next()) {
		(*stmt)->execute("DELETE FROM Tasks WHERE taskName = '" +
			msclr::interop::marshal_as<std::string>(this->deleteTextBox->Text) + "'");
		this->deleteTextBox->Text = "";
		this->deleteMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
		this->deleteMessageLabel->Text = "Задача успешно удалена";
		return;
	}
	this->deleteMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->deleteMessageLabel->Text = "Ошибка: задача с таким именем не найдена";
}

#pragma region Windows Form Designer generated code
void ChangeTasks::InitializeComponent(void) {
	this->backButton = (gcnew System::Windows::Forms::Button());
	this->tabControl = (gcnew System::Windows::Forms::TabControl());
	this->addPage = (gcnew System::Windows::Forms::TabPage());
	this->addDateTextBox = (gcnew System::Windows::Forms::MaskedTextBox());
	this->addMessageLabel = (gcnew System::Windows::Forms::Label());
	this->addButton = (gcnew System::Windows::Forms::Button());
	this->addNameTextBox = (gcnew System::Windows::Forms::TextBox());
	this->addLabel = (gcnew System::Windows::Forms::Label());
	this->changePage = (gcnew System::Windows::Forms::TabPage());
	this->changeDateButton = (gcnew System::Windows::Forms::Button());
	this->newNameTextBox = (gcnew System::Windows::Forms::TextBox());
	this->changeDateTextBox = (gcnew System::Windows::Forms::MaskedTextBox());
	this->oldNameTextBox = (gcnew System::Windows::Forms::TextBox());
	this->changeLabel = (gcnew System::Windows::Forms::Label());
	this->changeMessageLabel = (gcnew System::Windows::Forms::Label());
	this->changeNameButton = (gcnew System::Windows::Forms::Button());
	this->deletePage = (gcnew System::Windows::Forms::TabPage());
	this->deleteTextBox = (gcnew System::Windows::Forms::TextBox());
	this->deleteLabel = (gcnew System::Windows::Forms::Label());
	this->deleteMessageLabel = (gcnew System::Windows::Forms::Label());
	this->deleteButton = (gcnew System::Windows::Forms::Button());
	this->tabControl->SuspendLayout();
	this->addPage->SuspendLayout();
	this->changePage->SuspendLayout();
	this->deletePage->SuspendLayout();
	this->SuspendLayout();
	// 
	// backButton
	// 
	this->backButton->AutoSize = true;
	this->backButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->backButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->backButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->backButton->Location = System::Drawing::Point(313, 263);
	this->backButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->backButton->Name = L"backButton";
	this->backButton->Size = System::Drawing::Size(84, 44);
	this->backButton->TabIndex = 19;
	this->backButton->Text = L"Назад";
	this->backButton->UseVisualStyleBackColor = true;
	this->backButton->Click += gcnew System::EventHandler(this, &ChangeTasks::backButton_Click);
	// 
	// tabControl
	// 
	this->tabControl->Controls->Add(this->addPage);
	this->tabControl->Controls->Add(this->changePage);
	this->tabControl->Controls->Add(this->deletePage);
	this->tabControl->Location = System::Drawing::Point(11, 13);
	this->tabControl->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->tabControl->Name = L"tabControl";
	this->tabControl->SelectedIndex = 0;
	this->tabControl->Size = System::Drawing::Size(386, 242);
	this->tabControl->TabIndex = 18;
	this->tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &ChangeTasks::tabControl_SelectedIndexChanged);
	this->tabControl->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// addPage
	// 
	this->addPage->Controls->Add(this->addDateTextBox);
	this->addPage->Controls->Add(this->addMessageLabel);
	this->addPage->Controls->Add(this->addButton);
	this->addPage->Controls->Add(this->addNameTextBox);
	this->addPage->Controls->Add(this->addLabel);
	this->addPage->Location = System::Drawing::Point(4, 25);
	this->addPage->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->addPage->Name = L"addPage";
	this->addPage->Padding = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->addPage->Size = System::Drawing::Size(378, 213);
	this->addPage->TabIndex = 0;
	this->addPage->Text = L"Добавить задачу";
	this->addPage->UseVisualStyleBackColor = true;
	this->addPage->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// addDateTextBox
	// 
	this->addDateTextBox->Location = System::Drawing::Point(94, 31);
	this->addDateTextBox->Mask = L"00/00/0000";
	this->addDateTextBox->Name = L"addDateTextBox";
	this->addDateTextBox->Size = System::Drawing::Size(279, 24);
	this->addDateTextBox->TabIndex = 17;
	this->addDateTextBox->ValidatingType = System::DateTime::typeid;
	this->addDateTextBox->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// addMessageLabel
	// 
	this->addMessageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->addMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->addMessageLabel->Location = System::Drawing::Point(4, 179);
	this->addMessageLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->addMessageLabel->Name = L"addMessageLabel";
	this->addMessageLabel->Size = System::Drawing::Size(370, 30);
	this->addMessageLabel->TabIndex = 16;
	this->addMessageLabel->Text = L" ";
	this->addMessageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->addMessageLabel->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// addButton
	// 
	this->addButton->AutoSize = true;
	this->addButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->addButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->addButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->addButton->Location = System::Drawing::Point(136, 94);
	this->addButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->addButton->Name = L"addButton";
	this->addButton->Size = System::Drawing::Size(115, 44);
	this->addButton->TabIndex = 12;
	this->addButton->Text = L"Добавить";
	this->addButton->UseVisualStyleBackColor = true;
	this->addButton->Click += gcnew System::EventHandler(this, &ChangeTasks::addButton_Click);
	// 
	// addNameTextBox
	// 
	this->addNameTextBox->Location = System::Drawing::Point(94, 4);
	this->addNameTextBox->Name = L"addNameTextBox";
	this->addNameTextBox->Size = System::Drawing::Size(279, 24);
	this->addNameTextBox->TabIndex = 10;
	this->addNameTextBox->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// addLabel
	// 
	this->addLabel->AutoSize = true;
	this->addLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->addLabel->Location = System::Drawing::Point(4, 4);
	this->addLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->addLabel->Name = L"addLabel";
	this->addLabel->Size = System::Drawing::Size(85, 48);
	this->addLabel->TabIndex = 7;
	this->addLabel->Text = L"Имя:\r\nДедлайн:";
	this->addLabel->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// changePage
	// 
	this->changePage->Controls->Add(this->changeDateButton);
	this->changePage->Controls->Add(this->newNameTextBox);
	this->changePage->Controls->Add(this->changeDateTextBox);
	this->changePage->Controls->Add(this->oldNameTextBox);
	this->changePage->Controls->Add(this->changeLabel);
	this->changePage->Controls->Add(this->changeMessageLabel);
	this->changePage->Controls->Add(this->changeNameButton);
	this->changePage->Location = System::Drawing::Point(4, 25);
	this->changePage->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->changePage->Name = L"changePage";
	this->changePage->Padding = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->changePage->RightToLeft = System::Windows::Forms::RightToLeft::No;
	this->changePage->Size = System::Drawing::Size(378, 213);
	this->changePage->TabIndex = 1;
	this->changePage->Text = L"Изменить задачу";
	this->changePage->UseVisualStyleBackColor = true;
	this->changePage->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// changeDateButton
	// 
	this->changeDateButton->AutoSize = true;
	this->changeDateButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->changeDateButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->changeDateButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeDateButton->Location = System::Drawing::Point(176, 92);
	this->changeDateButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->changeDateButton->Name = L"changeDateButton";
	this->changeDateButton->Size = System::Drawing::Size(172, 44);
	this->changeDateButton->TabIndex = 23;
	this->changeDateButton->Text = L"Изменить дедлайн";
	this->changeDateButton->UseVisualStyleBackColor = true;
	this->changeDateButton->Click += gcnew System::EventHandler(this, &ChangeTasks::changeDateButton_Click);
	// 
	// newNameTextBox
	// 
	this->newNameTextBox->Location = System::Drawing::Point(112, 30);
	this->newNameTextBox->Name = L"newNameTextBox";
	this->newNameTextBox->Size = System::Drawing::Size(261, 24);
	this->newNameTextBox->TabIndex = 22;
	this->newNameTextBox->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// changeDateTextBox
	// 
	this->changeDateTextBox->Location = System::Drawing::Point(112, 56);
	this->changeDateTextBox->Mask = L"00/00/0000";
	this->changeDateTextBox->Name = L"changeDateTextBox";
	this->changeDateTextBox->Size = System::Drawing::Size(261, 24);
	this->changeDateTextBox->TabIndex = 21;
	this->changeDateTextBox->ValidatingType = System::DateTime::typeid;
	this->changeDateTextBox->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// oldNameTextBox
	// 
	this->oldNameTextBox->Location = System::Drawing::Point(112, 4);
	this->oldNameTextBox->Name = L"oldNameTextBox";
	this->oldNameTextBox->Size = System::Drawing::Size(261, 24);
	this->oldNameTextBox->TabIndex = 20;
	this->oldNameTextBox->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// changeLabel
	// 
	this->changeLabel->AutoSize = true;
	this->changeLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeLabel->Location = System::Drawing::Point(4, 4);
	this->changeLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->changeLabel->Name = L"changeLabel";
	this->changeLabel->Size = System::Drawing::Size(103, 72);
	this->changeLabel->TabIndex = 19;
	this->changeLabel->Text = L"Старое имя:\r\nНовое имя:\r\nДедлайн:";
	this->changeLabel->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// changeMessageLabel
	// 
	this->changeMessageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->changeMessageLabel->Location = System::Drawing::Point(4, 149);
	this->changeMessageLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->changeMessageLabel->Name = L"changeMessageLabel";
	this->changeMessageLabel->Size = System::Drawing::Size(370, 60);
	this->changeMessageLabel->TabIndex = 18;
	this->changeMessageLabel->Text = L" ";
	this->changeMessageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->changeMessageLabel->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// changeNameButton
	// 
	this->changeNameButton->AutoSize = true;
	this->changeNameButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->changeNameButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->changeNameButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeNameButton->Location = System::Drawing::Point(29, 92);
	this->changeNameButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->changeNameButton->Name = L"changeNameButton";
	this->changeNameButton->Size = System::Drawing::Size(132, 44);
	this->changeNameButton->TabIndex = 15;
	this->changeNameButton->Text = L"Изменить имя";
	this->changeNameButton->UseVisualStyleBackColor = true;
	this->changeNameButton->Click += gcnew System::EventHandler(this, &ChangeTasks::changeNameButton_Click);
	// 
	// deletePage
	// 
	this->deletePage->Controls->Add(this->deleteTextBox);
	this->deletePage->Controls->Add(this->deleteLabel);
	this->deletePage->Controls->Add(this->deleteMessageLabel);
	this->deletePage->Controls->Add(this->deleteButton);
	this->deletePage->Location = System::Drawing::Point(4, 25);
	this->deletePage->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->deletePage->Name = L"deletePage";
	this->deletePage->Padding = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->deletePage->Size = System::Drawing::Size(378, 213);
	this->deletePage->TabIndex = 2;
	this->deletePage->Text = L"Удалить задачу";
	this->deletePage->UseVisualStyleBackColor = true;
	this->deletePage->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// deleteTextBox
	// 
	this->deleteTextBox->Location = System::Drawing::Point(54, 4);
	this->deleteTextBox->Name = L"deleteTextBox";
	this->deleteTextBox->Size = System::Drawing::Size(319, 24);
	this->deleteTextBox->TabIndex = 25;
	this->deleteTextBox->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// deleteLabel
	// 
	this->deleteLabel->AutoSize = true;
	this->deleteLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->deleteLabel->Location = System::Drawing::Point(4, 4);
	this->deleteLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->deleteLabel->Name = L"deleteLabel";
	this->deleteLabel->Size = System::Drawing::Size(45, 24);
	this->deleteLabel->TabIndex = 24;
	this->deleteLabel->Text = L"Имя:";
	this->deleteLabel->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// deleteMessageLabel
	// 
	this->deleteMessageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->deleteMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->deleteMessageLabel->Location = System::Drawing::Point(4, 179);
	this->deleteMessageLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->deleteMessageLabel->Name = L"deleteMessageLabel";
	this->deleteMessageLabel->Size = System::Drawing::Size(370, 30);
	this->deleteMessageLabel->TabIndex = 23;
	this->deleteMessageLabel->Text = L" ";
	this->deleteMessageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->deleteMessageLabel->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	// 
	// deleteButton
	// 
	this->deleteButton->AutoSize = true;
	this->deleteButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->deleteButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->deleteButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->deleteButton->Location = System::Drawing::Point(140, 81);
	this->deleteButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->deleteButton->Name = L"deleteButton";
	this->deleteButton->Size = System::Drawing::Size(106, 44);
	this->deleteButton->TabIndex = 21;
	this->deleteButton->Text = L"Удалить";
	this->deleteButton->UseVisualStyleBackColor = true;
	this->deleteButton->Click += gcnew System::EventHandler(this, &ChangeTasks::deleteButton_Click);
	// 
	// ChangeTasks
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(408, 320);
	this->Controls->Add(this->backButton);
	this->Controls->Add(this->tabControl);
	this->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 10));
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->MaximizeBox = false;
	this->MinimizeBox = false;
	this->Name = L"ChangeTasks";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	this->Text = L"Добавить задачу";
	this->Click += gcnew System::EventHandler(this, &ChangeTasks::ClearMessageLabels);
	this->tabControl->ResumeLayout(false);
	this->addPage->ResumeLayout(false);
	this->addPage->PerformLayout();
	this->changePage->ResumeLayout(false);
	this->changePage->PerformLayout();
	this->deletePage->ResumeLayout(false);
	this->deletePage->PerformLayout();
	this->ResumeLayout(false);
	this->PerformLayout();

}
#pragma endregion