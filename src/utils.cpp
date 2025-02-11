#include "utils.hpp"
#include <fstream>
#include <istream>
#include <stdexcept>
#include <string>
#include <unistd.h>

auto Utils::getProcArgs(const pid_t pid) -> std::string {
    std::string pid_s = std::to_string(pid);
    std::ifstream proc_args("/proc/" + pid_s + "/cmdline");

    if (proc_args.is_open()) {
        std::string info;
        std::getline(proc_args, info);
        return info;
    } else
        throw std::runtime_error("Invalid PID: " + pid_s);
}

auto Utils::truncateFile(std::ofstream& file, const fs::path& filepath) -> void {
    file.close();
    file.open(filepath, std::iostream::trunc);
    if (!file.is_open())
        throw std::runtime_error("[ERR] Couldn't open the file.\n");
}

auto Utils::execCommand(const std::string& command) -> std::string {
    FILE* pipe = popen(command.c_str(), "r");

    if (!pipe) {
        std::cerr << "[ERR] Failed to execute command: " << command << ".\n";
        return {};
    }

    char buffer[128];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    return result;
}
