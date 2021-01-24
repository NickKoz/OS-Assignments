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

}

// Checks if file is symbolic link.
bool check_if_link(char* filename){

    struct stat stat_buffer = {0};
    lstat(filename, &stat_buffer);   

    return S_ISLNK(stat_buffer.st_mode);
}

// Checks if file exists.
bool file_exists(char* filename){

    struct stat buffer_stat;

    return(stat(filename, &buffer_stat) == 0);
}


// Checks if the 2 files are same.
bool files_same(char* file1, char* file2){

    struct stat buffer1, buffer2;

    stat(file1, &buffer1);
    stat(file2, &buffer2);

    if(buffer1.st_size == buffer2.st_size && buffer2.st_ctim.tv_sec > buffer1.st_ctim.tv_sec){
        return true;
    }

    return false;

}

// Copies src to dest.
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

    if(verbose){
        printf("File %s copied!\n", dest);
    }

    close(fd_src);
    close(fd_dest);

}


// Checks if directory is empty.
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


// Cleans up given directory, deletes all its contents. 
void cleanup_directory(char* direct){

    DIR* dir_ptr = opendir(direct);

    char path[BUFFER_SIZE] = {0};

    struct dirent* dir_buffer;

    while((dir_buffer = readdir(dir_ptr)) != NULL){

        memset(path, 0, BUFFER_SIZE);
        sprintf(path, "%s/%s", direct, dir_buffer->d_name);

        // If current path is directory, we dive into it with recursive call.
        if(check_if_dir(path)){
            if(strcmp(dir_buffer->d_name, ".") && strcmp(dir_buffer->d_name, "..")){
                cleanup_directory(path);
            }
        }
        else{
            unlink(path);
            if(verbose){
                printf("File %s is deleted!\n", path);
            }
        }
    }

    // If current directory is empty, we remove it.
    if(check_if_dir_empty(direct)){
        rmdir(direct);
        if(verbose){
                printf("Directory %s is deleted!\n", direct);
            }
    }

    closedir(dir_ptr);

}


// Cleanups destination directory from files/folders that are not in source 
// directory anymore.
static int cleanup_dest(char* origindir, char* destdir){

    DIR* dp_dest = opendir(destdir);
    DIR* dp_origin = opendir(origindir);

    char buffer_path_src[BUFFER_SIZE] = {0};
    char buffer_path_dest[BUFFER_SIZE] = {0};

    struct dirent* origin_buffer, *dest_buffer;

    bool found;
    int returned;

    // Iterating destination directory.
    while((dest_buffer = readdir(dp_dest)) != NULL){

        memset(buffer_path_dest, 0, BUFFER_SIZE);
        sprintf(buffer_path_dest, "%s/%s",destdir, dest_buffer->d_name);

        // If current item is directory, does a recursive call for it.
        if(check_if_dir(buffer_path_dest)){
            if(strcmp(dest_buffer->d_name, ".") && strcmp(dest_buffer->d_name, "..")){

                memset(buffer_path_src, 0, BUFFER_SIZE);
                sprintf(buffer_path_src, "%s/%s", origindir, dest_buffer->d_name);
                // Recursive call for inner directory.
                returned = cleanup_dest(buffer_path_src, buffer_path_dest);
                // If it returns -1, source directory is empty so we should clean
                // destination directory.
                if(returned == -1){
                    cleanup_directory(buffer_path_dest);
                }
            }
        }
        
        // If current item is a regular file, we should search source directory
        // for it.

        found = false;

        dp_origin = opendir(origindir);
        if(dp_origin == NULL){
            return -1;
        }

        while((origin_buffer = readdir(dp_origin)) != NULL){

            memset(buffer_path_src, 0, BUFFER_SIZE);
            sprintf(buffer_path_src, "%s/%s", origindir, origin_buffer->d_name);
            
            if(check_if_dir(buffer_path_src)){
                continue;
            }
            if(!strcmp(origin_buffer->d_name, dest_buffer->d_name)){
                found = true;
                break;
            }

        }

        // If we find the item, we delete it.
        if(!found && !check_if_dir(buffer_path_dest)){
            unlink(buffer_path_dest);
            if(verbose){
                printf("File %s is deleted!\n", buffer_path_dest);
            }
        }

    }

    closedir(dp_origin);
    closedir(dp_dest);
    return 0;
}



// Recursive auxilliary function for quic program.
static void aux_quic(char* origindir, char* destdir){

    DIR* dp_dest = opendir(destdir);

    // If destination directory does not exist, we create it.
    if(errno == ENOENT){
        make_dir(origindir, destdir);
    }
    
    DIR* dp_origin = opendir(origindir);


    char buffer_path_src[BUFFER_SIZE] = {0};
    char buffer_path_dest[BUFFER_SIZE] = {0};
    char link_content[BUFFER_SIZE] = {0};

    struct dirent* origin_buffer, *dest_buffer = NULL;
    
    // Iterating source directory's contents
    while((origin_buffer = readdir(dp_origin)) != NULL){

        // and destination's.
        if(dest_buffer != NULL){
            dest_buffer = readdir(dp_dest);
        }

        memset(buffer_path_src, 0, BUFFER_SIZE);
        memset(buffer_path_dest, 0, BUFFER_SIZE);

        sprintf(buffer_path_src, "%s/%s",origindir, origin_buffer->d_name);
        sprintf(buffer_path_dest, "%s/%s",destdir, origin_buffer->d_name);

        // Printing each path.
        if(strcmp(origin_buffer->d_name, ".") && strcmp(origin_buffer->d_name, "..")){
            printf("%s\n", buffer_path_src);
            entities++;
        }

        memset(link_content, 0, BUFFER_SIZE);

        // If current item is directory, we enter it with a recursive call. 
        if(check_if_dir(buffer_path_src)){
            if(strcmp(origin_buffer->d_name, ".") && strcmp(origin_buffer->d_name, "..")){
                quic(buffer_path_src, buffer_path_dest);
            }
        }
        // If current item is symbolic link, we create a new one
        else if(links && check_if_link(buffer_path_src)){
            readlink(buffer_path_src, link_content, BUFFER_SIZE);
            // for linking the source file.
            symlink(link_content, buffer_path_dest);
        }
        else{
            if(!file_exists(buffer_path_dest)){
                copy_file(buffer_path_dest, buffer_path_src);
            }
            else if(file_exists(buffer_path_dest) && !files_same(buffer_path_src, buffer_path_dest)){
                copy_file(buffer_path_dest, buffer_path_src);
            }
            else if(file_exists(buffer_path_dest) && files_same(buffer_path_src, buffer_path_dest)){
                continue;
            }
        }

    }

    closedir(dp_origin);
    closedir(dp_dest);

}



// Main function for quic program.
void quic(char* origindir, char* destdir){

    aux_quic(origindir, destdir);

    // If -d flag is added, we cleanup destination directory.
    if(deleted){
        cleanup_dest(origindir, destdir);
    }

}

