#include "Buglist.h"

using namespace kursovaya;

Buglist::Buglist(std::auto_ptr<sql::Statement>& stmt,
	std::auto_ptr<sql::PreparedStatement>& pstmt,
	std::auto_ptr<sql::ResultSet>& res,
	std::vector<std::string>& args) {
	InitializeComponent();
	this->stmt = &stmt;
	this->pstmt = &pstmt;
	this->res = &res;
	this->args = &args;
	this->args->at(0) = "exit";
	UpdateBugsListBox();
}

Buglist::~Buglist() {
	if (components) {
		delete components;
	}
}

void kursovaya::Buglist::UpdateBugsListBox() {
	this->bugsListBox->Items->Clear();
	(*stmt)->execute("SELECT description, isFixed FROM Bugs WHERE taskId = " +
		args->at(3));
	do {
		res->reset((*stmt)->getResultSet());
		while ((*res)->next()) {
			this->bugsListBox->Items->Add(msclr::interop::marshal_as<System::String^>((*res)->getString("description").asStdString()));
			if ((*res)->getBoolean("isFixed"))
				this->bugsListBox->SetItemChecked(this->bugsListBox->Items->Count - 1, 1);
		}
	} while ((*stmt)->getMoreResults());
}

void kursovaya::Buglist::ClearInfoLabels() {
	this->bugIdLabel->Text = " ";
	this->taskNameLabel->Text = " ";
	this->isFixedLabel->Text = " ";
	this->projectNameLabel->Text = " ";
	this->creatorLabel->Text = " ";
	this->descriptionLabel->Text = " ";
}

System::Void kursovaya::Buglist::backButton_Click(System::Object^ sender, System::EventArgs^ e) {
	args->pop_back();
	args->at(0) = "TaskList";
	this->Close();
}

System::Void kursovaya::Buglist::ClearMessageLabel(System::Object^ sender, System::EventArgs^ e) {
	this->messageLabel->Text = " ";
}

System::Void kursovaya::Buglist::bugsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->bugsListBox->SelectedItem == nullptr)
		return;
	res->reset((*stmt)->executeQuery("SELECT B.id, T.taskName, B.isFixed, P.projectName, U.username, B.description" \
		" FROM Bugs AS B LEFT JOIN Tasks AS T ON T.id = B.taskId LEFT JOIN Projects AS P ON P.id = T.projectId" \
		" LEFT JOIN Users AS U ON U.id = B.userId WHERE B.description = '" +
		msclr::interop::marshal_as<std::string>(this->bugsListBox->SelectedItem->ToString()) + "'"));
	while ((*res)->next()) {
		this->bugIdLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("id").asStdString());
		this->taskNameLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("taskName").asStdString());
		this->isFixedLabel->Text = (*res)->getBoolean("isFixed") ? "да" : "нет";
		this->projectNameLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("projectName").asStdString());
		this->creatorLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("username").asStdString());
		this->descriptionLabel->Text = msclr::interop::marshal_as<System::String^>((*res)->getString("description").asStdString());
	}
}

System::Void kursovaya::Buglist::bugsListBox_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e) {
	ClearMessageLabel(sender, e);
	if (this->bugsListBox->SelectedItem == nullptr)
		return;
	std::string isFixed;
	if (e->CurrentValue == System::Windows::Forms::CheckState::Checked)
		isFixed = "FALSE";
	else if (e->CurrentValue == System::Windows::Forms::CheckState::Unchecked)
		isFixed = "TRUE";
	else {
		this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
		this->messageLabel->Text = "Непредвиденная ошибка";
		return;
	}
	(*stmt)->execute("UPDATE Bugs SET isFixed = " + isFixed + " WHERE description = '" +
		msclr::interop::marshal_as<std::string>(this->bugsListBox->SelectedItem->ToString()) + "'");
	UpdateBugsListBox();
	ClearInfoLabels();
	this->bugsListBox->SetSelected(e->Index, 1);
	this->messageLabel->ForeColor = System::Drawing::Color::DarkGreen;
	if (isFixed == "TRUE")
		this->messageLabel->Text = "Баг помечен как исправленный";
	else
		this->messageLabel->Text = "Баг помечен как неисправленный";
}

System::Void kursovaya::Buglist::changeBugsButton_Click(System::Object^ sender, System::EventArgs^ e) {
	res->reset((*stmt)->executeQuery("SELECT changeBugs From Roles WHERE id = (SELECT roleId FROM Users WHERE id = " + args->at(1) + ")"));
	while ((*res)->next())
		if (!(*res)->getBoolean("changeBugs")) {
			this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
			this->messageLabel->Text = "Ошибка: отказано в доступе";
			return;
		}
	args->at(0) = "ChangeBugs";
	this->Close();
}

#pragma region Windows Form Designer generated code
void kursovaya::Buglist::InitializeComponent(void)
{
	this->descriptionLabel = (gcnew System::Windows::Forms::Label());
	this->creatorLabel = (gcnew System::Windows::Forms::Label());
	this->projectNameLabel = (gcnew System::Windows::Forms::Label());
	this->isFixedLabel = (gcnew System::Windows::Forms::Label());
	this->taskNameLabel = (gcnew System::Windows::Forms::Label());
	this->bugIdLabel = (gcnew System::Windows::Forms::Label());
	this->bugInfoLabel = (gcnew System::Windows::Forms::Label());
	this->backButton = (gcnew System::Windows::Forms::Button());
	this->changeBugsButton = (gcnew System::Windows::Forms::Button());
	this->messageLabel = (gcnew System::Windows::Forms::Label());
	this->bugsListBox = (gcnew System::Windows::Forms::CheckedListBox());
	this->SuspendLayout();
	// 
	// descriptionLabel
	// 
	this->descriptionLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->descriptionLabel->Location = System::Drawing::Point(123, 297);
	this->descriptionLabel->Name = L"descriptionLabel";
	this->descriptionLabel->Size = System::Drawing::Size(350, 72);
	this->descriptionLabel->TabIndex = 40;
	this->descriptionLabel->Text = L" ";
	this->descriptionLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// creatorLabel
	// 
	this->creatorLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->creatorLabel->Location = System::Drawing::Point(123, 273);
	this->creatorLabel->Name = L"creatorLabel";
	this->creatorLabel->Size = System::Drawing::Size(350, 24);
	this->creatorLabel->TabIndex = 39;
	this->creatorLabel->Text = L" ";
	this->creatorLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// projectNameLabel
	// 
	this->projectNameLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->projectNameLabel->Location = System::Drawing::Point(123, 249);
	this->projectNameLabel->Name = L"projectNameLabel";
	this->projectNameLabel->Size = System::Drawing::Size(350, 24);
	this->projectNameLabel->TabIndex = 38;
	this->projectNameLabel->Text = L" ";
	this->projectNameLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// isFixedLabel
	// 
	this->isFixedLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->isFixedLabel->Location = System::Drawing::Point(123, 225);
	this->isFixedLabel->Name = L"isFixedLabel";
	this->isFixedLabel->Size = System::Drawing::Size(350, 24);
	this->isFixedLabel->TabIndex = 37;
	this->isFixedLabel->Text = L" ";
	this->isFixedLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// taskNameLabel
	// 
	this->taskNameLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->taskNameLabel->Location = System::Drawing::Point(123, 201);
	this->taskNameLabel->Name = L"taskNameLabel";
	this->taskNameLabel->Size = System::Drawing::Size(350, 24);
	this->taskNameLabel->TabIndex = 36;
	this->taskNameLabel->Text = L" ";
	this->taskNameLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// bugIdLabel
	// 
	this->bugIdLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->bugIdLabel->Location = System::Drawing::Point(123, 177);
	this->bugIdLabel->Name = L"bugIdLabel";
	this->bugIdLabel->Size = System::Drawing::Size(350, 24);
	this->bugIdLabel->TabIndex = 35;
	this->bugIdLabel->Text = L" ";
	this->bugIdLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// bugInfoLabel
	// 
	this->bugInfoLabel->AutoSize = true;
	this->bugInfoLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->bugInfoLabel->Location = System::Drawing::Point(12, 177);
	this->bugInfoLabel->Name = L"bugInfoLabel";
	this->bugInfoLabel->Size = System::Drawing::Size(100, 144);
	this->bugInfoLabel->TabIndex = 34;
	this->bugInfoLabel->Text = L"id:\r\nЗадача:\r\nИсправлен:\r\nПроект:\r\nСоздатель:\r\nОписание:";
	this->bugInfoLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// backButton
	// 
	this->backButton->AutoSize = true;
	this->backButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->backButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->backButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->backButton->Location = System::Drawing::Point(322, 373);
	this->backButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->backButton->Name = L"backButton";
	this->backButton->Size = System::Drawing::Size(151, 44);
	this->backButton->TabIndex = 33;
	this->backButton->Text = L"Назад";
	this->backButton->UseVisualStyleBackColor = true;
	this->backButton->Click += gcnew System::EventHandler(this, &Buglist::backButton_Click);
	// 
	// changeBugsButton
	// 
	this->changeBugsButton->AutoSize = true;
	this->changeBugsButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->changeBugsButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->changeBugsButton->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->changeBugsButton->Location = System::Drawing::Point(12, 373);
	this->changeBugsButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->changeBugsButton->Name = L"changeBugsButton";
	this->changeBugsButton->Size = System::Drawing::Size(304, 44);
	this->changeBugsButton->TabIndex = 32;
	this->changeBugsButton->Text = L"Настроить список багов";
	this->changeBugsButton->UseVisualStyleBackColor = true;
	this->changeBugsButton->Click += gcnew System::EventHandler(this, &Buglist::changeBugsButton_Click);
	// 
	// messageLabel
	// 
	this->messageLabel->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 15));
	this->messageLabel->ForeColor = System::Drawing::Color::DarkRed;
	this->messageLabel->Location = System::Drawing::Point(12, 421);
	this->messageLabel->Name = L"messageLabel";
	this->messageLabel->Size = System::Drawing::Size(461, 30);
	this->messageLabel->TabIndex = 31;
	this->messageLabel->Text = L" ";
	this->messageLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->messageLabel->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	// 
	// bugsListBox
	// 
	this->bugsListBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	this->bugsListBox->FormattingEnabled = true;
	this->bugsListBox->Location = System::Drawing::Point(12, 12);
	this->bugsListBox->Name = L"bugsListBox";
	this->bugsListBox->Size = System::Drawing::Size(461, 154);
	this->bugsListBox->TabIndex = 30;
	this->bugsListBox->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &Buglist::bugsListBox_ItemCheck);
	this->bugsListBox->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	this->bugsListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Buglist::bugsListBox_SelectedIndexChanged);
	// 
	// Buglist
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(485, 460);
	this->Controls->Add(this->descriptionLabel);
	this->Controls->Add(this->creatorLabel);
	this->Controls->Add(this->projectNameLabel);
	this->Controls->Add(this->isFixedLabel);
	this->Controls->Add(this->taskNameLabel);
	this->Controls->Add(this->bugIdLabel);
	this->Controls->Add(this->bugInfoLabel);
	this->Controls->Add(this->backButton);
	this->Controls->Add(this->changeBugsButton);
	this->Controls->Add(this->messageLabel);
	this->Controls->Add(this->bugsListBox);
	this->Font = (gcnew System::Drawing::Font(L"Bahnschrift SemiCondensed", 10));
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->MaximizeBox = false;
	this->MinimizeBox = false;
	this->Name = L"Buglist";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	this->Text = L"Списока багов";
	this->Click += gcnew System::EventHandler(this, &Buglist::ClearMessageLabel);
	this->ResumeLayout(false);
	this->PerformLayout();

}
#pragma endregion
