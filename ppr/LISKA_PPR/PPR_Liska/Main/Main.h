#pragma once
#include "InputParser.h"
#include "TMeasuredValue.h"

void printHelp();
void printProgramStart(Parser::InputParser *params);
void runComputation(std::unique_ptr<Common::Segment>& segment, int *windowSize, bool dayParalelism, Parser::InputParser *params);
void runSolution(Parser::InputParser *params);
double runSerial(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data);
double runParallel(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, bool dayParalelism);
void runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data);
