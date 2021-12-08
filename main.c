#include<stdio.h>
#include<string.h>
#include"cblock.h"

char filename[32];
int cache_size[5] = {1024,2048,4096,8192,16384};
int block_size[3] = {8,16,64};
int assos[4] = {1,2,4,8};

/**
 * 2 : L1,L2 정책
 * 3 : D캐시, 16,64바이트 I캐시
 * 5: 캐시 사이즈
 * 4: 1,2,4,8way
 * 2 : L1,L2
 */
double report_miss[2][3][5][4][2]; 
int write_back[2][3][5][4];

struct missReturn mycache(int l1_size, int block_size, int set_size, char* filename);
struct missReturn mycache_dependent(int l1_size, int block_size, int set_size, char* filename);
int main(int args, char* argv[])
{
    if(args<2)
    {
        printf("Insert filename :");
        scanf("%s",filename);
    }
    else strcpy(filename,argv[1]);


    struct missReturn my;

    printf("Multi-level Cache Policy: inclusive\n\n");
    int i,j,k;
    for(i=0;i<5;i++)
    {
        /**
         * Cache Size
         * 
         */
        for(j=0;j<4;j++)
        {
            /**
             * Associate
             * 
             */
            for(k=0;k<3;k++)
            {
                /**
                 * @Block Size
                 * 
                 */
                my = mycache(cache_size[i],block_size[k],assos[j],filename);
                report_miss[0][k][i][j][0] = my.L1miss;
                report_miss[0][k][i][j][1] = my.L2miss;
                write_back[0][k][i][j] = my.write_back;
            }
        }
    }
    /**
     * Print inclusive report
     * 
     */
    
    for(i=0;i<3;i++)
    {
        if(i==0) printf("Cache Miss Ratio (I-Cache)\n");
        else printf("Cache Miss Ratio (block size = %dB)\n",block_size[i]);
        printf("|LRU/%2d    |",block_size[i]);
        for(k=0;k<5;k++)
            printf("%10d|",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("|Direct    |");
            else printf("|%d-way     |",assos[j]);
            for(k=0;k<5;k++)
                printf("%10.4lf|",report_miss[0][i][k][j][0]);
            printf("\n");
        }
        printf("\n");
    }

    for(i=0;i<3;i++)
    {
        printf("L2 Miss Ratio (block size = %dB)\n",block_size[i]);
        printf("|LRU/%2d    |",block_size[i]);
        for(k=0;k<5;k++)
            printf("%10d|",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("|Direct    |");
            else printf("|%d-way     |",assos[j]);
            for(k=0;k<5;k++)
                printf("%10.4lf|",report_miss[0][i][k][j][1]);
            printf("\n");
        }
        printf("\n");
    }

    /**
     * Run Exclousive Report
     * 
     */

    for(i=0;i<5;i++)
    {
        /**
         * Cache Size
         * 
         */
        for(j=0;j<4;j++)
        {
            /**
             * Associate
             * 
             */
            for(k=0;k<3;k++)
            {
                /**
                 * @Block Size
                 * 
                 */
                my = mycache_dependent(cache_size[i],block_size[k],assos[j],filename);
                report_miss[1][k][i][j][0] = my.L1miss;
                report_miss[1][k][i][j][1] = my.L2miss;
                write_back[1][k][i][j] = my.write_back;
            }
        }
    }

    /**
     * @Print Block Write Report
     * 
     */
    for(i=1;i<3;i++)
    {
        printf("Number of Memory Block Writes (Inclusive)\n",block_size[i]);
        printf("|LRU/%2d    |",block_size[i]);
        for(k=0;k<5;k++)
            printf("%10d|",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("|Direct    |");
            else printf("|%d-way     |",assos[j]);
            for(k=0;k<5;k++)
                printf("%10d|",write_back[0][i][k][j]);
            printf("\n");
        }
        printf("\n");
    }
 
    /**
     * Print Exclusive Report
     * 
     */
    printf("Multi-level Cache Policy: exclusive\n\n");
    for(i=0;i<3;i++)
    {
        if(i==0) printf("Cache Miss Ratio (I-Cache)\n");
        else printf("Cache Miss Ratio (block size = %dB)\n",block_size[i]);
        printf("|LRU/%2d    |",block_size[i]);
        for(k=0;k<5;k++)
            printf("%10d|",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("|Direct    |");
            else printf("|%d-way     |",assos[j]);
            for(k=0;k<5;k++)
                printf("%10.4lf|",report_miss[1][i][k][j][0]);
            printf("\n");
        }
        printf("\n");
    }

    for(i=0;i<3;i++)
    {
        printf("L2 Miss Ratio (block size = %dB)\n",block_size[i]);
        printf("|LRU/%2d    |",block_size[i]);
        for(k=0;k<5;k++)
            printf("%10d|",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("|Direct    |");
            else printf("|%d-way     |",assos[j]);
            for(k=0;k<5;k++)
                printf("%10.4lf|",report_miss[1][i][k][j][1]);
            printf("\n");
        }
        printf("\n");
    }


    /**
     * @Print Block Write Report
     * 
     */
    for(i=1;i<3;i++)
    {
        printf("Number of Memory Block Writes (Exclusive)\n",block_size[i]);
        printf("|LRU/%2d    |",block_size[i]);
        for(k=0;k<5;k++)
            printf("%10d|",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("|Direct    |");
            else printf("|%d-way     |",assos[j]);
            for(k=0;k<5;k++)
                printf("%10d|",write_back[1][i][k][j]);
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}