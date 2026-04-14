/*
 *	DKU Operating System Lab (2026)
 *	    Lab1 (Scheduler Algorithm Simulator)
 *	    Student id : 32213679
 *	    Student name : 이현서
    *	Date : 2026.04.15
        Contents : FCFS, SPN, RR, Feedback scheduler implementation
 */

#include <string>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include "sched.h"

class FCFS : public Scheduler
{
private:
   
public:
    FCFS(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead)
    {
        name = "FCFS";
    }

    int run() override
    {
    
        // 
        if (current_job_.name ==0) {

            if (job_queue_.empty()) {
                return -1; // 대기 작업 없으면 종료
            }
                // job_queue에서 current_job_에 할당
                else {
                    current_job_ = job_queue_.front();
                    job_queue_.pop();
                }
            
            // 첫 작업이 아니라면 context switch 시간 추가
            if (!end_jobs_.empty()) {
                current_time_ += switch_time_;
            }

            // first run time 기록
            current_job_.first_run_time = current_time_;
        }

        // 1s 실행
        current_job_.remain_time--;
        current_time_++;

        // 완료된 작업 end_jobs_에 push back
        if (current_job_.remain_time == 0) {
            current_job_.completion_time = current_time_;
            end_jobs_.push_back(current_job_);

            int finished_job_name = current_job_.name;
            current_job_ = Job(); // 초기화
            return finished_job_name; // 완료 작업 이름 반환

        }

        return current_job_.name; // 실행 중인 작업 이름 반환
    }
};

class SPN : public Scheduler
{
private:
    // 도착했으나 실행되지 않은 작업 저장 위한 ready_queue
    std::vector<Job> ready_queue_; 

public:
    SPN(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead)
    {
        name = "SPN";
    }
     
    int run() override
    {
        // 최소 작업 시간 순으로 job_quueue에 할당
        while (!job_queue_.empty() && job_queue_.front().arrival_time <= current_time_) {
            ready_queue_.push_back(job_queue_.front());
            job_queue_.pop();
         }
        
        if (current_job_.name == 0) {
            if (job_queue_.empty() && ready_queue_.empty()) {
                return -1; // 실행, 도착 예정, 대기 작업 없으면 종료
            }  
        
            // 최소 작업 시간 구하는 함수
            int min_index = 0;
            for (size_t i=1; i<ready_queue_.size(); i++) {
            if (ready_queue_[i].service_time < ready_queue_[min_index].service_time) {
                min_index = i;
                }
            }

            // 최소 작업 시간 순으로 ready_queue에서 current_job_에 할당
            current_job_ = ready_queue_[min_index];
            ready_queue_.erase(ready_queue_.begin() + min_index);
                         
            
            // 첫 작업이 아니라면 context switch 시간 추가
            if (!end_jobs_.empty()) {
                current_time_ += switch_time_;
                }

            // first run time 기록
            current_job_.first_run_time = current_time_;
         }

        // 1s 실행
        current_job_.remain_time--;
        current_time_++;

        // 완료된 작업 end_jobs_에 push back
        if (current_job_.remain_time == 0) {
            current_job_.completion_time = current_time_;
            end_jobs_.push_back(current_job_);

            int finished_job_name = current_job_.name;
            current_job_ = Job(); // 초기화
            return finished_job_name; // 완료 작업 이름 반환

        }

        return current_job_.name; // 실행 중인 작업 이름 반환

    }
};

class RR : public Scheduler
{
private:
    int time_slice_;
    int left_slice_;
    std::queue<Job> waiting_queue;
    /* 
    * 구현 (멤버 변수/함수 추가 및 삭제 가능)
    */

public:
    RR(std::queue<Job> jobs, double switch_overhead, int time_slice) : Scheduler(jobs, switch_overhead)
    {
        name = "RR_" + std::to_string(time_slice);
        /*
         * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
         * 나머지는 자유롭게 수정 및 작성 가능 (아래 코드 수정 및 삭제 가능)
         */
        time_slice_ = time_slice;
        left_slice_ = time_slice;
    }
    
    // RR 스케줄링 함수
    // 매 호출마다 도착 작업 wating_queue로 옮긴 후
    // 현재 작업이 없으면 FCFS 방식으로 다음 작업 선택
    // 1초 실행 후 작업 완료 / time slice 종료 여부 체크
    // 모든 작업 완료 시 return -1
    int run() override
    {
       // 도착 작업 옮기기
       while (!job_queue_.empty() && job_queue_.front().arrival_time <= current_time_) {
        waiting_queue.push(job_queue_.front());
        job_queue_.pop();
       }

       if (current_job_.name == 0) {
            if (waiting_queue.empty() && job_queue_.empty()) {
                return -1; // 실행, 도착 예정, 대기 작업 없으면 종료
              }
            else {
                current_job_ = waiting_queue.front();
                waiting_queue.pop();

                // 첫 작업이 아니라면 context switch 시간 추가
                if (!end_jobs_.empty()) {
                    current_time_ += switch_time_;
                }

                // 첫 실행에만 first run time 기록
                if (current_job_.remain_time == current_job_.service_time) { 
                    current_job_.first_run_time = current_time_;
                }

                // time slice 초기화
                left_slice_ = time_slice_; 
            }
        }

        // 1s 실행
        current_job_.remain_time--;
        current_time_++;
        left_slice_--; // 남은 time slice 감소

        // 완료된 작업 end_jobs_에 push back
        if (current_job_.remain_time == 0) { 
            current_job_.completion_time = current_time_;
            end_jobs_.push_back(current_job_);

            int finished_job_name = current_job_.name;
            current_job_ = Job(); // 초기화
            return finished_job_name; // 완료 작업 이름 반환

        }

        // time slice 종료 체크
        if (left_slice_ == 0) { 
           while (!job_queue_.empty() && job_queue_.front().arrival_time <= current_time_) {
                waiting_queue.push(job_queue_.front());
                job_queue_.pop();
                }
            waiting_queue.push(current_job_); // 현재 작업 대기 큐로 이동
            current_job_ = Job(); // 초기화
        }
    
        return current_job_.name; // 실행 중인 작업 이름 반환

    }
};


// FeedBack 스케줄러 (queue 개수 : 4 / boosting 없음)
class FeedBack : public Scheduler
{
private:
    /*
    * 구현 (멤버 변수/함수 추가 및 삭제 가능)
    */

public:
    FeedBack(std::queue<Job> jobs, double switch_overhead, bool is_2i) : Scheduler(jobs, switch_overhead) {
        if (is_2i) {
            name = "FeedBack_2i";
        }
        else {
            name = "FeedBack_1";
        }
        /*
         * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
         * 나머지는 자유롭게 수정 및 작성 가능
         */
    }

    int run() override {
        /*
        * 구현
        */
        return -1;
    }
};
