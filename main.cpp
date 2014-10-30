#include "Distance.h"

int main(int argc, char const *argv[])
{
    string s5("((g:1,(a:1,(b:1,c:1):1):1):1,(f:1,(e:1,d:1):1):1);");
    string s6("((g:2,(a:2,(b:2,c:2):2):2):2,(d:2,(e:2,f:2):2):2);");
    auto t5 = PhyloTree(s5, true);
    auto t6 = PhyloTree(s6, true);

    clock_t start = clock();
    for (size_t i = 0; i < 1; ++i) {
        Distance::getGeodesicDistance(t5, t6, false);
    }
    printf("Time taken: %.0f microsec\n", 1000000 * (double)(clock() - start)/CLOCKS_PER_SEC);
    return 0;
}
