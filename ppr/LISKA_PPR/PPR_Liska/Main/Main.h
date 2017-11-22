#pragma once
#include "InputParser.h"
#include "TMeasuredValue.h"

void printHelp();
void printProgramStart(Parser::InputParser *params);
void runComputation(std::unique_ptr<Common::Segment>& segment, int *windowSize, bool dayParalelism);
void runSolution(Parser::InputParser *params);
double runSerial(Parser::InputParser *params);
double runParallel(Parser::InputParser *params, bool dayParalelism);
void runOnGraphics(Parser::InputParser *params, std::vector<int> *segmentIds);
