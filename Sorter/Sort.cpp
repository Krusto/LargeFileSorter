#include "Sort.h"
#include <fstream>
#include "Util.h"
namespace Sorter {
    ExternalMergeSort::ExternalMergeSort(std::string_view inputFileName, uint32_t bufferSize) {
        m_InputFilePath = fs::absolute(inputFileName).parent_path();
        m_InputFileName = fs::path(inputFileName).filename().stem();
        m_OutputFileName = fs::path(m_InputFileName.string() + ".output.txt");
 
        m_Size = bufferSize;
        m_Buffer.reserve(m_Size);
    }

    void ExternalMergeSort::GetChunksCount()
    {
        std::fstream inputFile(m_InputFilePath.string() + "\\" + m_InputFileName.string() + ".txt", std::ios::in);
        uint32_t numberCount{};
        uint32_t number{};
        while (inputFile >> number) {
            numberCount++;
        }

        uint32_t neededChunks = numberCount / m_Size;
        m_NumChunkFiles = neededChunks;
    }
    void ExternalMergeSort::Sort()
    {
        m_Buffer.resize(m_Size,0);

        m_InputFile = openFile((m_InputFilePath.string() + "\\" + m_InputFileName.string() + ".txt").data()
                                , "r");

        GetChunksCount();
        m_ChunkFiles.resize(m_NumChunkFiles,nullptr);
        for (size_t i = 0; i < m_NumChunkFiles; i++)
        {
            m_ChunkFiles[i] = openFile((m_InputFilePath.string() + "\\" +
                m_InputFileName.stem().string() +
                std::to_string(i) + ".txt").data(),"w");
        }

        bool moreInput = true;
        uint32_t element{}, i{};
        int nextOutputFile = 0;

        while (moreInput) {

            for (i = 0; i < m_Size ; i++) {
                if (fscanf_s(m_InputFile, "%d ", &m_Buffer[i]) != 1) {
                    moreInput = false;
                    break;
                }
            }

            if(i != 0)
            MergeSort(m_Buffer.data(), 0, i - 1);

            for (int j = 0; j < i; j++)
                fprintf(m_ChunkFiles[nextOutputFile],
                    "%d ", m_Buffer[j]);

            nextOutputFile++;
        }
        for (int i = 0; i < m_NumChunkFiles; i++)
            fclose(m_ChunkFiles[i]);

        fclose(m_InputFile);

        MergeFiles();

    }
    void ExternalMergeSort::MergeFiles()
    {
        for (int i = 0; i < m_NumChunkFiles; i++)
        {
            m_ChunkFiles[i] = openFile((m_InputFilePath.string() + "\\" +
                m_InputFileName.stem().string() +
                std::to_string(i) + ".txt").data(), "r");
        }
       
        m_OutputFile = openFile((m_InputFilePath.string()+"\\"+
                                m_OutputFileName.string()).data(),
                        "w");
        
        MinHeapNode* harr = new MinHeapNode[m_NumChunkFiles];
        int i;
        for (i = 0; i < m_NumChunkFiles; i++) {
            if (fscanf_s(m_ChunkFiles[i], "%d ", &harr[i].element) != 1)
                break;

            harr[i].index = i;
        }
        MinHeap  hp(harr, i);
        uint32_t count{};

        while (count != i) {
            MinHeapNode root = hp.GetMin();
            fprintf_s(m_OutputFile, "%d ", root.element);

            if (fscanf_s(m_ChunkFiles[root.index], "%d ",
                &root.element)
                != 1) {
                root.element = INT_MAX;
                count++;
            }

            hp.ReplaceMin(root);
        }
        for ( i = 0; i < m_ChunkFiles.size(); i++)
        {
            fclose(m_ChunkFiles[i]);
        }
        fclose(m_OutputFile);
        delete[] harr;

    }
    void ExternalMergeSort::Merge(uint32_t array[], uint32_t left, uint32_t mid, uint32_t right)
    {
        auto const subArrayOne = mid - left + 1;
        auto const subArrayTwo = right - mid;

        auto* leftArray = new uint32_t[subArrayOne],
            * rightArray = new uint32_t[subArrayTwo];

        for (auto i = 0; i < subArrayOne; i++)
            leftArray[i] = array[left + i];
        for (auto j = 0; j < subArrayTwo; j++)
            rightArray[j] = array[mid + 1 + j];

        auto indexOfSubArrayOne = 0, 
            indexOfSubArrayTwo = 0; 
        int indexOfMergedArray = left; 

        
        while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
            if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
                array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                indexOfSubArrayOne++;
            }
            else {
                array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                indexOfSubArrayTwo++;
            }
            indexOfMergedArray++;
        }
        
        
        while (indexOfSubArrayOne < subArrayOne) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
            indexOfMergedArray++;
        }
        
        
        while (indexOfSubArrayTwo < subArrayTwo) {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
            indexOfMergedArray++;
        }
        delete[] leftArray;
        delete[] rightArray;
    }

    void ExternalMergeSort::MergeSort(uint32_t array[], uint32_t begin, uint32_t end)
    {
        if (begin >= end)
            return;
        uint32_t mid = begin + (end - begin) / 2;
        MergeSort(array, begin, mid);
        MergeSort(array, mid + 1, end);
        Merge(array, begin, mid, end);
    }

    MinHeap::MinHeap(ExternalMergeSort::MinHeapNode arr[], uint32_t size)
    {
        m_Size = size;
        m_Data = arr; 
        int i = (m_Size - 1) / 2;
        while (i >= 0) {
            Heapify(i);
            i--;
        }
    }

    void MinHeap::Heapify(uint32_t i)
    {
        uint32_t left = Left(i);
        uint32_t right = Right(i);
        uint32_t smallest = i;

        if (left < m_Size && m_Data[left].element < m_Data[i].element)
            smallest = left;

        if (right < m_Size && m_Data[right].element < m_Data[smallest].element)
            smallest = right;

        if (smallest != i) {
            Swap(&m_Data[i], &m_Data[smallest]);
            Heapify(smallest);
        }
    }

    void MinHeap::ReplaceMin(ExternalMergeSort::MinHeapNode node)
    {
        m_Data[0] = node;
        Heapify(0);
    }

    void MinHeap::Swap(ExternalMergeSort::MinHeapNode* l, ExternalMergeSort::MinHeapNode* r)
    {
        ExternalMergeSort::MinHeapNode temp = *r;
        *r = *l;
        *l = temp;
    }

}