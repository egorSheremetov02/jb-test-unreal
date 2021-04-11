#include "boyer_moore.h"


void BoyerMoore::precalc() {
    std::string t = s;
    std::reverse(t.begin(), t.end());

    // boyer-moore precalculation in O(substring.size())

    // zeta-function precalc
    int l = 0, r = 0;
    for (int i = 1; i < (int) s.size(); ++i) {
        int k = std::max(0 ,std::min (r - i, zeta_function[i - l]));
        while (t[i + k] == t[k])
            k++;
        zeta_function[i] = k;
        if (i + zeta_function[i] > r) l = i, r = i + zeta_function[i];
    }



    for (int i = 0; i < (int) t.size(); ++i)
        pos[t[i]].push_back(i) ;

    for (int j = s.size() - 1; j >= 0; --j)
        shift[zeta_function[j]] = r;
}

BoyerMoore::BoyerMoore(std::string && s_) : s(std::move(s_)), zeta_function(s.size(), 0), pos(1 << 9), shift(s.size(), 0) {
    precalc();
}

BoyerMoore::BoyerMoore(std::string const& s_) : s(s_), zeta_function(s.size(), 0), pos(1 << 9), shift(s.size(), 0) {
    precalc();
}

bool BoyerMoore::check_substring(std::string const &t) const {
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
