#ifndef _CONFIG_H_
#define _CONFIG_H_

class Config{
public:
    Config();
    ~Config(){};

    void parse_arg(int argc,char *argv[]);
};

#endif /* _CONFIG_H_ */
