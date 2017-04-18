//
//  filelog.cpp
//  filelog
//
//  Created by 韩培元 on 2017/4/17.
//  Copyright © 2017年 韩培元. All rights reserved.
//

#include <stdio.h>
#include "filelog.hpp"
#include <string>
#include <sys/file.h>

using std::string;

filelog::filelog(string name){
    filename = name;
    file = fopen(name.c_str(), "a+");
}

size_t filelog::append(const std::string& content){
    int result = flock(fileno(file), LOCK_EX);
    if(!result){
        try{
            size_t cnt = fwrite(content.c_str(), sizeof(char), content.size(), file);
            fwrite("\n", sizeof(char), 1, file);
            fflush(file);
            flock(fileno(file), LOCK_UN);
            return cnt;
        }catch(int e){
            flock(fileno(file), LOCK_UN);
            printf("Error to append log. Errono: %d", e);
            return -1;
        }
    }else{
        return result;
    }
}

size_t filelog::append_nolock(const std::string& content){
    size_t n = fwrite(content.c_str(), sizeof(char), content.size(), file);
    fwrite("\n", sizeof(char), 1, file);
    fflush(file);
    return n;
}
