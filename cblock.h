struct cblock{
    int tag;
    int hit;
    char type;
};

struct missReturn{
    double L1miss, L2miss;
    int total, write_back;
};