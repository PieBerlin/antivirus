#include "antivirus.h" 


Database *filter(Database *input, function f){
    Database *output;
    Entry p;
    int32 n;
    bool predicate;

    output = mkdatabase();

    for (n = 0; n < input ->num; n++){
        p = input->entries[n];
        predicate = f(p);
        if (predicate)
            addtodb(output,p);
    }

    destroydb(input);


    return output;
}
void showdb(Database *db){
    int32 n;
    printf("Capacity:\t%d\nnum:\t\t%d\n",db->capacity,db->num);

    for (n=0;n<db->num;n++){
        printf("%s/%s%c\n",db->entries[n].dir, db->entries[n].file,
                (db->entries[n].type == dir) ? '/' : 0);
    }

}

void destroydb(Database *db){
    db->capacity = 0;
    db->num = 0;
    free(db->entries);
    free(db);
    return;
}

void addtodb(Database *db,Entry e){
    int32 size,cap,index;

    if (db->num == db->capacity){
        cap = db->capacity + BlockSize;
        size = cap * sizeof(struct s_entry);
        db->entries = realloc(db->entries,size);
        assert(db->entries);
        db->capacity = cap;
    }

    index = db->num;
    memcpy(&db->entries[index], $c &e ,sizeof(struct s_entry));
    db->num++;
    return;
    
}


Database* prepare (){
    Database *db;

    db = mkdatabase();
    // log("%s","Enumerating filesystem...");
    // adddir(db,$8 "/");
    // log("Found %d files\nFiltering out non-executables...",db->num);
    // db = filter(db,&iself);
    // log("%d left\n",db->num);
    // showdb(db2);
    // destroydb(db2);

    return db;

}

/*
 * virus1 13320850498\n
 
 */ 
int8 readbyte(Buffer* buf){
    int8 tmp[2];
    signed int ret;
    int32 n,x;
    int8 c;

    if (buf->state == idle){
        zero(buf->buf,BufSize);
        ret = read($i buf->fd,$c buf->buf,(BufSize - 1));
        if (ret < 1)
            return 0;
        else 
            n = $32 ret;

        //if we have read everything
        if (n < (BufSize - 1)){
            buf->eof = true;
        }

        ret = lseek($i buf->fd,0,SEEK_CUR);
        if (ret == -1)
            return 0;
        else 
            buf->filepos = ret;

        x =$i 0;

        buf->bufpos = buf->start = buf->buf;
        buf->end = buf->start + n -1 ;

        while (*buf->end != '\n'){
            if (buf->filepos){
                buf->filepos--;
                ret = lseek($i buf->fd,$i buf->filepos, SEEK_SET);
                if (ret != -1){
                    *tmp =  0;
                    *(tmp+1) =  0;
                    ret = read($i buf->fd, $c tmp, 1);
                    if (ret == 1){
                        x++;
                        if (*tmp != '\n'){
                            // buf ->filepos--;
                            // ret = lseek($i buf->fd,$i buf->filepos, SEEK_SET);
                            // if (ret != 1)
                            //     continue;
                            }
                            else {
                                buf->end -= x - 1 ;
                                zero(buf->end + 1,x);
                                buf->state = newline;
                                break;
                            }
                        }
                    }
                }
            }
            buf->state = newline;
            c = *buf ->bufpos;
            return c;
        }
        else if (buf->state == newline){
            if (buf->bufpos > buf->end)
                return 0;
            buf->bufpos++;
            c = * buf->bufpos;
            return c;
        }
        else if (buf->state == space){
            if (buf->bufpos == buf->start)
                return 0;
            buf->bufpos--;
            c = *buf->bufpos;

            return c;
        }
    return 0;

}
Database *scan(Database *db,int32 fd){
    int8 c;
    int8 virus[32];
    int8 fingerprint[BufSize];
   Buffer buf = {0};
   Database *output;

   output = mkdatabase();
   buf.fd = fd;
   buf.state = idle;
   buf.filepos = 0;
   buf.bufpos = $8 0;
   buf.start = $8 0;
   buf.end = $8 0;
   buf.eof = false;
   buf.eol = $8 0;


   do {
       c = readbyte(&buf);
       if (!c)
           return db;

       if ((c == '\n') && (buf.state == newline)){
           *buf.bufpos = 0;
           buf.state = space;
           buf.eol = buf.bufpos;
       }
       else if ((c == ' ') && (buf.state == space)){
           *buf.bufpos = 0;
           zero(fingerprint,BufSize);
           strncpy($c fingerprint, $c (buf.bufpos + 1),$i (BufSize - 1));
           zero(virus,32);
           strncpy($c virus,$c buf.start,31);

           printf("Virus: '%s'\nFingerprint: '%s'\n\n",$c virus,$c fingerprint);
           buf.bufpos = buf.eol + 1;

           if (buf.bufpos > buf.end){
                if (buf.eof)
                    return db;
                else {
                    buf.state = idle;
                }
           }
           else 
                buf.state = newline;

       }
   }while(true);

   return db;

}

