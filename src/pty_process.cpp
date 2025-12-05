#include "pty_process.h"
#include "terminal_buffer.h" // For writing output to the terminal buffer
#include "ansi_parser.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

PtyProcess::PtyProcess(TerminalBuffer* buffer)
    : masterFd_(-1), childPid_(-1), terminalBuffer_(buffer)
{
    ansiParser_ = std::make_unique<AnsiParser>(terminalBuffer_);
}

PtyProcess::~PtyProcess()
{
    if (masterFd_ != -1) {
        close(masterFd_);
    }
    if (childPid_ != -1) {
        // Send SIGHUP to the child process group to terminate it
        killpg(childPid_, SIGHUP);
        waitpid(childPid_, nullptr, 0); // Optionally wait for child to exit
    }
}

bool PtyProcess::startShell(const std::string& shellPath)
{
    // Open a new pseudo-terminal
    masterFd_ = posix_openpt(O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (masterFd_ == -1) {
        std::cerr << "Error: posix_openpt failed: " << strerror(errno) << std::endl;
        return false;
    }

    if (grantpt(masterFd_) == -1) {
        std::cerr << "Error: grantpt failed: " << strerror(errno) << std::endl;
        close(masterFd_);
        masterFd_ = -1;
        return false;
    }

    if (unlockpt(masterFd_) == -1) {
        std::cerr << "Error: unlockpt failed: " << strerror(errno) << std::endl;
        close(masterFd_);
        masterFd_ = -1;
        return false;
    }

    char* slaveName = ptsname(masterFd_);
    if (slaveName == nullptr) {
        std::cerr << "Error: ptsname failed: " << strerror(errno) << std::endl;
        close(masterFd_);
        masterFd_ = -1;
        return false;
    }

    childPid_ = fork();
    if (childPid_ == -1) {
        std::cerr << "Error: fork failed: " << strerror(errno) << std::endl;
        close(masterFd_);
        masterFd_ = -1;
        return false;
    }

    if (childPid_ == 0) { // Child process
        // Disconnect from controlling terminal
        setsid();
        // Open the slave PTY
        int slaveFd = open(slaveName, O_RDWR | O_NOCTTY);
        if (slaveFd == -1) {
            std::cerr << "Error: open slave pty failed in child: " << strerror(errno) << std::endl;
            _exit(1);
        }

        // Make the slave PTY the controlling terminal
        ioctl(slaveFd, TIOCSCTTY, 0);

        // Duplicate stdin, stdout, stderr to the slave PTY
        dup2(slaveFd, STDIN_FILENO);
        dup2(slaveFd, STDOUT_FILENO);
        dup2(slaveFd, STDERR_FILENO);

        // Close all unused file descriptors
        if (masterFd_ != -1) {
            close(masterFd_);
        }
        if (slaveFd != -1) {
            close(slaveFd);
        }

        // Set up terminal settings (optional, but good practice)
        struct termios slave_orig_term_settings; // Save original terminal settings
        struct termios new_term_settings;       // New terminal settings for the child
        tcgetattr(slaveFd, &slave_orig_term_settings);
        new_term_settings = slave_orig_term_settings;
        cfmakeraw(&new_term_settings); // Set raw mode
        tcsetattr(slaveFd, TCSANOW, &new_term_settings);

        // Execute the shell with output unbuffered
        execlp("/usr/bin/stdbuf", "stdbuf", "-o0", shellPath.c_str(), (char *)0);

        // If execlp returns, an error occurred
        std::cerr << "Error: execlp failed: " << strerror(errno) << std::endl;
        _exit(1);
    } else { // Parent process
        // Set up terminal settings for the master PTY (raw mode)
        struct termios tty_settings;
        tcgetattr(masterFd_, &tty_settings);
        cfmakeraw(&tty_settings);
        tcsetattr(masterFd_, TCSANOW, &tty_settings);

        std::cout << "Shell started with PID: " << childPid_ << std::endl;
        return true;
    }
}

void PtyProcess::write(const std::string& input)
{
    if (masterFd_ != -1) {
        ssize_t bytesWritten = ::write(masterFd_, input.c_str(), input.length());
        if (bytesWritten == -1) {
            std::cerr << "Error writing to PTY: " << strerror(errno) << std::endl;
        }
    }
}

void PtyProcess::resize(int cols, int rows)
{
    if (masterFd_ != -1) {
        struct winsize ws;
        ws.ws_col = cols;
        ws.ws_row = rows;
        ws.ws_xpixel = 0; // Not used currently
        ws.ws_ypixel = 0; // Not used currently
        if (ioctl(masterFd_, TIOCSWINSZ, &ws) == -1) {
            std::cerr << "Error setting PTY window size: " << strerror(errno) << std::endl;
        }
    }
}

void PtyProcess::readOutput()
{
    if (masterFd_ == -1 || terminalBuffer_ == nullptr) {
        return;
    }

    char buffer[256];
    ssize_t bytesRead = ::read(masterFd_, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        ansiParser_->parse(buffer); // Write to the terminal buffer
    } else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        // No data available, non-blocking read
    } else if (bytesRead == 0) {
        // End of file, PTY closed by child (shell exited)
        std::cerr << "PTY master closed by child. Shell exited." << std::endl;
        // TODO: Handle shell exit (e.g., notify main application)
    } else if (bytesRead == -1) {
        std::cerr << "Error reading from PTY: " << strerror(errno) << std::endl;
    }
}
