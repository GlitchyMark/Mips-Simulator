#include<iostream>
#include "MipsGui.h"
using namespace System;
using namespace System::Windows::Forms;

int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MipsSimulator::MipsGui mg;
	Application::Run(% mg);

	return 0;
}

