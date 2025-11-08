#include "cube.h"

void cube(
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  V.resize(8,3);
  F.resize(6,4);
  UV.resize(14,2);
  UF.resize(6,4);
  NV.resize(6,3);
  NF.resize(6,4);

  // 8 vertices of a unit cube
  V << 0, 0, 0,
       0, 1, 0,
       0, 1, 1,
       0, 0, 1,
       1, 0, 0,
       1, 1, 0,
       1, 1, 1,
       1, 0, 1;

  // 6 quad faces (counterclockwise winding)
  F << 3, 2, 1, 0,
       4, 5, 6, 7,
       0, 4, 7, 3,
       1, 2, 6, 5,
       0, 1, 5, 4,
       2, 3, 7, 6;

  // 6 face normals
  NV << -1, 0, 0,
        1, 0, 0,
        0, -1, 0,
        0, 1, 0,
        0, 0, -1,
        0, 0, 1;

  NF << 0, 0, 0, 0,
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4,
        5, 5, 5, 5;

  // 14 UV coordinates (cross unfolding pattern)
  UV << 1 - 0, 0.25,
        1 - 0, 0.5,
        1 - 0.25, 0.5,
        1 - 0.25, 0.75,
        1 - 0.5,  0.75,
        1 - 0.5, 0.5,
        1 - 0.75, 0.5,
        1 - 1, 0.5,
        1 - 1, 0.25,
        1 - 0.75, 0.25,
        1 - 0.5, 0.25,
        1 - 0.5, 0,
        1 - 0.25, 0,
        1 - 0.25, 0.25;

  UF << 13, 2, 1, 0,
        10, 5, 6, 9,
        13, 10, 11, 12,
        2, 3, 4, 5,
        9, 6, 7, 8,
        2, 5, 10, 13;
}
