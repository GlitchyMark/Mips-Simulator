#define DATA_MEMORY_SIZE 1000000

#include<iostream>
using namespace std;
struct Wire {
	int32_t data;
	uint32_t bitwise;
};

struct SubWire {
	int wire;
	int offset;
	uint32_t bitwise;
};

struct ALUOutput {
	int32_t data;
	bool zero;
};

struct ControlOutput {
	bool RegDst = 0;
	bool Jump = 0;
	bool Branch = 0;
	bool MemRead = 0;
	bool MemtoReg = 0;
	int8_t ALUOp = 0;
	bool MemWrite = 0;
	bool ALUSrc = 0;
	bool RegWrite = 0;
};

struct DataMemOutput {
	int32_t data;
	bool zero;
};
struct RegistersOutput {
	int32_t data1;
	int32_t data2;
};



int32_t registers[32] = { 0 };
int32_t pc = 0;

uint32_t *instructionmemory;
uint32_t datamemory[DATA_MEMORY_SIZE] = { 0 };
ControlOutput control;
Wire wires[32] = { 0 };

size_t instructionCount = 0;

enum WIRES {
	INSTRUCTION = 0, WRITEREGISTER = 1, JUMP = 2, READDATA1 = 3, READDATA2 = 4, ALURD2 = 5, ALURES = 6, READDATA = 7, WRITEDATA = 8, SIGNEXTENDED = 9, SHIFTLEFT2ALU = 10,
	MUXALURES = 11, PCPLUS4 = 12, PCOUT = 14, PCIN = 15, SHIFTLEFT2INST = 16, ZERO = 17, ALUCONTROLRES = 18, FOUR = 19, NULLWIRE = 20
};


enum ALUOPEARTIONS { ADD = 0, SUBTRACT = 1, BITWISEAND = 2, BITWISEOR = 3, SHIFTLEFT = 4, SHIFTRIGHT = 5, SHIFTLEFTA = 6, SHIFTRIGHTA = 7 };


void initWire(int wire, int length) {
	uint32_t len = 0;
	for (int i = 0; length > i; i++)
		len = (len << 1) + 1;
	wires[wire].bitwise = len;
}
void initWires() {
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
int32_t getWire(int wire)
{
	return wires[wire].data & wires[wire].bitwise;
}
int32_t getWire(SubWire wire)
{
	return (wires[wire.wire].data >> wire.offset)& wire.bitwise;
}

void setWire(int wire, int32_t data)
{
	wires[wire].data = data & wires[wire].bitwise;
}


void setWire(SubWire wire, int32_t data)
{
	wires[wire.wire].data &= ~(wire.bitwise << wire.offset);
	wires[wire.wire].data |= ((data >> wire.offset)& wire.bitwise)& wires[wire.wire].bitwise;
}

SubWire getSub(int wire, int offset, int size)
{
	SubWire subwire = SubWire{ wire, offset, (uint16_t)size };
	uint32_t len = 0;
	for (int i = 0; size > i; i++)
		len = (len << 1) + 1;
	subwire.bitwise = len;
	return subwire;
}

void InstructionMemory(int readaddr, int instruction)//done
{
	if (readaddr >= instructionCount) {
		std::cout << "[Error][InstructionMemory] outside of instruction memory range: " << getWire(readaddr) << '\n';
		setWire(instruction, NULL);
	}
	else
		setWire(instruction, instructionmemory[readaddr]);
}

void Control(SubWire instruction)
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

void ALU(int busA, int busB, uint8_t operation, int data, int zero)//ALUOutput& output)
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

void RegistersRead(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2)//RegistersOutput& output)//done
{
	setWire(data1, registers[getWire(reg1)]);
	setWire(data2, registers[getWire(reg2)]);
}
void RegistersWrite(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2)//RegistersOutput& output)//done
{
	setWire(data1, registers[getWire(reg1)]);
	setWire(data2, registers[getWire(reg2)]);

	if (control.RegWrite) {
		registers[getWire(regw)] = getWire(writedata);
	}
}

void DataMemory(uint32_t address, uint32_t writedata, uint32_t output)
{

	if (control.MemRead) {
		setWire(output, datamemory[getWire(address)]);
	}

	if (control.MemWrite) {
		datamemory[address] = getWire(writedata);
	}
}

void Mux(int busA, int busB, uint8_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
}

void Mux(SubWire busA, SubWire busB, uint8_t select, int output)
{
	setWire(output, (select > 0) ? getWire(busB) : getWire(busA));
	//output = (select > 0) ? busB : busA;
}



void ALUControl(int shamt, int opcode)//Not needed?
{

}

void SignExtend(SubWire input, int output)
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

void createJumpAddress(SubWire inst, SubWire pcp4, int output)
{
	setWire(output, getWire(inst) | (getWire(pcp4) << 28));
}

void loadInstructions(int32_t *insts, size_t ic)
{
	instructionCount = ic;
	instructionmemory = (uint32_t*)calloc(instructionCount, sizeof(uint32_t));
	memcpy(instructionmemory, insts, ic);
}
 
int run()
{
	setWire(PCOUT, instructionmemory[pc]);//Initalize PC to first instruction
	setWire(FOUR, 4);
	while (pc < instructionCount)
	{
		InstructionMemory(PCOUT, INSTRUCTION);
		Control(getSub(INSTRUCTION, 26, 6));
		Mux(getSub(INSTRUCTION, 16, 5), getSub(INSTRUCTION, 11, 5), control.RegDst, WRITEREGISTER);
		RegistersRead(getSub(INSTRUCTION, 21, 5), getSub(INSTRUCTION, 16, 5), WRITEREGISTER, WRITEDATA, READDATA1, READDATA2);//Don't write until next time through
		SignExtend(getSub(INSTRUCTION, 15, 16), SIGNEXTENDED);

		Mux(READDATA2, SIGNEXTENDED, control.ALUSrc, ALURD2);
		ALU(READDATA1, ALURD2, control.ALUOp, ALURES, ZERO);
		DataMemory(ALURES, READDATA2, READDATA);
		Mux(ALURES, READDATA, control.MemtoReg, WRITEDATA);

		setWire(MUXALURES, control.Branch && getWire(ZERO));//Boolean mux for ALU result

		ALU(PCOUT, FOUR, ADD, PCPLUS4, NULLWIRE);
		setWire(JUMP, (getWire(PCPLUS4) & 0xF0000000) | ((getWire(INSTRUCTION) << 2) & 0x0FFFFFFFF));
		Mux(MUXALURES, JUMP, control.Jump, PCIN);
	}
	return 0;
}

