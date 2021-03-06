// FormatProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "TextManager.h"
#include "TaskCreator.h"
#include "TaskCleaner.h"
#include "InputReader.h"
int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << "no work for me\n";
		return 1;
	}
	InputReader reader;
	//extract info from main arguments
	try{
	reader.Read(argc, argv);
	}
	catch (std::bad_alloc &e)
	{
		std::cout << e.what()
		 << '\n';
		return -1;
	}
	catch (std::exception &e)
	{
		std::cout << e.what()
		 << '\n';
		return -1;
	}
	//if there is HTML command it should be the last one
	reader.sortHTML();
	//make extracted info useful
	char ** commandsList = reader.getCommands();
	unsigned int ncommands = reader.getNumCommands();
	if (ncommands <= 0)
	{
		std::cout << "no commands has been entered \n";
		return 1;
	}
	char** paths = reader.getPaths();
	unsigned int npaths = reader.getNumPaths();
	if (npaths <= 0 )
	{
		std::cout << "no paths has been entered \n";
		return 1;
	}
	//collection from commands
	Command ** commands;
	try{
		commands = new Command*[ncommands];
	}
	catch(std::bad_alloc& e)
	{
		std::cout << e.what()
		 << '\n';
		return -1;
	}
	//creating commands
	for (int j = 0;
	j < ncommands;
	j++)
	{
		try{
			commands[j] = TaskCreator::Create(commandsList[j]);
			}
		catch (std::bad_alloc& e)
		{
			TaskCleaner::Clear(j, commands);
			std::cout << e.what()
			 << '\n';
			return -1;
		}
		catch (std::exception& e)
		{
			TaskCleaner::Clear(j, commands);
			std::cout << e.what()
			 << '\n';
			return -1;
		}
	}
	TextManager manager;
	for (int i = 0;
	i < npaths;
	i++)
	{
		try{
			manager.Read(paths[i]);
		}
		catch(std::bad_alloc& e)
		{
			manager.Clear();
			std::cout << e.what();
			continue;
		}
		catch (std::exception& e)
		{
			manager.Clear();
			std::cout << e.what();
			continue;
		}
		for (int j = 0;
		j < ncommands;
		j++)
		{
			commands[j]->Format(manager);
		}
		try{
			manager.Write(paths[i]);
		}
		catch (std::bad_alloc &e)
		{
			manager.Clear();
			std::cout << e.what();
			continue;
		}
		catch (std::exception & e)
		{
			manager.Clear();
			std::cout << e.what();
			continue;
		}
		manager.Clear();
		}
	TaskCleaner::Clear(ncommands, commands);
	return 0;
	}
