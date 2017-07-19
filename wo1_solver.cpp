#include "wo1_solver.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QPixmap>
#include <cassert>

WO1_Solver::WO1_Solver(int h, int w):
    h(h), w(w)
{
    for (int i = 0; i < h; i ++) {
        color.push_back(vector<bool>(w, false));
        state.push_back(vector<BlockType>(w, EMPTY));
    }
    bt2res.insert({
                      {RAW,        ":/img/rawblock"},
                      {H_DIR,      ":/img/hdblock"},
                      {V_DIR,      ":/img/vdblock"},
                      {OUT,        ":/img/outblock"},
                      {ADD,        ":/img/addblock"},
                      {YELLOW,     ":/img/ylblock"},
                      {DPOINT,     ":/img/dpblock"},
                  });
    ct2res.insert({
                      {ClickType::SINGLE,   ":/img/click"},
                      {ClickType::DOUBLE,   ":/img/dclick"},
                      {ClickType::DBFIRST,  ":/img/dfclick"},
                      {ClickType::FIRST,    ":/img/fclick"},
                  });
}

void WO1_Solver::setBlock(BlockType bt, int x, int y)
{
    if (x < 0 || x >= w || y < 0 || y >= h) {
        return;
    }
    enableSolution(false);
    if (bt != COLOR) {
        state[y][x] = bt;
        color[y][x] = false;
    }
    else if (state[y][x] != EMPTY){
        color[y][x] = true;
    }
}

void WO1_Solver::enableSolution(bool ss)
{
    if (!this->ss && ss) {
        getSolution();
        this->ss = ss;
    }
    else if (this->ss && !ss) {
        sltn.clear();
        pos2i.clear();
        i2pos.clear();
        this->ss = ss;
    }
}

void WO1_Solver::paintState(QPainter &p)
{
    for (int r = 0; r < h; r ++) {
        for (int c = 0; c < w; c ++) {
            if (state[r][c] != EMPTY) {
                QString res = bt2res[state[r][c]];
                if (color[r][c]) {
                    res += "_color";
                }
                int dx = c * B_LEN + PAD, dy = r * B_LEN + PAD;
                p.drawPixmap(dx, dy, B_LEN -  2 * PAD, B_LEN - 2 * PAD, QPixmap(res));
            }
        }
    }
    for (int i = 0; i < sltn.size(); i ++) {
        QString cres = ct2res[sltn[i].ct];
        int dx = sltn[i].pos.x * B_LEN + PAD, dy = sltn[i].pos.y * B_LEN + PAD;
        p.drawPixmap(dx, dy, B_LEN -  2 * PAD, B_LEN - 2 * PAD, QPixmap(cres));
    }
}

void WO1_Solver::getSolution()
{
    int idx = 0;
    bool dpc;
    bool dp_diff = false;
    for (Position p(0, 0); p.y < h; p.y ++) {
        for (p.x = 0; p.x < w; p.x ++) {
            if (state[p.y][p.x] != EMPTY) {
                pos2i[p] = idx;
                i2pos[idx] = p;
                if (state[p.y][p.x] == DPOINT) {
                    if (dp_blocks.empty()) {
                        dpc = color[p.y][p.x];
                    }
                    else if (dpc != color[p.y][p.x]) {
                        dp_diff = true;
                    }
                    dp_blocks.push_back(p);
                }
                idx ++;
            }
        }
    }
    auto res = getResult(dpc);
    if (dp_diff) {
        checkDPoint(res, dpc);
        auto res2 = getResult(!dpc);
        checkDPoint(res2, !dpc);
        res.insert(res.end(), res2.begin(), res2.end());
    }
    checkYellow(res);
    dp_blocks.clear();
    if (res.size() == 0) {
        QMessageBox::information(NULL, "无解","没有找到解决方案",QMessageBox::Yes);
        return;
    }
    auto f_cnt = [res](int i) -> int {
        return count_if(res[i].begin(), res[i].end(), [](ClickPos m) -> bool {
            return m.ct == ClickType::DOUBLE || m.ct == ClickType::DBFIRST;
        }) + res[i].size();
    };
    int mi = 0, min = f_cnt(0);
    for (int i = 1; i < res.size(); i ++) {
        int t = f_cnt(i);
        if (t < min) {
            min = t;
            mi = i;
        }
    }
    sltn = res[mi];
}

vector<vector<ClickPos> > WO1_Solver::getResult(bool dpc)
{
    Mod2_Equations m2e(pos2i.size());
    for (auto it = pos2i.begin(); it != pos2i.end(); it ++) {
        vector<bool> lhs(pos2i.size(), false);
        Position pos = it->first;
        set<Position> v;
        getNH(pos, v);
        for (auto vit = v.begin(); vit != v.end(); vit ++) {
            lhs[pos2i[Position(vit->x, vit->y)]] = true;
        }
        mod2_t rhs = state[pos.y][pos.x] == DPOINT ? dpc : color[pos.y][pos.x];
        rhs = state[pos.y][pos.x] == YELLOW ? !rhs : rhs;
        m2e.addEquation(lhs, rhs);
    }
    auto er = m2e.getResult();
    vector<vector<ClickPos> > cp;
    for (auto r : er) {
        cp.push_back(vector<ClickPos>());
        for (int i = 0; i < r.size(); i ++) {
            if (r[i]) {
                cp.back().push_back(i2pos[i]);
            }
        }
    }
    return cp;
}

void WO1_Solver::checkYellow(vector<vector<ClickPos> > &res)
{
    auto r = res.begin();
    while (r != res.end()) {
        auto ok = fixYellow(*r);
        if (!ok) {
            r = res.erase(r);
            continue;
        }
        r ++;
    }
}

vector<Position> WO1_Solver::getAloneYl(const vector<ClickPos> &r)
{
    vector<Position> alone;// 找出未消除的黄色块
    for (auto pi : pos2i) {
        if (state[pi.first.y][pi.first.x] == YELLOW) {
            alone.push_back(pi.first);
        }
    }
    auto it = alone.begin();
    while (it != alone.end()) {
        set<Position> ps;
        getNH(*it, ps);
        bool ok = false;
        for (auto nh : ps) {
            if (find(r.begin(), r.end(), nh) != r.end()
                    && find(alone.begin(), alone.end(), nh) == alone.end()) {
                ok = true;
                break;
            }
        }
        if (ok) {
            alone.erase(it);
            it = alone.begin();
            continue;
        }
        it ++;
    }
    return alone;
}

set<Position> WO1_Solver::getAloneYlNH(const vector<ClickPos> &r)
{
    auto alone = getAloneYl(r);
    set<Position> ps;
    for (auto al : alone) {
        set<Position> ts;
        getNH(al, ts);
        for (auto p : ts) {
            auto it = ps.find(p);
            if (it != ps.end()) {
                // 用于在 fixYellow 中排序进行优化剪枝
                it->val ++;
            }
            else {
                ps.insert(p);
            }
        }
    }
    for (auto al : alone) {
        ps.erase(find(ps.begin(), ps.end(), al));
    }
    return ps;
}

bool WO1_Solver::fixYellow(vector<ClickPos> &r, int max)
{   // 暴力法，利用剪枝优化
    if (getAloneYl(r).empty()) {
        return true;
    }
    auto ps = getAloneYlNH(r);
    if (ps.empty()) {
        return false;
    }
    if (r.size() >= max) {
        r.resize(max + 1);
        return true;
    }
    int min = max + 1;
    vector<ClickPos> mr(max + 1);
    vector<Position> pv(ps.size());
    copy(ps.begin(), ps.end(), pv.begin());
    sort(pv.begin(), pv.end(), [](Position& a, Position& b) -> bool {
       return a.val > b.val;
    });
    for (auto nh : pv) {
        auto tr = r;
        tr.push_back(ClickPos(nh, ClickType::DOUBLE));
        if (!fixYellow(tr, min - 1)) {
            return false;
        }
        if (tr.size() < min) {
            min = tr.size();
            mr = tr;
        }
        if (tr.size() == r.size() + 1) {
            break;
        }
    }
    r = mr;
    return true;
}

void WO1_Solver::checkDPoint(vector<vector<ClickPos> > &res, bool dpc)
{
    for (auto r = res.begin(); r != res.end(); r ++) {
        bool ok = false;
        Position p_dp;
        for (auto pi : pos2i) {
            Position p = pi.first;
            if (state[p.y][p.x] != DPOINT || color[p.y][p.x] != dpc) {
                continue;
            }
            p_dp = p;
            vector<ClickPos>::iterator it;
            if ((it = find(r->begin(), r->end(), p)) != r->end()) {
                ok = true;
                it->ct = ClickType::FIRST;
                goto INNER_BREAK;
            }
            set<Position> ps;
            getNH(p, ps, RAW);
            for (auto nh: ps) {
                if (state[nh.y][nh.x] != DPOINT
                        && (it = find(r->begin(), r->end(), nh)) != r->end()) {
                    ok = true;
                    it->ct = ClickType::FIRST;
                    goto INNER_BREAK;
                }
            }
        }
        INNER_BREAK:
        if (!ok) {
            r->push_back(ClickPos(p_dp, ClickType::DBFIRST));
        }
    }
}

void WO1_Solver::getNH(const Position &pos, set<Position>& ps)
{
    getNH(pos, ps, state[pos.y][pos.x]);
}

void WO1_Solver::getNH(const Position &pos, set<Position> &ps, BlockType bt)
{
    ps.insert(pos);
    if (bt == OUT) {
        return;
    }
    Position arr[] = {
        {pos.x - 1, pos.y},
        {pos.x + 1, pos.y},
        {pos.x, pos.y - 1},
        {pos.x, pos.y + 1},
    };
    for (auto ap : arr) {
        if (ap.y >= 0 && ap.y < h
                && ap.x >= 0 && ap.x < w) {
            switch (state[ap.y][ap.x]) {
            // add new blockType here:
            case RAW:
            case OUT:
            case DPOINT:
            case YELLOW:
                ps.insert(ap);
                break;
            case H_DIR:
                if (ap.y == pos.y) {
                    ps.insert(ap);
                }
                break;
            case V_DIR:
                if (ap.x == pos.x) {
                    ps.insert(ap);
                }
                break;
            case ADD:
                if (ps.insert(ap).second) {
                    getNH(ap, ps);
                }
                break;
            default:
                assert(state[ap.y][ap.x] == EMPTY);
                break;
            }
        }
    }
    if (bt == DPOINT) {
        for (auto it : dp_blocks) {
            if (ps.insert(it).second) {
                getNH(it, ps);
            }
        }
    }
}
