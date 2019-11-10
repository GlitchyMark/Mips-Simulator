#include "MIPS.h"
#include <iostream>
#include <string>
using namespace std;


MIPS::MIPS()
{

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
	initWire(INSTRUCTION, 32);
	initWire(WRITEREGISTER, 5);
	initWire(JUMP, 32);
	initWire(READDATA1, 32);
	initWire(READDATA2, 32);
	initWire(ALURD2, 32);
	initWire(ALURES, 32);
	initWire(READDATA, 32);
	initWire(WRITEDATA, 32);
	initWire(SIGNEXTENDED, 32);
	initWire(SHIFTLEFT2ALU, 32);
	initWire(MUXALURES, 32);
	initWire(PCPLUS4, 4);
	initWire(SHIFTLEFT2INST, 32);
	initWire(PCOUT, 32);
	initWire(PCIN, 32);
	initWire(ZERO, 1);
	initWire(ALUCONTROLRES, 32);
	initWire(FOUR, 32);
	initWire(NULLWIRE, 32);
}
int32_t MIPS::getWire(int wire)
{
	return wires[wire].data & wires[wire].bitwise;
}
int32_t MIPS::getWire(SubWire wire)
{
	return (wires[wire.wire].data >> wire.offset)& wire.bitwise;
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
	if (readaddr >= (int32_t)instructionCount) {
		std::cout << "[Error][InstructionMemory] outside of instruction memory range: " << getWire(readaddr) << '\n';
		setWire(instruction, NULL);
	}
	else
		setWire(instruction, instructionmemory[readaddr]);
}

void MIPS::Control(SubWire instruction)//Supports 16 opcodes
{
	control = ControlOutput();
	switch (getWire(instruction)) {
	case 0x00://Add
		control.RegDst = 0;
		control.Jump = 0;
		control.Branch = 0;
		control.MemRead = 0;
		control.MemtoReg = 0;
		control.ALUOp = 0;
		control.MemWrite = 0;
		control.ALUSrc = 0;
		control.RegWrite = 0;
		break;
	case 0x01:
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
	switch (control.ALUOp) {
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

void MIPS::Mux(int busA, int busB, uint8_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
}

void MIPS::Mux(SubWire busA, SubWire busB, uint8_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
	//output = (select > 0) ? busB : busA;
}



void MIPS::ALUControl(int shamt, int opcode)//Not needed?
{

}

void MIPS::SignExtend(SubWire input, int output)
{
	int32_t value = getWire(input.wire);
	int offset = input.bitwise;
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
	setWire(PCOUT, instructionmemory[pc]);//Initalize PC to first instruction
	setWire(FOUR, 4);
	SubWire rs = getSub(INSTRUCTION, 6, 3);
	SubWire rd = getSub(INSTRUCTION, 3, 3);
	SubWire rt = getSub(INSTRUCTION, 0, 3);
	SubWire sgnextend = getSub(INSTRUCTION, 0, 4);
	SubWire opcode = getSub(INSTRUCTION, 11, 5);
	while (pc < (int32_t)instructionCount)
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

		setWire(MUXALURES, control.Branch && getWire(ZERO));//Boolean mux for ALU result

		ALU(PCOUT, FOUR, ADD, PCPLUS4, NULLWIRE);
		setWire(JUMP, (getWire(PCPLUS4) & 0xF000) | ((getWire(INSTRUCTION) << 2) & 0x0FFF));
		Mux(MUXALURES, JUMP, control.Jump, PCIN);

		registerHistory.push_back(registers);
	}
	return 0;
}

void MIPS::insertInstructionStart()
{
	instructionmemory = (uint32_t*)calloc(1000, sizeof(uint32_t));
}

int MIPS::insertInstruction(std::string instructionstr)
{
	size_t commacnt = std::count(instructionstr.begin(), instructionstr.end(), ',');
	int32_t instruction = 0;
	
	stringstream ss();
	//ss << instructionstr;
	vector<string> result = vector<string>();
	/*while (ss.good())
	{
		string substr;
		std::getline(ss, substr, ',');
		result.push_back(substr);
	}*/
	//instruction.find(',')
	//instruction.substr(instruction.find(','))
	string insttype = result.at(0);
	//instruction |= std::stoi(result[1]) & 0x0700;//rs
	//instruction |= std::stoi(result[2]) & 0x00E0;//rd
	//instruction |= std::stoi(result[3]) & 0x001C;//rt
	if ("addi") {
		instruction |= 0x0000 & 0xF800;//Control
		instruction |= std::stoi(result[1]) & 0x0700;//rs
		instruction |= std::stoi(result[2]) & 0x00E0;//rd
		instruction |= std::stoi(result[3]) & 0x001F;//Immediate
	} else {
		instruction |= 0xF000;
	}


}

std::vector<int32_t*> MIPS::getRegisterHistory()
{
	return registerHistory;
}