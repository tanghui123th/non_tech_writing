struct PARTINFO {
    int cost;
    int supplier;
};

struct SUBASSYINFO {
    int n_parts;
    struct {
        char partno[10];
        short quan;
    } parts[MAXPARTS];
};

struct INVREC {
    char partno[10];
    int quan;
    enum { PART, SUBASSY } type;
    union {
        struct PARTINFO part;
        struct SUBASSYINFO subassy;
    } info;
};

if (rec.type == PART) {
    y = rec.info.part.cost;
    x = rec.info.part.supplier;
} else {
    y = rec.info.subassy.nparts;
    z = rec.info.subassy.parts[0].quan;
}