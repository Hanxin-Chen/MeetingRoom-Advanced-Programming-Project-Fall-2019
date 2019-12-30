#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<cstring>
#include<cmath>                                                
#include<cstdio>                                                   
#include<cstdlib>
#include<algorithm>
#include<set>
#include<map>
#include<cctype>
#include<sstream>
#include<queue>
using namespace std;
#define N 10010
#define inf 1<<30
int dp[N];
int main()
{
	ios::sync_with_stdio(false);
	int value, cost;
	int e, f;
	int total;
	int t,n;
	cin >> t;
	while (t--)
	{
		
		cin >> e >> f;
		total = f - e;
		cin >> n;
		dp[0] = 0;
		for (int i = 1;i <= total;i++)
			dp[i] = inf;


		for (int i = 1;i <= n;i++)
		{
			cin >> value >> cost;
			for (int j = cost;j <= total;j++)
			{
				dp[j] = min(dp[j], dp[j - cost] + value);
			}
		}
		if (dp[total] == inf)
			cout << "This is impossible." << endl;
		else
			cout << "The minimum amount of money in the piggy-bank is " << dp[total]<<"."<<endl;
	}
	return 0;
}
/*#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<queue>
#include<map>
#include<math.h>
using namespace std;

const int maxn=10000+10;
const int inf=0x3f3f3f3f;




int min(int a, int b)
{
	return a > b ? b : a;
}

int main()
{
	int dp[maxn], i, j, n, h, v, t, em, fi;
	cin >> t;
	while (t--)
	{
		cin >> em >> fi;
		cin >> n;
		for (i = 1;i <= fi - em;i++)
			dp[i] = inf;
		dp[0] = 0;
		for (i = 1;i <= n;i++)
		{
			cin >> v >> h;
			for (j = h;j <= fi - em;j++)
				dp[j] = min(dp[j], dp[j - h] + v);
		}
		if (dp[fi - em] == inf)
			puts("This is impossible.");
		else
			printf("The minimum amount of money in the piggy-bank is %d.\n", dp[fi - em]);
	}
	return 0;
}*/
