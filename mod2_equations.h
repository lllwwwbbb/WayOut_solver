#ifndef MOD2_EQUATIONS_H
#define MOD2_EQUATIONS_H
#include <vector>
using namespace std;

typedef bool mod2_t;
class Mod2_Equations
{
public:
    Mod2_Equations(int n_val);
    void init(int n_val);
    void addEquation(const vector<mod2_t>& lhs, mod2_t rhs);
    vector<vector<mod2_t> > getResult();
private:
    int n_val;
    struct Equation {
        Equation(const vector<mod2_t>& lhs, mod2_t rhs) {
            this->lhs = lhs;
            this->rhs = rhs;
        }

        vector<mod2_t> lhs;
        mod2_t rhs;
    };
    vector<Equation> eqns;
    vector<int> fvs;

    void sortEqns();
    void simplify();
    void merge(Equation& to, const Equation &from);
    int cntSolution();
    vector<mod2_t> genResult(int n);
};

#endif // MOD2_EQUATIONS_H
