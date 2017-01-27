#include "all_headers.h"

std::string MainWindow::exec(std::string string_cmd)
{
    char* cmd = new char[string_cmd.size() + 1];
          memcpy(cmd, string_cmd.c_str(), string_cmd.size() + 1);//convert command to char*
    FILE* pipe = popen(cmd, "r");//execute command
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;//return data output of command
}


std::string read_batt::exec(std::string string_cmd)
{
    char* cmd = new char[string_cmd.size() + 1];
          memcpy(cmd, string_cmd.c_str(), string_cmd.size() + 1);//convert command to char*
    FILE* pipe = popen(cmd, "r");//execute command
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;//return data output of command

}

