/* antivirus.c */
#include "antivirus.h"

bool iself(Entry e){
    int32 fd; 
    signed int ret;
    int8 path[98]; // dir size (64) + file size (32)  + 2 extra
    char buf[4];

    if (e.type != file)
       return  false;
    zero(path,sizeof(path));
    snprintf( $c path, sizeof(path) - 1, "%s/%s",$c e.dir, $c e.file);
    ret = open($c path, O_RDONLY);
    if (ret < 1)
        return false ;
    else 
        fd = $32 ret;
    zero($8 buf,4);
    read($i fd,buf,4);
    close($i fd);

    // checking if the file is a elf binary
    // elf binary always starts with : 7f 45 4c 46 
    if (
            (buf[0] == 0x7f)
            && (buf[1] == 0x45)
            && (buf[2] == 0x4c)
            && (buf[3] == 0x46)
       )
        return true;
    else 
        return false;


}

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

bool adddir(Database *db,int8 *path){
    Entry e;
    int32 fd;
    int64 n;
    signed int ret;
   struct linux_dirent *p;
   int8 *p2;
   int8 buf[102400] , tmp[64];
   char *filename;

   ret = open($c path,O_RDONLY|O_DIRECTORY);
   if (ret < 1)
       return false;
   else 
       fd = $32 ret;
   do {
       memset($c buf,0,sizeof(buf));
       ret = syscall(SYS_getdents,$i fd, buf,sizeof(buf) -1);

       if (ret < 0){
           close($i fd);
           return false;
       }
       else if (!ret)
           break;
       n = ret ;

       for (p2 = buf;n ;n -= p->d_reclen,p2 += p->d_reclen){
           p = (struct linux_dirent*)p2;
           zero($8 &e,sizeof(struct s_entry));

           filename = p->d_name - 1 ;

           // checking for . and .. dirs to skip
           if ( *filename == '.' && (*(filename + 1) == '.' || !(*(filename + 1))))
               continue;


           if (p->d_type & DT_REG){
               e.type = file;
               strncpy($c e.dir ,$c path,63);
               strncpy($c e.file , filename ,31);
               addtodb(db,e);
           }
           else if (p->d_type & DT_DIR){
               e.type = dir;
               strncpy($c e.dir,$c path,63);
               strncpy($c e.file , filename,31);
               addtodb(db,e);
               zero(tmp ,64);
               snprintf($c tmp, 63, "%s/%s",$c path , $c e.file);
               adddir(db,tmp);
           }
       }
   }while(true);
   close($i fd);
   return true;
}


int main(int argc , char *argv[]){
    Database *db, *db2;

    assert(argc > 1);
   
    db = mkdatabase();
    adddir(db,$8 argv[1]);
    db2 = filter(db,&iself);
    showdb(db2);
    destroydb(db2);
    return 0;
}
