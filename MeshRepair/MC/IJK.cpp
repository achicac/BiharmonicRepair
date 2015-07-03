#include "MC/IJK.h"
#include <assert.h>

using namespace std;


IJK::IJK(std::string s)
{
    string number;

    int n0 = s.find("-",0);
    number = s.substr(0,n0);
    istringstream strin(number);
    strin>> i;

    int n1 = s.find("-",n0+1);
    number = s.substr(n0+1,n1-n0-1);
    istringstream strin1(number);
    strin1>> j;

    number = s.substr(n1+1);
    istringstream strin2(number);
    strin2>> k;

}

vector<IJK> IJK::getNeighborhood(unsigned int neighbour)
{
    vector<IJK> result;
    result = vector<IJK>(neighbour);
    switch (neighbour)
    {
    case 26:
        result[18] = IJK (-1, -1, -1);
        result[19] = IJK (1, -1, -1);
        result[20] = IJK (1, 1, -1);
        result[21] = IJK (1, -1, 1);
        result[22] = IJK (-1, 1, -1);
        result[23] = IJK (-1, 1, 1);
        result[24] = IJK (-1, -1, 1);
        result[25] = IJK (1, 1, 1);
    case 18:
        result[6] = IJK (-1, -1, 0);
        result[7] = IJK (-1, 1, 0);
        result[8] = IJK (1, -1, 0);
        result[9] = IJK (1, 1, 0);
        result[10] = IJK (-1, 0, -1);
        result[11] = IJK (-1, 0, 1);
        result[12] = IJK (1, 0, -1);
        result[13] = IJK (1, 0, 1);
        result[14] = IJK (0, -1, -1);
        result[15] = IJK (0, -1, 1);
        result[16] = IJK (0, 1, -1);
        result[17] = IJK (0, 1, 1);
    case 6:
        result[0] = IJK (0, 0, -1);
        result[1] = IJK (0, 0, 1);
        result[2] = IJK (0, -1, 0);
        result[3] = IJK (0, 1, 0);
        result[4] = IJK (-1, 0, 0);
        result[5] = IJK (1, 0, 0);
        break;
    case 4:
        result[0] = IJK (-1, 0, 0);
        result[1] = IJK (1, 0, 0);
        result[2] = IJK (0, -1, 0);
        result[3] = IJK (0, 1, 0);
        break;
    default: cerr << "Neighborhood not defined" << neighbour << endl;
        assert(false);
    }

    return result;
}


IJK minimum(const IJK& a, const IJK& b)
{
    IJK result;

    result.i = min(a.i, b.i);
    result.j = min(a.j, b.j);
    result.k = min(a.k, b.k);

    return result;
}
IJK maxim(const IJK& a, const IJK& b)
{
    IJK result;

    result.i = max(a.i, b.i);
    result.j = max(a.j, b.j);
    result.k = max(a.k, b.k);

    return result;
}

