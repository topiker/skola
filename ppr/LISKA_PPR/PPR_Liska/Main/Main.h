#pragma once
#include "InputParser.h"
#include "TMeasuredValue.h"

void printHelp();
void printProgramStart(Parser::InputParser *params);
void runSolution(Parser::InputParser *params);
void runSerial(Parser::InputParser *params);
long long runParallel(Parser::InputParser *params, bool dayParalelism);
void runOnGraphics(Parser::InputParser *params, std::vector<int> *segmentIds);
