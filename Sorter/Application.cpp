#include "Application.h"
#include <iostream>
#include <filesystem>
#include "Sort.h"

namespace fs = std::filesystem;

void Sorter::Application::Init()
{
	std::cout << "Enter buffer size:"; std::cin >> m_N;

	std::string inputName;
	std::cout << "Please enter file name to sort : "; std::cin >> inputName;

	if (fs::path(inputName).is_relative()) {
		m_InputFilePath = fs::absolute(inputName).remove_filename().string();
		m_InputFileName = fs::path(inputName).filename().string();
		m_OutputFilePath = m_InputFilePath;
		m_OutputFileName = fs::path(inputName).filename().stem().string() + ".output.txt";
	}
}

int Sorter::Application::Run()
{
	Sorter::ExternalMergeSort sort(m_InputFilePath + "\\" + m_InputFileName, m_N);
	sort.Sort();
	
	return 0;
}
