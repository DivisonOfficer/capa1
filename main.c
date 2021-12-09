#include<stdio.h>
#include<string.h>
#include"cblock.h"


char filename[32];
int cache_size[5] = {1024,2048,4096,8192,16384};
int L1_block_size[3] = {8,16,64};
int assos[4] = {1,2,4,8};

/**
 * 2 : L1,L2 정책
 * 3 : D캐시, 16,64바이트 I캐시
 * 5: 캐시 사이즈
 * 4: 1,2,4,8way
 * 2 : L1,L2
 */
double report_miss[2][3][5][4][2]; 
int report_write_back[2][3][5][4];

struct missReturn mycache(int l1_size, int block_size, int set_size);
struct missReturn mycache_dependent(int l1_size, int block_size, int set_size);
void run_inclusive(){
    struct missReturn my;
    //my = mycache(16384,16,8,filename);
    //printf("%lf %lf",my.L1miss,my.L2miss);

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
            for(k=1;k<3;k++)
            {
                /**
                 * @Block Size
                 * 
                 */
                my = mycache(cache_size[i],L1_block_size[k],assos[j]);
                report_miss[0][k][i][j][0] = my.L1miss;
                report_miss[0][k][i][j][1] = my.L2miss;
                report_write_back[0][k][i][j] = my.write_back;
            }
        }
    }
    /**
     * Print inclusive report
     * 
     */
    
    for(i=1;i<3;i++)
    {
        if(i==0) printf("Cache Miss Ratio (I-Cache)\n");
        else printf("Cache Miss Ratio (block size = %dB)\n",L1_block_size[i]);
        printf("LRU/%d\t",L1_block_size[i]);
        for(k=0;k<5;k++)
            printf("%d\t",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("Direct\t");
            else printf("%d-way\t",assos[j]);
            for(k=0;k<5;k++)
                printf("%.4lf\t",report_miss[0][i][k][j][0]);
            printf("\n");
        }
        printf("\n");
    }

    for(i=1;i<3;i++)
    {
        printf("L2 Miss Ratio (block size = %dB)\n",L1_block_size[i]);
        printf("LRU/%d\t",L1_block_size[i]);
        for(k=0;k<5;k++)
            printf("%d\t",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("Direct\t");
            else printf("%d-way\t",assos[j]);
            for(k=0;k<5;k++)
                printf("%.4lf\t",report_miss[0][i][k][j][1]);
            printf("\n");
        }
        printf("\n");
    }

}
void run_program(){
    int i,j,k;
    struct missReturn my;
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
            for(k=1;k<3;k++)
            {
                /**
                 * @Block Size
                 * 
                 */
                my = mycache_dependent(cache_size[i],L1_block_size[k],assos[j]);
                report_miss[1][k][i][j][0] = my.L1miss;
                report_miss[1][k][i][j][1] = my.L2miss;
                report_write_back[1][k][i][j] = my.write_back;
            }
        }
    }

    /**
     * @Print Block Write Report
     * 
     */
    for(i=1;i<3;i++)
    {
        printf("Number of Memory Block Writes (Inclusive)\n",L1_block_size[i]);
        printf("LRU/%d\t",L1_block_size[i]);
        for(k=0;k<5;k++)
            printf("%d\t",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("Direct\t");
            else printf("%d-way\t",assos[j]);
            for(k=0;k<5;k++)
                printf("%d\t",report_write_back[0][i][k][j]);
            printf("\n");
        }
        printf("\n");
    }
 
    /**
     * Print Exclusive Report
     * 
     */
    printf("Multi-level Cache Policy: exclusive\n\n");
    for(i=1;i<3;i++)
    {
        if(i==0) printf("Cache Miss Ratio (I-Cache)\n");
        else printf("Cache Miss Ratio (block size = %dB)\n",L1_block_size[i]);
        printf("LRU/%d\t",L1_block_size[i]);
        for(k=0;k<5;k++)
            printf("%d\t",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("Direct\t");
            else printf("%d-way\t",assos[j]);
            for(k=0;k<5;k++)
                printf("%.4lf\t",report_miss[1][i][k][j][0]);
            printf("\n");
        }
        printf("\n");
    }

    for(i=1;i<3;i++)
    {
        printf("L2 Miss Ratio (block size = %dB)\n",L1_block_size[i]);
        printf("LRU/%d\t",L1_block_size[i]);
        for(k=0;k<5;k++)
            printf("%d\t",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("Direct\t");
            else printf("%d-way\t",assos[j]);
            for(k=0;k<5;k++)
                printf("%.4lf\t",report_miss[1][i][k][j][1]);
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
        printf("Number of Memory Block Writes (Exclusive)\n",L1_block_size[i]);
        printf("LRU/%2d\t",L1_block_size[i]);
        for(k=0;k<5;k++)
            printf("%d\t",cache_size[k]);    
        printf("\n");    
        for(j=0;j<4;j++)
        {
            if(j==0) printf("Direct\t");
            else printf("%d-way\t",assos[j]);
            for(k=0;k<5;k++)
                printf("%d\t",report_write_back[1][i][k][j]);
            printf("\n");
        }
        printf("\n");
    }
}
int main(int args, char* argv[])
{
  //  if(args<2)
  //  {
  //      printf("Insert filename :");
  //      scanf("%s",filename);
  //  }
   // else strcpy(filename,argv[1]);

    run_inclusive();
    run_program();
    //struct missReturn my;
    //my=mycache(cache_size[0],L1_block_size[0],assos[1],filename);
    
    return 0;
}