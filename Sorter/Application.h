#include <cstdint>
#include <vector>
#include <fstream>

namespace Sorter {
	class Application {
	public:
		Application() = default;
		/**
		* Инициализира всички променливи
		*/
		void Init();

		/**
		* Пуска апликацията и връща код с резултат как се е изпълнил кода
		*/
		int Run();
		/**
		* Път до входящ файл
		*/
		std::string m_InputFilePath{};
		/**
		* Име на входящ файл
		*/
		std::string m_InputFileName{};
		/**
		* Път до изходен файл
		*/
		std::string m_OutputFilePath{};
		/**
		* Име на изходен файл
		*/
		std::string m_OutputFileName{};
		/**
		* Големина на буфера
		*/
		 uint32_t m_N{0};
	};
}