#ifndef WO1_SOLVER_H
#define WO1_SOLVER_H
#include <vector>
#include <set>
#include <map>
#include <QPainter>
#include "common.h"
#include "mod2_equations.h"
using namespace std;

struct Position {
    Position(int x = -1, int y = -1, int val = 0):
        x(x), y(y), val(val)
    {}
    bool operator < (const Position& other) const {
        return x < other.x ||
                (x == other.x && y < other.y);
    }
    bool operator == (const Position& other) const {
        return (x == other.x && y == other.y);
    }
    bool operator != (const Position& other) const {
        return !(*this == other);
    }

    int x, y;
    mutable int val;
};
enum class ClickType { SINGLE, DOUBLE, DBFIRST, FIRST };
struct ClickPos {
    ClickPos(const Position& p = Position(), const ClickType c = ClickType::SINGLE):
        pos(p), ct(c)
    {}
    bool operator == (const Position& p) const {
        return pos == p;
    }

    Position pos;
    ClickType ct;
};

class WO1_Solver
{
public:
    WO1_Solver(int h, int w);
    void setBlock(BlockType bt, int x, int y);
    void enableSolution(bool ss);
    void paintState(QPainter& p);
private:
    int h, w;
    vector<vector<BlockType> > state;
    vector<vector<bool> > color;
    bool ss;
    vector<ClickPos> sltn;
    map<Position, int> pos2i;
    map<int, Position> i2pos;
    vector<Position> dp_blocks;
    map<BlockType, QString> bt2res;
    map<ClickType, QString> ct2res;

    void getSolution();
    vector<vector<ClickPos> > getResult(bool dpc);
    void checkYellow(vector<vector<ClickPos> >& res);
    vector<Position> getAloneYl(const vector<ClickPos>& r);
    set<Position> getAloneYlNH(const vector<ClickPos>& r);
    bool fixYellow(vector<ClickPos> &r, int max = B_WID * B_HGT);
    void checkDPoint(vector<vector<ClickPos> >& res, bool dpc);
    void getNH(const Position& pos, set<Position> &ps);
    void getNH(const Position& pos, set<Position> &ps, BlockType bt);
};

#endif // WO1_SOLVER_H
