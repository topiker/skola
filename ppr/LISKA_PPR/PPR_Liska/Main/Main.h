#pragma once
#include "InputParser.h"
#include "TMeasuredValue.h"

void printHelp();
void printProgramStart(Parser::InputParser *params);
void runSolution(Parser::InputParser *params);
double runSerial(Parser::InputParser *params);
double runParallel(Parser::InputParser *params, bool dayParalelism);
void runOnGraphics(Parser::InputParser *params, std::vector<int> *segmentIds);
