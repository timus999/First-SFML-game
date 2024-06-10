#include<cstdlib>
#include "utility.h"

int randInRange(int lower, int upper){
    return lower + ( rand() % ( upper - lower + 1));
}