#include "Login.h"

using namespace kursovaya;

Login::Login(
	std::auto_ptr<sql::Statement>& stmt,
	std::auto_ptr<sql::PreparedStatement>& pstmt,
	std::auto_ptr<sql::ResultSet>& res,
	std::vector<std::string> &args) {
	InitializeComponent();
	this->stmt = &stmt;
	this->pstmt = &pstmt;
	this->res = &res;
	this->args = &args;
	this->args->clear();
	this->args->push_back("exit");
}

Login::~Login() {
	if (components) {
		delete components;
	}
}

System::Void Login::enterButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (this->loginTextBox->Text == "" || this->passwordTextBox->Text == "")
		return;
	int id = -1;
	res->reset((*stmt)->executeQuery(
		"SELECT id FROM Users WHERE BINARY userName = '" +
		msclr::interop::marshal_as<std::string>(this->loginTextBox->Text) +
		"' AND BINARY password = '" +
		msclr::interop::marshal_as<std::string>(this->passwordTextBox->Text) + "'"));
	while ((*res)->next())
		id = (*res)->getInt("id");
	if (id == -1)
		this->errorLabel->Text = "������: ������� �����\n����� ��� ������";
	else
	{
		args->clear();
		args->push_back("SelectProject");
		args->push_back(std::to_string(id));
		this->Close();
	}
}

System::Void Login::signUpButton_Click(System::Object^ sender, System::EventArgs^ e) {
	args->clear();
	args->push_back("SignUp");
	this->Close();
}

System::Void Login::ClearErrorLabel(System::Object^ sender, System::EventArgs^ e) {
	this->errorLabel->Text = " ";
}

#pragma region Windows Form Designer generated code
void Login::InitializeComponent(void) {
    this->title = (gcnew System::Windows::Forms::Label());
    this->loginLabel = (gcnew System::Windows::Forms::Label());
    this->passwordLabel = (gcnew System::Windows::Forms::Label());
    this->passwordTextBox = (gcnew System::Windows::Forms::TextBox());
    this->enterButton = (gcnew System::Windows::Forms::Button());
    this->orLabel = (gcnew System::Windows::Forms::Label());
    this->signUpButton = (gcnew System::Windows::Forms::Button());
    this->errorLabel = (gcnew System::Windows::Forms::Label());
    this->loginTextBox = (gcnew System::Windows::Forms::TextBox());
    this->SuspendLayout();
    // 
    // title
    // 
    this->title->AutoSize = true;
    this->title->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 24.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(204)));
    this->title->Location = System::Drawing::Point(12, 11);
    this->title->Name = L"title";
    this->title->Size = System::Drawing::Size(263, 40);
    this->title->TabIndex = 0;
    this->title->Text = L"������� � �������";
    this->title->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    // 
    // loginLabel
    // 
    this->loginLabel->AutoSize = true;
    this->loginLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->loginLabel->Location = System::Drawing::Point(12, 60);
    this->loginLabel->Name = L"loginLabel";
    this->loginLabel->Size = System::Drawing::Size(62, 24);
    this->loginLabel->TabIndex = 1;
    this->loginLabel->Text = L"�����:";
    this->loginLabel->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    // 
    // passwordLabel
    // 
    this->passwordLabel->AutoSize = true;
    this->passwordLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->passwordLabel->Location = System::Drawing::Point(12, 90);
    this->passwordLabel->Name = L"passwordLabel";
    this->passwordLabel->Size = System::Drawing::Size(71, 24);
    this->passwordLabel->TabIndex = 2;
    this->passwordLabel->Text = L"������:";
    this->passwordLabel->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    // 
    // passwordTextBox
    // 
    this->passwordTextBox->Location = System::Drawing::Point(89, 95);
    this->passwordTextBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->passwordTextBox->Name = L"passwordTextBox";
    this->passwordTextBox->Size = System::Drawing::Size(186, 24);
    this->passwordTextBox->TabIndex = 4;
    this->passwordTextBox->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    // 
    // enterButton
    // 
    this->enterButton->AutoSize = true;
    this->enterButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    this->enterButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->enterButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->enterButton->Location = System::Drawing::Point(106, 186);
    this->enterButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->enterButton->Name = L"enterButton";
    this->enterButton->Size = System::Drawing::Size(75, 44);
    this->enterButton->TabIndex = 5;
    this->enterButton->Text = L"�����";
    this->enterButton->UseVisualStyleBackColor = true;
    this->enterButton->Click += gcnew System::EventHandler(this, &Login::enterButton_Click);
    // 
    // orLabel
    // 
    this->orLabel->AutoSize = true;
    this->orLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->orLabel->Location = System::Drawing::Point(123, 228);
    this->orLabel->Name = L"orLabel";
    this->orLabel->Size = System::Drawing::Size(40, 24);
    this->orLabel->TabIndex = 6;
    this->orLabel->Text = L"���";
    this->orLabel->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    // 
    // signUpButton
    // 
    this->signUpButton->AutoSize = true;
    this->signUpButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->signUpButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->signUpButton->Location = System::Drawing::Point(53, 261);
    this->signUpButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->signUpButton->Name = L"signUpButton";
    this->signUpButton->Size = System::Drawing::Size(181, 44);
    this->signUpButton->TabIndex = 7;
    this->signUpButton->Text = L"������������������";
    this->signUpButton->UseVisualStyleBackColor = true;
    this->signUpButton->Click += gcnew System::EventHandler(this, &Login::signUpButton_Click);
    // 
    // errorLabel
    // 
    this->errorLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
    this->errorLabel->ForeColor = System::Drawing::Color::DarkRed;
    this->errorLabel->Location = System::Drawing::Point(12, 123);
    this->errorLabel->Name = L"errorLabel";
    this->errorLabel->Size = System::Drawing::Size(263, 59);
    this->errorLabel->TabIndex = 8;
    this->errorLabel->Text = L" ";
    this->errorLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
    this->errorLabel->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    // 
    // loginTextBox
    // 
    this->loginTextBox->Location = System::Drawing::Point(89, 65);
    this->loginTextBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->loginTextBox->Name = L"loginTextBox";
    this->loginTextBox->Size = System::Drawing::Size(186, 24);
    this->loginTextBox->TabIndex = 3;
    this->loginTextBox->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    // 
    // Login
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(287, 320);
    this->Controls->Add(this->errorLabel);
    this->Controls->Add(this->signUpButton);
    this->Controls->Add(this->enterButton);
    this->Controls->Add(this->passwordTextBox);
    this->Controls->Add(this->loginTextBox);
    this->Controls->Add(this->passwordLabel);
    this->Controls->Add(this->loginLabel);
    this->Controls->Add(this->title);
    this->Controls->Add(this->orLabel);
    this->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 10));
    this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
    this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
    this->MaximizeBox = false;
    this->MinimizeBox = false;
    this->Name = L"Login";
    this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
    this->Text = L"���� � �������";
    this->Click += gcnew System::EventHandler(this, &Login::ClearErrorLabel);
    this->ResumeLayout(false);
    this->PerformLayout();

}
#pragma endregion
