#include<stdio.h>
#include<stdlib.h>
#include"cblock.h"

#define L2SZ 16384
#define L2BSZ 8
#define L2SSZ 8
#define WRITE 1

struct missReturn mycache_dependent(int l1_size, int block_size, int set_size, char* filename)
{
    
    struct cblock *L1, *L2;
    L1 = (struct cblock*)malloc(sizeof(struct cblock)*l1_size);
    L2 = (struct cblock*)malloc(sizeof(struct cblock)*L2SZ);

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
    int write_back = 0;
    while(fscanf(fin,"%d %x",&order,&addr)!=EOF){
        total++;
       // printf("%dth item\n",total);
        int l1set = (addr%l1_size)/block_size/set_size;
        int l1tag = (addr/l1_size);
        int l1miss= 0;
        int minHit = 99999999, minloc = -1;
        
        //printf("%d %d %d\n",addr,l1set,l1tag);
        
        int ca_pos;
        for(i=0;i<set_size;i++)
        {
            ca_pos = l1set*block_size*set_size + i*block_size;
            if(L1[ca_pos].tag == -1)
            {
                /**
                 * L1 Miss : empty Cache
                 * 
                 */
                miss++;
                minloc= -1;
                //l1miss=1;
                break;
            }
            else if(L1[ca_pos].tag == l1tag)
            {
                /**
                 * L1 Hit
                 * 
                 */
                minloc=-1;
                L1[ca_pos].hit = total;
                if(order == WRITE) L1[ca_pos].type = WRITE;
                ca_pos = -1;
                break;
            }
            else if(minHit>L1[ca_pos].hit){
                minHit = L1[ca_pos].hit;
                minloc = ca_pos;
            }
            
        }
        int is_save_l2 = 0;
        int l2order;
        if(minloc!=-1)
        {
            /**
             * L1 miss : No avaliable space
             * save previous value in L2
             */
            miss++;
            addr = L1[minloc].tag * l1_size + minloc;
            L1[minloc].hit = total;
            l1miss=1;
            l2order = L1[minloc].type;
            L1[minloc].tag = l1tag;
            L1[minloc].type = order;
            is_save_l2 = 1;
            
            
        }
        else if(ca_pos!=-1)
        {
            /**
             * L1 miss : empty block
             * 
             */
            L1[ca_pos].tag = l1tag;
            L1[ca_pos].hit = total;
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
                    L2[cpos].tag = l1tag;
                    L2[cpos].hit = total;
                    L2[cpos].type = l2order;
                    minloc=-1;
                    l1miss=1;
                    break;
                }
                else if(L2[cpos].tag == l1tag)
                {
                    /**
                     * L2 Hit
                     * 
                     */
                    minloc=-1;
                    L2[cpos].hit =total;
                    if(l2order==WRITE) L2[cpos].type = WRITE;
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
                if(L2[minloc].type ==WRITE) write_back ++;
                L2[minloc].type = l2order;
                L2[minloc].hit = total;
                L2[minloc].tag = l2tag;
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