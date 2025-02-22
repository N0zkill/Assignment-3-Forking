#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

#define SIZE 20


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
    int pipefd[2];  
    pid_t pid;

    srand(time(0));  

    
    cout << "Generated Array: ";
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100;  
        cout << arr[i] << " ";
    }
    cout << endl;

    
    if (pipe(pipefd) == -1) {
        cerr << "Pipe creation failed!" << endl;
        return 1;
    }

    
    pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    } 
    else if (pid == 0) { 
        close(pipefd[0]);  
        int minChild = findMin(arr, SIZE / 2, SIZE);
        cout << "Child Process (PID: " << getpid() << ") found min: " << minChild << endl;
        write(pipefd[1], &minChild, sizeof(minChild));
        close(pipefd[1]);  
    } 
    else { 
        close(pipefd[1]);  
        int minParent = findMin(arr, 0, SIZE / 2);
        int minChild;
        read(pipefd[0], &minChild, sizeof(minChild));
        close(pipefd[0]);  

        cout << "Parent Process (PID: " << getpid() << ") found min: " << minParent << endl;

        int minOverall = (minParent < minChild) ? minParent : minChild;
        cout << "Overall Minimum: " << minOverall << endl;

        wait(NULL);  
    }

    return 0;
}
