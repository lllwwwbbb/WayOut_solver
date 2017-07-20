#ifndef COMMON_H
#define COMMON_H

//#define VERSION2

const int B_WID = 30, B_HGT = 30, B_LEN = 65, BAR_HGT = 40, PAD = 3;
const int SCR_WID = B_WID * B_LEN, SCR_HGT = B_HGT * B_LEN;
enum BlockType {
    EMPTY, COLOR, RAW, V_DIR, H_DIR, OUT, ADD, YELLOW, DPOINT,
    LU_DIR, RU_DIR,
};

#endif // COMMON_H
