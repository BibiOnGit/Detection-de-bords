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

double binomiale(double k,double n,const double p,const std::vector<int> &t){
    assert(k <=n);
    double tail = 0;
    if(n<=30){
        k = int(k);
        n = int(n);
        for(int i=k;i<=n;i++){
            int ind = (n+1)*(n+2)/2 - (n-k+1)-k+i; /*index which go through the n-k+1 last elements of
                                              the pascal triangle*/
            tail+= t[ind]*pow(p,i)*pow(1-p,n-i);
        }
    }
    else
        tail = betai(k,n-k+1,p);
    return tail;
}

double betai(double a, double b, double x)
{
    assert(x>=0 && x<=1);
    double bt;
    if (x == 0.0 || x == 1.0) bt=0.0;
    else //Factors in front of the continued fraction.
        bt=exp(lgamma(a+b)-lgamma(a)-lgamma(b)+a*log(x)+b*log(1.0-x));
    if (x < (a+1.0)/(a+b+2.0)) //Use continued fraction directly
        return bt*betacf(a,b,x)/a;
    else //Use continued fraction after making the symmetry transformation
        return 1.0-(bt*betacf(b,a,1.0-x)/b);
}
double betacf(double a, double b, double x)
{
    int m,m2;
    double aa,c,d,del,h,qab,qam,qap;
    qab=a+b;
    qap=a+1.0;
    qam=a-1.0;
    c=1.0;
    d=1.0-qab*x/qap;
    if (fabs(d) < FPMIN)
        d=FPMIN;
    d=1.0/d;
    h=d;
    for (m=1;m<=MAXIT;m++) {
        m2=2*m;
        aa=m*(b-m)*x/((qam+m2)*(a+m2));
        d=1.0+aa*d;
        if (fabs(d) < FPMIN)
            d=FPMIN;
        c=1.0+aa/c;
        if (fabs(c) < FPMIN)
            c=FPMIN;
        d=1.0/d;
        h *= d*c;
        aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
        d=1.0+aa*d;
        if (fabs(d) < FPMIN)
            d=FPMIN;
        c=1.0+aa/c;
        if (fabs(c) < FPMIN)
            c=FPMIN;
        d=1.0/d;
        del=d*c;
        h *= del;
        if (fabs(del-1.0) < EPS)
            break;
    }
    assert(m<=MAXIT);
    return h;
}
