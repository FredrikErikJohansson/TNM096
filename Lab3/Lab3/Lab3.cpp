#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm>

struct Claus {
    std::vector<std::string> p;
    std::vector<std::string> n;
    bool tautology = false;

    bool operator==(const Claus& c) {
        return (this->p == c.p && this->n == c.n);
    }

    bool operator!=(const Claus& c) {
        return !(this->p == c.p && this->n == c.n);
    }

    void clear() {
        p.clear();
        n.clear();
    }

    void removeDuplicates() {
        sort(p.begin(), p.end());
        sort(n.begin(), n.end());
        p.erase(std::unique(p.begin(), p.end()), p.end());
        n.erase(std::unique(n.begin(), n.end()), n.end());
    }
};

bool compare(std::vector<Claus>& l, std::vector<Claus>& r) {
    if (l.size() != r.size()) return false;

    for (int i = 0; i < l.size(); i++) {
        if (l[i] != r[i]) {
            return false;
        }
    }
    return true;
}

template<typename S>
auto selectRandom(const S& s, size_t n) {
    auto it = std::begin(s);
    // 'advance' the iterator n times
    std::advance(it, n);
    return it;
}

Claus resolution(Claus A, Claus B) {
    Claus C;
    std::vector<std::string> ApBn;
    std::vector<std::string> AnBp;
    std::sort(A.p.begin(), A.p.end());
    std::sort(A.n.begin(), A.n.end());
    std::sort(B.p.begin(), B.p.end());
    std::sort(B.n.begin(), B.n.end());
    std::set_intersection(A.p.begin(), A.p.end(), B.n.begin(), B.n.end(), std::back_inserter(ApBn));
    std::set_intersection(A.n.begin(), A.n.end(), B.p.begin(), B.p.end(), std::back_inserter(AnBp));
    
    if (ApBn.empty() && AnBp.empty()) {
        C.tautology = true;
        return C; // False
    }

    if (!ApBn.empty()) {
        std::string a = *selectRandom(ApBn, rand() % ApBn.size());
        A.p.erase(std::remove(A.p.begin(), A.p.end(), a), A.p.end());
        B.n.erase(std::remove(B.n.begin(), B.n.end(), a), B.n.end());
    }
    else {
        std::string a = *selectRandom(AnBp, rand() % AnBp.size());
        A.n.erase(std::remove(A.n.begin(), A.n.end(), a), A.n.end());
        B.p.erase(std::remove(B.p.begin(), B.p.end(), a), B.p.end());
    }

    std::set_union(A.p.begin(), A.p.end(), B.p.begin(), B.p.end(), std::back_inserter(C.p));
    std::set_union(A.n.begin(), A.n.end(), B.n.begin(), B.n.end(), std::back_inserter(C.n));
    std::vector<std::string> CpCn;
    std::set_intersection(C.p.begin(), C.p.end(), C.n.begin(), C.n.end(), std::back_inserter(CpCn));

    if (!CpCn.empty()) {
        C.tautology = true;
        return C; // False
    }

    C.removeDuplicates();
    return C;
}


std::vector<Claus> incorporateClause(Claus A, std::vector<Claus> KB) {
    // Subset = if B intersection A = B
    //if B in KB such B.p subset of A.p and B.n subset of A.n
    std::sort(A.p.begin(), A.p.end());
    std::sort(A.n.begin(), A.n.end());

    for (auto B : KB) {
        Claus BA;
        std::sort(B.p.begin(), B.p.end());
        std::sort(B.n.begin(), B.n.end());
        std::set_intersection(B.p.begin(), B.p.end(), A.p.begin(), A.p.end(), std::back_inserter(BA.p));
        std::set_intersection(B.n.begin(), B.n.end(), A.n.begin(), A.n.end(), std::back_inserter(BA.n));

        if (BA.p == B.p && BA.n == B.n) {
            return KB;
        }
    }


;
    for (auto it = KB.begin(); it != KB.end();) {
        Claus AB;
        std::sort(it->p.begin(), it->p.end());
        std::sort(it->n.begin(), it->n.end());
        std::set_intersection(A.p.begin(), A.p.end(), it->p.begin(), it->p.end(), std::back_inserter(AB.p));
        std::set_intersection(A.n.begin(), A.n.end(), it->n.begin(), it->n.end(), std::back_inserter(AB.n));

        //TODO: Fix this
        if ((AB.p == A.p && AB.n == A.n) && (!AB.p.empty() || !AB.n.empty())) {
            it = KB.erase(it);
        }
        else {
            ++it;
        }
    }

    /*Claus KBA;
    for (auto KBi : KB) {
        std::set_union(KBi.p.begin(), KBi.p.end(), A.p.begin(), A.p.end(), std::back_inserter(KBA.p));
        std::set_union(KBi.n.begin(), KBi.n.end(), A.n.begin(), A.n.end(), std::back_inserter(KBA.n));
    }*/
    if (std::find(KB.begin(), KB.end(), A) == KB.end())
        KB.push_back(A);

    return KB;
}

std::vector<Claus> incorporate(std::vector<Claus> S, std::vector<Claus> KB) {
    for (auto A : S) {
        KB = incorporateClause(A, KB);
    }
    return KB;
}


std::vector<Claus> solver(std::vector<Claus> KB) {
    std::vector<Claus> S;
    std::vector<Claus> KBprime = KB;

    while(true) {
        S.clear();
        for (int i = 0; i < KB.size(); i++) {
            for (int j = 0; j < KB.size(); j++) {
                if (i == j) continue;
                Claus C = resolution(KB[i], KB[j]);
                if (!C.tautology) {
                    Claus SC;

                   /* for (auto Si : S) {
                        std::sort(C.p.begin(), C.p.end());
                        std::sort(C.n.begin(), C.n.end());
                        std::sort(Si.p.begin(), Si.p.end());
                        std::sort(Si.n.begin(), Si.n.end());
                        std::set_intersection(Si.p.begin(), Si.p.end(), C.p.begin(), C.p.end(), std::back_inserter(SC.p));
                        std::set_intersection(Si.n.begin(), Si.n.end(), C.n.begin(), C.n.end(), std::back_inserter(SC.n));
                    }*/
                    //if (S.empty()) SC = C;
                    //SC.removeDuplicates();
                    //if(std::find_if(S.begin(), S.end(), compare) == S.end())
                    if (std::find(S.begin(), S.end(), C) == S.end())
                        S.push_back(C);
                    //if(SC == C) S.push_back(C);
                }
            }
        }
        if (S.empty()) {
            return KB;
        }

        KB = incorporate(S, KB);

        if (compare(KBprime, KB)) {
            return KB;
        }
    }
}


int main()
{
    //Claus resolution(Claus A, Claus B)
    Claus A;
    Claus B;
    Claus C;
    Claus D;
    Claus E;
    A.p = { "ice" };
    A.n = { "sun", "money" };
    B.p = { "ice", "movie" };
    B.n = { "money" };
    C.p = { "money" };
    C.n = { "movie" };
    D.p = { };
    D.n = { "movie", "ice" };
    E.p = { "movie" };
    std::vector<Claus> KB;
    std::vector<Claus> result;
    KB.push_back(A);
    KB.push_back(B);
    KB.push_back(C);
    KB.push_back(D);
    KB.push_back(E);
    //Claus C = resolution(A, B);
    result = solver(KB);
    return 1;
}
