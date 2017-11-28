#pragma once
#include "InputParser.h"
#include "TMeasuredValue.h"

/// <summary>
/// Procedura pro vypocet konkretniho segmentu na procesoru
/// </summary>
void runComputation(std::unique_ptr<Common::Segment>& segment, bool dayParalelism,int *windowSize, std::shared_ptr< std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>& detectedPeaks);
/// <summary>
/// Procedura spoustejici konkretni vypocet podle vstupnich parametru
/// </summary>
void runSolution(Parser::InputParser *params);
/// <summary>
/// Seriovy vypocet, vraci dobu behu v sekundach
/// </summary>
double runSerial(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>>& allPeaks);
/// <summary>
/// Paralelni vypocet, vraci dobu behu v sekundach
/// </summary>
double runParallel(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data,std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>>& allPeaks);
/// <summary>
/// Vypocet na akceleratoru, vraci dobu behu v sekundach
/// </summary>
double runOnGraphics(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> &allPeaks);
/// <summary>
/// Funkce pro export dat do SVG
/// </summary>
double printToSvg(Parser::InputParser *params, const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> &allPeaks);
/// <summary>
/// Funkce pro vypis dat na STDIN
/// </summary>
double printAsCSV(const std::unique_ptr<std::vector<std::unique_ptr<Common::Segment>>>& data, std::vector<std::shared_ptr<std::vector<std::vector<std::shared_ptr<PeakPeakDetector::Peak>>>>> &allPeaks);



