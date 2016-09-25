#include "flow2color.h"
#include <iostream>


void PrintHelp() {
  std::cout << "Usage: flow2color <flow_x.txt> <flow_y.txt> <output.png>"
            << std::endl;
}


int main(int argc, char **argv) {
  if (argc < 3) {
    PrintHelp();
    return 0;
  }

  if (argc == 3) {
    std::string flowFile(argv[1]);
    std::string outputFileName(argv[2]);

    Flow2Color converter(flowFile);
    converter.Convert();
    converter.WriteToFile(outputFileName);
  } else {
    std::string flowXFileName(argv[1]);
    std::string flowYFileName(argv[2]);
    std::string outputFileName(argv[3]);

    Flow2Color converter(flowXFileName, flowYFileName);
    converter.Convert();
    converter.WriteToFile(outputFileName);
  }

  return 0;
}