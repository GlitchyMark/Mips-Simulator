#pragma once
#define DATA_MEMORY_SIZE 1000000
#include <vector>
#include <string>
class MIPS
{
private:
	struct Wire {
		int16_t data;
		uint16_t bitwise;
	};
	struct SubWire {
		int wire;
		int offset;
		uint16_t bitwise;
	};
	struct ALUOutput {
		int16_t data;
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
		int16_t data;
		bool zero;
	};
	struct RegistersOutput {
		int16_t data1;
		int16_t data2;
	};
	struct labels {
		int line;
		std::string label;
	};
	std::vector<int16_t*> registerHistory;
	std::vector<int16_t*> memoryHistory;
	std::vector<labels> labelList;
	int16_t registers[8] = { 0 };
	uint16_t* instructionmemory;
	uint16_t datamemory[DATA_MEMORY_SIZE] = { 0 };
	ControlOutput control;
	Wire wires[32] = { 0 };
	size_t instructionCount = 0;
	enum WIRES {
		INSTRUCTION = 0, WRITEREGISTER = 1, JUMP = 2, READDATA1 = 3, READDATA2 = 4, ALURD2 = 5, ALURES = 6, READDATA = 7, WRITEDATA = 8, SIGNEXTENDED = 9, SHIFTLEFT2ALU = 10,
		MUXALURES = 11, PCPLUS2 = 12, PCOUT = 14, PCIN = 15, SHIFTLEFT2INST = 16, ALT = 17, ALUCONTROLRES = 18, TWO = 19, NULLWIRE = 20, PCREG = 21, MUXPCP4ALU = 22, SL2ALU = 23,
		MUXPCP4ALURES = 24
	};
	enum ALUOPEARTIONS { ADD = 0, SUBTRACT = 1, BITWISEAND = 2, BITWISEOR = 3, SHIFTLEFT = 4, SHIFTRIGHT = 5, SHIFTLEFTA = 6, SHIFTRIGHTA = 7, COMPARE = 8, COMPAREZ = 9, COMPARE256 = 10, BITWISEXOR = 11,DONOTHING = 12};

	void initWire(int wire, int length);
	void initWires();
	int16_t getWire(int wire);
	int16_t getWire(SubWire wire);
	void setWire(int wire, int16_t data);
	void setWire(SubWire wire, int16_t data);
	SubWire getSub(int wire, int offset, int size);
	void InstructionMemory(int readaddr, int instruction);
	void Control(SubWire instruction);
	void ALU(int busA, int busB, uint8_t operation, int data, int zero);
	void RegistersRead(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2);
	void RegistersWrite(SubWire reg1, SubWire reg2, int regw, int writedata, int data1, int data2);
	void DataMemory(uint16_t address, uint16_t writedata, uint16_t output);
	void Mux(int busA, int busB, uint16_t select, int output);
	void Mux(SubWire busA, SubWire busB, uint16_t select, int output);
	void ALUControl(int shamt, int opcode);
	void SignExtend(SubWire input, int output);
	uint16_t MIPS::getrVal(std::string str, std::string instruction, int currLine);
public:
	MIPS();
	~MIPS();
	void loadInstructions(std::vector<int16_t> instructions);
	void insertInstructionStart();
	int insertInstructions(std::vector<std::string> instructions);
	void clearInstructions( );

	int run();
	std::vector<int16_t*> getRegisterHistory();
	std::vector<int16_t*> getMemoryHistory();


};

