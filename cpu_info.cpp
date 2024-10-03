#include "utils.h"
#include <fstream>
#include <string>
#include <sstream>

std::string get_cpu_info() {
    std::ifstream cpuinfo_file("/proc/cpuinfo");
    std::string line, cpu_model;

    if (cpuinfo_file.is_open()) {
        while (std::getline(cpuinfo_file, line)) {
            if (line.find("model name") != std::string::npos) {
                std::stringstream ss(line);
                std::string key, value;
                std::getline(ss, key, ':');
                std::getline(ss, value);
                cpu_model = trim(value);
                break;
            }
        }
        cpuinfo_file.close();
    } else {
        cpu_model = "Unable to get CPU info.";
    }

    return sanitize_string(cpu_model);  // Sanitize the string to remove invalid characters
}

// Function to get the correct CPU temperature using the 'sensors' command
std::string get_cpu_temp() {
    const char* cmd = "sensors";  // Command to get system sensor info
    char buffer[256];
    std::string result;

    // Use popen to execute the command and capture the output
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        return "Unable to get CPU temperature";
    }

    // Read the output of the sensors command
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);

    // Parse the output to find the Tctl temperature in k10temp-pci-00c3 section
    std::istringstream ss(result);
    std::string line;
    bool in_k10temp_section = false;

    while (std::getline(ss, line)) {
        // Check if we're in the k10temp section
        if (line.find("k10temp-pci-00c3") != std::string::npos) {
            in_k10temp_section = true;
        }

        // Once in k10temp section, find the "Tctl" line
        if (in_k10temp_section && line.find("Tctl:") != std::string::npos) {
            size_t pos = line.find("+");
            if (pos != std::string::npos) {
                // Extract the temperature value
                std::string temp_str = trim(line.substr(pos + 1, 5)); // Extract temperature value
                return sanitize_string(temp_str + "°C");  // Sanitize the string and return temperature
            }
        }
    }

    return "Unable to find Tctl temperature";
}

