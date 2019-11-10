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
	uint32_t len = 0;
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
	initWire(PCPLUS4, 16);
	initWire(SHIFTLEFT2INST, 16);
	initWire(PCOUT, 16);
	initWire(PCIN, 16);
	initWire(ZERO, 1);
	initWire(ALUCONTROLRES, 16);
	initWire(FOUR, 16);
	initWire(NULLWIRE, 16);
	initWire(PCREG, 16);
	initWire(MUXPCP4ALU, 16);
	initWire(SL2ALU, 16);
	initWire(MUXPCP4ALURES, 16);
}
int32_t MIPS::getWire(int wire)
{
	return wires[wire].data & wires[wire].bitwise;
}
int32_t MIPS::getWire(SubWire wire)
{
	return (getWire(wire.wire) >> wire.offset) & wire.bitwise;
}

void MIPS::setWire(int wire, int32_t data)
{
	wires[wire].data = data & wires[wire].bitwise;
}


void MIPS::setWire(SubWire wire, int32_t data)
{
	wires[wire.wire].data &= ~(wire.bitwise << wire.offset);
	wires[wire.wire].data |= ((data >> wire.offset)& wire.bitwise)& wires[wire.wire].bitwise;
}

MIPS::SubWire MIPS::getSub(int wire, int offset, int size)
{
	SubWire subwire = SubWire{ wire, offset, (uint16_t)size };
	uint32_t len = 0;
	for (int i = 0; size > i; i++)
		len = (len << 1) + 1;
	subwire.bitwise = len;
	return subwire;
}

void MIPS::InstructionMemory(int readaddr, int instruction)//done
{
	if (getWire(readaddr) >= (int32_t)instructionCount*4) {
		std::cout << "[Error][InstructionMemory] outside of instruction memory range: " << getWire(readaddr) << '\n';
		setWire(instruction, NULL);
	}
	else
		setWire(instruction, instructionmemory[getWire(readaddr)/4]);
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
	case 0x02:
		break;
	case 0x03:
		break;
	case 0x04:
		break;
	case 0x05:
		break;
	case 0x06:
		break;
	case 0x07:
		break;
	case 0x08:
		break;
	case 0x09:
		break;
	case 0x0A:
		break;
	case 0x0B:
		break;
		break;
	case 0x0C:
		break;
	case 0x0D:
		break;
	case 0x0E:
		break;
	case 0x0F:
		break;
	default:
		std::cout << "[Error][Control] invlid instruction " << getWire(instruction) << '\n';
	}
}

void MIPS::ALU(int busA, int busB, uint8_t operation, int data, int zero)//ALUOutput& output)
{
	switch (operation) {
	case ADD://Add
		setWire(data, getWire(busA) + getWire(busB));
		break;
	case SUBTRACT://Subtract
		setWire(data, getWire(busA) - getWire(busB));
		break;
	case BITWISEAND://Bitwise AND
		setWire(data, getWire(busA) & getWire(busB));
		break;
	case BITWISEOR://Bitwise OR
		setWire(data, getWire(busA) | getWire(busB));
		break;
	case SHIFTLEFT://Shift left
		setWire(data, (uint32_t)getWire(busA) << (uint32_t)getWire(busB));
		break;
	case SHIFTRIGHT://Shift right
		setWire(data, (uint32_t)getWire(busA) >> (uint32_t)getWire(busB));
		break;
	case SHIFTLEFTA://Shift left arithmetic
		setWire(data, getWire(busA) << getWire(busB));
		break;
	case SHIFTRIGHTA://Shift right arithmetic
		setWire(data, getWire(busA) >> getWire(busB));
		break;
	default:
		std::cout << "[ERROR][ALU] invlid opcode " << control.ALUOp << '\n';
	}
	setWire(zero, getWire(data) == 0);
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

void MIPS::DataMemory(uint32_t address, uint32_t writedata, uint32_t output)
{

	if (control.MemRead) {
		setWire(output, datamemory[getWire(address)]);
	}

	if (control.MemWrite) {
		datamemory[address] = getWire(writedata);
	}
}

void MIPS::Mux(int busA, int busB, uint32_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
}

void MIPS::Mux(SubWire busA, SubWire busB, uint32_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
	//output = (select > 0) ? busB : busA;
}



void MIPS::ALUControl(int shamt, int opcode)//Not needed?
{

}

void MIPS::SignExtend(SubWire input, int output)
{
	int32_t value = getWire(input);
	int offset = input.offset;
	int size = -1;
	for (int i = 31; i >= 0; i--)
		if ((offset & (1 << i)) > 0)
		{
			size = i;
			break;
		}
	int32_t type = (value & (1 << size)) >> size;//Get if positive or negative

	for (int i = size; 32 > i; i++)
		value |= type << i;//Extend the sign

	setWire(output, value);
}

/*void MIPS::createJumpAddress(SubWire inst, SubWire pcp4, int output)
{
	setWire(output, getWire(inst) | (getWire(pcp4) << 28));
}*/

/*void MIPS::loadInstructions(int32_t* insts, size_t ic)
{
	instructionCount = ic;
	instructionmemory = (uint32_t*)calloc(instructionCount, sizeof(uint32_t));
	memcpy(instructionmemory, insts, ic);
}*/
void MIPS::loadInstructions(std::vector<int32_t> instructions)
{

}

int MIPS::run()
{
	setWire(PCIN, 0);//Initalize PC to first instruction
	setWire(FOUR, 4);
	setWire(PCREG, 7);
	SubWire rs = getSub(INSTRUCTION, 8, 3);
	SubWire rt = getSub(INSTRUCTION, 5, 3);
	SubWire rd = getSub(INSTRUCTION, 2, 3);
	SubWire sgnextend = getSub(INSTRUCTION, 0, 5);
	SubWire opcode = getSub(INSTRUCTION, 11, 5);
	while (getWire(PCOUT) < (int32_t)(instructionCount*4))
	{
		InstructionMemory(PCOUT, INSTRUCTION);
		Control(opcode);
		Mux(rt, rd, control.RegDst, WRITEREGISTER);
		RegistersRead(rs, rt, WRITEREGISTER, WRITEDATA, READDATA1, READDATA2);//Don't write until next time through
		SignExtend(sgnextend, SIGNEXTENDED);
		Mux(READDATA2, SIGNEXTENDED, control.ALUSrc, ALURD2);
		ALU(READDATA1, ALURD2, control.ALUOp, ALURES, ZERO);
		DataMemory(ALURES, READDATA2, READDATA);
		Mux(ALURES, READDATA, control.MemtoReg, WRITEDATA);
		RegistersWrite(rs, rt, WRITEREGISTER, WRITEDATA, READDATA1, READDATA2);//Write since Write data has been updated
		
		setWire(MUXPCP4ALU, control.Branch && getWire(ZERO));//Boolean mux for ALU result

		ALU(PCOUT, FOUR, ADD, PCPLUS4, NULLWIRE);
		setWire(JUMP, (getWire(PCPLUS4) & 0xF000) | ((getWire(INSTRUCTION) << 2) & 0x0FFF));
		setWire(SL2ALU, getWire(SIGNEXTENDED) << 2);
		ALU(PCPLUS4, SL2ALU, ADD, MUXALURES, NULLWIRE);
		Mux(PCPLUS4, MUXALURES, getWire(MUXPCP4ALU), MUXPCP4ALURES);
		Mux(MUXPCP4ALURES, JUMP, control.Jump, PCIN);

		registers[7] = getWire(PCIN);
		setWire(PCOUT, registers[7]);//Set PC value of register
		int32_t* registernew = (int32_t*)malloc(sizeof(int32_t) * 8);
		memcpy(registernew, registers, sizeof(int32_t) * 8);
		registerHistory.push_back(registernew);
	}
	return 0;
}

void MIPS::insertInstructionStart()
{
	instructionmemory = (uint32_t*)calloc(1000, sizeof(uint32_t));
}

uint32_t MIPS::getrVal(std::string str)
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
				return labelList.at(i).line;
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

	instructionmemory = (uint32_t*)malloc(sizeof(uint32_t) * instructions.size());

	for (int i = 0; instructions.size() > i; i++)
	{
		std::string instructionstr = instructions.at(i);

		size_t commacnt = std::count(instructionstr.begin(), instructionstr.end(), ',');
		int32_t instruction = 0;

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
			instruction |= 0x0000 & 0xF800;//Control
			instruction |= (getrVal(result[2]) << 8) & 0x0700;//rs
			instruction |= (getrVal(result[1]) << 5) & 0x00E0;//rt
			instruction |= (getrVal(result[3])) & 0x001F;//Immediate
		}
		else {
			instruction |= 0xF000;
		}
		instructionmemory[i] = instruction;
	}

	return 0;
}

std::vector<int32_t*> MIPS::getRegisterHistory()
{
	return registerHistory;
}