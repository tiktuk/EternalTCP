#include "ConsoleUtils.hpp"

namespace et {
std::string commandToString(const char* cmd) {
  char buffer[128];
  std::string result = "";
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get())) {
    if (fgets(buffer, 128, pipe.get()) != NULL) result += buffer;
  }
  return result;
}

std::string getTerminal() {
#if __APPLE__
  string terminal = commandToString(
      "dscl /Search -read \"/Users/$USER\" UserShell | awk '{print $2}'");
  for (int a = 0; a < terminal.length(); a++) {
    if (terminal[a] == '\n') {
      return terminal.substr(0, a);
    }
  }
  throw runtime_error("Oops");
#else
  string terminal =
      commandToString("grep ^$(id -un): /etc/passwd | cut -d : -f 7-");
  for (int a = 0; a < (int)terminal.length(); a++) {
    if (terminal[a] == '\n') {
      return terminal.substr(0, a);
    }
  }

  // If we didn't find the terminal that way, let's try the environment variable
  terminal = ::getenv("SHELL");
  if (terminal.length() > 0) {
    return terminal;
  }

  LOG(FATAL) << "Could not find terminal";
  return "";
#endif
}
}
