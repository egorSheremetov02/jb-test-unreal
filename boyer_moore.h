#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H
#include <vector>
#include <string>
#include <algorithm>


struct BoyerMoore {
private:
    std::string s;
    std::vector<int> zetaFunction;
    std::vector<std::vector<int>> pos;
    std::vector<int> shift;

    void precalc();

public:

    explicit BoyerMoore(std::string  s_);

    bool checkSubstring(std::string const &t) const;
};

#endif // BOYER_MOORE_H
