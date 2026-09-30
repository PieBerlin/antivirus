/* antivirus.c */
#include "antivirus.h"

bool adddir(Database *db,int8 *path){
    Entry e;
    int32 fd;
    int64 n;
    signed int ret;
   struct linux_dirent *p;
   int8 *p2;
   int8 buf[102400] , tmp[256];
   char *filename;
   unsigned char *dtype;

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
            // go to the next file , -1 means go to the last variable inside that structure 
           dtype = p2 + p->d_reclen -1;

           if (*dtype == DT_REG){
               e.type = file;
               e.state = mkstate();
               e.lastscanned = 0;
               strncpy($c e.dir ,$c path,255);
               strncpy($c e.file , filename ,63);
               addtodb(db,e);
           }
           else if (*dtype == DT_DIR){
               e.type = dir;
               e.state = mkstate();
               e.state.stage = unstaged;
               e.lastscanned = 0;
               strncpy($c e.dir,$c path,255);
               strncpy($c e.file , filename,63);
               addtodb(db,e);
               zero(tmp ,sizeof(tmp));
               snprintf($c tmp, 255, "%s/%s",$c path , $c e.file);
               if (strcmp($c tmp,$c path))
                   adddir(db,tmp);
           }
       }
   }while(true);
   close($i fd);
   return true;
}



int main(int argc , char *argv[]){
    int32 fd;
    signed int ret;
    Database *db,*scandb;

    int8 virusfile[] = "./virii.def";
    int8 hex[] = "5c3301dd98\x00\x00";
    int8 *hexs;
    int32 *p;
    void *mem;

    hexs = parsehex(hex);
    mem = hexs;
    p = mem;
    printf("0x%x\n",*p);
    exit(0);

   log("Antivirus Software v%s\n",Version);
   db = prepare();
   ret = open($c virusfile,O_RDONLY);
   if (ret < 1){
       fprintf(stderr, "Unable to open virus definitions: %s\n",virusfile);
       exit(-1);
   }else{
       fd = $32 ret;
   }
   log("%s","Scanning...");
   scandb = scan(db,fd);
   // log ("\r%s\n","Scanning... done");
   destroydb(db);
    return 0;
}
