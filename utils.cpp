#include "utils.h"
void pascalTriangle(std::vector<int>& t,const int n){
    for(int i=1; i<=n;i++){
        for(int j=0;j<=i/2;j++){
            if(j==0)
                t.push_back(1);
            else{
                int indPreviousCoeff = ((i-1)*i)/2 + j-1; /*index of the binomial coefficient
                                                            j-1 choose i-1*/
                t.push_back(t[indPreviousCoeff] + t[indPreviousCoeff+1]);//Pascal formula
           }
        }
        int size = t.size();
        for(int j=0;j<=(i-1)/2;j++){
            int indSym = (i%2 ==1)?size-1-j:size-2-j;
            t.push_back(t[indSym]); //fill the "right" part of the Pascal triangle using the symetry
        }
    }
}

double binomiale(const int k,const int n,const double p){
    std::vector<int> t;
    t.push_back(1);//zero choose zero
    assert(k <=n);
    pascalTriangle(t,n);
    double sum = 0;
    for(int i=k;i<=n;i++){
        int ind = t.size() - (n-k+1)-k+i; /*index which go through the n-k+1 last elements of
                                          the pascal triangle*/
        sum+= t[ind]*pow(p,i)*pow(1-p,n-i);
    }
    return sum;
}

