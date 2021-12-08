#include<stdio.h>
#include<stdlib.h>
#include"cblock.h"

#define L2SZ 16384
#define L2BSZ 8
#define L2SSZ 8
#define WRITE 1


struct missReturn mycache(int l1_size, int block_size, int set_size, char* filename)
{
    
    struct cblock *L1, *L2;
    L1 = (struct cblock*)malloc(sizeof(struct cblock)*l1_size);
    L2 = (struct cblock*)malloc(sizeof(struct cblock)*L2SZ);

    int write_back = 0;
    int miss= 0, miss2 = 0;
    int total = 0;
    struct missReturn myreturn;

    int i;
    for(i=0;i<l1_size;i+=block_size)
    {
        L1[i].hit=0;
        L1[i].tag=-1;
    }
    for(i=0;i<L2SZ;i+=block_size)
    {
        L2[i].hit=0;
        L2[i].tag=-1;
    }
    FILE *fin = fopen(filename,"r");
    int order, addr;
    while(fscanf(fin,"%d %x",&order,&addr)!=EOF){
        total++;
       // printf("%dth item\n",total);
       // addr/=4;
        int l1set = (addr%l1_size)/block_size/set_size;
        int l1tag = (addr/l1_size);
        int l1miss= 0;
        int minHit = 99999999, minloc = -1;
       
        //printf("%d %d %d\n",addr,l1set,l1tag);
        for(i=0;i<set_size;i++)
        {
            int ca_pos = l1set*block_size*set_size + i*block_size;
            if(L1[ca_pos].tag == -1)
            {
                /**
                 * L1 Miss : empty Cache
                 * 
                 */
                miss++;
                L1[ca_pos].tag = l1tag;
                L1[ca_pos].hit = total;
                L1[ca_pos].type = order;
                l1miss=1;
                minloc=-1;
                break;
            }
            else if(L1[ca_pos].tag == l1tag)
            {
                /**
                 * L1 Hit
                 * 
                 */
                if(order==WRITE) L1[ca_pos].type = WRITE;
                L1[ca_pos].hit = total;
                minloc=-1;
                break;
            }
            else if(minHit>L1[ca_pos].hit){
                minHit = L1[ca_pos].hit;
                
                minloc = ca_pos;
            }
            
        }
        if(minloc!=-1)
        {
            /**
             * L1 miss : No avaliable space
             * 
             */
            miss++;
            L1[minloc].hit = 1;
            l1miss=1;
            L1[minloc].tag = l1tag;
            L1[minloc].type = order;
        }
        if(l1miss==1)
        {
            minHit = 99999999;
            minloc = -1;
            int l2tag= addr/L2SZ;
            int l2set = (addr%L2SZ) / block_size / L2SSZ;
            for(i=0;i<L2SSZ;i++)
            {
                int cpos = l2set*block_size*L2SSZ + i*block_size;
                if(L2[cpos].tag == -1)
                {
                    /**
                     * L2 Miss : empty Cache
                     * 
                     */
                    miss2++;
                    minloc=-1;
                    L2[cpos].tag = l1tag;
                    L2[cpos].hit = 1;
                    L2[cpos].type = order;
                    l1miss=1;
                    break;
                }
                else if(L2[cpos].tag == l1tag)
                {
                    /**
                     * L2 Hit
                     * 
                     */
                    L2[cpos].hit = total;
                    minloc=-1;
                    break;
                }
                else if(minHit>L2[cpos].hit){
                    minHit = L2[cpos].hit;
                    minloc = cpos;
                }
            }if(minloc!=-1)
            {
                /**
                 * L2 miss : No avaliable space
                 * 
                 */
                miss2++;
                L2[minloc].hit = 1;
                L2[minloc].tag = l2tag;
                if(L2[minloc].type == WRITE) write_back++;
                L2[minloc].type = order;
            }
        }
    }

    close(fin);  
    myreturn.L1miss = (double)miss/(double)total;
    myreturn.L2miss = (double)miss2/(double)total;
    myreturn.write_back = write_back;
    myreturn.total=total;
    return myreturn;
}