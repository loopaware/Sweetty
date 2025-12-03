#ifndef PTY_PROCESS_H
#define PTY_PROCESS_H

#include <string>
#include <vector>

// Forward declaration for TerminalBuffer if needed
class TerminalBuffer;

class PtyProcess {
public:
    PtyProcess(TerminalBuffer* buffer);
    ~PtyProcess();

    bool startShell(const std::string& shellPath);
    void write(const std::string& input);
    void resize(int cols, int rows);

    // Should be called continuously to read output from the shell
    void readOutput();

private:
    int masterFd_; // File descriptor for the master end of the PTY
    pid_t childPid_; // Process ID of the spawned shell
    TerminalBuffer* terminalBuffer_; // Pointer to the terminal buffer to write output to

    // No copying or assignment
    PtyProcess(const PtyProcess&) = delete;
    PtyProcess& operator=(const PtyProcess&) = delete;
};

#endif // PTY_PROCESS_H
