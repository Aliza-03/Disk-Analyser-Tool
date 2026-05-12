#include <stddef.h>
#include<stdio.h>
#include<mntent.h>
#include<stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <strings.h>
#include<stdbool.h>


int mnt_size=0;

typedef struct mnt_info{

    unsigned long long int size;
    unsigned long long int avail;
    unsigned long long int free;
    char* m_name;
    char* dir;

}mnt_info;

void json_file(mnt_info* m, int s)
{
    FILE *fp = fopen("disk.json", "w");
    unsigned long long int ss;
    unsigned long long int f;
    unsigned long long int a;
    char* mn;
    char* d;

    fprintf(fp, "{\n \" Disk Info \" :[");

    for (int i=0;i<s;i++){
        if(fp)
        {
           ss= m[i].size;
           f=m[i].free;
           a=m[i].avail;
           mn=m[i].m_name;
           d=m[i].dir;
            
         //  fprintf(fp,"{ \n \"%s\" :{",mn);
           if (i!=0){
                fprintf(fp,",");
           }
          
           fprintf(fp, " {\n \"Mount\" :\"%s\", \n \"Directory \" : \"%s\" , \n\"size\" : \"%llu\", \n \"free\" : \"%llu\",\n \"avail\": \"%llu\"\n\n}",mn,d,ss,f,a);
           //fprintf(fp,"] \n }");
        }

    }
    fprintf(fp," ] \n }");
    fclose(fp);
}
void printmnt_info(mnt_info* m, int s){

    for (int i=0;i<s;i++)
    {
         printf("%s %s size=%llu free=%llu avail=%llu\n",
                m[i].m_name,m[i].dir, m[i].size, m[i].free, m[i].avail);
    }
}

void escape_char(char* s) // in process
{
    int i=0;
    int j=0;
    char* w;
    bool slash=false;

    while(s[i]!='\0')
    {
    
        if (s[i]=='\\')
         {
            slash =true;
            w[j]=s[i];

         }
        else if (slash==true)
         {
            j++;
            w[j]='/';
         }


         w[j]=s[i];
    }
}
mnt_info* get_mnt_info()
{
    FILE* fp;

    int s=8;
    mnt_info* x=malloc(s*sizeof(mnt_info));
    int i=0;

    fp = setmntent("/proc/mounts", "r");
    if (fp == NULL) {
        perror("setmntent");
        exit(1);
    }

    struct mntent* m;
    struct mntent mnt;
    char strings[4096];


    printf("%d\n", s);
    //*(&arr + 1) - arr
    while ((m = getmntent_r(fp, &mnt, strings, sizeof(strings)))) {
        struct statvfs fs;
        if ((mnt.mnt_dir != NULL) && (statvfs(mnt.mnt_dir, &fs) == 0)) {
            
            if(i<s){

                printf("NOT Reallocated \n\n\n");
                x[i].size= fs.f_blocks * fs.f_bsize;
                x[i].free=fs.f_bfree*fs.f_bsize;
                x[i].avail=fs.f_bavail*fs.f_bsize;
                 x[i].m_name=strdup(mnt.mnt_fsname);
                x[i].dir= strdup(mnt.mnt_dir);
                printf("VALUE OF I: %d %s %s size=%llu free=%llu avail=%llu\n",i,
                x[i].m_name, x[i].dir, x[i].size, x[i].free, x[i].avail);
                i++;
            }
            else {

                s*=2;
                mnt_info* temp=realloc(x,s * sizeof(mnt_info));
                printf("%d\n", s);
                x=temp;

                printf("Reallocated \n\n\n");
                x[i].size= fs.f_blocks * fs.f_bsize;
                x[i].free=fs.f_bfree*fs.f_bsize;
                x[i].avail=fs.f_bavail*fs.f_bsize;
               // strcpy(x[i].m_name, mnt.mnt_fsname);
                x[i].m_name=strdup(mnt.mnt_fsname);
                x[i].dir= strdup(mnt.mnt_dir);
                
                printf(" VALUE OF I: %d %s %s size=%lld free=%lld avail=%lld\n",
               i, x[i].m_name, x[i].dir, x[i].size, x[i].free, x[i].avail);
                i++;
            }

            
    }
  }

    endmntent(fp);
    mnt_size=i;
    //json_file(x, s);
    return x;

}

int main(){

    mnt_info* info;

    info = get_mnt_info();
    json_file(info, mnt_size);
    printmnt_info(info,mnt_size);

    return 0;
}

 //json_file(info, mnt_size);
   //printf("%d",mnt_size);

