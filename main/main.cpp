#include "ImageDatabase.h"

int main() {
  Matrix<int> m(10, 10, 2);
  ImageDatabase<int, int> db;
  Matrix<int> m2 = m;
  m.Fill(5);
  m2.Fill(10);
  return db.getImageCount();
}
