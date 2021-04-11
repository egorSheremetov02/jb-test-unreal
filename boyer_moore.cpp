#include "boyer_moore.h"

#include <utility>


void BoyerMoore::precalc() {
    std::string t = s;
    std::reverse(t.begin(), t.end());

    // boyer-moore precalculation in O(substring.size())

    // zeta-function precalc
    int l = 0, r = 0;
    for (int i = 1; i < (int) s.size(); ++i) {
        int k = std::max(0 ,std::min (r - i, zetaFunction[i - l]));
        while (t[i + k] == t[k])
            k++;
        zetaFunction[i] = k;
        if (i + zetaFunction[i] > r) l = i, r = i + zetaFunction[i];
    }



    for (int i = 0; i < (int) t.size(); ++i)
        pos[t[i]].push_back(i) ;

    for (int j = (int) s.size() - 1; j >= 0; --j)
        shift[zetaFunction[j]] = r;
}

BoyerMoore::BoyerMoore(std::string  s_) : s(std::move(s_)), zetaFunction(s.size(), 0), pos(1 << 9), shift(s.size(), 0) {
    precalc();
}

bool BoyerMoore::checkSubstring(std::string const &t) const {
    int dp = 0;
    int p, k = s.size() - 1, i;

    for (p = 0; p <= (int) (t.size() - s.size()); p += std::max(dp, shift[s.size() - k - 1])) {
        for (k = s.size() - 1; k >= 0; --k)
            if (t[p + k] != s[k])
               break;
        if (k < 0){
            return true;
            break;
        }
        auto &v = pos[t[p + k]];
        for (i = v.size() - 1; i >= 0 && v[i] >= k; --i)
        ;
        dp = (k - (i < 0 ? -1 : v[i]));
    }
    return false;
}
