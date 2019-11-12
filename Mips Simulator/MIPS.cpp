#include "MIPS.h"
#include <iostream>
#include <string>
#include <vector>


MIPS::MIPS()
{
	initWires();
}
MIPS::~MIPS()
{

}

void MIPS::initWire(int wire, int length) {
	uint16_t len = 0;
	for (int i = 0; length > i; i++)
		len = (len << 1) + 1;
	wires[wire].bitwise = len;
}
void MIPS::initWires() {
	initWire(INSTRUCTION, 16);
	initWire(WRITEREGISTER, 5);
	initWire(JUMP, 16);
	initWire(READDATA1, 16);
	initWire(READDATA2, 16);
	initWire(ALURD2, 16);
	initWire(ALURES, 16);
	initWire(READDATA, 16);
	initWire(WRITEDATA, 16);
	initWire(SIGNEXTENDED, 16);
	initWire(SHIFTLEFT2ALU, 16);
	initWire(MUXALURES, 16);
	initWire(PCPLUS2, 16);
	initWire(SHIFTLEFT2INST, 16);
	initWire(PCOUT, 16);
	initWire(PCIN, 16);
	initWire(ALT, 1);
	initWire(ALUCONTROLRES, 16);
	initWire(TWO, 16);
	initWire(NULLWIRE, 16);
	initWire(PCREG, 16);
	initWire(MUXPCP4ALU, 16);
	initWire(SL2ALU, 16);
	initWire(MUXPCP4ALURES, 16);
}
int16_t MIPS::getWire(int wire)
{
	return wires[wire].data & wires[wire].bitwise;
}
int16_t MIPS::getWire(SubWire wire)
{
	return (getWire(wire.wire) >> wire.offset) & wire.bitwise;
}

void MIPS::setWire(int wire, int16_t data)
{
	wires[wire].data = data & wires[wire].bitwise;
}


void MIPS::setWire(SubWire wire, int16_t data)
{
	wires[wire.wire].data &= ~(wire.bitwise << wire.offset);
	wires[wire.wire].data |= ((data >> wire.offset)& wire.bitwise)& wires[wire.wire].bitwise;
}

MIPS::SubWire MIPS::getSub(int wire, int offset, int size)
{
	SubWire subwire = SubWire{ wire, offset, (uint16_t)size };
	uint16_t len = 0;
	for (int i = 0; size > i; i++)
		len = (len << 1) + 1;
	subwire.bitwise = len;
	return subwire;
}

void MIPS::InstructionMemory(int readaddr, int instruction)//done
{
	if (getWire(readaddr) >= (int16_t)instructionCount*2) {
		std::cout << "[Error][InstructionMemory] outside of instruction memory range: " << getWire(readaddr) << '\n';
		setWire(instruction, NULL);
	}
	else
		setWire(instruction, instructionmemory[getWire(readaddr)/2]);
}

void MIPS::Control(SubWire instruction)//Supports 16 opcodes
{
	control = ControlOutput();
	switch (getWire(instruction)) {
	case 0x00://Addi
		control.RegDst = 0;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = ADD;
		control.MemWrite = 0;
		control.ALUSrc = 1;
		control.RegWrite = 1;//yes
		break;
	case 0x01://Subi
		control.RegDst = 0;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = SUBTRACT;
		control.MemWrite = 0;
		control.ALUSrc = 1;
		control.RegWrite = 1;//yes
		break;
	case 0x02://Add
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = ADD;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;
	case 0x03://Sub
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = SUBTRACT;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;  
	case 0x04://lh
		control.RegDst = 0;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 1;
		control.MemtoReg = 1;
		control.ALUOp = ADD;
		control.MemWrite = 0;
		control.ALUSrc = 1;
		control.RegWrite = 1;//yes
		break;
	case 0x05://sh
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = ADD;
		control.MemWrite = 1;
		control.ALUSrc = 1; 
		break;
	case 0x06://sll
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = SHIFTLEFT;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;
	case 0x07://slr
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = SHIFTRIGHT;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;
	case 0x08://or
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = BITWISEOR;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;
	case 0x09://xor
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = BITWISEXOR;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;
	case 0x0A://slli
		control.RegDst = 1;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = SHIFTLEFT;
		control.MemWrite = 0;
		control.ALUSrc = 1;
		control.RegWrite = 1;//yes
		break;
		break;
	case 0x0B:
		break;
	case 0x0C://slt
		control.RegDst = 0;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = COMPARE;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;
	case 0x0D://Branch
		control.RegDst = 0;
		control.Jump = 1;
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = DONOTHING;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 0;
		break;
	case 0x0E://Beq
		control.RegDst = 0;
		control.Jump = 0;
		control.Branch = 1;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = ADD;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 0;
		break;
	case 0x0F://Bgt
		control.RegDst = 0;
		control.Jump = 0;
		control.Branch = 1;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = COMPARE;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 0;
		break;
	case 0x10://Bgtz
		control.RegDst = 0;
		control.Jump = 0;
		control.Branch = 1;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = COMPAREZ;
		control.MemWrite = 0;
		control.ALUSrc = 1;
		control.RegWrite = 0;
		break;
	case 0x11://Bgt256
		control.RegDst = 0;
		control.Jump = 0;
		control.Branch = 1;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = COMPARE256;
		control.MemWrite = 0;
		control.ALUSrc = 1;
		control.RegWrite = 0;
		break;
	case 0x12://res
		control.RegDst = 0;
		control.Jump = 0;//no
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = DONOTHING;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 1;//yes
		break;
	default:
		std::cout << "[Error][Control] invlid instruction " << getWire(instruction) << '\n';
	}
}

void MIPS::ALU(int busA, int busB, uint8_t operation, int data, int alt)//ALUOutput& output)
{
	switch (operation) {
	case ADD://Add
		setWire(data, getWire(busA) + getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case SUBTRACT://Subtract
		setWire(data, getWire(busA) - getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case BITWISEAND://Bitwise AND
		setWire(data, getWire(busA) & getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case BITWISEOR://Bitwise OR
		setWire(data, getWire(busA) | getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case SHIFTLEFT://Shift left
		setWire(data, (uint16_t)getWire(busA) << (uint16_t)getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case SHIFTRIGHT://Shift right
		setWire(data, (uint16_t)getWire(busA) >> (uint16_t)getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case SHIFTLEFTA://Shift left arithmetic
		setWire(data, getWire(busA) << getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case SHIFTRIGHTA://Shift right arithmetic
		setWire(data, getWire(busA) >> getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case COMPARE://Shift right arithmetic
		setWire(data, getWire(busA) > getWire(busB));
		setWire(alt, (getWire(busA) > getWire(busB)) & 0x1);
		break;
	case COMPAREZ://Shift right arithmetic
		setWire(data, getWire(busA) > 0);
		setWire(alt, (getWire(busA) > 0) & 0x1);
		break;
	case COMPARE256://Shift right arithmetic
		setWire(data, getWire(busA) > 0x0100);
		setWire(alt, (getWire(busA) > 0x01000) & 0x1);
		break;
	case BITWISEXOR://Bitwise OR
		setWire(data, getWire(busA) ^ getWire(busB));
		setWire(alt, getWire(data) == 0);
		break;
	case DONOTHING://Shift right arithmetic
		setWire(data, 0);
		setWire(alt, getWire(data) == 0);
		break;
	default:
		std::cout << "[ERROR][ALU] invlid opcode " << control.ALUOp << '\n';
	}
}

void MIPS::RegistersRead(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2)//RegistersOutput& output)//done
{
	setWire(data1, registers[getWire(reg1)]);
	setWire(data2, registers[getWire(reg2)]);
}
void MIPS::RegistersWrite(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2)//RegistersOutput& output)//done
{
	setWire(data1, registers[getWire(reg1)]);
	setWire(data2, registers[getWire(reg2)]);

	if (control.RegWrite) {
		registers[getWire(regw)] = getWire(writedata);
	}
}

void MIPS::DataMemory(uint16_t address, uint16_t writedata, uint16_t output)
{

	if (control.MemRead) {
		setWire(output, datamemory[getWire(address)]);
	}

	if (control.MemWrite) {
		datamemory[getWire(address)] = getWire(writedata);
	}
}

void MIPS::Mux(int busA, int busB, uint16_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
}

void MIPS::Mux(SubWire busA, SubWire busB, uint16_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
	//output = (select > 0) ? busB : busA;
}



void MIPS::ALUControl(int shamt, int opcode)//Not needed?
{

}

void MIPS::SignExtend(SubWire input, int output)
{
	int16_t value = getWire(input);
	int offset = input.offset;
	int size = -1;
	for (int i = 31; i >= 0; i--)
		if ((offset & (1 << i)) > 0)
		{
			size = i;
			break;
		}
	int16_t type = (value & (1 << size)) >> size;//Get if positive or negative

	for (int i = size; 32 > i; i++)
		value |= type << i;//Extend the sign

	setWire(output, value);
}

/*void MIPS::createJumpAddress(SubWire inst, SubWire pcp4, int output)
{
	setWire(output, getWire(inst) | (getWire(pcp4) << 28));
}*/

/*void MIPS::loadInstructions(int16_t* insts, size_t ic)
{
	instructionCount = ic;
	instructionmemory = (uint16_t*)calloc(instructionCount, sizeof(uint16_t));
	memcpy(instructionmemory, insts, ic);
}*/
void MIPS::loadInstructions(std::vector<int16_t> instructions)
{

}

int MIPS::run()
{
	setWire(PCIN, 0);//Initalize PC to first instruction
	setWire(TWO, 2);
	setWire(PCREG, 7);
	SubWire rs = getSub(INSTRUCTION, 8, 3);
	SubWire rt = getSub(INSTRUCTION, 5, 3);
	SubWire rd = getSub(INSTRUCTION, 2, 3);
	SubWire sgnextend = getSub(INSTRUCTION, 0, 5);
	SubWire opcode = getSub(INSTRUCTION, 11, 5);
	while (getWire(PCOUT) < (int16_t)(instructionCount*2))
	{
		InstructionMemory(PCOUT, INSTRUCTION);
		Control(opcode);
		Mux(rt, rd, control.RegDst, WRITEREGISTER);
		RegistersRead(rs, rt, WRITEREGISTER, WRITEDATA, READDATA1, READDATA2);//Don't write until next time through
		SignExtend(sgnextend, SIGNEXTENDED);
		Mux(READDATA2, SIGNEXTENDED, control.ALUSrc, ALURD2);
		ALU(READDATA1, ALURD2, control.ALUOp, ALURES, ALT);
		DataMemory(ALURES, READDATA2, READDATA);
		Mux(ALURES, READDATA, control.MemtoReg, WRITEDATA);
		RegistersWrite(rs, rt, WRITEREGISTER, WRITEDATA, READDATA1, READDATA2);//Write since Write data has been updated
		
		setWire(MUXPCP4ALU, control.Branch && getWire(ALT));//Boolean mux for ALU result

		ALU(PCOUT, TWO, ADD, PCPLUS2, NULLWIRE);
		setWire(JUMP, (getWire(PCPLUS2) & 0xF800) | ((getWire(INSTRUCTION) << 1) & 0x07FF));
		setWire(SL2ALU, getWire(SIGNEXTENDED) << 1);
		ALU(PCPLUS2, SL2ALU, ADD, MUXALURES, NULLWIRE);
		//setWire(MUXALURES, getWire(SL2ALU));
		Mux(PCPLUS2, MUXALURES, getWire(MUXPCP4ALU), MUXPCP4ALURES);
		Mux(MUXPCP4ALURES, JUMP, control.Jump, PCIN);

		registers[7] = getWire(PCIN);
		setWire(PCOUT, registers[7]);//Set PC value of register
		int16_t* registernew = (int16_t*)malloc(sizeof(int16_t) * 8);
		memcpy(registernew, registers, sizeof(int16_t) * 8);
		registerHistory.push_back(registernew);
		int16_t* memorynew = (int16_t*)malloc(sizeof(int16_t) * 32);
		memcpy(memorynew, datamemory, sizeof(int16_t) * 32);
		memoryHistory.push_back(memorynew);
	}
	return 0;
}

void MIPS::insertInstructionStart()
{
	instructionmemory = (uint16_t*)calloc(1000, sizeof(uint16_t));
}

uint16_t MIPS::getrVal(std::string str, std::string instruction, int currLine)
{
	if (str.find("$") != std::string::npos)
	{
		if (str.find("$pc") != std::string::npos)
		{
			return 7;
		}
		else
		{
			return std::stoi(str.substr(2));
		}
	}
	else if(str.c_str()[0] >= '0' && str.c_str()[0] <= '9')//starts with number
		return std::stoi(str);
	else
	{
		for (int i = 0; labelList.size(); i++)
		{
			if (strcmp(str.c_str(), labelList.at(i).label.c_str()) == 0)
			{
				//std::cout << "Line at " << labelList.at(i).line - currLine << "\n";
				if(instruction == "b")
					return labelList.at(i).line;
				else
					return labelList.at(i).line - currLine - 1;
			}
		}
	}
	return NULL;
}

int MIPS::insertInstructions(std::vector<std::string> instructions)
{
	labelList.clear();
	instructionCount = instructions.size();
	for (int i = 0; instructions.size() > i; i++)
	{
		std::string instructionstr = instructions.at(i);

		int pos = instructionstr.find(":");
		if (pos != std::string::npos)
		{
			labels label = labels{ i, instructionstr.substr(0, pos) };
			labelList.push_back(label);
		}
	}

	instructionmemory = (uint16_t*)malloc(sizeof(uint16_t) * instructions.size());

	for (int i = 0; instructions.size() > i; i++)
	{
		std::string instructionstr = instructions.at(i);

		size_t commacnt = std::count(instructionstr.begin(), instructionstr.end(), ',');
		int16_t instruction = 0;

		std::vector<std::string> result;
		std::string working = instructionstr;
		int pos;
		while ((pos = working.find(',')) != std::string::npos)
		{
			result.push_back(working.substr(0, pos));
			working = working.substr(pos+1);
		}
		result.push_back(working);
		std::string insttype = result.at(0);
		if (insttype.find(":"))
			insttype = insttype.substr(insttype.find(":") + 1);
		//instruction |= std::stoi(result[1]) & 0x0700;//rs
		//instruction |= std::stoi(result[2]) & 0x00E0;//rd
		//instruction |= std::stoi(result[3]) & 0x001C;//rt
		if (insttype == "addi") {
			instruction |= 0x00 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "subi") {
			instruction |= 0x01 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "add") {
			instruction |= 0x02 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[3], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[1], insttype, i) << 2) & 0x001C;//rd
		}
		else if (insttype == "sub") {
			instruction |= 0x03 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[3], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[1], insttype, i) << 2) & 0x001C;//rd
		}
		else if (insttype == "or") {
			instruction |= 0x08 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[3], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[1], insttype, i) << 2) & 0x001C;//rd
		}
		else if (insttype == "xor") {
			instruction |= 0x09 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[3], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[1], insttype, i) << 2) & 0x001C;//rd
		}
		else if (insttype == "lh") {
			instruction |= 0x04 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "sh") {
			instruction |= 0x05 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "sll") {
			instruction |= 0x06 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[3], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[1], insttype, i) << 2) & 0x001C;//rd
		}
		else if (insttype == "slr") {
			instruction |= 0x07 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[3], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[1], insttype, i) << 2) & 0x001C;//rd
		}
		else if (insttype == "slli") {
			instruction |= 0x0A << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "b") {
			instruction |= 0x0D << 11;//Control
			instruction |= (getrVal(result[1], insttype, i)) & 0x07FF;//branch
		}
		else if (insttype == "slt") {
			instruction |= 0x0C << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3], insttype, i) << 2) & 0x001C;//rd
		}
		else if (insttype == "beq") {
			instruction |= 0x0E << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "bgt") {
			instruction |= 0x0F << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00F0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "bgtz") {
			instruction |= 0x10 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00F0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "bgt256") {
			instruction |= 0x11 << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00F0;//rt
			instruction |= (getrVal(result[3], insttype, i)) & 0x001F;//Immediate
		}
		else if (insttype == "res") {
			instruction |= 0x0F << 11;//Control
			instruction |= (getrVal(result[2], insttype, i) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1], insttype, i) << 5) & 0x00F0;//rt
			instruction |= (getrVal(result[1], insttype, i) << 2) & 0x001C;//rd
		}
		else
		{
			instruction |= 0xFFFF;
			std::cout << "[Error][Assembler] unknown instruction " << insttype << '\n';
		}
		instructionmemory[i] = instruction;
	}
	int a0tmp = 0x0010;
	datamemory[a0tmp] = 0x0101;
	datamemory[a0tmp + 2] = 0x0110;
	datamemory[a0tmp + 4] = 0x0011;
	datamemory[a0tmp + 6] = 0x00F0;
	datamemory[a0tmp + 8] = 0x00FF;
	registers[0] = a0tmp;
	registers[1] = 0x0005;
	registers[2] = 0x0000;
	registers[3] = 0x0040;
	registers[4] = 0x1010;
	registers[5] = 0x000F;
	registers[6] = 0x00F;

	return 0;
}

std::vector<int16_t*> MIPS::getRegisterHistory()
{
	return registerHistory;
}
std::vector<int16_t*> MIPS::getMemoryHistory()
{
	return memoryHistory;
}