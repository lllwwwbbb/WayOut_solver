#include "mod2_equations.h"
#include <cassert>
#include <algorithm>
using namespace std;

Mod2_Equations::Mod2_Equations(int n_val)
{
    init(n_val);
}

void Mod2_Equations::init(int n_val)
{
    this->n_val = n_val;
    eqns.clear();
    fvs.clear();
}

void Mod2_Equations::addEquation(const vector<mod2_t> &lhs, mod2_t rhs)
{
    assert(lhs.size() == n_val);
    eqns.push_back(Equation(lhs, rhs));
}

vector<vector<mod2_t> > Mod2_Equations::getResult()
{
    simplify();
    vector<vector<mod2_t> > res;
    int n_sln = cntSolution();
    if (n_sln == 0) {
        return res;
    }
    for (int i = 0; i < n_sln; i ++) {
        res.push_back(genResult(i));
    }
    return res;
}

void Mod2_Equations::sortEqns()
{
    // 永远让比较函数对相等的值返回false(来自Effective C++)
    // 否则可能死循环或者越界触发段错误
    sort(eqns.begin(), eqns.end(), [](Equation& a, Equation& b) -> bool {
       bool less = false;
       for (int i = 0; i < a.lhs.size(); i ++) {
           if (a.lhs[i] == 1) {
               if (b.lhs[i] == 1) {
                   less = false;
               }
               else{
                   less = true;
               }
               break;
           }
           if (b.lhs[i] == 1) {
               less = false;
               break;
           }
       }
       return less;
    });
}

void Mod2_Equations::simplify()
{
    int idx = 0;
    for (int i = 0; i < eqns.size() && idx < n_val; idx ++, i ++) {
        sortEqns();
        if (eqns[i].lhs[idx] == 0) {
            i --;
            continue;
        }
        for (int k = 0; k < eqns.size(); k ++) {
            if (k != i && eqns[k].lhs[idx] == 1) {
                merge(eqns[k], eqns[i]);
            }
        }
    }
}

void Mod2_Equations::merge(Mod2_Equations::Equation &to, const Mod2_Equations::Equation &from)
{
    to.rhs ^= from.rhs;
    for (int i = 0; i < to.lhs.size(); i ++) {
        to.lhs[i] = to.lhs[i] ^ from.lhs[i];
    }
}

int Mod2_Equations::cntSolution()
{
    vector<bool> stricted;
    stricted.resize(n_val, false);
    for (int i = 0; i < eqns.size(); i ++) {
        int cnt = 0;
        for (int k = 0; k < n_val; k ++) {
            if (eqns[i].lhs[k] != 0) {
                if (cnt == 0) {
                    stricted[k] = true;
                }
                cnt ++;
            }
        }
        if (cnt == 0) {
            if (eqns[i].rhs != 0) {
                return 0;
            }
            else {
                eqns.erase(eqns.begin() + i);
                i --;
                continue;
            }
        }
    }
    int cnt = 1;
    for (int i = 0; i < stricted.size(); i ++) {
        if (stricted[i] == false) {
            fvs.push_back(i);
            cnt *= 2;
        }
    }
    return cnt;
}

vector<mod2_t> Mod2_Equations::genResult(int n)
{
    auto t_eqns = eqns;
    vector<mod2_t> res;
    res.resize(n_val);
    vector<int> fvs_val;
    int t_n = n;
    for (int i = 0; i < fvs.size(); i ++) {
        fvs_val.push_back(t_n % 2);
        t_n /= 2;
        res[fvs[i]] = fvs_val[i];
    }
    for (int i = 0; i < t_eqns.size(); i ++) {
        for (int k = 0; k < fvs.size(); k ++) {
            int idx = fvs[k];
            if (t_eqns[i].lhs[idx] == 1) {
                t_eqns[i].lhs[idx] = 0;
                t_eqns[i].rhs ^= fvs_val[k];
            }
        }
    }
    int idx = 0;
    for (int i = 0; i < t_eqns.size(); idx ++, i ++) {
        if (t_eqns[i].lhs[idx] == 1) {
            res[idx] = t_eqns[i].rhs;
        }
        else {
            i --;
            continue;
        }
    }
    return res;
}

