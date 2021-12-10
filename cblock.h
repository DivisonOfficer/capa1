struct cblock{
    int tag;
    int hit;
    char type;
};

struct missReturn{
    double L1miss, L2miss,L1Imiss;
    int total, write_back;
};

int write_back;
int miss;
int miss2;
int missi;
int total;
int l2total;
int block_size;
int set_size;
int l1_size;
int totald, totali;
struct cblock *L1, *L2, *L1_i,*L1_d;