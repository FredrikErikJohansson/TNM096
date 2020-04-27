#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm>

struct Claus {
    std::vector<std::string> p;
    std::vector<std::string> n;
    bool tautology = false;

    bool operator==(const Claus& c) {
        return (p == c.p && n == c.n);
    }

    bool operator!=(const Claus& c) {
        return !(p == c.p && n == c.n);
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

    void print() {
        for (int i = 0; i < p.size(); i++) {
            std::cout << p[i] << " ";
            if (!n.empty()) std::cout << "v ";
        }
        for (int i = 0; i < n.size(); i++) {
            std::cout << "-" << n[i] << " ";
            if (i != n.size() - 1) std::cout << "v ";
        }
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

std::vector<std::string> setIntersection(std::vector<std::string>& A, std::vector<std::string>& B) {
    std::vector<std::string> C;
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(), std::back_inserter(C));
    return C;
}

std::vector<std::string> setUnion(std::vector<std::string>& A, std::vector<std::string>& B) {
    std::vector<std::string> C;
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    std::set_union(A.begin(), A.end(), B.begin(), B.end(), std::back_inserter(C));
    return C;
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
    std::vector<std::string> ApBn = setIntersection(A.p, B.n);
    std::vector<std::string> AnBp = setIntersection(A.n, B.p);
    
    if (ApBn.empty() && AnBp.empty()) {
        C.tautology = true;
        return C; // Tautology
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

    C.p = setUnion(A.p, B.p);
    C.n = setUnion(A.n, B.n);
    std::vector<std::string> CpCn = setIntersection(C.p, C.n);

    if (!CpCn.empty()) {
        C.tautology = true;
        return C; // Tautology
    }

    C.removeDuplicates();
    return C;
}

std::vector<Claus> incorporateClause(Claus A, std::vector<Claus> KB) {
    for (auto B : KB) {
        Claus BA;
        BA.p = setIntersection(B.p, A.p);
        BA.n = setIntersection(B.n, A.n);

        if (BA.p == B.p && BA.n == B.n) {
            return KB;
        }
    }

    for (auto it = KB.begin(); it != KB.end();) {
        Claus AB;
        AB.p = setIntersection(A.p, it->p);
        AB.n = setIntersection(A.n, it->n);

        size_t orgP = it->p.size();
        size_t orgN = it->n.size();
        size_t size1 = AB.p.size();
        size_t size2 = AB.n.size();

        if ((AB.p == A.p && AB.n == A.n) && (size1 < orgP || size2 < orgN)) {
            it = KB.erase(it);
        }
        else {
            it++;
        }
    }

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
    std::vector<Claus> KBprime;

    while(true) {
        S.clear();
        KBprime = KB;
        for (int i = 0; i < KB.size(); i++) {
            for (int j = 0; j < KB.size(); j++) {
                if (i == j) continue;
                Claus C = resolution(KB[i], KB[j]);
                if (!C.tautology) {
                    if (std::find(S.begin(), S.end(), C) == S.end())
                        S.push_back(C);
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

int main() {
    Claus A;
    Claus B;
    Claus C;
    Claus D;
    Claus E;

    //TASK A:
    //A.p = { "ice" };
    //A.n = { "sun", "money" };
    //B.p = { "ice", "movie" };
    //B.n = { "money" };
    //C.p = { "money" };
    //C.n = { "movie" };
    //D.p = {};
    //D.n = { "movie", "ice" };
    //E.p = { "movie" };
    //E.n = {};

    //TASK B:
    //(B => C) &
    //(B => A) &
    //(C <=> A) &
    //(A | B | C)
   
    //(-B v C) &
    //(-B v A) &
    //(-C v A) &
    //(C v -A) &
    //(A v B v C)

    A.p = { "C" };
    A.n = { "B" };
    B.p = { "A" };
    B.n = { "B" };
    C.p = { "A" };
    C.n = { "C" };
    D.p = { "C" };
    D.n = { "A" };
    E.p = { "A", "B", "C" };
    E.n = {  };

    std::vector<Claus> KB;
    KB.push_back(A);
    KB.push_back(B);
    KB.push_back(C);
    KB.push_back(D);
    KB.push_back(E);
    KB = solver(KB);
    std::cout << "KB = { ";
    for (int i = 0; i < KB.size(); i++) {
        KB[i].print();
        if(i != KB.size() - 1) std::cout << ", ";
    }     
    std::cout << "}" << std::endl;
    return 1;
}
