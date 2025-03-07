#include "hi_wifi_api.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include <stdio.h>

#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "cJSON.h"

#include "car_test.h"

//dij
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#define debug printf("test\n")
#define N 400
#define INF 2000000007

//
struct Edge {
	int to, next, val;
	char oper; // 记录操作的3个操作  'l', 'r', 'f'
} e[N];

int head[N], cnt = 0;
int dist[N];
bool vis[N];
int prev[N]; // 用于记录路径
char oper_prev[N]; // 用于记录路径上的操作
// 添加边的函数
void add(int u, int v, int val, char oper) {
	//printf("u:%d,v:%d\n",u,v);
	e[++cnt].to = v;
	e[cnt].val = val;
	e[cnt].oper = oper;
	e[cnt].next = head[u];
	head[u] = cnt;
}

// 优先队列的节点
typedef struct {
	int u, dist;
} Node;

// 优先队列的比较函数
int cmp(const void* a, const void* b) {
	return ((Node*)a)->dist - ((Node*)b)->dist;
}

// 堆优化Dijkstra算法
void dijkstra(int start) {
	for (int i = 0; i < N; i++) {
		dist[i] = INF;
		vis[i] = false;
		prev[i] = -1;
	}
	dist[start] = 0;

	Node pq[N];
	int pq_size = 0;

	pq[pq_size++] = (Node){ start, 0 };

	while (pq_size > 0) {
		qsort(pq, pq_size, sizeof(Node), cmp);
		Node node = pq[0];
		pq[0] = pq[--pq_size];

		int u = node.u;
		if (vis[u]) continue;
		vis[u] = true;

		for (int i = head[u]; i; i = e[i].next) {
			int v = e[i].to;
			int w = e[i].val;

			if (dist[v] > dist[u] + w) {
				dist[v] = dist[u] + w;
				prev[v] = u;
				oper_prev[v] = e[i].oper;
				pq[pq_size++] = (Node){ v, dist[v] };
			}
		}
	}
}

char load[N];
int pos[N];
int tot;
void find_load(int start,int end)
{
	tot=0;
	int now=end;
	while(now!=start)
	{
		//printf("%d->%d=%c   ",prev[now],now,oper_prev[now]);
		load[++tot]=oper_prev[now];
		now=prev[now];
        pos[tot]=now;
	}
}

bool check(int u,int v)
{
	int tab[4]={15,18,33,36};
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
		{
			if(u==tab[i]+j*36||v==tab[i]+j*36)return 0;
		}
	}
	return 1;
}
int table[5][20];
int table_cnt[5];
void init()
{
	for(int i=0;i<=4;i++)table_cnt[i]=0;
	
	//出餐 
	for(int i=0;i<=3;i++)
		table[0][++table_cnt[0]]=1+i*36;
	
	//1
	for(int i=0;i<=3;i++)
		table[1][++table_cnt[1]]=9+i*36;
	for(int i=0;i<=3;i++)
		table[1][++table_cnt[1]]=14+i*36;
	for(int i=0;i<=3;i++)
		table[1][++table_cnt[1]]=16+i*36;
	for(int i=0;i<=3;i++)
		table[1][++table_cnt[1]]=21+i*36;
	
	//2
	for(int i=0;i<=3;i++)
		table[2][++table_cnt[2]]=12+i*36;
	for(int i=0;i<=3;i++)
		table[2][++table_cnt[2]]=17+i*36;
	for(int i=0;i<=3;i++)
		table[2][++table_cnt[2]]=24+i*36;
		
	//3
	for(int i=0;i<=3;i++)
		table[3][++table_cnt[3]]=27+i*36;
	for(int i=0;i<=3;i++)
		table[3][++table_cnt[3]]=32+i*36;
	for(int i=0;i<=3;i++)
		table[3][++table_cnt[3]]=34+i*36;
		
	//4
	for(int i=0;i<=3;i++)
		table[4][++table_cnt[4]]=30+i*36;
	for(int i=0;i<=3;i++)
		table[4][++table_cnt[4]]=35+i*36;
	
	for(int i=1;i<=6;i++)
	{
		for(int j=i+6;j<=36;j+=6)
		{
			int u=j,v=j-6;
			if(!check(u,v))continue;
			add(u,v,1,'f');
		}
	}
	for(int i=37;i<=42;i++)
	{
		for(int j=i+6;j<=72;j+=6)
		{
			int v=j,u=j-6;
			if(!check(u,v))continue;
			add(u,v,1,'f');
		}
	}
	for(int i=73;i<=108;i+=6)
	{
		for(int j=i+1;j<=i+5;j++)
		{
			int u=j,v=j-1;
			if(!check(u,v))continue;
			add(u,v,1,'f');
		}
	}
	for(int i=109;i<=144;i+=6)
	{
		for(int j=i+1;j<=i+5;j++)
		{
			int u=j-1,v=j;
			if(!check(u,v))continue;
			add(u,v,1,'f');
		}
	}
	for(int i=1;i<=36;i++)
	{
		int x=i,y=i+36,z=i+72,w=i+108;
		add(x,z,1,'l');
		add(z,y,1,'l');
		add(y,w,1,'l');
		add(w,x,1,'l');
		
		add(z,x,1,'r');
		add(y,z,1,'r');
		add(w,y,1,'r');
		add(x,w,1,'r');
	}
	return;
}

int work(int now,int goal)//当前状态，目标餐位 
{
	int start=now,end=table[goal][1];
	dijkstra(start);
	for(int i=1;i<=table_cnt[goal];i++)
	{
		int v=table[goal][i];
		//printf("%d-%d\n",v,dist[v]);
		if(dist[v]<dist[end])end=v;
	}
	//printf("%d_%d\n",start,end);
	find_load(start,end);
    return end;
}

//


char recvline[2048];

int start=109,end=0;


void cotrl_handle(char* recvline, int ret)
{
    cJSON *recvjson;
    //进行json解析
    recvjson = cJSON_Parse(recvline);
    printf("%s accept\r\n", recvline);
    if(recvjson != NULL)
    {
        bool load_flag=0, song_flag=0,show_flag=0;
        if(cJSON_GetObjectItem(recvjson, "cmd")->valuestring != NULL)
        {
            printf("cmd : %s\r\n", cJSON_GetObjectItem(recvjson, "cmd")->valuestring);
            
            if(strcmp("forward", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                set_car_status(CAR_STATUS_FORWARD);
                printf("forward\r\n");
            }

            if(strcmp("backward", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                set_car_status(CAR_STATUS_BACKWARD);
                printf("backward\r\n");
            }

            if(strcmp("left", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                set_car_status(CAR_STATUS_LEFT);
                printf("left\r\n");
            }

            if(strcmp("right", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                set_car_status(CAR_STATUS_RIGHT);
                printf("right\r\n");
            }

            if(strcmp("stop", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                set_car_status(CAR_STATUS_STOP);
                printf("stop\r\n");
            }

            if(strcmp("goal", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                load_flag=1;
            }

            if(strcmp("song", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                song_flag=1;
            }

            if(strcmp("show", cJSON_GetObjectItem(recvjson, "cmd")->valuestring) == 0)
            {
                show_flag=1;
            }
        }
        // printf("step_mode_set\r\n");
        // set_car_mode(CAR_MODE_STEP);
        // printf("mode step\r\n");
        if(cJSON_GetObjectItem(recvjson, "mode")->valuestring != NULL)
        {
            if(strcmp("step", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
            {
                printf("step_mode_set\r\n");
                set_car_mode(CAR_MODE_STEP);
                printf("mode step\r\n");
            }

            if(strcmp("alway", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
            {
                printf("alway_mode_set\r\n");
                set_car_mode(CAR_MODE_ALWAY);
                printf("mode alway\r\n");
            }

            if(load_flag)
            {
                if(strcmp("out_food", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    end=0;
                    table_state=0;
                }
                if(strcmp("table1", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    end=1;
                    table_state=1;
                }
                if(strcmp("table2", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    end=2;
                    table_state=2;
                }
                if(strcmp("table3", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    end=3;
                    table_state=3;
                }
                if(strcmp("table4", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    end=4;
                    table_state=4;
                }
                int new_start = work(start,end);
                printf("start:%d goal_table:%d\n",start,end);
                printf("Load_find:\n");
                for(int i=tot;i>=1;i--)
                {
                    printf("%c",load[i]);
                }printf("\n");
                printf("Pos_find:\n");
                for(int i=tot;i>=1;i--)
                {
                    printf("%d",pos[i]);
                }printf("\n");
                for(int i=tot;i>=1;i--)
                {
                    pos_state=pos[i];
                    //printf("Doing %c\n",load[i]);
                    if(load[i] == 'l')
                    {
                        set_car_status(CAR_STATUS_LEFT);
                        set_car_mode(CAR_MODE_ALWAY);
                        usleep(500000);
                    }
                    else if(load[i] == 'r')
                    {
                        set_car_status(CAR_STATUS_RIGHT);
                        set_car_mode(CAR_MODE_ALWAY);
                        usleep(500000);
                    }
                    else
                    {
                        set_car_status(CAR_STATUS_FORWARD);
                        set_car_mode(CAR_MODE_ALWAY);
                        usleep(1000000);
                    }
                    //printf("Finish %c\n",load[i]);
                    //printf("Now pos:%d\n",pos[i]);
                }
                set_car_status(CAR_STATUS_STOP);
                printf("Arrive\r\n");
                pos_state=new_start;
                start=new_start;
                table_state = -1;
            }
            if(song_flag)
            {
                if(strcmp("turn", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    if(song_turn==1)song_turn=0;
                    else song_turn=1;
                }

                if(strcmp("song_1", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    song=1;
                }

                if(strcmp("song_2", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    song=2;
                }

                if(strcmp("song_3", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    song=3;
                }
            }
            if(show_flag)
            {
                if(strcmp("turn", cJSON_GetObjectItem(recvjson, "mode")->valuestring) == 0)
                {
                    if(show_state==1)show_state=0;
                    else show_state=1;
                }

            }
        }
        cJSON_Delete(recvjson);
    }
}

void udp_thread(void *pdata)
{
    int ret;
    struct sockaddr_in servaddr;

    pdata = pdata;

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
 
    //服务器 ip port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(50001);

    printf("udp_thread \r\n");
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    init();

    while(1)
    {
        struct sockaddr_in addrClient;
        int sizeClientAddr = sizeof(struct sockaddr_in);

        memset(recvline, sizeof(recvline), 0);
        ret = recvfrom(sockfd, recvline, 1024, 0, (struct sockaddr*)&addrClient,(socklen_t*)&sizeClientAddr);
        
        if(ret>0)
        {
            char *pClientIP =inet_ntoa(addrClient.sin_addr);
 
            printf("%s-%d(%d) says:%s\n",pClientIP,ntohs(addrClient.sin_port),addrClient.sin_port, recvline);

            cotrl_handle(recvline, ret);
		}
    }
}


void start_udp_thread(void)
{
    osThreadAttr_t attr;

    attr.name = "wifi_config_thread";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 16384;
    attr.priority = 36;

    if (osThreadNew((osThreadFunc_t)udp_thread, NULL, &attr) == NULL) {
        printf("[LedExample] Falied to create LedTask!\n");
    }
}

