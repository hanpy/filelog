//
//  filelog.h
//  filelog
//
//  Created by 韩培元 on 2017/4/17.
//  Copyright © 2017年 韩培元. All rights reserved.
//

#ifndef filelog_h
#define filelog_h
#include <string>
#include <stdio.h>
class filelog{
public:
    filelog(std::string);
    size_t append(const std::string &);
    size_t append_nolock(const std::string &);
    virtual ~filelog(){
        fclose(file);
    }
private:
    FILE* file;
    std::string filename;
};

#endif /* filelog_h */
