#include "antivirus.h"


bool iself(Entry e){
    int32 fd; 
    signed int ret;
    int8 path[324]; // dir size (256) + file size (64)  + 2 extra
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


// returns the unix time in seconds since epoch
Timestamp  unixtime(){
    int ret;
    struct timespec ts;

    ret = clock_gettime(CLOCK_REALTIME,&ts);
    if (ret)
        return 0;

    return (Timestamp)ts.tv_sec;

}

// eg 0xab
// aaaa bbbb
int8 ascii2hex(int8 *str){
    int8 a, b;
    int8 res;
    // making sure that the input has a length of 2 
    assert(!*(str+2));

    a = a2h(*str);
    b = a2h(*(str+1));
    res = (a << 4) | b;

    return res;

}


int8 *parsehex(int8 *str){
    int8 *p,*ret,*retp;
    int16 n,size;
    int8 buf[3];

    for (n = 0,p=str;*p;n++,p +=2);

    size = (n + 1);

    ret = $8 malloc($i size);
    assert(ret);
    zero(ret,size);

    for (p=str, retp = ret ;*p;p += 2,retp++){
        zero(buf,3);
        *buf = *p;
        *(buf+1) = *(p+1);

        *retp = ascii2hex(buf);

    }
    return ret;

}
