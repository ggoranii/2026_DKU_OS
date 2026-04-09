/*
 *	DKU Operating System Lab (2026)
 *	    Lab1 (Scheduler Algorithm Simulator)
 *	    Student id : 
 *	    Student name :
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
   std::queue<Job> ready_queue;
    /*
    * 구현 (멤버 변수/함수 추가 및 삭제 가능)
    */

public:
    FCFS(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead)
    {
        name = "FCFS";
    }

    int run() override
    {
        // 1. 도착 작업들을 ready_queue로 이동
        while (!job_queue_.empty() && job_queue_.front().arrival_time <= current_time_) {
            ready_queue.push(job_queue_.front());
            job_queue_.pop();
        }

        // 2. 모든 작업 완료 확인
        if (ready_queue.empty() && current_job_.name == 0) {
            if (job_queue_.empty()) return -1; // 모든 작업 완료
            current_time_++; // 도착 안한 작업 대기
            return 0; // idle 상태
        }
        
        // 3. 현재 작업이 없으면 ready_queue에서 새 작업 선택
        if (current_job_.name == 0 && !ready_queue.empty()) {
            current_job_ = ready_queue.front();
            ready_queue.pop();

            // context switch 시간 추가
            current_time_ += switch_time_;

            // 첫 실행 시간 기록
            if (current_job_.first_run_time == 0) {
                current_job_.first_run_time = current_time_;
            }
        }

        // 4. 1s 실행
        current_job_.remain_time--;
        current_time_++;

        // 5. 작업 완료 처리
        if (current_job_.remain_time == 0) {
            current_job_.completion_time = current_time_;
            end_jobs_.push_back(current_job_);

            int finished_job_name = current_job_.name;
            current_job_ = Job(); // 초기화
            return finished_job_name; // 완료 작업 이름 반환

        }

        return -1;
    }
};

class SPN : public Scheduler
{
private:
    /*
    * 구현 (멤버 변수/함수 추가 및 삭제 가능)
    */

public:
    SPN(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead)
    {
        /* 
        * 구현 (멤버 변수/함수 추가 및 삭제 가능)
        */
    }

    int run() override
    {
        /*
        * 구현
        */
        return -1;
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

    int run() override
    {
        /* 
        * 구현
        */
        return -1;
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
