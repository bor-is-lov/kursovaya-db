#include "ChangeRoles.h"

using namespace kursovaya;

ChangeRoles::ChangeRoles(
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

ChangeRoles::~ChangeRoles() {
	if (components) {
		delete components;
	}
}

System::Void ChangeRoles::ClearMessageLabel(System::Object^ sender, System::EventArgs^ e) {
	this->messageLabel->Text = " ";
}

System::Void ChangeRoles::loginTextBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabel(sender, e);
	this->idLabel->Text = " ";
	this->roleLabel->Text = " ";
	if (this->loginTextBox->Text == "")
		return;
	res->reset((*stmt)->executeQuery("SELECT Users.id, Roles.roleName FROM Users" \
		" LEFT JOIN Roles ON Roles.id = Users.roleId" \
		" WHERE BINARY Users.username = '" +
		msclr::interop::marshal_as<std::string>(this->loginTextBox->Text) + "'"));
	while ((*res)->next()) {
		this->idLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("id").asStdString());
		this->roleLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("roleName").asStdString());
		return;
	}
}

System::Void ChangeRoles::changeRoleButton_Click(System::Object^ sender, System::EventArgs^ e) {
	ClearMessageLabel(sender, e);
	if (this->idLabel->Text == " " || this->roleLabel->Text == " ") {
		this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->messageLabel->Text = "Ошибка: сначал введите логин пользователя";
		return;
	}
	if (this->roleLabel->Text == "Admin") {
		this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->messageLabel->Text = "Ошибка: у администратора нельзя сменить роль";
		return;
	}
	if (this->moderRadio->Checked) {
		(*stmt)->execute("UPDATE Users SET roleId = (SELECT id FROM Roles WHERE roleName = 'Moder') WHERE id = " +
			msclr::interop::marshal_as<std::string>(this->idLabel->Text));
	}
	else if (this->testerRadio->Checked) {
		(*stmt)->execute("UPDATE Users SET roleId = (SELECT id FROM Roles WHERE roleName = 'Tester') WHERE id = " +
			msclr::interop::marshal_as<std::string>(this->idLabel->Text));
	}
	else if (this->userRadio->Checked) {
		(*stmt)->execute("UPDATE Users SET roleId = (SELECT id FROM Roles WHERE roleName = 'User') WHERE id = " +
			msclr::interop::marshal_as<std::string>(this->idLabel->Text));
	}
	else {
		this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->messageLabel->Text = "Ошибка: сначала выберите назначаемую роль";
		return;
	}
	loginTextBox_TextChanged(sender, e);
	this->messageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	this->messageLabel->Text = "Роль пользователя успешно изменена";
}

System::Void ChangeRoles::deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
	res->reset((*stmt)->executeQuery("SELECT roleName FROM Roles" \
		" WHERE id = (SELECT roleId FROM Users WHERE id = " +
		msclr::interop::marshal_as<std::string>(this->idLabel->Text) + ")"));
	while ((*res)->next())
		if ((*res)->getString("roleName") == "Admin") {
			this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
			this->messageLabel->Text = "Ошибка: администратора нельзя удалить";
			return;
		}
	(*stmt)->execute("DELETE FROM Users WHERE id = " +
		msclr::interop::marshal_as<std::string>(this->idLabel->Text));
	this->loginTextBox->Text = "";
	this->messageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	this->messageLabel->Text = "Пользователь успешно удалён";
}

System::Void ChangeRoles::BackButton_Click(System::Object^ sender, System::EventArgs^ e) {
	args->at(0) = "SelectProject";
	this->Close();
}

#pragma region Windows Form Designer generated code
void ChangeRoles::InitializeComponent(void) {
	this->userInfoLabel = (gcnew System::Windows::Forms::Label());
	this->idLabel = (gcnew System::Windows::Forms::Label());
	this->loginTextBox = (gcnew System::Windows::Forms::TextBox());
	this->roleLabel = (gcnew System::Windows::Forms::Label());
	this->messageLabel = (gcnew System::Windows::Forms::Label());
	this->moderRadio = (gcnew System::Windows::Forms::RadioButton());
	this->userRadio = (gcnew System::Windows::Forms::RadioButton());
	this->testerRadio = (gcnew System::Windows::Forms::RadioButton());
	this->changeRoleButton = (gcnew System::Windows::Forms::Button());
	this->deleteButton = (gcnew System::Windows::Forms::Button());
	this->BackButton = (gcnew System::Windows::Forms::Button());
	this->SuspendLayout();
	// 
	// userInfoLabel
	// 
	this->userInfoLabel->AutoSize = true;
	this->userInfoLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->userInfoLabel->Location = System::Drawing::Point(12, 9);
	this->userInfoLabel->Name = L"userInfoLabel";
	this->userInfoLabel->Size = System::Drawing::Size(62, 72);
	this->userInfoLabel->TabIndex = 6;
	this->userInfoLabel->Text = L"id:\r\nЛогин:\r\nРоль:";
	this->userInfoLabel->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	// 
	// idLabel
	// 
	this->idLabel->AutoSize = true;
	this->idLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->idLabel->Location = System::Drawing::Point(80, 9);
	this->idLabel->Name = L"idLabel";
	this->idLabel->Size = System::Drawing::Size(15, 24);
	this->idLabel->TabIndex = 7;
	this->idLabel->Text = L" ";
	this->idLabel->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	// 
	// loginTextBox
	// 
	this->loginTextBox->Location = System::Drawing::Point(80, 33);
	this->loginTextBox->Name = L"loginTextBox";
	this->loginTextBox->Size = System::Drawing::Size(192, 24);
	this->loginTextBox->TabIndex = 10;
	this->loginTextBox->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	this->loginTextBox->TextChanged += gcnew System::EventHandler(this, &ChangeRoles::loginTextBox_TextChanged);
	// 
	// roleLabel
	// 
	this->roleLabel->AutoSize = true;
	this->roleLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->roleLabel->Location = System::Drawing::Point(80, 57);
	this->roleLabel->Name = L"roleLabel";
	this->roleLabel->Size = System::Drawing::Size(15, 24);
	this->roleLabel->TabIndex = 9;
	this->roleLabel->Text = L" ";
	this->roleLabel->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	// 
	// messageLabel
	// 
	this->messageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->messageLabel->ForeColor = System::Drawing::SystemColors::ControlText;
	this->messageLabel->Location = System::Drawing::Point(12, 81);
	this->messageLabel->Name = L"messageLabel";
	this->messageLabel->Size = System::Drawing::Size(260, 48);
	this->messageLabel->TabIndex = 15;
	this->messageLabel->Text = L" ";
	this->messageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->messageLabel->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	// 
	// moderRadio
	// 
	this->moderRadio->AutoSize = true;
	this->moderRadio->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->moderRadio->Location = System::Drawing::Point(12, 132);
	this->moderRadio->Name = L"moderRadio";
	this->moderRadio->Size = System::Drawing::Size(75, 28);
	this->moderRadio->TabIndex = 16;
	this->moderRadio->TabStop = true;
	this->moderRadio->Text = L"Moder";
	this->moderRadio->UseVisualStyleBackColor = true;
	this->moderRadio->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	// 
	// userRadio
	// 
	this->userRadio->AutoSize = true;
	this->userRadio->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->userRadio->Location = System::Drawing::Point(208, 132);
	this->userRadio->Name = L"userRadio";
	this->userRadio->Size = System::Drawing::Size(64, 28);
	this->userRadio->TabIndex = 17;
	this->userRadio->TabStop = true;
	this->userRadio->Text = L"User";
	this->userRadio->UseVisualStyleBackColor = true;
	this->userRadio->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	// 
	// testerRadio
	// 
	this->testerRadio->AutoSize = true;
	this->testerRadio->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->testerRadio->Location = System::Drawing::Point(110, 132);
	this->testerRadio->Name = L"testerRadio";
	this->testerRadio->Size = System::Drawing::Size(75, 28);
	this->testerRadio->TabIndex = 18;
	this->testerRadio->TabStop = true;
	this->testerRadio->Text = L"Tester";
	this->testerRadio->UseVisualStyleBackColor = true;
	this->testerRadio->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	// 
	// changeRoleButton
	// 
	this->changeRoleButton->AutoSize = true;
	this->changeRoleButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->changeRoleButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->changeRoleButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeRoleButton->Location = System::Drawing::Point(12, 167);
	this->changeRoleButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->changeRoleButton->Name = L"changeRoleButton";
	this->changeRoleButton->Size = System::Drawing::Size(147, 44);
	this->changeRoleButton->TabIndex = 19;
	this->changeRoleButton->Text = L"Назначить роль";
	this->changeRoleButton->UseVisualStyleBackColor = true;
	this->changeRoleButton->Click += gcnew System::EventHandler(this, &ChangeRoles::changeRoleButton_Click);
	// 
	// deleteButton
	// 
	this->deleteButton->AutoSize = true;
	this->deleteButton->BackColor = System::Drawing::Color::IndianRed;
	this->deleteButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->deleteButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->deleteButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->deleteButton->Location = System::Drawing::Point(165, 167);
	this->deleteButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->deleteButton->Name = L"deleteButton";
	this->deleteButton->Size = System::Drawing::Size(107, 44);
	this->deleteButton->TabIndex = 20;
	this->deleteButton->Text = L"Удалить";
	this->deleteButton->UseVisualStyleBackColor = false;
	this->deleteButton->Click += gcnew System::EventHandler(this, &ChangeRoles::deleteButton_Click);
	// 
	// BackButton
	// 
	this->BackButton->AutoSize = true;
	this->BackButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->BackButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->BackButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->BackButton->Location = System::Drawing::Point(202, 219);
	this->BackButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->BackButton->Name = L"BackButton";
	this->BackButton->Size = System::Drawing::Size(70, 44);
	this->BackButton->TabIndex = 21;
	this->BackButton->Text = L"Назад";
	this->BackButton->UseVisualStyleBackColor = true;
	this->BackButton->Click += gcnew System::EventHandler(this, &ChangeRoles::BackButton_Click);
	// 
	// ChangeRoles
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(284, 276);
	this->Controls->Add(this->BackButton);
	this->Controls->Add(this->deleteButton);
	this->Controls->Add(this->changeRoleButton);
	this->Controls->Add(this->testerRadio);
	this->Controls->Add(this->userRadio);
	this->Controls->Add(this->moderRadio);
	this->Controls->Add(this->messageLabel);
	this->Controls->Add(this->loginTextBox);
	this->Controls->Add(this->roleLabel);
	this->Controls->Add(this->userInfoLabel);
	this->Controls->Add(this->idLabel);
	this->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 10));
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->MaximizeBox = false;
	this->MinimizeBox = false;
	this->Name = L"ChangeRoles";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	this->Text = L"Назначить роли";
	this->Click += gcnew System::EventHandler(this, &ChangeRoles::ClearMessageLabel);
	this->ResumeLayout(false);
	this->PerformLayout();

}
#pragma endregion
