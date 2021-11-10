#include<iostream>  
#include<algorithm>
#include"omp.h"  
#include<cstdio>
#include<cstdlib>
#include<ctime>
#define For(i,j,n) for(int i=j;i<=n;++i)
#include<sys/time.h>
using namespace std;
int a[1000005],n,p, c[1000005];
int b[25];
int Flag = 0;
int Tsort = 0;
int H[1000005];
int v[1000005], Next[1000005];
struct  timeval start1;
struct  timeval end1;
int main(int argc,char **argv)
{
    srand((unsigned)time(0));
    n=atoi(argv[1]);
	p=atoi(argv[2]);
	cout<<"n= "<<n<<" p="<<p<<endl;
    //scanf("%d", &n);scanf("%d", &p);
    
#pragma omp parallel for num_threads(p)  
    For(i, 1, n){a[i] = rand()%100;} 
#pragma omp sections
    {
       #pragma omp section
        {b[0] = 1; For(i, 1, 20)b[i] = 2 * b[i - 1]; }
      // #pragma omp section
        //{cout<<"a[i]:"<<endl;For(i, 1, n)cout << a[i] << " "; cout << endl; cout<<endl;}
       #pragma omp section
        {For(i, 1, n)c[i] = a[i]; }
        
    }
    gettimeofday(&start1,NULL);
    sort(c + 1, c + 1 + n);
    gettimeofday(&end1,NULL);
    
    long long startusec=start1.tv_sec*1000000+start1.tv_usec;
    long long endusec=end1.tv_sec*1000000+end1.tv_usec;
    double elapsed=(double)(endusec-startusec)/1000000.0;
    printf("qsort:  %.6f seconds\n",elapsed);
    cout<<endl;
    
    for (int i = 20; i >= 0; --i)if (n > b[i]) { Flag = i; break; }

#pragma omp parallel for num_threads(p)  
    For(i, 1, n)
    {
        v[i] = a[i]; H[i] = i;
    }

    gettimeofday(&start1,NULL);

    For(k, 0, Flag)
    {
        int L = b[k];
       #pragma omp parallel for num_threads(p)  
        for (int i = 1; i <= n; i += 2 * L)
        {
            int hx = i, hy = hx + L;
            //cout <<"hx:"<< hx << " hy:" << hy << endl;
            

            if (hy > n){continue;}
            int x = H[hx], y = H[hy],lastx=-1,lasty=-1;
            while (x != 0 && y != 0)
            {
                if (v[x] <= v[y]){lastx=x;x = Next[x];}
                else
                {
                    if (lasty != -1){Next[lasty] = Next[y];}
                    else {H[hy] = Next[y];}

                    
                    if (lastx != -1){Next[lastx] = y;}
                    else {H[hx] = y;}

                    Next[y] = x; 

                    lastx = y;
                    if (lasty != -1){y = Next[lasty];}
                    else {y = H[hy];}
                }
            }

            

            if (y == 0)
            {
                //int t = H[hx];
                //while (t != 0)cout << v[t] << " ", t = Next[t]; cout << endl;
                continue;
            }
            Next[lastx] = y;

            //int t = H[hx];
            //while (t != 0)cout << v[t] << " ", t = Next[t]; cout << endl;
        }
    }
    
    gettimeofday(&end1,NULL);
    
    startusec=start1.tv_sec*1000000+start1.tv_usec;
    endusec=end1.tv_sec*1000000+end1.tv_usec;
    elapsed=(double)(endusec-startusec)/1000000.0;
    printf("omp:  %.6f seconds\n",elapsed);
    cout<<endl;
    
    /*cout<<"omp result"<<endl;
    int x = H[1];
    while (x != 0){cout << v[x] << " "; x = Next[x];} cout << endl;


    cout<<"qsort result"<<endl;
    For(i, 1, n){cout << c[i] << " ";} cout << endl;*/ 
    return 0;
}
