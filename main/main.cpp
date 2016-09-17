#include "ImageDatabase.h"

int main() {
  ImageDatabase<double, int> db;
  db.ReadFilesFromList("../test/test_data/DatabaseFiles.txt");

  return 0;
}
