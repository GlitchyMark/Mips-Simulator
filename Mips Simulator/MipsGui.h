#pragma once
#include "MIPS.h"
#include <list>
#include <string>
#include <vector>
namespace MipsSimulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	static MIPS *mips_run;
	/// <summary>
	/// Summary for MyForm
	/// </summary>

	public ref class MipsGui : public System::Windows::Forms::Form
	{
	public:
		MipsGui(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MipsGui()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ instructionBox;
	protected:

	protected:
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::ListBox^ cycleBox;



	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ registerBox;
	private: System::Windows::Forms::TextBox^ memoryBox;

	private: System::Windows::Forms::Label^ label4;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->instructionBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->cycleBox = (gcnew System::Windows::Forms::ListBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->registerBox = (gcnew System::Windows::Forms::TextBox());
			this->memoryBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// instructionBox
			// 
			this->instructionBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->instructionBox->Location = System::Drawing::Point(12, 32);
			this->instructionBox->Multiline = true;
			this->instructionBox->Name = L"instructionBox";
			this->instructionBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->instructionBox->Size = System::Drawing::Size(415, 578);
			this->instructionBox->TabIndex = 0;
			this->instructionBox->Text = L"addi $s1, $s1, 5";
			this->instructionBox->TextChanged += gcnew System::EventHandler(this, &MipsGui::textBox1_TextChanged);
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(8, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(92, 20);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Instructions";
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button1->Location = System::Drawing::Point(12, 616);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(415, 54);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Execute";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MipsGui::button1_Click);
			// 
			// cycleBox
			// 
			this->cycleBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->cycleBox->FormattingEnabled = true;
			this->cycleBox->ItemHeight = 20;
			this->cycleBox->Location = System::Drawing::Point(433, 32);
			this->cycleBox->Name = L"cycleBox";
			this->cycleBox->Size = System::Drawing::Size(119, 624);
			this->cycleBox->TabIndex = 3;
			this->cycleBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MipsGui::cycleBox_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(429, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(47, 20);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Cycle";
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(558, 9);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(122, 20);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Register Values";
			// 
			// registerBox
			// 
			this->registerBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->registerBox->Location = System::Drawing::Point(558, 32);
			this->registerBox->Multiline = true;
			this->registerBox->Name = L"registerBox";
			this->registerBox->Size = System::Drawing::Size(292, 624);
			this->registerBox->TabIndex = 7;
			// 
			// memoryBox
			// 
			this->memoryBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->memoryBox->Location = System::Drawing::Point(859, 32);
			this->memoryBox->Multiline = true;
			this->memoryBox->Name = L"memoryBox";
			this->memoryBox->Size = System::Drawing::Size(292, 624);
			this->memoryBox->TabIndex = 9;
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(859, 9);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(118, 20);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Memory Values";
			// 
			// MipsGui
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1164, 682);
			this->Controls->Add(this->memoryBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->registerBox);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->cycleBox);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->instructionBox);
			this->Name = L"MipsGui";
			this->Text = L"Mips Simulator";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void listBox2_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}


private: 
	void updateCycleList()
	{
		cycleBox->Items->Clear();
		for (int i = 0; mips_run->getRegisterHistory().size() > i; i++)
		{
			cycleBox->Items->Add(i);
		}
	}

	void MarshalString(String^ s, std::string& os) {
		using namespace Runtime::InteropServices;
		const char* chars =
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}
	void populateInstructions()
	{
		array<Char>^ sep = gcnew array<Char>{ '\n' };
		array<String^>^ insts = instructionBox->Text->Split(sep, StringSplitOptions::None);
		array<Char>^ aurgsep = gcnew array<Char>{ ',' };
		std::vector<std::string> instructions = std::vector<std::string>();
		for (int i = 0; insts->Length > i; i++)
		{
			String^ currLine = insts[i]->ToString();
			currLine = currLine->Substring(0, currLine->IndexOf(" ")) + "," + currLine->Substring(currLine->IndexOf(" ") + 1);
			currLine = currLine->Replace(" ", "");
			currLine = currLine->Replace("\r", "");

			std::string str = "";
			MarshalString(currLine, str);
			instructions.push_back(str);
			//instructions.push_back(str);

			//cycleBox->Items->Add(insts[i]->ToString());
		}
		mips_run->insertInstructions(instructions);
	}
	System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
	{
 		//cycleBox->Items->Add(1);
		if (mips_run != NULL)
			delete mips_run;
		mips_run = new MIPS();
		
		populateInstructions();
		mips_run->run();
		updateCycleList();
	
}
private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
	private: System::Void cycleBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if (cycleBox->SelectedIndex > mips_run->getRegisterHistory().size() || mips_run->getRegisterHistory().size() < 0)
		return;
		registerBox->Text = "";
		for (int i = 0; 8 > i; i++)
		{
			int32_t regVal = mips_run->getRegisterHistory().at(cycleBox->SelectedIndex)[i];
			if (i == 7)
				registerBox->Text += ("$pc: " + regVal + " | line: " + ((regVal /2) ));
			else
				registerBox->Text += ("$s" + i + ": " + regVal + "\r\n");

		}

		memoryBox->Text = "";
		for (int i = 0; 32 > i; i++)
		{
			int32_t memVal = mips_run->getMemoryHistory().at(cycleBox->SelectedIndex)[i];
			memoryBox->Text += ("mem " + i + ": " + memVal + "\r\n");
		}
}
};
}

