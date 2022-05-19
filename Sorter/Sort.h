#pragma once
#include <fstream>
#include <cstdint>
#include <vector>
#include <filesystem>
namespace Sorter {
	namespace fs = std::filesystem;

	/**
	* Клас който се използва за сортиране на голям обем информация
	*/
	class ExternalMergeSort {
	public:
		struct MinHeapNode {
			uint32_t element;
			uint32_t index;
		};

		ExternalMergeSort() = default;
		ExternalMergeSort(std::string_view inputFileName,uint32_t bufferSize);

		/**
		* Функция която се използва за сортиране на цял файл
		*/
		void Sort();

	private:
		/**
		* Функция която изчислява на колко части трябва да се раздели файла
		*/
		void GetChunksCount();
		/**
		* Функция която събира отделни файлове в един и сортира информацията
		*/
		void MergeFiles();
		/**
		* Функция която събира два подниза
		*/
		void Merge(uint32_t array[], uint32_t left, uint32_t mid, uint32_t right);
		/**
		* Функция която сортира низ
		*/
		void MergeSort(uint32_t array[], uint32_t begin, uint32_t end);

		/**
		* Буфер с низ от прочетената информация
		*/
		std::vector<uint32_t> m_Buffer;
		/**
		* Големина на буфера
		*/
		uint32_t m_Size;

		/**
		* Низ от частични файлове
		*/
		std::vector<FILE*> m_ChunkFiles;
		/**
		* Входен файл
		*/
		FILE* m_InputFile;
		/**
		* Изходен файл
		*/
		FILE* m_OutputFile;

		/**
		* Път до входния файл
		*/
		fs::path m_InputFilePath;
		/**
		* име на входния файл
		*/
		fs::path m_InputFileName;
		/**
		* Име на изходния файл
		*/
		fs::path m_OutputFileName;
		/**
		* Брой частични файлове
		*/
		uint32_t m_NumChunkFiles;
	};

	class MinHeap {
		/**
		* Низ от възли
		*/
		ExternalMergeSort::MinHeapNode* m_Data;
		/**
		* Големина на масива
		*/
		uint32_t m_Size;
	public:
		MinHeap(ExternalMergeSort::MinHeapNode arr[], uint32_t size);
		/**
		* Връща индекс на лявата клетка
		*/
		uint32_t Left(uint32_t i){return(2 * i + 1);}
		/**
		* Връща индекс на дясната клетка
		*/
		uint32_t Right(uint32_t i){return(2 * i + 2);}
		/**
		* Връща минималната точка
		*/
		ExternalMergeSort::MinHeapNode GetMin(){return m_Data[0];}
		/**
		* Функция която пренарежда възлите на дададен индекс
		*/
		void Heapify(uint32_t i);
		/**
		* Функция която замества "корена" с нов възел и пренарежда отново цялото дърво
		*/
		void ReplaceMin(ExternalMergeSort::MinHeapNode node);
		/**
		* Функция която сменя местата на два възела
		*/
		void Swap(ExternalMergeSort::MinHeapNode* l, ExternalMergeSort::MinHeapNode* r);
	};

}