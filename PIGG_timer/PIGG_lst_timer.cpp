#include "PIGG_lst_timer.h"
#include <ctime>
#include <sys/epoll.h>
#include <sys/unistd.h>
#include <cassert>
#include <signal.h>
#include "../PIGG_http/PIGG_http.h"


/*************************************************PIGG_sort_timer_lst*********************************************/
PIGG_sort_timer_lst::PIGG_sort_timer_lst(){

}

PIGG_sort_timer_lst::~PIGG_sort_timer_lst(){
    
}

void PIGG_sort_timer_lst::add_timer(PIGG_util_timer *timer){
    if(!timer)
        return;
    if(!head){
        head = tail = timer;
        return;
    }
    if(timer->expire < head->expire){
        timer->next = head;
        head->prev = timer;
        head = timer;
        return;
    }
    add_timer(timer,head);
}

void PIGG_sort_timer_lst::add_timer(PIGG_util_timer *timer, PIGG_util_timer* lst_head){
    PIGG_util_timer *prev = lst_head;
    PIGG_util_timer *tmp = prev->next;
    while(tmp){
        if(timer->expire < tmp->expire){
            prev->next = timer;
            timer->next = tmp;
            tmp->prev = timer;
            timer->prev = prev;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    if(!tmp){
        prev->next = timer;
        timer->prev = prev;
        timer->next = NULL;
        tail = timer;
    }
}

void PIGG_sort_timer_lst::adjust_timer(PIGG_util_timer *timer){
    if(!timer){
        return;
    }
    PIGG_util_timer *tmp = timer->next;
    if(!tmp || (timer->expire < tmp->expire)){
        return;
    }

    if(timer == head){
        head = head->next;
        head->prev = NULL;
        timer->next = NULL;
        add_timer(timer,head);
    }else{
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        add_timer(timer,head);
    }
}

void PIGG_sort_timer_lst::del_timer(PIGG_util_timer *timer){
    if(!timer){
        return;
    }
    if((timer == head) && (timer == tail)){
        delete timer;
        head = NULL;
        tail = NULL;
        return;
    }
    if(timer == head){
        head = head->next;
        head->prev = NULL;
        delete timer;
        return;
    }
    if(timer == tail){
        tail = tail->prev;
        tail->next = NULL;
        delete timer;
        return;
    }
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;
}

void PIGG_sort_timer_lst::tick(){
    if(!head){
        return;
    }

    time_t cur = time(NULL);
    PIGG_util_timer* tmp = head;
    while(tmp){
        if(cur < tmp->expire){
            break;
        }
        tmp->PIGG_cb_func(tmp->user_data);
        head = tmp->next;
        if(head){
            head->prev = NULL;
        }
        delete tmp;
        tmp = head;
    }

}




/*************************************************PIGG_Utils*********************************************/

void PIGG_Utils::init(int timeslot){

}

int PIGG_Utils::setnonblocking(int fd){
    
}

void PIGG_Utils::addfd(int epollfd, int fd, bool one_shot, int TrigMode){
    
}


void PIGG_Utils::sig_handle(int sig){
    
}

void PIGG_Utils::addsig(int sig, void(handler)(int), bool restart){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if(restart)
        sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}


void PIGG_Utils::timer_handler(){
    
}

void PIGG_Utils::show_error(int connfd, const char* info){
    
}


int PIGG_Utils::PIGG_epollfd = 0;

class PIGG_Utils;

/*************************************************独立的*********************************************/
void PIGG_cb_func(PIGG_client_data *user_data){
    epoll_ctl(PIGG_Utils::PIGG_epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    PIGG_http_conn::PIGG_user_count--;
}