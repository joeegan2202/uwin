#include "LaunchHelper.hpp"

#include <unistd.h>
#include <iostream>
#include <list>

void LauncherHelper::launch(std::string program_name, const std::vector<std::string> &arguments) {
  char **args = new char *[arguments.size()+1];

  std::list<char *> args_mem;
  int i = 0;
  for(auto s : arguments) {
    char *tmp = new char[s.size()+1];
    args_mem.push_back(tmp);
    s.copy(tmp, s.size());
    tmp[s.size()] = 0;
    args[i] = tmp;
    i++;
  }
  args[i] = nullptr;

	pid_t id = fork();
	if(id == 0) {
		execvp(program_name.c_str(), args);
		std::cerr << "Didn't exec!!!" << std::endl;
		return;
	}

  for(char *c : args_mem) {
    delete c;
  }
  delete[] args;
}
