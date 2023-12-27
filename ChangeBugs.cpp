#include "ChangeBugs.h"

using namespace kursovaya;

ChangeBugs::ChangeBugs(
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

ChangeBugs::~ChangeBugs() {
	if (components) {
		delete components;
	}
}

System::Void ChangeBugs::ClearMessageLabels(System::Object^ sender, System::EventArgs^ e) {
	this->addMessageLabel->Text = " ";
	this->changeMessageLabel->Text = " ";
	this->deleteMessageLabel->Text = " ";
}

System::Void kursovaya::ChangeBugs::tabControl_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	this->Text = this->tabControl->SelectedTab->Text;
	ClearMessageLabels(sender, e);
}

System::Void kursovaya::ChangeBugs::backButton_Click(System::Object^ sender, System::EventArgs^ e) {
	args->at(0) = "BugList";
	this->Close();
}

System::Void kursovaya::ChangeBugs::addButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabels(sender, e);
	if (this->addTextBox->Text == "")
		return;
	res->reset((*stmt)->executeQuery("SELECT id FROM Bugs WHERE BINARY description = '" +
		msclr::interop::marshal_as<std::string>(this->addTextBox->Text)));
	while ((*res)->next()) {
		this->addMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->addMessageLabel->Text = "Ошибка: такой баг уже есть";
		return;
	}
	(*stmt)->execute("INSERT INTO Bugs(description, taskId, userId) VALUES('" +
		msclr::interop::marshal_as<std::string>(this->addTextBox->Text) +
		"', " + args->at(3) + ", " + args->at(1) + ")");
	this->addTextBox->Text = "";
	this->addMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	this->addMessageLabel->Text = "Баг успешно добавлен";
}

System::Void kursovaya::ChangeBugs::changeButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabels(sender, e);
	if (this->oldTextBox->Text == "" || this->newTextBox->Text == "")
		return;
	res->reset((*stmt)->executeQuery("SELECT id FROM Bugs WHERE BINARY description = '" +
		msclr::interop::marshal_as<std::string>(this->newTextBox->Text) + "'"));
	while ((*res)->next()) {
		this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->changeMessageLabel->Text = "Ошибка: такой баг уже есть";
		return;
	}
	res->reset((*stmt)->executeQuery(
		"SELECT id FROM Bugs WHERE BINARY description = '" +
		msclr::interop::marshal_as<std::string>(this->oldTextBox->Text) +
		"' AND taskId = " + args->at(3)));
	while ((*res)->next()) {
		(*stmt)->execute("UPDATE Bugs SET description = '" +
			msclr::interop::marshal_as<std::string>(this->newTextBox->Text) +
			"' WHERE BINARY description = '" +
			msclr::interop::marshal_as<std::string>(this->oldTextBox->Text) + "'");
		this->oldTextBox->Text = "";
		this->newTextBox->Text = "";
		this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
		this->changeMessageLabel->Text = "Баг успешно изменен";
		return;
	}
	this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->changeMessageLabel->Text = "Ошибка: нет бага с таким описанием";
}

System::Void kursovaya::ChangeBugs::deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabels(sender, e);
	if (this->deleteTextBox->Text == "")
		return;
	res->reset((*stmt)->executeQuery(
		"SELECT id FROM Bugs WHERE BINARY description = '" +
		msclr::interop::marshal_as<std::string>(this->deleteTextBox->Text) +
		"' AND taskId = " + args->at(3)));
	while ((*res)->next()) {
		(*stmt)->execute("DELETE FROM Bugs WHERE description = '" +
			msclr::interop::marshal_as<std::string>(this->deleteTextBox->Text) + "'");
		this->deleteTextBox->Text = "";
		this->deleteMessageLabel->ForeColor = System::Drawing::Color::DarkGreen;
		this->deleteMessageLabel->Text = "Баг успешно удалён";
		return;
	}
	this->deleteMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->deleteMessageLabel->Text = "Ошибка: нет бага с таким описанием";
}

#pragma region Windows Form Designer generated code
void kursovaya::ChangeBugs::InitializeComponent(void) {
	this->backButton = (gcnew System::Windows::Forms::Button());
	this->tabControl = (gcnew System::Windows::Forms::TabControl());
	this->addPage = (gcnew System::Windows::Forms::TabPage());
	this->addMessageLabel = (gcnew System::Windows::Forms::Label());
	this->addButton = (gcnew System::Windows::Forms::Button());
	this->addTextBox = (gcnew System::Windows::Forms::TextBox());
	this->addLabel = (gcnew System::Windows::Forms::Label());
	this->changePage = (gcnew System::Windows::Forms::TabPage());
	this->changeMessageLabel = (gcnew System::Windows::Forms::Label());
	this->newTextBox = (gcnew System::Windows::Forms::TextBox());
	this->newLabel = (gcnew System::Windows::Forms::Label());
	this->changeButton = (gcnew System::Windows::Forms::Button());
	this->oldTextBox = (gcnew System::Windows::Forms::TextBox());
	this->oldLabel = (gcnew System::Windows::Forms::Label());
	this->deletePage = (gcnew System::Windows::Forms::TabPage());
	this->deleteMessageLabel = (gcnew System::Windows::Forms::Label());
	this->deleteButton = (gcnew System::Windows::Forms::Button());
	this->deleteTextBox = (gcnew System::Windows::Forms::TextBox());
	this->deleteLabel = (gcnew System::Windows::Forms::Label());
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
	this->backButton->Click += gcnew System::EventHandler(this, &ChangeBugs::backButton_Click);
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
	this->tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &ChangeBugs::tabControl_SelectedIndexChanged);
	this->tabControl->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// addPage
	// 
	this->addPage->Controls->Add(this->addMessageLabel);
	this->addPage->Controls->Add(this->addButton);
	this->addPage->Controls->Add(this->addTextBox);
	this->addPage->Controls->Add(this->addLabel);
	this->addPage->Location = System::Drawing::Point(4, 25);
	this->addPage->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->addPage->Name = L"addPage";
	this->addPage->Padding = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->addPage->Size = System::Drawing::Size(378, 213);
	this->addPage->TabIndex = 0;
	this->addPage->Text = L"Добавить баг";
	this->addPage->UseVisualStyleBackColor = true;
	this->addPage->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// addMessageLabel
	// 
	this->addMessageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->addMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->addMessageLabel->Location = System::Drawing::Point(6, 177);
	this->addMessageLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->addMessageLabel->Name = L"addMessageLabel";
	this->addMessageLabel->Size = System::Drawing::Size(367, 30);
	this->addMessageLabel->TabIndex = 16;
	this->addMessageLabel->Text = L" ";
	this->addMessageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->addMessageLabel->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// addButton
	// 
	this->addButton->AutoSize = true;
	this->addButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->addButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->addButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->addButton->Location = System::Drawing::Point(132, 69);
	this->addButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->addButton->Name = L"addButton";
	this->addButton->Size = System::Drawing::Size(115, 44);
	this->addButton->TabIndex = 12;
	this->addButton->Text = L"Добавить";
	this->addButton->UseVisualStyleBackColor = true;
	this->addButton->Click += gcnew System::EventHandler(this, &ChangeBugs::addButton_Click);
	// 
	// addTextBox
	// 
	this->addTextBox->Location = System::Drawing::Point(6, 37);
	this->addTextBox->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->addTextBox->Name = L"addTextBox";
	this->addTextBox->Size = System::Drawing::Size(366, 24);
	this->addTextBox->TabIndex = 10;
	this->addTextBox->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// addLabel
	// 
	this->addLabel->AutoSize = true;
	this->addLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->addLabel->Location = System::Drawing::Point(95, 4);
	this->addLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->addLabel->Name = L"addLabel";
	this->addLabel->Size = System::Drawing::Size(188, 24);
	this->addLabel->TabIndex = 7;
	this->addLabel->Text = L"Описание нового бага:";
	this->addLabel->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// changePage
	// 
	this->changePage->Controls->Add(this->changeMessageLabel);
	this->changePage->Controls->Add(this->newTextBox);
	this->changePage->Controls->Add(this->newLabel);
	this->changePage->Controls->Add(this->changeButton);
	this->changePage->Controls->Add(this->oldTextBox);
	this->changePage->Controls->Add(this->oldLabel);
	this->changePage->Location = System::Drawing::Point(4, 25);
	this->changePage->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->changePage->Name = L"changePage";
	this->changePage->Padding = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->changePage->RightToLeft = System::Windows::Forms::RightToLeft::No;
	this->changePage->Size = System::Drawing::Size(378, 213);
	this->changePage->TabIndex = 1;
	this->changePage->Text = L"Изменить баг";
	this->changePage->UseVisualStyleBackColor = true;
	this->changePage->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// changeMessageLabel
	// 
	this->changeMessageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->changeMessageLabel->Location = System::Drawing::Point(6, 177);
	this->changeMessageLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->changeMessageLabel->Name = L"changeMessageLabel";
	this->changeMessageLabel->Size = System::Drawing::Size(367, 30);
	this->changeMessageLabel->TabIndex = 18;
	this->changeMessageLabel->Text = L" ";
	this->changeMessageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->changeMessageLabel->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// newTextBox
	// 
	this->newTextBox->Location = System::Drawing::Point(6, 98);
	this->newTextBox->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->newTextBox->Name = L"newTextBox";
	this->newTextBox->Size = System::Drawing::Size(366, 24);
	this->newTextBox->TabIndex = 17;
	this->newTextBox->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// newLabel
	// 
	this->newLabel->AutoSize = true;
	this->newLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->newLabel->Location = System::Drawing::Point(99, 65);
	this->newLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->newLabel->Name = L"newLabel";
	this->newLabel->Size = System::Drawing::Size(181, 24);
	this->newLabel->TabIndex = 16;
	this->newLabel->Text = L"Новое описание бага:";
	this->newLabel->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// changeButton
	// 
	this->changeButton->AutoSize = true;
	this->changeButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->changeButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->changeButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeButton->Location = System::Drawing::Point(131, 130);
	this->changeButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->changeButton->Name = L"changeButton";
	this->changeButton->Size = System::Drawing::Size(116, 44);
	this->changeButton->TabIndex = 15;
	this->changeButton->Text = L"Изменить";
	this->changeButton->UseVisualStyleBackColor = true;
	this->changeButton->Click += gcnew System::EventHandler(this, &ChangeBugs::changeButton_Click);
	// 
	// oldTextBox
	// 
	this->oldTextBox->Location = System::Drawing::Point(6, 37);
	this->oldTextBox->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->oldTextBox->Name = L"oldTextBox";
	this->oldTextBox->Size = System::Drawing::Size(366, 24);
	this->oldTextBox->TabIndex = 14;
	this->oldTextBox->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// oldLabel
	// 
	this->oldLabel->AutoSize = true;
	this->oldLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->oldLabel->Location = System::Drawing::Point(95, 4);
	this->oldLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->oldLabel->Name = L"oldLabel";
	this->oldLabel->Size = System::Drawing::Size(188, 24);
	this->oldLabel->TabIndex = 13;
	this->oldLabel->Text = L"Старое описание бага:";
	this->oldLabel->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// deletePage
	// 
	this->deletePage->Controls->Add(this->deleteMessageLabel);
	this->deletePage->Controls->Add(this->deleteButton);
	this->deletePage->Controls->Add(this->deleteTextBox);
	this->deletePage->Controls->Add(this->deleteLabel);
	this->deletePage->Location = System::Drawing::Point(4, 25);
	this->deletePage->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->deletePage->Name = L"deletePage";
	this->deletePage->Padding = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->deletePage->Size = System::Drawing::Size(378, 213);
	this->deletePage->TabIndex = 2;
	this->deletePage->Text = L"Удалить баг";
	this->deletePage->UseVisualStyleBackColor = true;
	this->deletePage->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// deleteMessageLabel
	// 
	this->deleteMessageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->deleteMessageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->deleteMessageLabel->Location = System::Drawing::Point(6, 177);
	this->deleteMessageLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->deleteMessageLabel->Name = L"deleteMessageLabel";
	this->deleteMessageLabel->Size = System::Drawing::Size(367, 30);
	this->deleteMessageLabel->TabIndex = 23;
	this->deleteMessageLabel->Text = L" ";
	this->deleteMessageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->deleteMessageLabel->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// deleteButton
	// 
	this->deleteButton->AutoSize = true;
	this->deleteButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->deleteButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->deleteButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->deleteButton->Location = System::Drawing::Point(136, 69);
	this->deleteButton->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->deleteButton->Name = L"deleteButton";
	this->deleteButton->Size = System::Drawing::Size(106, 44);
	this->deleteButton->TabIndex = 21;
	this->deleteButton->Text = L"Удалить";
	this->deleteButton->UseVisualStyleBackColor = true;
	this->deleteButton->Click += gcnew System::EventHandler(this, &ChangeBugs::deleteButton_Click);
	// 
	// deleteTextBox
	// 
	this->deleteTextBox->Location = System::Drawing::Point(6, 37);
	this->deleteTextBox->Margin = System::Windows::Forms::Padding(2, 4, 2, 4);
	this->deleteTextBox->Name = L"deleteTextBox";
	this->deleteTextBox->Size = System::Drawing::Size(366, 24);
	this->deleteTextBox->TabIndex = 20;
	this->deleteTextBox->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// deleteLabel
	// 
	this->deleteLabel->AutoSize = true;
	this->deleteLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->deleteLabel->Location = System::Drawing::Point(75, 4);
	this->deleteLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
	this->deleteLabel->Name = L"deleteLabel";
	this->deleteLabel->Size = System::Drawing::Size(228, 24);
	this->deleteLabel->TabIndex = 19;
	this->deleteLabel->Text = L"Описание удаляемого бага:";
	this->deleteLabel->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
	// 
	// ChangeBugs
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(410, 321);
	this->Controls->Add(this->backButton);
	this->Controls->Add(this->tabControl);
	this->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 10));
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->MaximizeBox = false;
	this->MinimizeBox = false;
	this->Name = L"ChangeBugs";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	this->Text = L"Добавить баг";
	this->Click += gcnew System::EventHandler(this, &ChangeBugs::ClearMessageLabels);
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