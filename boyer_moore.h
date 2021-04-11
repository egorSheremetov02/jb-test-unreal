#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H
#include <vector>
#include <string>
#include <algorithm>


struct BoyerMoore {
private:
    std::string s;
    std::vector<int> zeta_function;
    std::vector<std::vector<int>> pos;
    std::vector<int> shift;

    void precalc();

public:

    BoyerMoore(std::string const& s_);

    BoyerMoore(std::string && s_);

    bool check_substring(std::string const &t) const;
};

#endif // BOYER_MOORE_H
