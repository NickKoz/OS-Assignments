#include "common_types.h"


// Checks if given is directory or not.
bool check_if_dir(char* dir){

    struct stat stat_buffer = {0};

    stat(dir, &stat_buffer);

    return S_ISDIR(stat_buffer.st_mode);
}

// Creates dir for dest with origin's mode.
void make_dir(char* origin, char* dest){

    struct stat stat_buffer = {0};
    stat(origin, &stat_buffer);

    mkdir(dest, stat_buffer.st_mode);

    if(verbose){
        printf("Created directory %s\n", dest);
    }

    entities_copied++;

}

bool file_exists(char* filename){

    struct stat buffer_stat;

    return(stat(filename, &buffer_stat) == 0);
}


bool files_same(char* file1, char* file2){

    struct stat buffer1, buffer2;

    stat(file1, &buffer1);
    stat(file2, &buffer2);

    if(buffer1.st_size == buffer2.st_size && buffer2.st_ctim.tv_sec > buffer1.st_ctim.tv_sec){
        return true;
    }

    return false;

}


static void copy_file(char* dest, char* src){

    int fd_src = open(src, O_RDONLY);

    struct stat stat_buffer = {0};
    stat(src, &stat_buffer);

    int fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, stat_buffer.st_mode);

    char buffer[BUFFER_SIZE];
    ssize_t nreads;

    clock_t begin,end;

    while((nreads = read(fd_src, buffer, BUFFER_SIZE)) > 0){
        begin = clock();
        if(write(fd_dest, buffer, nreads) < nreads){
            assert(0);
        }
        end = clock();

        bytes_copied += nreads;
        seconds_for_copy += (double)(end-begin) / CLOCKS_PER_SEC;
    }

    entities_copied++;

    close(fd_src);
    close(fd_dest);

}


bool check_if_dir_empty(char* direct){
    
    DIR* dir_ptr = opendir(direct);

    struct dirent* dir_buffer;

    int n = 0;

    while((dir_buffer = readdir(dir_ptr)) != NULL){
        if(++n > 2){
            return false;
        }
    }

    return true;
}


void cleanup_directory(char* direct){

    DIR* dir_ptr = opendir(direct);
    if(check_if_dir_empty(direct)){
        printf("KILL DIR %s\n", direct);
        // rmdir(path);
    }

    char path[BUFFER_SIZE*2] = {0};

    struct dirent* dir_buffer;

    while((dir_buffer = readdir(dir_ptr)) != NULL){

        memset(path, 0, BUFFER_SIZE*2);
        sprintf(path, "%s/%s", direct, dir_buffer->d_name);

        if(check_if_dir(path)){
            if(strcmp(dir_buffer->d_name, ".") && strcmp(dir_buffer->d_name, "..")){
                // if(check_if_dir_empty(path)){
                //     printf("KILL DIR %s\n", dir_buffer->d_name);
                //     // rmdir(path);
                // }
                cleanup_directory(path);
            }
        }
        else{
            printf("TARGET2: %s\n", path);
            unlink(path);
        }
    }

    if(check_if_dir_empty(direct)){
            printf("KILL DIR %s\n", direct);
            rmdir(direct);
    }

    closedir(dir_ptr);

}



int cleanup_dest(char* origindir, char* destdir){

    DIR* dp_dest = opendir(destdir);
    DIR* dp_origin = opendir(origindir);

    char buffer_path_src[BUFFER_SIZE*2] = {0};
    char buffer_path_dest[BUFFER_SIZE*2] = {0};

    struct dirent* origin_buffer, *dest_buffer;

    bool found;
    int returned;

    while(true){
        // Reading destination directory.
        dest_buffer = readdir(dp_dest);

        if(dest_buffer == NULL){
            break;
        }
        memset(buffer_path_dest, 0, BUFFER_SIZE*2);
        sprintf(buffer_path_dest, "%s/%s",destdir, dest_buffer->d_name);

        if(check_if_dir(buffer_path_dest)){
            if(strcmp(dest_buffer->d_name, ".") && strcmp(dest_buffer->d_name, "..")){
                printf("%s\n", dest_buffer->d_name);

                memset(buffer_path_src, 0, BUFFER_SIZE*2);
                sprintf(buffer_path_src, "%s/%s", origindir, dest_buffer->d_name);
                returned = cleanup_dest(buffer_path_src, buffer_path_dest);
                if(returned == -1){
                    cleanup_directory(buffer_path_dest);
                }
            }
            // continue;
        }
        

        found = false;

        dp_origin = opendir(origindir);
        if(dp_origin == NULL){
            printf("DUMP\n");
            return -1;
        }

        while((origin_buffer = readdir(dp_origin)) != NULL){

            memset(buffer_path_src, 0, BUFFER_SIZE*2);
            sprintf(buffer_path_src, "%s/%s", origindir, origin_buffer->d_name);
            
            if(check_if_dir(buffer_path_src)){
                continue;
            }
            if(!strcmp(origin_buffer->d_name, dest_buffer->d_name)){
                found = true;
                break;
            }

        }

        if(!found && !check_if_dir(buffer_path_dest)){
            printf("TARGET: %s\n", dest_buffer->d_name);
            unlink(buffer_path_dest);
        }

    }

    closedir(dp_origin);
    closedir(dp_dest);
    return 0;
}




static void aux_quic(char* origindir, char* destdir){

    DIR* dp_dest = opendir(destdir);

    // If destination directory does not exist, we create it.
    if(errno == ENOENT){
        make_dir(origindir, destdir);
    }
    
    DIR* dp_origin = opendir(origindir);


    char buffer_path_src[BUFFER_SIZE*2] = {0};
    char buffer_path_dest[BUFFER_SIZE*2] = {0};

    struct dirent* origin_buffer, *dest_buffer = NULL;

    while((origin_buffer = readdir(dp_origin)) != NULL){

        if(dest_buffer != NULL){
            dest_buffer = readdir(dp_dest);
        }

        memset(buffer_path_src, 0, BUFFER_SIZE*2);
        memset(buffer_path_dest, 0, BUFFER_SIZE*2);

        sprintf(buffer_path_src, "%s/%s",origindir, origin_buffer->d_name);
        sprintf(buffer_path_dest, "%s/%s",destdir, origin_buffer->d_name);

        if(strcmp(origin_buffer->d_name, ".") && strcmp(origin_buffer->d_name, "..")){
            printf("%s\n", buffer_path_src);
            entities++;
        }

        // if(origin_buffer->d_type == DT_DIR){
        if(check_if_dir(buffer_path_src)){
            if(strcmp(origin_buffer->d_name, ".") && strcmp(origin_buffer->d_name, "..")){
                quic(buffer_path_src, buffer_path_dest);
            }
        }
        else{
            if(!file_exists(buffer_path_dest)){
                copy_file(buffer_path_dest, buffer_path_src);
                printf("1)\n");
            }
            else if(file_exists(buffer_path_dest) && !files_same(buffer_path_src, buffer_path_dest)){
                copy_file(buffer_path_dest, buffer_path_src);
                printf("2)\n");
            }
            else if(file_exists(buffer_path_dest) && files_same(buffer_path_src, buffer_path_dest)){
                printf("3)\n");
                continue;
            }
        }

    }

    closedir(dp_origin);
    closedir(dp_dest);

}




void quic(char* origindir, char* destdir){

    aux_quic(origindir, destdir);

    cleanup_dest(origindir, destdir);

}

