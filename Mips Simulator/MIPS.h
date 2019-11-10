#pragma once
#define DATA_MEMORY_SIZE 1000000
#include <vector>
class MIPS
{
private:
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
	std::vector<int32_t*> registerHistory;
	int32_t registers[8] = { 0 };
	int32_t pc = 0;
	uint32_t* instructionmemory;
	uint32_t datamemory[DATA_MEMORY_SIZE] = { 0 };
	ControlOutput control;
	Wire wires[32] = { 0 };
	size_t instructionCount = 0;
	enum WIRES {
		INSTRUCTION = 0, WRITEREGISTER = 1, JUMP = 2, READDATA1 = 3, READDATA2 = 4, ALURD2 = 5, ALURES = 6, READDATA = 7, WRITEDATA = 8, SIGNEXTENDED = 9, SHIFTLEFT2ALU = 10,
		MUXALURES = 11, PCPLUS4 = 12, PCOUT = 14, PCIN = 15, SHIFTLEFT2INST = 16, ZERO = 17, ALUCONTROLRES = 18, FOUR = 19, NULLWIRE = 20
	};
	enum ALUOPEARTIONS { ADD = 0, SUBTRACT = 1, BITWISEAND = 2, BITWISEOR = 3, SHIFTLEFT = 4, SHIFTRIGHT = 5, SHIFTLEFTA = 6, SHIFTRIGHTA = 7 };

	void initWire(int wire, int length);
	void initWires();
	int32_t getWire(int wire);
	int32_t getWire(SubWire wire);
	void setWire(int wire, int32_t data);
	void setWire(SubWire wire, int32_t data);
	SubWire getSub(int wire, int offset, int size);
	void InstructionMemory(int readaddr, int instruction);
	void Control(SubWire instruction);
	void ALU(int busA, int busB, uint8_t operation, int data, int zero);
	void RegistersRead(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2);
	void RegistersWrite(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2);
	void DataMemory(uint32_t address, uint32_t writedata, uint32_t output);
	void Mux(int busA, int busB, uint8_t select, int output);
	void Mux(SubWire busA, SubWire busB, uint8_t select, int output);
	void ALUControl(int shamt, int opcode);
	void SignExtend(SubWire input, int output);
public:
	MIPS();
	~MIPS();
	void loadInstructions(std::vector<int32_t> instructions);
	void insertInstructionStart();
	int insertInstruction(std::string instruction);
	void clearInstructions( );

	int run();
	std::vector<int32_t*> getRegisterHistory();


};

