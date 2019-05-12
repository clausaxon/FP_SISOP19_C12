#include <ao/ao.h>
#include <mpg123.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>
#include <dirent.h>

#define BITS 8

pthread_t tid1,tid2;
char filename[100]; 
int flag;
int y=0;
char foldername[100];
typedef struct{
char list[100];
}Name;
Name listku[100];

void* mp3(void *arg)
{
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;


    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, filename);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
    int i = 0;
    while(1){
    if(flag == 0){
    mpg123_read(mh, buffer, buffer_size, &done);
    ao_play(dev, buffer, done); 
    //printf("tes\n");   
    }
    else if(flag == 1){
        sleep(0);
    }
    else if(flag==2){
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
    }
    else if (flag==3){
        printf("stopped\n");
        break;
    }
    //mh = mpg123_new(NULL, &err);
    //buffer_size = mpg123_outblock(mh);
    //buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char)); 
    //}    
    }


}

int stringlist(void *arg[])
{
int n=0, i=0;
DIR *d;
struct dirent *dir;
d = opendir(foldername);

//Determine the number of files
while((dir = readdir(d)) != NULL) {
    if ( !strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") )
    {

    } else {
        n++;
    }
}
rewinddir(d);

char *filesList[n];
//Put file names into the array
while((dir = readdir(d)) != NULL) {
    if ( !strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") )
    {}
    else {
        filesList[i] = (char*) malloc (strlen(dir->d_name)+1);
        strncpy (filesList[i],dir->d_name, strlen(dir->d_name) );
        i++;
    }
}
rewinddir(d);
for(int i=0; i<n; i++){
    char b[100];
    strcpy(b, filesList[i]);
    strcpy(listku[i].list, b);
    printf("%d.", i);
    printf("%s\n", listku[i].list);

    //strcpy(list[i], b);

}
/*for(int i=0; i<=n; i++){
    strncpy (list[i],filesList[i], strlen(filesList[i]) );
    
}*/

}




void* menu(void* arg){
        char cmd[100];
    while(1){
    scanf("%s" ,cmd);
    if(strcmp(cmd, "pause")==0)
        {
            flag = 1;
        }
    else if(strcmp(cmd,"resm")==0)
        {
            flag=0;
        }   
    else if(strcmp(cmd,"play")==0)
        {
            flag=3;
            flag=1;
          //  foldername = ".";
            stringlist(foldername);
            int x;
            printf("Pick song number : \n");
            scanf("%d", &x);
            y=x;
            printf("%s\n", listku[x].list);
            strcpy(filename,listku[x].list);
            flag = 0;
            pthread_create(&(tid1), NULL, mp3, NULL);
        }
    else if(strcmp(cmd,"stop")==0)
        {
            flag = 3;
        }
    else if(strcmp(cmd,"list")==0)
        {
        //    Daftar            printf("%d", y);
    else if(strcmp(cmd,"next")==0)
        {
            flag=3;
            flag=1;
            y=y+1;
            strcpy(filename,listku[y].list);
            flag=0;
            pthread_create(&(tid1), NULL, mp3, NULL);
        }
    else if(strcmp(cmd ,"prev")==0)
        {
            flag=3;
            flag=1;
            y=y-1;
            strcpy(filename,listku[y].list);
            flag=0;
            pthread_create(&(tid1), NULL, mp3, NULL);
        }
    //else if(strcmp(cmd,"exit")==0)
        //{
            //flag=2;
        //}
    }


}


int main()

{   
    flag=1;
    scanf("%s", foldername);
    stringlist(foldername);
    int x;
    printf("Pick song number : \n");    scanf("%d", &x);
    y=x;
    printf("%s\n", listku[x].list);
    strcpy(filename,listku[x].list);
    flag = 0;
    pthread_create(&(tid1), NULL, mp3, NULL);
    pthread_create(&(tid2), NULL, menu, NULL);
   pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
