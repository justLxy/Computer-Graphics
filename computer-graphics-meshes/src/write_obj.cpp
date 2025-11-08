#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
  const std::string & filename,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & UV,
  const Eigen::MatrixXi & UF,
  const Eigen::MatrixXd & NV,
  const Eigen::MatrixXi & NF)
{
  assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");
  
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }
  
  // Write vertices
  for (int i = 0; i < V.rows(); i++) {
    file << "v " << V(i, 0) << " " << V(i, 1) << " " << V(i, 2) << std::endl;
  }
  
  // Write UV coordinates
  for (int i = 0; i < UV.rows(); i++) {
    file << "vt " << UV(i, 0) << " " << UV(i, 1) << std::endl;
  }
  
  // Write normals
  for (int i = 0; i < NV.rows(); i++) {
    file << "vn " << NV(i, 0) << " " << NV(i, 1) << " " << NV(i, 2) << std::endl;
  }
  
  // Write faces (OBJ indices start at 1)
  for (int i = 0; i < F.rows(); i++) {
    file << "f";
    for (int j = 0; j < F.cols(); j++) {
      file << " " << (F(i, j) + 1);
      
      if (UF.size() > 0) {
        file << "/" << (UF(i, j) + 1);
      }
      
      if (NF.size() > 0) {
        if (UF.size() == 0) {
          file << "/";
        }
        file << "/" << (NF(i, j) + 1);
      }
    }
    file << std::endl;
  }
  
  file.close();
  return true;
}
