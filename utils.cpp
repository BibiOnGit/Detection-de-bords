#include "utils.h"

void pascalTriangle(std::vector<int>& t, int n, int k){//Rempli le tableau t avec les coefficients binomiaux dont on a besoin
    for(int i=1; i<=n;i++){
        for(int j=(i-k>=0)?i-k+1:0;j<=i;j++){
            if(j==0 || j==i)
                t.push_back(1);
            else{
                int indPrecedent;
                if(i+1 <= k){
                    indPrecedent = ((i-1)*i)/2 + j-1;
                }
                else{
                    indPrecedent = ((k-1)*k)/2 + k*(i-k) + j - (i-k)-1;
               }
               t.push_back(t[indPrecedent] + t[indPrecedent +1]);
           }
        }
    }

}

float binomiale(int k, int n, float p){
    std::vector<int> t;
    t.push_back(1);//zéro parmi zéro
    pascalTriangle(t,n,n-k+1);
    float sum = 0;
    for(int i=k;i<=n;i++){
        int ind = t.size() - (n-k+1)-k+i;
        sum+= t[ind]*pow(p,i)*pow(1-p,n-i);
    }
    return sum;
}












