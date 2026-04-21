#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm> 

using namespace std;

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
    }
}

int partitionLast(vector<int>& arr, int low, int high) {
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

void quickSortLast(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionLast(arr, low, high);
        quickSortLast(arr, low, pi - 1);
        quickSortLast(arr, pi + 1, high);
    }
}

void startQuickSortLast(vector<int>& arr) {
    if (!arr.empty()) quickSortLast(arr, 0, arr.size() - 1);
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

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        swap(arr[min_idx], arr[i]);
    }
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

void countSortByDigit(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);

    for (int i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) arr[i] = output[i];
}

void radixSort(vector<int>& arr) {
    if (arr.empty()) return;

    int max_val = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] > max_val) max_val = arr[i];
    }

    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countSortByDigit(arr, exp);
    }
}

void pigeonholeSort(vector<int>& arr) {
    if (arr.empty()) return;

    int min_val = arr[0], max_val = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
    }

    int range = max_val - min_val + 1;
    vector<vector<int>> holes(range);

    for (int i = 0; i < arr.size(); i++) {
        holes[arr[i] - min_val].push_back(arr[i]);
    }

    int index = 0;
    for (int i = 0; i < range; i++) {
        for (int j = 0; j < holes[i].size(); j++) {
            arr[index++] = holes[i][j];
        }
    }
}

void countingSort(vector<int>& arr) {
    if (arr.empty()) return;

    int max_val = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] > max_val) max_val = arr[i];
    }

    vector<int> count(max_val + 1, 0);
    for (int i = 0; i < arr.size(); i++) {
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

void gnomeSort(vector<int>& arr) {
    int index = 0;
    int n = arr.size();
    while (index < n) {
        if (index == 0 || arr[index] >= arr[index - 1]) {
            index++;
        }
        else {
            swap(arr[index], arr[index - 1]);
            index--;
        }
    }
}

void shellSort(vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
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
}

void startMergeSort(vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

///////////////////////////////////////////////////////////
int main() {
    string outputPath = "D:/uvt/methods/data_gen/data_gen/benchmark_results3.csv";
    ofstream resultsFile(outputPath, ios::app);

    string fullPath = "D:/uvt/methods/data_gen/data_gen/data/1kk/sort/reverse_1kk.txt";
    string displayName = "1kk/reverse_1kk.txt";

    ifstream file(fullPath);

    if (!file.is_open()) {
        cout << "Error: Could not open " << fullPath << endl;
        return 1;
    }

    int size;
    file >> size;
    vector<int> unsorted_numbers(size);
    for (int i = 0; i < size; i++) {
        file >> unsorted_numbers[i];
    }
    file.close();

    cout << "Loaded " << size << " elements from " << displayName << "\n";
    cout << "Starting massive benchmark...\n\n";

    //TEST 1: BUBBLE SORT
    vector<int> copy1 = unsorted_numbers;
    auto start1 = chrono::high_resolution_clock::now();
    bubbleSort(copy1);
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration1 = end1 - start1;
    cout << "Bubble Sort: " << duration1.count() << " seconds\n";
    resultsFile << displayName << ",BubbleSort," << duration1.count() << "\n";

    //TEST 2: SELECTION SORT
    vector<int> copy2 = unsorted_numbers;
    auto start2 = chrono::high_resolution_clock::now();
    selectionSort(copy2);
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration2 = end2 - start2;
    cout << "Selection Sort: " << duration2.count() << " seconds\n";
    resultsFile << displayName << ",SelectionSort," << duration2.count() << "\n";

    //TEST 3: INSERTION SORT
    vector<int> copy3 = unsorted_numbers;
    auto start3 = chrono::high_resolution_clock::now();
    insertionSort(copy3);
    auto end3 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration3 = end3 - start3;
    cout << "Insertion Sort: " << duration3.count() << " seconds\n";
    resultsFile << displayName << ",InsertionSort," << duration3.count() << "\n";

    //TEST 4: SHELL SORT
    vector<int> copy4 = unsorted_numbers;
    auto start4 = chrono::high_resolution_clock::now();
    shellSort(copy4);
    auto end4 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration4 = end4 - start4;
    cout << "Shell Sort: " << duration4.count() << " seconds\n";
    resultsFile << displayName << ",ShellSort," << duration4.count() << "\n";

    //TEST 5: GNOME SORT
    vector<int> copy5 = unsorted_numbers;
    auto start5 = chrono::high_resolution_clock::now();
    gnomeSort(copy5);
    auto end5 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration5 = end5 - start5;
    cout << "Gnome Sort: " << duration5.count() << " seconds\n";
    resultsFile << displayName << ",GnomeSort," << duration5.count() << "\n";

    // TEST 6A: QUICK SORT (LAST)
    if (displayName == "100k/flat_100k-3val.txt" || displayName == "100k/reverse_100k.txt" || displayName == "10k/sorted_10k.txt" || displayName == "100k/sorted_100k.txt" ) {
        cout << "Quick Sort (Last Pivot): SKIPPED (To prevent Stack Overflow!)\n";
        resultsFile << displayName << ",QuickSort_LastPivot,SKIPPED\n";
        resultsFile.flush();
    }
    else {
        vector<int> copy6a = unsorted_numbers;
        auto start6a = chrono::high_resolution_clock::now();
        startQuickSortLast(copy6a);
        auto end6a = chrono::high_resolution_clock::now();
        chrono::duration<double> duration6a = end6a - start6a;
        cout << "Quick Sort (Last Pivot): " << duration6a.count() << " seconds\n";
        resultsFile << displayName << ",QuickSort_LastPivot," << duration6a.count() << "\n";
        resultsFile.flush();
    }

    //TEST 6B: QUICK SORT (MIDDLE)
    vector<int> copy6b = unsorted_numbers;
    auto start6b = chrono::high_resolution_clock::now();
    startQuickSortMiddle(copy6b);
    auto end6b = chrono::high_resolution_clock::now();
    chrono::duration<double> duration6b = end6b - start6b;
    cout << "Quick Sort (Middle Pivot): " << duration6b.count() << " seconds\n";
    resultsFile << displayName << ",QuickSort_MidPivot," << duration6b.count() << "\n";
    resultsFile.flush();

    //TEST 7: MERGE SORT
    vector<int> copy7 = unsorted_numbers;
    auto start7 = chrono::high_resolution_clock::now();
    startMergeSort(copy7);
    auto end7 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration7 = end7 - start7;
    cout << "Merge Sort: " << duration7.count() << " seconds\n";
    resultsFile << displayName << ",MergeSort," << duration7.count() << "\n";

    //TEST 8: HEAP SORT
    vector<int> copy8 = unsorted_numbers;
    auto start8 = chrono::high_resolution_clock::now();
    heapSort(copy8);
    auto end8 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration8 = end8 - start8;
    cout << "Heap Sort: " << duration8.count() << " seconds\n";
    resultsFile << displayName << ",HeapSort," << duration8.count() << "\n";

    //TEST 9: COUNTING SORT
    vector<int> copy9 = unsorted_numbers;
    auto start9 = chrono::high_resolution_clock::now();
    countingSort(copy9);
    auto end9 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration9 = end9 - start9;
    cout << "Counting Sort: " << duration9.count() << " seconds\n";
    resultsFile << displayName << ",CountingSort," << duration9.count() << "\n";

    //TEST 10: PIGEONHOLE SORT
    vector<int> copy10 = unsorted_numbers;
    auto start10 = chrono::high_resolution_clock::now();
    pigeonholeSort(copy10);
    auto end10 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration10 = end10 - start10;
    cout << "Pigeonhole Sort: " << duration10.count() << " seconds\n";
    resultsFile << displayName << ",PigeonholeSort," << duration10.count() << "\n";

    //TEST 11: RADIX SORT
    vector<int> copy11 = unsorted_numbers;
    auto start11 = chrono::high_resolution_clock::now();
    radixSort(copy11);
    auto end11 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration11 = end11 - start11;
    cout << "Radix Sort: " << duration11.count() << " seconds\n";
    resultsFile << displayName << ",RadixSort," << duration11.count() << "\n";

    //TEST 12: C++ SORT
    vector<int> copy12 = unsorted_numbers;
    auto start12 = chrono::high_resolution_clock::now();
    std::sort(copy12.begin(), copy12.end());
    auto end12 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration12 = end12 - start12;
    cout << "C++ std::sort: " << duration12.count() << " seconds\n";
    resultsFile << displayName << ",StdSort," << duration12.count() << "\n";


    resultsFile.close();
    cout << "\nResults successfully saved to benchmark_results.csv" << endl;

    return 0;
}