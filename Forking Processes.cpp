#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

#define SIZE 20

// Function to find the minimum in an array range
int findMin(int arr[], int start, int end) {
    int minVal = INT_MAX;
    for (int i = start; i < end; i++) {
        if (arr[i] < minVal)
            minVal = arr[i];
    }
    return minVal;
}

int main() {
    int arr[SIZE];
    int pipefd[2];  // Pipe for communication
    pid_t pid;

    srand(time(0));  // Seed random number generator

    // Generate 20 random numbers
    cout << "Generated Array: ";
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100;  // Numbers between 0 and 99
        cout << arr[i] << " ";
    }
    cout << endl;

    // Create pipe
    if (pipe(pipefd) == -1) {
        cerr << "Pipe creation failed!" << endl;
        return 1;
    }

    // Forking a new process
    pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    } 
    else if (pid == 0) { // Child Process
        close(pipefd[0]);  // Close unused read end
        int minChild = findMin(arr, SIZE / 2, SIZE);
        cout << "Child Process (PID: " << getpid() << ") found min: " << minChild << endl;
        write(pipefd[1], &minChild, sizeof(minChild));
        close(pipefd[1]);  // Close write end
    } 
    else { // Parent Process
        close(pipefd[1]);  // Close unused write end
        int minParent = findMin(arr, 0, SIZE / 2);
        int minChild;
        read(pipefd[0], &minChild, sizeof(minChild));
        close(pipefd[0]);  // Close read end

        cout << "Parent Process (PID: " << getpid() << ") found min: " << minParent << endl;

        int minOverall = (minParent < minChild) ? minParent : minChild;
        cout << "Overall Minimum: " << minOverall << endl;

        wait(NULL);  // Wait for child to finish
    }

    return 0;
}
