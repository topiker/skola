#pragma once
#include <iostream>
#include "sqlite3.h"

/// <summary>
/// Trida zajistuje pripojeni k databazi
/// </summary>
class DatabaseConnector
{
public:
	/// <summary>
	/// Inicializuje spojeni
	/// </summary>
	/// <param name="dbPath">Cesta k databazi</param>
	DatabaseConnector(const char **dbPath);
	/// <summary>
	/// Destruktor -> zruší spojeni
	/// </summary>
	~DatabaseConnector();
	/// <summary>
	/// Funkce obalujici jednotliva sql volani
	/// </summary>
	/// <param name="query">Dotaz v SQL</param>
	/// <param name="callback">pointer na metodu, ktera se ma vykonat po nacteni dat</param>
	/// <param name="data">struktura, do ktere se maji data ulozit</param>
	/// <returns>true, pokud prikaz probehl v poradku</returns>
	bool executeCommand(const char * query, int callback(void *pArg, int argc, char **argv, char **columnNames), void *data);
private:
	/// <summary>
	/// Priznak, zda je pripojeni v poradku
	/// </summary>
	bool connectionOk;
	/// <summary>
	/// Pointer na spojeni s databazi
	/// </summary>
	sqlite3 *db;
};

