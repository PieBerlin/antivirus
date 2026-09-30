#include "antivirus.h"


Database *mkdatabase(){
    Database *db;
    Entry *p;
    int32 size;

    size = sizeof(Database);
    db = (Database*)malloc($i size);
    zero($8 db,size);

    db->num = 0;
    db->capacity = BlockSize;
    size = BlockSize * sizeof(Entry);
    p = (Entry*)malloc($i size);
    assert(p);
    zero($8 p,size);

    db->entries = p;

    return db;
}


State mkstate(){
    State s = {0};
    s.stage  = unscanned;
    return s;
}
