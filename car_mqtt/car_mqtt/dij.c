// #include <stdio.h>
// #include <stdlib.h>
// #include <limits.h>
// #include <stdbool.h>
// #define debug printf("test\n")
// const int N = 5005;
// const int INF = 2e9+7;

// struct Edge {
// 	int to, next, val;
// 	char oper; // 记录操作的3个操作  'l', 'r', 'f'
// } e[N];

// int head[N], cnt = 0;
// int dist[N];
// bool vis[N];
// int prev[N]; // 用于记录路径
// char oper_prev[N]; // 用于记录路径上的操作
// // 添加边的函数
// void add(int u, int v, int val, char oper) {
// 	//printf("u:%d,v:%d\n",u,v);
// 	e[++cnt].to = v;
// 	e[cnt].val = val;
// 	e[cnt].oper = oper;
// 	e[cnt].next = head[u];
// 	head[u] = cnt;
// }

// // 优先队列的节点
// typedef struct {
// 	int u, dist;
// } Node;

// // 优先队列的比较函数
// int cmp(const void* a, const void* b) {
// 	return ((Node*)a)->dist - ((Node*)b)->dist;
// }

// // 堆优化Dijkstra算法
// void dijkstra(int start) {
// 	for (int i = 0; i < N; i++) {
// 		dist[i] = INF;
// 		vis[i] = false;
// 		prev[i] = -1;
// 	}
// 	dist[start] = 0;

// 	Node pq[N];
// 	int pq_size = 0;

// 	pq[pq_size++] = (Node){ start, 0 };

// 	while (pq_size > 0) {
// 		qsort(pq, pq_size, sizeof(Node), cmp);
// 		Node node = pq[0];
// 		pq[0] = pq[--pq_size];

// 		int u = node.u;
// 		if (vis[u]) continue;
// 		vis[u] = true;

// 		for (int i = head[u]; i; i = e[i].next) {
// 			int v = e[i].to;
// 			int w = e[i].val;

// 			if (dist[v] > dist[u] + w) {
// 				dist[v] = dist[u] + w;
// 				prev[v] = u;
// 				oper_prev[v] = e[i].oper;
// 				pq[pq_size++] = (Node){ v, dist[v] };
// 			}
// 		}
// 	}
// }

// char load[N];
// int tot;
// void find_load(int start,int end)
// {
// 	tot=0;
// 	int now=end;
// 	while(now!=start)
// 	{
// 		//printf("%d->%d=%c   ",prev[now],now,oper_prev[now]);
// 		load[++tot]=oper_prev[now];
// 		now=prev[now];
// 	}
// }

// bool check(int u,int v)
// {
// 	int tab[4]={15,18,33,36};
// 	for(int i=0;i<4;i++){
// 		for(int j=0;j<4;j++)
// 		{
// 			if(u==tab[i]+j*36||v==tab[i]+j*36)return 0;
// 		}
// 	}
// 	return 1;
// }
// int table[5][20];
// int table_cnt[5];
// void init()
// {
// 	for(int i=0;i<=4;i++)table_cnt[i]=0;
	
// 	//出餐 
// 	for(int i=0;i<=3;i++)
// 		table[0][++table_cnt[0]]=1+i*36;
	
// 	//1
// 	for(int i=0;i<=3;i++)
// 		table[1][++table_cnt[1]]=9+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[1][++table_cnt[1]]=14+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[1][++table_cnt[1]]=16+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[1][++table_cnt[1]]=21+i*36;
	
// 	//2
// 	for(int i=0;i<=3;i++)
// 		table[2][++table_cnt[2]]=12+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[2][++table_cnt[2]]=17+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[2][++table_cnt[2]]=24+i*36;
		
// 	//3
// 	for(int i=0;i<=3;i++)
// 		table[3][++table_cnt[3]]=27+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[3][++table_cnt[3]]=32+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[3][++table_cnt[3]]=34+i*36;
		
// 	//4
// 	for(int i=0;i<=3;i++)
// 		table[4][++table_cnt[4]]=30+i*36;
// 	for(int i=0;i<=3;i++)
// 		table[4][++table_cnt[4]]=35+i*36;
	
// 	for(int i=1;i<=6;i++)
// 	{
// 		for(int j=i+6;j<=36;j+=6)
// 		{
// 			int u=j,v=j-6;
// 			if(!check(u,v))continue;
// 			add(u,v,1,'f');
// 		}
// 	}
// 	for(int i=37;i<=42;i++)
// 	{
// 		for(int j=i+6;j<=72;j+=6)
// 		{
// 			int v=j,u=j-6;
// 			if(!check(u,v))continue;
// 			add(u,v,1,'f');
// 		}
// 	}
// 	for(int i=73;i<=108;i+=6)
// 	{
// 		for(int j=i+1;j<=i+5;j++)
// 		{
// 			int u=j,v=j-1;
// 			if(!check(u,v))continue;
// 			add(u,v,1,'f');
// 		}
// 	}
// 	for(int i=109;i<=144;i+=6)
// 	{
// 		for(int j=i+1;j<=i+5;j++)
// 		{
// 			int u=j-1,v=j;
// 			if(!check(u,v))continue;
// 			add(u,v,1,'f');
// 		}
// 	}
// 	for(int i=1;i<=36;i++)
// 	{
// 		int x=i,y=i+36,z=i+72,w=i+108;
// 		add(x,z,1,'l');
// 		add(z,y,1,'l');
// 		add(y,w,1,'l');
// 		add(w,x,1,'l');
		
// 		add(z,x,1,'r');
// 		add(y,z,1,'r');
// 		add(w,y,1,'r');
// 		add(x,w,1,'r');
// 	}
// 	return;
// }

// int work(int now,int goal)//当前状态，目标餐位 
// {
// 	int start=now,end=table[goal][1];
// 	dijkstra(start);
// 	for(int i=1;i<=table_cnt[goal];i++)
// 	{
// 		int v=table[goal][i];
// 		printf("%d-%d\n",v,dist[v]);
// 		if(dist[v]<dist[end])end=v;
// 	}
// 	printf("%d_%d\n",start,end);
// 	find_load(start,end);
//     return end;
// }

// // int main() {
// // 	// 初始化图
// // 	init();
// // 	printf("init ok\n");

// // 	int now = 109,goal=4;
	
// // 	work(now,goal);
// // 	for(int i=tot;i>=1;i--)printf("%c",load[i]);
// // 	return 0;
// // }
