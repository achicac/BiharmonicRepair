#ifndef __IJK_H__
#define __IJK_H__

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>


class IJK {
public:
    short i, j, k;

    IJK (void) {}
    IJK (int ci, int cj, int ck)     { i = ci; j = cj; k = ck; }
	IJK (std::string s);
    IJK  operator+  (const IJK& p)  const { return IJK (i+p.i, j+p.j, k+p.k); }
    void operator+= (const IJK& p)  { i += p.i; j += p.j; k += p.k; }
    IJK  operator-  (const IJK& p)  const { return IJK (i-p.i, j-p.j, k-p.k); }
    void operator-= (const IJK& p)  { i -= p.i; j -= p.j; k -= p.k; }
	IJK  operator/  (int div)  const { return IJK (i/div, j/div, k/div); }
	IJK  operator/  (const IJK& div)  const { return IJK (i/div.i, j/div.j, k/div.k); }
	IJK  operator%  (const IJK& p)  const { return IJK (i%p.i, j%p.j, k%p.k); }
    IJK operator^   (const IJK&p) const {return IJK (i^p.i, j^p.j, k^p.k);}
    bool operator== (const IJK&p) const {return (p.i == i && p.j == j && p.k == k);}
    bool operator!= (const IJK&p) const {return !((*this) == p);}

    IJK distance(const IJK &id) const {return IJK (std::abs(id.i - i), std::abs(id.j - j), std::abs(id.k - k));}
	bool IsEven () const {return (!((i+j+k) & 1));}
	bool belongsToRange (IJK minId, IJK maxId) const {return ((minId.i <= i && i <= maxId.i) &&
			(minId.j <= j && j <= maxId.j) &&
			(minId.k <= k && k <= maxId.k));
	}
	operator std::string ()
	{
		std::ostringstream oss;
		oss << i << "-" << j << "-" << k;
		return oss.str();
	}
	friend std::ostream& operator<< (std::ostream &os, const IJK &ijk)
	{
	     os << ijk.i << " " << ijk.j << " " << ijk.k;

	     return os;
	}
	friend std::istream& operator>> (std::istream &is, IJK &ijk)
	{
	   is >> ijk.i >> ijk.j  >> ijk.k;

	   return is;
	}

    friend bool operator< (const IJK& a, const IJK& b)
	{
	    return (a.i < b.i || (a.i == b.i && a.j < b.j) ||
		    (a.i == b.i && a.j == b.j && a.k < b.k));
	}

	friend bool operator<= (const IJK& a, const IJK& b)
	{
	    return (a < b) || a == b;
	}

    friend IJK minimum(const IJK& a, const IJK& b);
    friend IJK maxim(const IJK& a, const IJK& b);
	static std::vector<IJK> getNeighborhood(unsigned int num);
	unsigned int resolution() const {return i*j*k;}
};

#endif
