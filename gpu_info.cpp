#include "gpu_info.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "gpu_info.h"
#include <cstdio>
#include <sstream>
#include <vector>
#include <string>
#include "utils.h"

std::string get_gpu_info() {
    std::string gpu_info;
    char buffer[128];
    std::vector<std::string> gpus;
    FILE* pipe = popen("lspci | grep VGA", "r");

    if (!pipe) {
        return "Unable to get GPU info.";
    }

    // Read each line of the lspci output and store it in the vector
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        gpus.push_back(buffer);
    }
    pclose(pipe);

    // Check if any GPUs were found
    if (gpus.empty()) {
        return "No GPU detected.";
    }

    // Create a readable result, labeling each GPU (e.g., "GPU 1", "GPU 2")
    std::stringstream result;
    for (size_t i = 0; i < gpus.size(); ++i) {
        result << "GPU " << (i + 1) << ": " << gpus[i];
    }

    return result.str();
}

std::string get_gpu_temp() {
    const char* cmd = "sensors";  // Command to get system sensor info
    char buffer[256];
    std::string result;

    // Use popen to execute the command and capture the output
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        return "Unable to get GPU temperature";
    }

    // Read the output of the sensors command
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);

    // Parse the output to find the GPU temperature in amdgpu-pci section
    std::istringstream ss(result);
    std::string line;
    bool in_amdgpu_section = false;

    while (std::getline(ss, line)) {
        // Check if we're in the amdgpu-pci section
        if (line.find("amdgpu-pci") != std::string::npos) {
            in_amdgpu_section = true;
        }

        // Once in the amdgpu-pci section, find the "edge" temperature line
        if (in_amdgpu_section && line.find("edge:") != std::string::npos) {
            size_t pos = line.find("+");
            if (pos != std::string::npos) {
                // Extract the temperature value
                std::string temp_str = trim(line.substr(pos + 1, 5)); // Extract temperature value
                return sanitize_string(temp_str + "°C");  // Sanitize the string and return temperature
            }
        }
    }

    return "Unable to find GPU temperature";
}

