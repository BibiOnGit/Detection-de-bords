#include "utils.h"

void pascalTriangle(std::vector<int>& t, int n){//Fill t with the pascal triangle
    for(int i=1; i<=n;i++){
        for(int j=0;j<=i/2;j++){
            if(j==0)
                t.push_back(1);
            else{
                int indPreviousCoeff = ((i-1)*i)/2 + j-1;
                t.push_back(t[indPreviousCoeff] + t[indPreviousCoeff+1]);
           }
        }
        int size = t.size();
        for(int j=0;j<=(i-1)/2;j++){
            int indSym = (i%2 ==1)?size-1-j:size-2-j;
            t.push_back(t[indSym]);
        }
    }
}

float binomiale(int k, int n, float p){//Compute the sum of the tail of the binomial from the index k
    std::vector<int> t;
    t.push_back(1);//zero choose zero
    pascalTriangle(t,n);
    float sum = 0;
    for(int i=k;i<=n;i++){
        int ind = t.size() - (n-k+1)-k+i;
        sum+= t[ind]*pow(p,i)*pow(1-p,n-i);
    }
    return sum;
}

