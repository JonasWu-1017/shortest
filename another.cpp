#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

// Function to print a vector
void printVector(const vector<int>& v) {
    for (int x : v) {
        cout << x << " ";
    }
    cout << "\n";
}
// Function to find the shortest subsequence with sum >= k
vector<int> shortestSubsequence(int k, vector<int>& arr) {
    // Initialize variables to store the current sum, start and end indices, and minimum length
    int currSum = 0;
    int start = 0;
    int end = 0;
    int minLength = arr.size() + 1;
    int minstart = 0;

    // Initialize a vector to store the result
    vector<int> result;

    // Loop through the array
    while (end < arr.size()) {
        // Add the current element to the current sum
        currSum += arr[end];

        // While the current sum is greater than or equal to k and start is less than or equal to end
        while (currSum >= k && start <= end) {
            // Update the minimum length and result if needed
            if (end - start + 1 < minLength) {
                minstart = start;
                minLength = end - start + 1;
                result = move(vector<int>(arr.begin() + start, arr.begin() + end + 1)); // use move to avoid copying
            }

            // Subtract the start element from the current sum and increment start
            currSum -= arr[start];
            start++;
        }

        // Increment end
        end++;
    }
    
    if (0==minstart) minstart=minLength-2;
    for(int i=0; i<=minstart; ++i)
        arr.erase(arr.begin());

    // Return the result
    return result;
}



// Main function
int main() {
    /*
    // Speed up input/output operations
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read the input from a text file
    ifstream fin("test.txt");
    if (!fin) {
        cerr << "Error opening file\n";
        return -1;
    }

    // Read the target sum
    int k;
    fin >> k;

    // Read the array
    vector<int> arr;
    int x;
    while (fin >> x) {
        arr.push_back(x);
    }

    // Close the file
    fin.close();
    */
    int k=80;
    vector<int> arr = {3, 17, 42, 1, 26, 8, 0, 63};

    // Start a timer
    auto start = chrono::steady_clock::now(); // use steady_clock instead of high_resolution_clock

    while(0 < arr.size()) {
    // Find the shortest subsequence with sum >= k
    vector<int> subseq = shortestSubsequence(k, arr);
    // Print the subsequence
    printVector(subseq);
    }

    // Stop the timer
    auto stop = chrono::steady_clock::now();

    // Calculate the duration
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

    // Print the duration
    cout << "duration: " << duration.count() << " ns\n";
}
