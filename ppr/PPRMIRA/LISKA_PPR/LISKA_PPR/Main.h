#pragma once
#include "InputParser.h"
#include "TMeasuredValue.h"


void runComputation(Parser::InputParser *params, std::unique_ptr<Common::Segment>& segment, int *windowSize, bool dayParalelism);
void runSolution(Parser::InputParser *params);
double runSerial(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data);
double runParallel(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, bool dayParalelism);
void runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data);
