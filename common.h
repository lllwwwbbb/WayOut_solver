#ifndef COMMON_H
#define COMMON_H

const int B_WID = 20, B_HGT = 20, B_LEN = 65, BAR_HGT = 35, PAD = 3;
const int SCR_WID = B_WID * B_LEN, SCR_HGT = B_HGT * B_LEN;
enum BlockType {
   EMPTY, COLOR, RAW, V_DIR, H_DIR, OUT, ADD, YELLOW, DPOINT
};

#endif // COMMON_H
