#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <new>
#include <string>

using namespace std;


size_t current_auxiliary_memory = 0;
size_t peak_auxiliary_memory = 0;

void* operator new(size_t size) {
    current_auxiliary_memory += size;
    if (current_auxiliary_memory > peak_auxiliary_memory) {
        peak_auxiliary_memory = current_auxiliary_memory;
    }
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    return ptr;
}

void operator delete(void* ptr, size_t size) noexcept {
    current_auxiliary_memory -= size;
    free(ptr);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void resetMemoryTracker() {
    peak_auxiliary_memory = current_auxiliary_memory;
}


void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int partitionMiddle(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    swap(arr[mid], arr[high]);

    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortMiddle(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionMiddle(arr, low, high);
        quickSortMiddle(arr, low, pi - 1);
        quickSortMiddle(arr, pi + 1, high);
    }
}

void startQuickSortMiddle(vector<int>& arr) {
    if (!arr.empty()) quickSortMiddle(arr, 0, arr.size() - 1);
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void startMergeSort(vector<int>& arr) {
    if (!arr.empty()) mergeSort(arr, 0, arr.size() - 1);
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void countingSort(vector<int>& arr) {
    if (arr.empty()) return;

    int max_val = arr[0];
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] > max_val) max_val = arr[i];
    }

    vector<int> count(max_val + 1, 0);
    for (size_t i = 0; i < arr.size(); i++) {
        count[arr[i]]++;
    }

    int index = 0;
    for (int i = 0; i <= max_val; i++) {
        while (count[i] > 0) {
            arr[index] = i;
            index++;
            count[i]--;
        }
    }
}



void runBenchmarkOnFile(const string& basePath, const string& relativePath, ofstream& resultsFile, bool dryRun) {
    string fullPath = basePath + relativePath;

    ifstream file(fullPath);
    if (!file.is_open()) {
        cout << "[FAIL] Could not open file: " << relativePath << "\n";
        return;
    }

    int size;
    file >> size;

    if (dryRun) {
        cout << "[ OK ] Found: " << relativePath << " (" << size << " elements)\n";
        file.close();
        return;
    }

    vector<int> unsorted_numbers(size);
    for (int i = 0; i < size; i++) {
        file >> unsorted_numbers[i];
    }
    file.close();

    cout << "\n==================================================\n";
    cout << "Processing: " << relativePath << " (" << size << " elements)\n";
    cout << "==================================================\n";

    size_t base_mem, peak_mem_used;

    auto runTest = [&](const string& algoName, void(*sortFunc)(vector<int>&)) {
        vector<int> copy = unsorted_numbers;
        base_mem = current_auxiliary_memory;
        resetMemoryTracker();

        auto start = chrono::high_resolution_clock::now();
        sortFunc(copy);
        auto end = chrono::high_resolution_clock::now();

        peak_mem_used = peak_auxiliary_memory - base_mem;
        chrono::duration<double> duration = end - start;

        cout << algoName << ": " << duration.count() << " s | Mem: " << peak_mem_used << " bytes\n";
        resultsFile << relativePath << "," << algoName << "," << duration.count() << "," << peak_mem_used << "\n";
        resultsFile.flush();
        };

    auto runStdSort = [&]() {
        vector<int> copy = unsorted_numbers;
        base_mem = current_auxiliary_memory;
        resetMemoryTracker();

        auto start = chrono::high_resolution_clock::now();
        std::sort(copy.begin(), copy.end());
        auto end = chrono::high_resolution_clock::now();

        peak_mem_used = peak_auxiliary_memory - base_mem;
        chrono::duration<double> duration = end - start;

        cout << "StdSort: " << duration.count() << " s | Mem: " << peak_mem_used << " bytes\n";
        resultsFile << relativePath << ",StdSort," << duration.count() << "," << peak_mem_used << "\n";
        resultsFile.flush();
        };

    if (size >= 1000000) {
        cout << "InsertionSort: SKIPPED (Too slow for 1,000,000+ elements)\n";
    }
    else {
        runTest("InsertionSort", insertionSort);
    }

    runTest("QuickSort_MidPivot", startQuickSortMiddle);
    runTest("MergeSort", startMergeSort);
    runTest("HeapSort", heapSort);
    runTest("CountingSort", countingSort);
    runStdSort();
}


int main() {

    bool DRY_RUN =  false;

    string outputPath = "D:/uvt/methods/data_gen/data_gen/benchmark_results_final.csv";
    ofstream resultsFile;

    if (!DRY_RUN) {
        resultsFile.open(outputPath, ios::app);
        resultsFile.seekp(0, ios::end);
        if (resultsFile.tellp() == 0) {
            resultsFile << "Dataset,Algorithm,Time_Seconds,Peak_Heap_Memory_Bytes\n";
        }
    }

    string basePath = "D:/uvt/methods/data_gen/data_gen/data/";

    vector<string> filePaths = {
        /*
        "1k/random_1000.txt",
        "1k/random_1000-2.txt",
        "1k/random_1000-3.txt",

        "10k/almo/almost_10k-1-0.2.txt",
        "10k/almo/almost_10k-2-0.2.txt",
        "10k/almo/almost_10k-3-0.2.txt",
        "10k/rand/random_10k-1.txt",
        "10k/rand/random_10k-2.txt",
        "10k/rand/random_10k-3.txt",
        "10k/sort/reverse_10k.txt",
        "10k/sort/sorted_10k.txt",

        "100k/almo/almost_100k-1.txt",
        "100k/almo/almost_100k-2.txt",
        "100k/almo/almost_100k-3.txt",
        "100k/rand/random_100k-1.txt",
        "100k/rand/random_100k-2.txt",
        "100k/rand/random_100k-3.txt",
        "100k/sort/reverse_100k.txt",
        "100k/sort/sorted_100k.txt",
        "100k/flat/flat_100k-3val.txt",
        "100k/flat/flat_100k-5val.txt",
        "100k/flat/flat_100k-7val.txt",
        */
        "1kk/almo/almost_1kk-1.txt",
        "1kk/almo/almost_1kk-2.txt",
        "1kk/almo/almost_1kk-3.txt",
        "1kk/rand/random_1kk-1.txt",
        "1kk/rand/random_1kk-2.txt",
        "1kk/rand/random_1kk-3.txt",
        "1kk/sort/reverse_1kk.txt",
        "1kk/sort/sorted_1kk.txt",
    };

    if (DRY_RUN) {
        cout << "--- RUNNING DRY RUN (VALIDATING FILES ONLY) ---\n\n";
    }
    else {
        cout << "--- STARTING FULL BENCHMARK SUITE ---\n\n";
    }

    for (const string& relativePath : filePaths) {
        runBenchmarkOnFile(basePath, relativePath, resultsFile, DRY_RUN);
    }

    if (!DRY_RUN) {
        resultsFile.close();
        cout << "\n==================================================\n";
        cout << "ALL BENCHMARKS COMPLETE!\n";
        cout << "Results saved to: " << outputPath << "\n";
        cout << "==================================================\n";
    }

    return 0;
}
