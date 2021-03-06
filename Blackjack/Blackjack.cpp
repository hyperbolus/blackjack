#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <detours.h>
#include <vector>
#include <string>
#include <fstream>
#undef max
#include "argparse.h"

using namespace std::string_literals;

int main(int argc, char* argv[])
{
	argparse::ArgumentParser program("Blackjack", "1.1.1");

	program.add_argument("executable")
		.help("The executable you wish to launch");

	program.add_argument("-q", "--quiet")
		.action([=](const auto &){
			std::cout.rdbuf(NULL);
		})
		.help("Don't output any messages")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-d", "--working-dir")
		.append()
		.help("Set the working directory to launch");

	program.add_argument("-l", "--dll")
		.append()
		.help("DLL file to be injected (multiple)");

	program.add_argument("-e", "--env")
		.append()
		.help("Start proccess with environment variable (multiple)");

	try {
		program.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << program;
		std::exit(1);
	}

	// Figure out current directory if set
	auto exe = program.get("executable");
	std::string directory;
	if (auto _d = program.present("-d")) {
		directory = *_d;
	}
	else {
		// If not, choose the exe's
		const size_t last = exe.rfind('\\');
		if (std::string::npos != last)
		{
			directory = exe.substr(0, last);
		}
	}

	std::cout << "Executable: " << exe.c_str() << std::endl;

	std::string env;
	auto _env = program.present<std::vector<std::string>>("--env");
	if (_env) {
		for (auto& _v : *_env)
		{
			env.append(_v);
			env.append("\0"s);
		}
	}

	std::cout << "Environment Variables:  " << env << std::endl;
	
	auto _dlls = program.present<std::vector<std::string>>("--dll");
	std::vector<LPCSTR> dlls;
	if (_dlls) {
		for (auto& _dll : *_dlls)
		{
			std::cout << "Injecting " << _dll << std::endl;
			dlls.push_back(_dll.c_str());
		}
	}

	SetCurrentDirectory(directory.c_str());

	std::cout << "DLLs Loaded: " << dlls.size() << std::endl;

	// CreateProccess boilerplate
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	// Where the magic happens
	auto detour = DetourCreateProcessWithDlls(exe.c_str(), NULL, NULL, NULL, TRUE,
		//CREATE_DEFAULT_ERROR_MODE,
		CREATE_SUSPENDED,
		_env ? LPTSTR(env.c_str()) : NULL, directory.c_str(), &si, &pi, dlls.size(), &dlls[0], NULL);

	if (detour) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "Failed" << std::endl;
		return 1;
	}

	ResumeThread(pi.hThread);

	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD dwResult = 0;
	auto exit = GetExitCodeProcess(pi.hProcess, &dwResult);
	if (!exit)
	{
		std::cout << "Err: " << dwResult << std::endl;
		return false;
	}
	
	return 0;
}